#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

//#ifdef BOOST_VECTOR
//    #include <boost/container/vector.hpp>
//    #include <boost/range/algorithm.hpp>
//    #include <boost/range/algorithm_ext.hpp>
//
//    template<typename T>
//    using vector = boost::container::vector<T>;
//
//#elif defined BOOST_VECTOR_WITH_ALOCATOR
//    #include <boost/container/Vector.hpp>
//    #include <boost/container/allocator.hpp>
//    #include <boost/range/algorithm.hpp>
//    #include <boost/range/algorithm_ext.hpp>
//    template<typename T>
//    using vector = boost::container::vector<T,boost::container::allocator<T>>;
//#elif defined STD_VECTOR
//    #include <vector>
//    #include <algorithm>
//    template<typename T>
//    using vector = std::vector<T>;
//#else
//    #error No vector chosen see Config.h
//#endif

#include <vector>
#include <algorithm>
template<typename T>
using vector = std::vector<T>;
#endif // VECTOR_H_INCLUDED
