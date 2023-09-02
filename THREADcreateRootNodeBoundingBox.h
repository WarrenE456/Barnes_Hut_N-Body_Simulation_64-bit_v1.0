#pragma once

#include "particle.h"
#include "quadNode.h"
#include "octNode.h"

void THREADCreateRootNodeBoundingBox2D(particle* parts, int segment, boundary2D* bounds, unsigned long quantity, unsigned int multiThreadCount);
void THREADCreateRootNodeBoundingBox3D(particle* parts, int segment, boundary3D* bounds, unsigned long quantity, unsigned int multiThreadCount);