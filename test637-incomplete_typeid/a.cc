#include "resource.hpp"
#include "typemap.hpp"

typemap<int>& get_typemap();

void set_from_a(int val)
{
    get_typemap().set<incomplete_key>(val);
}
