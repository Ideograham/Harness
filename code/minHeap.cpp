#include "minHeap.h"


struct adjListNode*
newAdjListNode(int dest, int weight)
{
	adjListNode *node =
		(adjListNode*)malloc(sizeof(adjListNode));

	node->dest = dest;
	node->weight = weight;
	node->next = NULL;
	return node;
}


minHeapNode*
newMinHeapNode(int v, int dist)
{
	minHeapNode *Node = (minHeapNode*)malloc(sizeof(minHeapNode));
	Node->v = v;
	Node->dist = dist;
	return Node;
}

minHeap*
createMinHeap(int capacity)
{
	minHeap *Heap = (minHeap*)malloc(sizeof(minHeap));
	Heap->pos = (int*)malloc(capacity * sizeof(int));
	Heap->size = 0;
	Heap->capacity = capacity;
	Heap->array = (minHeapNode**) malloc(capacity * sizeof(minHeapNode*));
	return Heap;
}

void
swapMinHeapNode(minHeapNode **a, minHeapNode **b)
{
	minHeapNode *t = *a;
	*a = *b;
	*b = t;
}

void
minHeapify(minHeap *Heap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2*idx + 2;

	if (left < Heap->size &&
		Heap->array[left]->dist < Heap->array[smallest]->dist)
	{
		smallest = left;
	}

	if (right < Heap->size &&
		Heap->array[right]->dist < Heap->array[smallest]->dist)
	{
		smallest = right;
	}

	if (smallest != idx)
	{
		minHeapNode *SmallNode = Heap->array[smallest];
		minHeapNode *IdxNode = Heap->array[idx];

		// swap positions
		Heap->pos[SmallNode->v] = idx;
		Heap->pos[IdxNode->v] = smallest;

		//swap nodes
		swapMinHeapNode(&Heap->array[smallest], &Heap->array[idx]);

		minHeapify(Heap, smallest);
	}
}

int
isEmpty(minHeap *Heap)
{
	return Heap->size == 0;
}


minHeapNode*
extractMin(minHeap *Heap)
{
	if(isEmpty(Heap))
		return NULL;

	minHeapNode *Root = Heap->array[0];

	minHeapNode *Last = Heap->array[Heap->size -1];
	Heap->array[0] = Last;

	Heap->pos[Root->v] = Heap->size-1;
	Heap->pos[Last->v] = 0;

	--Heap->size;
	minHeapify(Heap, 0);

	return Root;
}

void
decreaseKey(minHeap *Heap, int v, int dist)
{
	int i = Heap->pos[v];

	Heap->array[i]->dist = dist;

	while (i && Heap->array[i]->dist < Heap->array[(i-1) /2 ]->dist)
	{
		Heap->pos[Heap->array[i]->v] = (i-1)/2;
		Heap->pos[Heap->array[(i-1)/2]->v] = i;
		swapMinHeapNode(&Heap->array[i], &Heap->array[(i-1)/2]);
		i = (i-1)/2;
	}
}

bool
isInMinHeap(minHeap *Heap, int v)
{
	if(Heap->pos[v] < Heap->size)
	{
		return true;
	}
	return false;
}

/*int
main(int argc, char **args)
{
	return 0;
}
*/
