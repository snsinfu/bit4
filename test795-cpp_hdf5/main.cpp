#include <array>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

#include "h5_vector_of_arrays.hpp"


namespace H5 = HighFive;

using std::size_t;


int main()
{
    H5::File store(
        "example.h5", H5::File::ReadWrite | H5::File::Create | H5::File::Truncate
    );
    store.createAttribute<std::string>("PYTABLES_FORMAT_VERSION", "2.1");
    store.createAttribute<std::string>("CLASS", "GROUP");
    store.createAttribute<std::string>("VERSION", "1.0");
    store.createAttribute<std::string>("TITLE", "");

    H5::Group steps_group = store.createGroup("steps");
    steps_group.createAttribute<std::string>("CLASS", "GROUP");
    steps_group.createAttribute<std::string>("VERSION", "1.0");
    steps_group.createAttribute<std::string>("TITLE", "");

    const size_t rows = 70000;
    const size_t cols = 3;
    const size_t max_chunksize = 200'000;
    const size_t max_row_chunk = max_chunksize / (cols * sizeof(float));
    const size_t row_chunk = std::min(rows, max_row_chunk);

    H5::DataSetCreateProps props;
    props.add(H5::Chunking(row_chunk, cols));
    props.add(H5::Shuffle());
    props.add(H5::Deflate(5));
    H5::DataSpace dataspace(rows, cols);

    std::vector<std::array<float, cols>> data(rows);
    std::mt19937_64 random_engine;
    std::uniform_real_distribution<float> dist(-10, 10);
    const float quant_unit = 1 << 15;

    for (int step = 1; step <= 10; step++) {
        const std::string name = std::to_string(step);
        H5::DataSet dataset = steps_group.createDataSet<float>(name, dataspace, props);
        dataset.createAttribute<std::string>("CLASS", "CARRAY");
        dataset.createAttribute<std::string>("VERSION", "1.1");
        dataset.createAttribute<std::string>("TITLE", "");

        for (auto& row : data) {
            row = {dist(random_engine), dist(random_engine), dist(random_engine)};

            for (auto& value : row) {
                value = std::nearbyint(value * quant_unit) / quant_unit;
            }
        }

        dataset.write(data);
    }
}
