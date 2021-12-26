#include <stdio.h>
#include "gcd.h"
#include "prime.h"

int main()
{
	int x, y;

	printf("\nGive me the first number: ");
	scanf("%d", &x);

	printf("Give me the second number: ");
	scanf("%d", &y);

	printf("\nYou typed in %d and %d.\n", x, y);

	float avg = (float)(x + y) / 2;

	printf("The average is: %.2f\n", avg);

	if(prime(x) == 0)
		printf("\n%d is not a prime number!\n", x);
	else
		printf("\n%d is a prime number!\n", x);

	if(prime(y) == 0)
		printf("%d is not a prime number!\n", y);
	else
		printf("%d is a prime number!\n", y);

	if(gcd(x, y) == 1)
		printf("%d and %d are relatively prime.\n\n", x, y);
	else
		printf("%d and %d are not relatively prime.\n\n", x, y);

	return 0;
}

