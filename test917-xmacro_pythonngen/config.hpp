#pragma once

#include <string>


struct config
{
#define X(var, T, value)    T var = value;
#include "config_members.hpp"
#undef X
};


template<typename F>
void foreach(config& c, F func)
{
#define X(var, T, value)    func(#var, c.var);
#include "config_members.hpp"
#undef X
}
