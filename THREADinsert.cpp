#include "THREADinsert.h"

extern const unsigned long QUANTITY;
extern const int MULTITHREADCOUNT;

template<typename T>
void THREADInsert(T* root, particle* parts, int segment)
{
	for (int i = QUANTITY / MULTITHREADCOUNT * segment; i < QUANTITY / MULTITHREADCOUNT * (segment + 1); i++)
		root->insert(&parts[i]);
	return;
}