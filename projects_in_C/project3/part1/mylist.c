#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"

/*
 * Create a node that holds the given data pointer,
 * and add the node to the front of the list.
 *
 * It returns the newly created node on success and NULL on failure.
 */
struct Node *addFront(struct List *list, void *data)
{
	struct Node *node = malloc(sizeof(struct Node));
	if (node == NULL)
		return NULL;

    node->data = data;
    node->next = list->head;
    list->head = node;

    return node;
}

/*
 * Traverse the list, calling f() with each data item.
 */
void traverseList(struct List *list, void (*f)(void *))
{
	struct Node *node = list->head;

	while(node)
	{
		f(node->data);
		node = node->next;
	}
}

/*
 * Traverse the list, comparing each data item with 'dataSought' using
 * 'compar' function.  ('compar' returns 0 if the data pointed to by
 * the two parameters are equal, non-zero value otherwise.)
 *
 * Returns the first node containing the matching data, 
 * NULL if not found.
 */
struct Node *findNode(struct List *list, const void *dataSought,
	int (*compar)(const void *, const void *))
{
	struct Node *target = list->head;
	
	int n = 0;
	while(target->next)
	{
		n++;
		target = target->next;
		if(target->next == 0)
			n++;
	}

	target = list->head;
	for(int i = 0; i < n; i++)
	{
		if((*compar)(target->data, dataSought) == 0)
		{
			return target;
		}
		target = target->next;
	}

	return NULL;
}

/*
 * Flip the sign of the double value pointed to by 'data' by
 * multiplying -1 to it and putting the result back into the memory
 * location.
 */
void flipSignDouble(void *data)
{
	*(double *)data *= -1;
}

/*
 * Compare two double values pointed to by the two pointers.
 * Return 0 if they are the same value, 1 otherwise.
 */
int compareDouble(const void *data1, const void *data2)
{
	if(*(double *)data1 == *(double *)data2)
		return 0;
	else
		return 1;
}

/*
 * Remove the first node from the list, deallocate the memory for the
 * ndoe, and return the 'data' pointer that was stored in the node.
 * Returns NULL is the list is empty.
 */
void *popFront(struct List *list)
{
	if(isEmptyList(list))
		return NULL;

	struct Node *target = list->head;
	void *popped = target->data;

	if(!target)
	{
		list->head = 0;
		free(target);
	}
	else
	{
		list->head = target->next;
		free(target);
	}

	return popped;
}
/*
 * Remove all nodes from the list, deallocating the memory for the
 * nodes.
 */
void removeAllNodes(struct List *list)
{	
    while (!isEmptyList(list))
	popFront(list);
}

/*
 * Create a node that holds the given data pointer,
 * and add the node right after the node passed in as the 'prevNode'
 * parameter.  If 'prevNode' is NULL, this function is equivalent to
 * addFront().
 *
 * It returns the newly created node on success and NULL on failure.
 */
struct Node *addAfter(struct List *list, 
	struct Node *prevNode, void *data)
{
	if (prevNode == NULL)
		return addFront(list, data);

    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL)
		return NULL;

    node->data = data;
    node->next = prevNode->next;
    prevNode->next = node;
    return node;
}

/* 
 * Reverse the list.
 */
void reverseList(struct List *list)
{
	struct Node *prv = NULL;
	struct Node *cur = list->head;
	struct Node *nxt;

	while (cur)
	{
		nxt = cur->next;
		cur->next = prv;
		prv = cur;
		cur = nxt;
	}
	list->head = prv;
}

