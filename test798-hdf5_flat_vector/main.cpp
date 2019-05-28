#include <array>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include <highfive/H5Attribute.hpp>
#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

#include "h5_shaper.hpp"


namespace H5 = HighFive;


int main()
{
    using std::size_t;

    H5::File store(
        "example.h5",
        H5::File::ReadWrite | H5::File::Create | H5::File::Truncate
    );

    const size_t rows = 70000;
    const size_t cols = 3;
    const size_t max_chunksize = 200'000;
    const size_t max_row_chunk = max_chunksize / (cols * sizeof(float));
    const size_t row_chunk = std::min(rows, max_row_chunk);

    // Original data is in float64.
    std::vector<double> data(rows * cols);
    std::mt19937_64 random_engine;
    std::uniform_real_distribution<double> dist(-10, 10);

    const double quantization_scale = 1 << 15;
    for (double& value : data) {
        value = dist(random_engine);
        value = std::nearbyint(value * quantization_scale) / quantization_scale;
    }

    H5::DataSpace dataspace(rows, cols);
    H5::DataSetCreateProps props;
    props.add(H5::Chunking(row_chunk, cols));
    props.add(H5::Shuffle());
    props.add(H5::Deflate(5));
    H5::DataSet dataset = store.createDataSet<float>("data", dataspace, props);

    dataset.write(shaper_2d(data.data(), rows, cols));
}
