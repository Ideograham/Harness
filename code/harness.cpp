#include "minHeap.cpp"
#include "harness.h"
#include "csv.cpp"

graph*
createGraph(int v)
{
	graph *Graph = (graph*)malloc(sizeof(graph));
	Graph->vertexCount = v;
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
printPath(int path[], int n, int src, int v)
{
	//int stack[n];
	int *stack = (int*)malloc(n * sizeof(int));
	int sp = 0;
	stack[sp++] = v;

	printf("Path: ");
	while (v < n )
	{
		//printf("%d ", path[v]);
		stack[sp++] = path[v];
		v = path[v];
		if (v == src)
			break;
	}
	while(sp > 0)
	{
		printf("%d ", stack[--sp]);
	}
	free(stack);
}


void
printArr(int dist[], int path[], int n, int src)
{
	printf("Vertex		Distance from Source\n");
	for (int i=0; i<n; ++i)
	{
		printf("%d \t\t %d ", i, dist[i]);
		if (path)
		{
			printPath(path, n, src, i);
		}
		printf("\n");
	}
}

void
dijkstra(graph *Graph, int src)
{
	int v = Graph->vertexCount;
	int *dist = (int*)malloc(v * sizeof(int));
	int *path = (int*)malloc(v * sizeof(int));

	minHeap *Heap = createMinHeap(v);

	for(int n=0; n<v; ++n)
	{
		dist[n] = INT_MAX;
		path[n] = -1;
		Heap->array[n] = newMinHeapNode(n, dist[n]);
		Heap->pos[n] = n;
	}

	Heap->array[src] = newMinHeapNode(src, dist[src]);
	Heap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(Heap, src, dist[src]);
	path[src] = src;

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
				isInMinHeap(Heap, vertex) && 
				dist[u] != INT_MAX &&
				Crawl->weight + dist[u] < dist[vertex])
			{
				dist[vertex] = dist[u] + Crawl->weight;
				//printf("The shortest path to %d now goes through %d\n", vertex, u);
				path[vertex] = u;
				decreaseKey(Heap, vertex, dist[vertex]);
			}
			Crawl = Crawl->next;
		}
	}
	printArr(dist, path, v, src);	
}

int
main(int argc, char **args)
{

	gr *G = allocateGraph();

	char *vertexCSV = processCSVVerticies(G, "Verticies.csv");
	char *edgeCSV = processCSVEdges(G, "Edges.csv");
	char *wireCSV = processCSVWires(G, "Wires.csv");

	
	graph *Graph = createGraph(G->VertexCount);
	for (int i=0; i<G->EdgeCount; ++i)
	{
		edge *E = G->Edges[i];
		addEdge(Graph, E->From->Id, E->To->Id, E->Len);
	}

	for (int i=0; i<G->WireCount; ++i)
	{
		wire *W = G->Wires[i];
		printf("WIRE %.*s:\t\tEndId:%d:::\n", (int)W->NameTok->TextLength, W->NameTok->Text, W->EndVertex->Id);
		dijkstra(Graph, W->StartVertex->Id);
		printf("\n");
	}

	/*
	graph *Graph = createGraph(9)
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
	*/

	//dijkstra(Graph, 0);

	return 0;
}