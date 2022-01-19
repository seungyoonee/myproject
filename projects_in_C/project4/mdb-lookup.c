#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mdb.h"
#include "mylist.h"

static int lineNumber;

static void printMdb(void *p)
{
	struct MdbRec *temp = (struct MdbRec *)p;
	printf("%4d: {%s} said {%s}\n", lineNumber, temp->name, temp->msg);
	lineNumber++;
}

int main(int argc, char **argv)
{
	/* ****************************
	 *
	 *  open the database (my-mdb)
	 *
	 * ***************************/

	// check if the filename is provided by user
	if (argc != 2)
	{
		fprintf(stderr, "%s\n", "usage: mdb-lookup <database_file>");
		exit(1);
	}

	// open the given file
	char *filename = argv[1];
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror(filename);
		exit(1);
	}

	/* ******************************************************
	 *
	 * read the database in a loop calling fread()
	 * put the Nodes in a linked list using Lab3 addAfter()
	 *
	 * *****************************************************/

	// initialize a lined list
	struct List list;
	initList(&list);

	// initialize a MdbRec and Node
	struct MdbRec rec;
	struct Node *node = NULL;

	// read the records one by one and put them into the linked list
	while(fread(&rec, sizeof(rec), 1, fp) == 1)
	{
		// allocate a MdbRec (40 bytes) and pointer to it
		struct MdbRec *recPtr = malloc(sizeof(struct MdbRec));
		if(!recPtr)
		{
			perror("malloc returned NULL");
			exit(1);
		}

		*recPtr = rec; // what recPtr is pointing to is the data in rec
		node = addAfter(&list, node, recPtr); // node->data points to what recPtr is pointing to
	}

	/* ******************************************
	 *
	 * lookup loop using fgets() - up to 5 chars
	 *
	 * ******************************************/

	int truncation = 5;
	char buf[100];
	
	printf("lookup: ");
	while(fgets(buf, sizeof(buf), stdin) != NULL)
	{
		lineNumber = 1;

		if(buf[0] == '\n')
		{
			traverseList(&list, &printMdb);
		}
		else
		{
			char target[5];
			struct Node *temp = list.head;
			struct MdbRec *targetRec = (struct MdbRec *)temp->data;
			strncpy(target, buf, truncation); // copy buf to target
			// if buf has less than 5 chars
			if(strlen(buf) <= truncation)
			{
				target[strlen(buf) - 1] = '\0';
			}
			// if buf has more than 5 chars
			else
			{
				target[truncation] = '\0';
			}
			// print out what's found
			while(temp)
			{
				// if found either among name or msg, print that record
				if(strstr(targetRec->name, target) || strstr(targetRec->msg, target))
				{
					printMdb(targetRec);
				}
				// if not found, skip the record
				else
					lineNumber++;
				temp = temp->next;
				if(temp != NULL)
					targetRec = (struct MdbRec *)temp->data;
			}
		}
		printf("\nlookup: ");
	}

	// remove all the nodes, close the file, and quit
	struct Node *toFree = list.head;
	while(toFree)
	{
		free(toFree->data);
		toFree = toFree->next;
	}
	removeAllNodes(&list);
	fclose(fp);
	return 0;
}

