#include "resource.hpp"
#include "typemap.hpp"

typemap<int>& get_typemap();

int get_from_b()
{
    return get_typemap().get<incomplete_key>();
}
