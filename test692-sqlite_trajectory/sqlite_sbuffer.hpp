#ifndef INCLUDED_SQLITE_SBUFFER_HPP
#define INCLUDED_SQLITE_SBUFFER_HPP

#include <msgpack.hpp>
#include <sqlite_orm.h>


namespace sqlite_orm
{
    template<>
    struct type_printer<msgpack::sbuffer> : public blob_printer
    {
    };

    template<>
    struct statement_binder<msgpack::sbuffer>
    {
        int bind(sqlite3_stmt* stmt, int index, msgpack::sbuffer const& value)
        {
            auto data = value.data();
            auto size = static_cast<int>(value.size());
            return sqlite3_bind_blob(stmt, index, data, size, SQLITE_TRANSIENT);
        }
    };
}

#endif
