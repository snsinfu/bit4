#include <array>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include <highfive/H5Attribute.hpp>
#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

#include "h5_vector_of_arrays.hpp"


namespace H5 = HighFive;

using std::size_t;


int main()
{
    // Store values to HDF5 file in this type. With shuffle-compression and
    // quantization the storage cost of float64 is almost identical to that
    // of float32. Memory cost of loader is doubled though.
    using float_type = float;

    H5::File store(
        "example.h5",
        H5::File::ReadWrite | H5::File::Create | H5::File::Truncate
    );
    H5::Group steps_group = store.createGroup("steps");

    const size_t rows = 70000;
    const size_t cols = 3;
    const size_t max_chunksize = 200'000;
    const size_t max_row_chunk = max_chunksize / (cols * sizeof(float_type));
    const size_t row_chunk = std::min(rows, max_row_chunk);

    H5::DataSpace dataspace(rows, cols);
    H5::DataSetCreateProps props;
    props.add(H5::Chunking(row_chunk, cols));
    props.add(H5::Shuffle());
    props.add(H5::Deflate(5));

    // Original data is in float64.
    std::vector<std::array<double, cols>> data(rows);
    std::mt19937_64 random_engine;
    std::uniform_real_distribution<double> dist(-10, 10);
    const double quant_unit = 1 << 15;

    for (int step = 1; step <= 10; step++) {
        // Store as float_type (float32 or float64). Looks like conversion is
        // done by the library.
        const std::string name = std::to_string(step);
        H5::DataSet dataset = steps_group.createDataSet<float_type>(
            name, dataspace, props
        );

        for (auto& row : data) {
            row = {dist(random_engine), dist(random_engine), dist(random_engine)};

            for (auto& value : row) {
                value = std::nearbyint(value * quant_unit) / quant_unit;
            }
        }

        dataset.write(data);
    }
}
