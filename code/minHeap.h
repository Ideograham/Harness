#include "ig_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct minHeapNode
{
	int v;
	int dist;
};

struct minHeap
{
	int size;
	int capacity;
	int *pos;
	minHeapNode **array;
};

struct adjListNode
{
	int dest;
	int weight;
	adjListNode *next;
};

struct adjList
{
	adjListNode *head;
};

struct graph
{
	int v;
	adjList *array;
};