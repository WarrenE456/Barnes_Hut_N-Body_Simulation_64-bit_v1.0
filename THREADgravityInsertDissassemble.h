#pragma once

#include "particle.h"
#include "quadNode.h"

void THREADGravity2D(quadNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, float thresholdTheta, double softening, double G);
void THREADGravity3D(octNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, float thresholdTheta, double softening, double G);
void THREADInsert2D(quadNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, unsigned int maxTreeDepth);
void THREADInsert3D(octNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, unsigned int maxTreeDepth);
void dissassembly(quadNode* root);