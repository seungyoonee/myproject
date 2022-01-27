#include "gcd.h"

int gcd(int x, int y)
{
	int z;
	while(y != 0)
	{
		z = x;
		x = y;
		y = z % y;
	}
	return x;
}

