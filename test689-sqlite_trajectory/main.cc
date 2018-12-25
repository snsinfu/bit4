#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string>

#include <msgpack.hpp>
#include <sqlite3.h>

#include "wrapped_ptr.hpp"


using namespace std::string_literals;


using sqlite3_ptr = wrapped_ptr<sqlite3, sqlite3_close>;
using sqlite3_stmt_ptr = wrapped_ptr<sqlite3_stmt, sqlite3_finalize>;


static char const* const schema_sql = R"(
CREATE TABLE IF NOT EXISTS sample_point (
    step INTEGER PRIMARY KEY,
    time REAL
);

CREATE TABLE IF NOT EXISTS structure_sample (
    key            INTEGER PRIMARY KEY,
    step           INTEGER REFERENCES sample_point(step),
    coords_msgpack BLOB
);
)";

static char const* const sample_point_sql = R"(
INSERT INTO sample_point VALUES (:step, :time);
)";

static char const* const structure_sample_sql = R"(
INSERT INTO structure_sample (step, coords_msgpack) VALUES (:step, :coords);
)";


static void create_tables(sqlite3* db)
{
    if (int err = sqlite3_exec(db, schema_sql, nullptr, nullptr, nullptr)) {
        throw std::runtime_error("sqlite3_exec: "s + sqlite3_errstr(err));
    }
}


static void store_trajectory(sqlite3* db)
{
    std::mt19937 random;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        random.seed(seed);
    }

    std::vector<double> coords;
    {
        std::uniform_real_distribution<double> coord{-5, 5};
        std::generate_n(std::back_inserter(coords), 180000, [&] { return coord(random); });
    }

    // Prepare SQL insert statements
    sqlite3_stmt_ptr sample_point_stmt;
    sqlite3_stmt_ptr structure_sample_stmt;
    sqlite3_stmt* stmt;

    if (int err = sqlite3_prepare_v2(db, sample_point_sql, -1, &stmt, nullptr)) {
        throw std::runtime_error("sqlite3_prepare_v2 (sample_point): "s + sqlite3_errstr(err));
    }
    sample_point_stmt.reset(stmt);

    int const sample_point_step = sqlite3_bind_parameter_index(sample_point_stmt, ":step");
    int const sample_point_time = sqlite3_bind_parameter_index(sample_point_stmt, ":time");

    if (int err = sqlite3_prepare_v2(db, structure_sample_sql, -1, &stmt, nullptr)) {
        throw std::runtime_error("sqlite3_prepare_v2 (structure_sample): "s + sqlite3_errstr(err));
    }
    structure_sample_stmt.reset(stmt);

    int const structure_sample_step = sqlite3_bind_parameter_index(structure_sample_stmt, ":step");
    int const structure_sample_coords = sqlite3_bind_parameter_index(structure_sample_stmt, ":coords");

    // Start recording trajectory
    std::vector<int> prev_coords;
    msgpack::sbuffer msgpack_buffer;

    for (int step = 1; step <= 100; step++) {
        // Dummy simulation logic
        double const time = step * 0.01;

        std::normal_distribution<double> delta{0, 0.01};
        for (double& coord : coords) {
            coord += delta(random);
        }

        // Sample point
        double const coord_resolution = 0.01;

        msgpack_buffer.clear();
        msgpack::packer packer{msgpack_buffer};

        packer.pack_array(static_cast<std::uint32_t>(coords.size()));
        prev_coords.resize(coords.size());

        for (std::size_t i = 0; i < coords.size(); i++) {
            int const x = int(std::round(coords[i] / coord_resolution));
            packer.pack(x - prev_coords[i]);
            prev_coords[i] = x;
        }

        // Insert into the database
        if (int err = sqlite3_reset(sample_point_stmt)) {
            throw std::runtime_error("sqlite3_reset: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_bind_int(sample_point_stmt, sample_point_step, step)) {
            throw std::runtime_error("sqlite3_bind_int: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_bind_double(sample_point_stmt, sample_point_time, time)) {
            throw std::runtime_error("sqlite3_bind_double: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_step(sample_point_stmt); err != SQLITE_DONE) {
            throw std::runtime_error("sqlite3_step: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_reset(structure_sample_stmt)) {
            throw std::runtime_error("sqlite3_reset: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_bind_int(structure_sample_stmt, structure_sample_step, step)) {
            throw std::runtime_error("sqlite3_bind_int: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_bind_blob(structure_sample_stmt, structure_sample_coords, msgpack_buffer.data(), int(msgpack_buffer.size()), nullptr)) {
            throw std::runtime_error("sqlite3_bind_blob: "s + sqlite3_errstr(err));
        }

        if (int err = sqlite3_step(structure_sample_stmt); err != SQLITE_DONE) {
            throw std::runtime_error("sqlite3_step: "s + sqlite3_errstr(err));
        }
    }
}


static void run()
{
    sqlite3_ptr trajectory_db;
    sqlite3* db;

    if (int err = sqlite3_open("trajectory.db", &db)) {
        throw std::runtime_error("sqlite3_open: "s + sqlite3_errstr(err));
    }
    trajectory_db.reset(db);

    create_tables(trajectory_db);
    store_trajectory(trajectory_db);
}


int main()
{
    try {
        run();
    } catch (std::exception const& ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
