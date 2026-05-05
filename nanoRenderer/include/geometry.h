#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

/**
 * @brief Fixed size vector with access and modify operations.
 * e.g. vec<3> for a vector of 3 elements
 */
template<int n> struct vec {
    double data[n] = {0};
    
    double& operator[] (const int i) 
    {
        assert (i >= 0 && i < n);
        return data[i];
    }

    double operator[] (const int i) const {
        assert (i >= 0 && i < n);
        return data[i];
    }
};

/**
 * @brief Insert vector into an ostream, then return the stream
 */
template<int n> std::ostream& operator<< (std::ostream& out, const vec<n>& v) {
    for (int i = 0; i < n; i++) out << v[i] << " ";
    return out;
}

/**
 * @brief Explicit specialization (template specialization) of the 
 * vec struct for when n is exactly 3
 */
   
template<> struct vec<3> {
    double x = 0, y = 0, z = 0;
    double& operator[] (const int i) 
    {
        assert (i >= 0 && i < 3);
        return i ? (i == 1 ? y : z) : x;
    }

    double operator[] (const int i) const {
        assert (i >= 0 && i < 3);
        return i ? (i == 1 ? y : z) : x;
    }
};

typedef vec<3> vec3;