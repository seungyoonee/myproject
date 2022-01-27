#include "prime.h"

int prime(int x)
{
	if (x == 1)
	{
		return 0;
	}
	else
	{
		for(int i = 2; i < x; i++)
		{
			if(x % i == 0)
				return 0;
		}
		return 1;
	}
}

