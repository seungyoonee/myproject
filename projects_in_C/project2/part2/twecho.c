#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char **duplicateArgs(int argc, char **argv)
{
	// allocating an array of pointer
	char **a;
	a = malloc(sizeof(char *) * (argc + 1));
	
	// check if malloc successfully worked
	if(a == NULL)
	{
		perror("malloc returned NULL");
		exit(1);
	}

	// copying the addresses of strings to allocated pointers
	for(int i = 0; i <= argc; i++)
	{
		a[i] = argv[i];
	}

	// allocating the arrays of strings and copy strings into the arrays
	for(int i = 0; i < argc; i++)
	{
		char *c;
		c = malloc(sizeof(char) * (strlen(argv[i]) + 1));

		if(c == NULL)
		{
			perror("malloc returned NULL");
			exit(1);
		}

		a[i] = c;
		strcpy(a[i], argv[i]);
	}

	// capitalizing all the characters
	for(int i = 1; i < argc; i++)
	{
		for(int j = 0; j <= strlen(argv[i]); j++)
		{
			a[i][j] = toupper(a[i][j]);
		}
	}

	return a;
}

static void freeDuplicatedArgs(char **copy)
{
	char **a;
	a = copy;

	// freeing allocated heap memories
	while(*copy)
	{
		free(*copy++);
	}
	free(a);
}

int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;

    char **copy = duplicateArgs(argc, argv);
    char **p = copy;

    argv++;
    p++;
    while (*argv) {
        printf("%s %s\n", *argv++, *p++);
    }

    freeDuplicatedArgs(copy);

    return 0;
}

