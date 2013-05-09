#pragma once
#include "precomp.h"

template <class T>
std::vector<std::shared_ptr<T> > * create_resource_cache()
{
    return new std::vector<std::shared_ptr<T> >();
}

template <class T>
inline std::shared_ptr<T> share(T* t)
{
    return std::shared_ptr<T>(t);
}
