#ifndef INCLUDED_SQLITE_SBUFFER_HPP
#define INCLUDED_SQLITE_SBUFFER_HPP

#include <msgpack.hpp>
#include <sqlite_orm.h>


namespace sqlite_orm
{
    template<>
    struct type_printer<msgpack::sbuffer> : blob_printer
    {
    };

    template<>
    struct statement_binder<msgpack::sbuffer>
    {
        int bind(sqlite3_stmt* stmt, int index, msgpack::sbuffer const& buf)
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
