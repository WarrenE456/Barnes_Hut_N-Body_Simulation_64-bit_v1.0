#pragma once

template <typename T>
class vec2
{
public:
	T x;
	T y;

	vec2() : x(0), y(0) {}

	vec2(T x, T y) : x(x), y(y) {}

	T& operator [] (int i)
	{
		if (i == 0)
		{
			return x;
		}
		if (i == 1)
		{ 
			return y;
		}
	}
};

template <typename T>
class vec3
{
public:
	T x;
	T y;
	T z;

	vec3() : x(0), y(0), z(0) {}

	vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	operator vec2<T>() const { return vec2<T>(x, y); };

	T& operator[](int i)
	{
		if (i == 0)
		{
			return x;
		}
		if (i == 1)
		{
			return y;
		}
		if (i == 2)
		{
			return z;
		}
	}
};