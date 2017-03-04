#ifndef AJIOB_RANDOM

#define AJIOB_RANDOM

#include <ctime>
#include <cstdlib>

inline long long int random()
{
	static bool isFirst = false;

	if (!isFirst)
	{
		srand(time(NULL));
	}

	return rand();
}
	
#endif