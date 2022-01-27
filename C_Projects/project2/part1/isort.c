#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sortAsc(const void *v1, const void *v2)
{
	int x = *(int *)v1;
	int y = *(int *)v2;
	if(x < y)
		return -1;
	else if(x > y)
		return 1;
	else return 0;
}

int sortDsc(const void *v1, const void *v2)
{
	int x = *(int *)v1;
	int y = *(int *)v2;
	if(x > y)
		return -1;
	else if(x < y)
		return 1;
	else return 0;
}

void sort_integer_array(int *begin, int *end, int ascending)
{
	int arrSize = end - begin;

	if(ascending == 1)
	{
		qsort(begin, arrSize, sizeof(int), &sortAsc);
	}
	else
	{
		qsort(begin, arrSize, sizeof(int), &sortDsc);
	}
}

int *newArray(int arrSize)
{
	int *a;
	a = malloc(sizeof(int) * arrSize);

	if(a == NULL)
	{
		perror("malloc returned NULL");
		exit(1);
	}

	return a;
}

int main()
{
	int arrSize;

	srandom(time(NULL));

	printf("Enter the array size: ");
	scanf("%d", &arrSize);

	int *org = newArray(arrSize);

	for(int i = 0; i < arrSize; i++)
	{
		org[i] = random() % 100;
	}

	// a copy the array, and sort the new array in ascending order
	int *asc = newArray(arrSize);

	for(int i = 0; i < arrSize; i++)
	{
		asc[i] = org[i];
	}

	int *ascEnd = asc;

	for(int i = 0; i < arrSize; i++)
	{
		ascEnd++;
	}
	
	sort_integer_array(asc, ascEnd, 1);	

	// make a second copy of the original array, and sort it in descending order
	int *dsc = newArray(arrSize);

	for(int i = 0; i < arrSize; i++)
	{
		dsc[i] = org[i];
	}

	int *dscEnd = dsc;

	for(int i = 0; i < arrSize; i++)
	{
		dscEnd++;
	}

	sort_integer_array(dsc, dscEnd, 0);

	// print out all three arrays: original, ascending, and descending
	// printing original
	printf("\noriginal: ");
	for (int i = 0; i < arrSize; i++)
	{
		printf("%d ", org[i]);
	}
	printf("\n");

	// printing ascending
	printf("ascending: ");
	for (int i = 0; i < arrSize; i++)
	{
		printf("%d ", asc[i]);
	}
	printf("\n");

	// printing descending
	printf("descending: ");
	for (int i = 0; i < arrSize; i++)
	{
		printf("%d ", dsc[i]);
	}
	printf("\n");

	// call free() to deallocate the arrays
	free(org);
	free(asc);
	free(dsc);

	return 0;
}

