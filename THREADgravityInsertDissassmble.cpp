#include "particle.h"
#include "quadNode.h"
#include "octNode.h"

void THREADGravity2D(quadNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, float thresholdTheta, double softening, double G)
{

	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
		root->searchForNodeThatSatisfysThreshold(&parts[i], thresholdTheta, softening, G);

	return;
}
void THREADGravity3D(octNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, float thresholdTheta, double softening, double G)
{

	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
		root->searchForNodeThatSatisfysThreshold(&parts[i], thresholdTheta, softening, G);

	return;
}

void THREADInsert2D(quadNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, unsigned int maxTreeDepth)
{
	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
		root->insert(&parts[i], maxTreeDepth);
	return;
}
void THREADInsert3D(octNode* root, particle* parts, int segment, unsigned long quantity, unsigned int multiThreadCount, unsigned int maxTreeDepth)
{
	for (int i = quantity / multiThreadCount * segment; i < quantity / multiThreadCount * (segment + 1); i++)
		root->insert(&parts[i], maxTreeDepth);
	return;
}

void dissassembly(quadNode* root)
{
	root->disassemble();
	delete root;
	return;
}