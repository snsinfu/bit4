
template<template<typename> class Map>
struct container
{
    template<typename T>
    Map<T> map()
    {
        return {};
    }
};

template<typename T>
using map = T const;

int main()
{
    container<map> c;
    c.map<int>();
}
