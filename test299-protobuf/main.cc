#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>

#include "profile.pb.h"

using perftools::profiles::Profile;

namespace
{
    Profile create_profile()
    {
        Profile profile;

        // string_table[0] must be ""
        profile.add_string_table("");

        auto const function_id = profile.function_size() + 1;
        auto& function = *profile.add_function();
        {
            auto const human_name_id = profile.string_table_size();
            profile.add_string_table("foo::bar(int)");

            auto const mangled_name_id = profile.string_table_size();
            profile.add_string_table("__Z3foo3barEi");

            auto const filename_id = profile.string_table_size();
            profile.add_string_table("foo.cc");
            auto const start_line = 123;

            function.set_id(function_id);
            function.set_name(human_name_id);
            function.set_system_name(mangled_name_id);
            function.set_filename(filename_id);
            function.set_start_line(start_line);
        }

        auto& sample_type = *profile.add_sample_type();
        {
            auto const type_id = profile.string_table_size();
            profile.add_string_table("cpu");

            auto const unit_id = profile.string_table_size();
            profile.add_string_table("nanosecond");

            sample_type.set_type(type_id);
            sample_type.set_unit(unit_id);
        }

        auto const mapping_id = profile.mapping_size() + 1;
        auto& mapping = *profile.add_mapping();
        {
            auto const filename_id = profile.string_table_size();
            profile.add_string_table("foo");

            mapping.set_id(mapping_id);
            mapping.set_memory_start(0x80000000);
            mapping.set_memory_limit(0x82000000);
            mapping.set_file_offset(0x1000);
            mapping.set_filename(filename_id);
            mapping.set_has_functions(true);
            mapping.set_has_filenames(true);
            mapping.set_has_line_numbers(true);
        }

        auto& sample = *profile.add_sample();
        {
            auto const location_id = profile.location_size() + 1;
            auto& location = *profile.add_location();
            location.set_id(location_id);
            location.set_mapping_id(mapping_id);
            location.set_address(0x80001000);

            auto& line = *location.add_line();
            line.set_line(135);
            line.set_function_id(function_id);

            sample.add_location_id(location_id);
            sample.add_value(10);
        }

        return profile;
    }

    std::error_condition save_profile(Profile const& prof, std::string const& name)
    {
        std::ofstream output{name, std::ios::binary};

        if (!prof.SerializeToOstream(&output)) {
            return std::errc::io_error;
        }

        return {};
    }
}

int main(int argc, char** argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2) {
        std::cerr << "Specify output file name\n";
        return EXIT_FAILURE;
    }
    std::string const output_name = argv[1];

    auto prof = create_profile();
    if (auto err = save_profile(prof, output_name)) {
        std::cerr << "error: " << err.message() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
