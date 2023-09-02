#pragma once

#include "particle.h"
#include "vectors.h"
#include "configurations.h"

void THREADSpawnParticles2D(particle* parts, int segment, unsigned long* totalSpawned, configuration* config);
void THREADSpawnParticles3D(particle* parts, int segment, unsigned long* totalgenerated, configuration* config);