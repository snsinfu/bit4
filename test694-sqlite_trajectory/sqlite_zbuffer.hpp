#ifndef INCLUDED_SQLITE_ZBUFFER_HPP
#define INCLUDED_SQLITE_ZBUFFER_HPP

#include <msgpack.hpp>
#include <msgpack/zbuffer.hpp>
#include <sqlite_orm.h>


namespace sqlite_orm
{
    template<>
    struct type_printer<msgpack::zbuffer> : blob_printer
    {
    };

    template<>
    struct statement_binder<msgpack::zbuffer>
    {
        int bind(sqlite3_stmt* stmt, int index, msgpack::zbuffer const& buf)
        {
            return sqlite3_bind_blob(
                stmt,
                index,
                buf.data(),
                static_cast<int>(buf.size()),
                SQLITE_TRANSIENT
            );
        }
    };
}

#endif
