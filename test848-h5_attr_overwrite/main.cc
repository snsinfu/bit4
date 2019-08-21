#include <iostream>
#include <string>
#include <vector>

#include <highfive/H5File.hpp>

namespace H5 = HighFive;


// Can't overwrite attribute with growing array of strings; the shape of the
// array must be fixed. So let's use plain string for storing a growing list.
// Here the list is encoded as a space-separated text. In general JSON would
// be a good choice.
//
// Another possibility: Create list in-memory and write attribute on exit.

int main()
{
    H5::File store{"test.h5", H5::File::ReadWrite | H5::File::Create | H5::File::Truncate};

    std::string sample_order;
    auto samples_group = store.createGroup("samples");
    auto sample_order_attr = samples_group.createAttribute("sample_order", sample_order);

    for (int i = 0; i <= 10; i++) {
        const auto sample_name = std::to_string(i * 10);
        samples_group.createGroup(sample_name);

        if (!sample_order.empty()) {
            sample_order += ' ';
        }
        sample_order += sample_name;
        sample_order_attr.write(sample_order);

        store.flush();
    }
}
