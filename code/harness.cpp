#include "minHeap.cpp"
#include "harness.h"

graph*
createGraph(int v)
{
	graph *Graph = (graph*)malloc(sizeof(graph));
	Graph->v = v;
	Graph->array = (adjList*)malloc(v * sizeof(adjList));
	for (int i=0; i<v; ++i)
	{
		Graph->array[i].head = NULL;
	}
	return Graph;
}

void
addEdge(graph *Graph, int src, int dest, int weight)
{
	adjListNode *Node = newAdjListNode(dest, weight);
	Node->next = Graph->array[src].head;
	Graph->array[src].head = Node;

	//Graph is undirected - same as in wiring harness
	Node = newAdjListNode(src, weight);
	Node->next = Graph->array[dest].head;
	Graph->array[dest].head = Node;
}

void
printArr(int dist[], int n)
{
	printf("Vertex		Distance from Source\n");
	for (int i=0; i<n; ++i)
	{
		printf("%d \t\t %d\n", i, dist[i]);
	}
}

void
dijkstra(graph *Graph, int src)
{
	int v = Graph->v;
	int *dist = (int*)malloc(v * sizeof(int));

	minHeap *Heap = createMinHeap(v);

	for(int n=0; n<v; ++n)
	{
		dist[n] = INT_MAX;
		Heap->array[n] = newMinHeapNode(n, dist[n]);
		Heap->pos[n] = n;
	}

	Heap->array[src] = newMinHeapNode(src, dist[src]);
	Heap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(Heap, src, dist[src]);

	Heap->size = v;
	while (!isEmpty(Heap))
	{
		minHeapNode *Min = extractMin(Heap);
		int u = Min->v;

		adjListNode *Crawl = Graph->array[u].head;

		while(Crawl != NULL)
		{
			int vertex = Crawl->dest;

			if (
				isInMinHeap(Heap, v) && 
				dist[u] != INT_MAX &&
				Crawl->weight + dist[u] < dist[vertex])
			{
				dist[vertex] = dist[u] + Crawl->weight;
				decreaseKey(Heap, vertex, dist[vertex]);
			}
			Crawl = Crawl->next;
		}
	}
	printArr(dist, v);
}

int
main(int argc, char **args)
{
	/*
	int V = 9;
	graph *Graph = createGraph(V);
	addEdge(Graph, 0, 1, 4);
	addEdge(Graph, 0, 7, 8);
	addEdge(Graph, 1, 2, 8);
	addEdge(Graph, 1, 7, 11);
	addEdge(Graph, 2, 3, 7);
	addEdge(Graph, 2, 8, 2);
	addEdge(Graph, 2, 5, 4);
	addEdge(Graph, 3, 4, 9);
	addEdge(Graph, 3, 5, 14);
	addEdge(Graph, 4, 5, 10);
	addEdge(Graph, 5, 6, 2);
	addEdge(Graph, 6, 7, 1);
	addEdge(Graph, 6, 8, 6);
	addEdge(Graph, 7, 8, 7);

	dijkstra(Graph, 0);
	*/

	
	
	return 0;
}