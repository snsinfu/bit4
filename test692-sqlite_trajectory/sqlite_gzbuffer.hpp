#ifndef INCLUDED_SQLITE_GZBUFFER_HPP
#define INCLUDED_SQLITE_GZBUFFER_HPP

#include <msgpack.hpp>
#include <sqlite_orm.h>

#include "gzbuffer.hpp"


namespace sqlite_orm
{
    template<>
    struct type_printer<schema::gzbuffer> : public blob_printer
    {
    };

    template<>
    struct statement_binder<schema::gzbuffer>
    {
        int bind(sqlite3_stmt* stmt, int index, schema::gzbuffer const& value)
        {
            auto data = value.data();
            auto size = static_cast<int>(value.size());
            return sqlite3_bind_blob(stmt, index, data, size, SQLITE_TRANSIENT);
        }
    };
}

#endif
