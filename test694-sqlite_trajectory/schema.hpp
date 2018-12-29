#ifndef INCLUDED_SCHEMA_HPP
#define INCLUDED_SCHEMA_HPP

#include <string>

#include <msgpack.hpp>
#include <sqlite_orm.h>

#include "sqlite_sbuffer.hpp"
#include "sqlite_zbuffer.hpp"


struct sample_point
{
    int id;
    int step;
    double time;
};


struct structure_sample
{
    int id;
    int sample_id;
    msgpack::zbuffer coords_msgpack;
    double coords_multiplier;
};


inline auto make_storage(std::string const& path)
{
    using sqlite_orm::make_storage;
    using sqlite_orm::make_table;
    using sqlite_orm::make_column;
    using sqlite_orm::primary_key;
    using sqlite_orm::foreign_key;

    auto storage = make_storage(
        path,
        make_table(
            "sample_point",
            make_column("id", &sample_point::id, primary_key()),
            make_column("step", &sample_point::step),
            make_column("time", &sample_point::time)
        ),
        make_table(
            "structure_sample",
            make_column("id", &structure_sample::id, primary_key()),
            make_column("sample_id", &structure_sample::sample_id),
            make_column("coords_msgpack", &structure_sample::coords_msgpack),
            make_column("coords_multiplier", &structure_sample::coords_multiplier),
            foreign_key(&structure_sample::sample_id).references(&sample_point::id)
        )
    );
    storage.sync_schema();

    return storage;
}


using storage_type = decltype(make_storage(""));


#endif
