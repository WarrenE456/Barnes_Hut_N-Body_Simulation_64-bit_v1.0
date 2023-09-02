#pragma once

enum velocitymodeindex : unsigned char
{
	spiral = 0,
	outward = 1,
	directionx = 2,
	directiony = 3,
	directionz = 4
};

enum spawnOptions2D : unsigned char
{
	square = 0,
	circleCentral = 1,
	circleEven = 2,
};

enum spawnOptions3D : unsigned char
{
	cuboid = 0,
	diskCentral = 1,
	diskEven = 2,
};