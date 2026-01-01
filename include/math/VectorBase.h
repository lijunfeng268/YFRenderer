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

template <class T>
Vector3T<T> operator-(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return Vector3T<T>{a.x - b.x, a.y - b.y, a.z - b.z};
}

template <class T>
Vector3T<T> operator+(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return Vector3T<T>{a.x + b.x, a.y + b.y, a.z + b.z};
}

template <class T>
Vector3T<T> operator*(const Vector3T<T>& a, float b)
{
	return Vector3T<T>{a.x * b, a.y * b, a.z * b};
}

// Vector2T 运算符
template <class T>
Vector2T<T> operator-(const Vector2T<T>& a, const Vector2T<T>& b)
{
	return Vector2T<T>{a.x - b.x, a.y - b.y};
}

template <class T>
Vector2T<T> operator+(const Vector2T<T>& a, const Vector2T<T>& b)
{
	return Vector2T<T>{a.x + b.x, a.y + b.y};
}

template <class T>
Vector2T<T> operator*(const Vector2T<T>& a, float b)
{
	return Vector2T<T>{static_cast<T>(a.x * b), static_cast<T>(a.y * b)};
}

template <class T>
Vector2T<T> operator*(float b, const Vector2T<T>& a)
{
	return Vector2T<T>{static_cast<T>(a.x * b), static_cast<T>(a.y * b)};
}
