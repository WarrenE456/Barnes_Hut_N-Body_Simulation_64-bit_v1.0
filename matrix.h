#pragma once

#include "vectors.h"

template<typename T>
class matrix3x3
{
public:
	T data[3][3];

	matrix3x3
	(T y0x0, T y0x1, T y0x2
   , T y1x0, T y1x1, T y1x2
   , T y2x0, T y2x1, T y2x2)
		:data{
		{y0x0, y0x1, y0x2},
		{y1x0, y1x1, y1x2},
		{y2x0, y2x1, y2x2}
		} {}

	matrix3x3()
		:data{
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
		} {}

	vec3<T> operator*(vec3<T> vector)
	{
		return
		{
			vector.x * data[0][0] + vector.y * data[0][1] + vector.z * data[0][2],
			vector.x * data[1][0] + vector.y * data[1][1] + vector.z * data[1][2],
			vector.x * data[2][0] + vector.y * data[2][1] + vector.z * data[2][2]
		};
	}

	matrix3x3<T> operator*(matrix3x3<T>& matrix)
	{
		return matrix3x3<T>(
		matrix.data[0][0] * data[0][0] + matrix.data[1][0] * data[0][1] + matrix.data[2][0] * data[0][2], matrix.data[0][1] * data[0][0] + matrix.data[1][1] * data[0][1] + matrix.data[2][1] * data[0][2], matrix.data[0][2] * data[0][0] + matrix.data[1][2] * data[0][1] + matrix.data[2][2] * data[0][2],
		matrix.data[0][0] * data[1][0] + matrix.data[1][0] * data[1][1] + matrix.data[2][0] * data[1][2], matrix.data[0][1] * data[1][0] + matrix.data[1][1] * data[1][1] + matrix.data[2][1] * data[1][2], matrix.data[0][2] * data[1][0] + matrix.data[1][2] * data[1][1] + matrix.data[2][2] * data[1][2],
		matrix.data[0][0] * data[2][0] + matrix.data[1][0] * data[2][1] + matrix.data[2][0] * data[2][2], matrix.data[0][1] * data[2][0] + matrix.data[1][1] * data[2][1] + matrix.data[2][1] * data[2][2], matrix.data[0][2] * data[2][0] + matrix.data[1][2] * data[2][1] + matrix.data[2][2] * data[2][2]
		);
	}
};