#pragma once

#include"DataType.h"

template<class T>
struct Vector3T
{
    T x;
    T y;
    T z;
};

template<class T>
struct Vector2T
{
    T x;
    T y;
};

typedef Vector3T<real> Vector3;
typedef Vector3T<Int32> Vector3i;

typedef Vector2T<real> Vector2;
typedef Vector2T<Int32> Vector2i;