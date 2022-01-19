#include <stdio.h>

int main()
{
	int x, y;

	scanf("%d", &x);
	printf("signed dec:   %d\n", x);
	printf("unsigned dec: %u\n", x);
	printf("hex:\t      %x\n", x);

	printf("binary:       ");
	int j = 4;
	for(int i = 31; i >= 0; i--)
	{
		y = (unsigned)x & (1 << i);

		if(y == 0)
			printf("0");
		else
			printf("1");
		j--;
		if(j == 0)
		{
			printf(" ");
			j = 4;
		}
	}

	printf("\n");

	return 0;
}

