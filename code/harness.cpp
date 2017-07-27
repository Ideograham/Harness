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
}

connection*
dijkstra(graph *Graph, gr *G, int src, int dst)
{
	connection *C = NULL;
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
		if (u == dst)
		{
			// printf("Found shortest path from %.*s to %.*s as %d\n",
			// 	(int)G->Verticies[src]->NameTok->TextLength, G->Verticies[src]->NameTok->Text,
			// 	(int)G->Verticies[dst]->NameTok->TextLength, G->Verticies[dst]->NameTok->Text,
			// 	dist[dst]);
			
			int *stack = (int*)malloc(v * sizeof(int));
			int sp = 0;
			stack[sp++] = u;

			//printf("Path: ");
			while (u < v )
			{
				stack[sp++] = path[u];
				u = path[u];
				if (u == src)
					break;
			}
			if (!C)
			{
				C = (connection*)malloc(sizeof(connection));
			}
			C->Len = dist[dst];
			C->PathSize = sp;
			C->Path = (vertex**)malloc(C->PathSize * sizeof(vertex*));
			int pos = 0;
			while(sp > 0)
			{
				//printf("%d ", stack[--sp]);
				int vId = stack[--sp];
				vertex *vtx = G->Verticies[vId];
				C->Path[pos++] = vtx;
				//printf("%.*s ", (int)vtx->NameTok->TextLength, vtx->NameTok->Text);
			}
			free(stack);

			break;
		}
	}
	//printArr(dist, path, v, src);
	return C;
}

void
printConnection(wire *W)
{
	if(W && W->Connection)
	{
		//printf("Len: %dmm\t Path: ", W->Connection->Len);
		for(int i=0; i<W->Connection->PathSize; ++i)
		{
			vertex *V = W->Connection->Path[i];
			if(i==0)
			{
				printf("[%.*s:%.*s] ", 
				(int)V->NameTok->TextLength, V->NameTok->Text,
				(int)W->StartCavity->TextLength, W->StartCavity->Text);
				
			}
			else if(i == W->Connection->PathSize - 1)
			{
				printf("[%.*s:%.*s]", 
					(int)V->NameTok->TextLength, V->NameTok->Text,
					(int)W->EndCavity->TextLength, W->EndCavity->Text);
			}
			else
			{
				printf("[%.*s] ",
					(int)V->NameTok->TextLength, V->NameTok->Text);
			}
		}
	}
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
		printf("WIRE NUMBER: %.*s - %.*s", 
			(int)W->WireNumberTok->TextLength, W->WireNumberTok->Text,
			(int)W->NameTok->TextLength, W->NameTok->Text);
		printf("\t[%.*s:%.*s => %.*s:%.*s]\n",
			(int)W->StartVertex->NameTok->TextLength, W->StartVertex->NameTok->Text,
			(int)W->StartCavity->TextLength, W->StartCavity->Text,
			(int)W->EndVertex->NameTok->TextLength, W->EndVertex->NameTok->Text,
			(int)W->EndCavity->TextLength, W->EndCavity->Text);
		
		//dijkstra(Graph, W->StartVertex->Id);
		W->Connection = dijkstra(Graph, G, W->StartVertex->Id, W->EndVertex->Id);
		printConnection(W);
		printf("\n\n");
	}

	printf("Verticies------------------------------------------\n\n\n");

	for(int v=0; v<G->VertexCount; ++v)
	{
		vertex *V = G->Verticies[v];
		
		if ((V->NameTok->TextLength >= 2) &&
				(V->NameTok->Text[0] == 'H') &&
				(V->NameTok->Text[1] == 'V'))
		{
			continue; 	//don't output for Harness Verticies.
		}

		else if ((V->NameTok->TextLength >= 2) &&
				(V->NameTok->Text[0] == 'S') &&
				(V->NameTok->Text[1] == 'P'))
		{
			printf("%.*s,,,Ultrasonic\n",
			(int)V->NameTok->TextLength, V->NameTok->Text);
			printf("%.*s,,,Weld Splice\n",
			(int)V->DescriptionTok->TextLength, V->DescriptionTok->Text);
		
			printf("Wire,Sz,Color,Target\n");
			
			for(int w=0; w<G->WireCount; ++w)
			{
				wire *W = G->Wires[w];
				
				if(W->StartVertex->Id == V->Id)
				{
					printf("%.*s,%d AWG,%.*s,%.*s:%.*s\n",
						(int)W->WireNumberTok->TextLength, W->WireNumberTok->Text,
						W->Gauge,
						(int)W->ColorTok->TextLength, W->ColorTok->Text,
						(int)W->EndVertex->NameTok->TextLength, W->EndVertex->NameTok->Text,
						(int)W->EndCavity->TextLength, W->EndCavity->Text);
				}
				
				if(W->EndVertex->Id == V->Id)
				{
					printf("%.*s,%d AWG,%.*s,%.*s:%.*s\n",
						(int)W->WireNumberTok->TextLength, W->WireNumberTok->Text,
						W->Gauge,
						(int)W->ColorTok->TextLength, W->ColorTok->Text,
						(int)W->StartVertex->NameTok->TextLength, W->StartVertex->NameTok->Text,
						(int)W->StartCavity->TextLength, W->StartCavity->Text);
				}
			}
			printf("\n\n\n");	
		}
		else
		{
			printf("%.*s,,,ConnectorMFG\n",
				(int)V->NameTok->TextLength, V->NameTok->Text);
			printf("%.*s,,,ConnectorPN\n",
				(int)V->DescriptionTok->TextLength, V->DescriptionTok->Text);
			
			printf("Pin,Sz,Color,Connection Target\n");
			
			for(int w=0; w<G->WireCount; ++w)
			{
				wire *W = G->Wires[w];
				
				if(W->StartVertex->Id == V->Id)
				{
					printf("%.*s,%d AWG,%.*s,%.*s:%.*s\n",
						(int)W->StartCavity->TextLength, W->StartCavity->Text,
						W->Gauge,
						(int)W->ColorTok->TextLength, W->ColorTok->Text,
						(int)W->EndVertex->NameTok->TextLength, W->EndVertex->NameTok->Text,
						(int)W->EndCavity->TextLength, W->EndCavity->Text);
				}
				
				if(W->EndVertex->Id == V->Id)
				{
					printf("%.*s,%d AWG,%.*s,%.*s:%.*s\n",
						(int)W->EndCavity->TextLength, W->EndCavity->Text,
						W->Gauge,
						(int)W->ColorTok->TextLength, W->ColorTok->Text,
						(int)W->StartVertex->NameTok->TextLength, W->StartVertex->NameTok->Text,
						(int)W->StartCavity->TextLength, W->StartCavity->Text);
				}
			}
			printf("\n\n\n");	
		}
	}

	printf("Wires----------------------------------------\n");
	printf("Number,Name,Gauge(AWG),Type,Color,Length(mm),Path\n");
	for(int w=0; w<G->WireCount; ++w)
	{
		wire *W = G->Wires[w];
		printf("%.*s,%.*s,%d,%.*s,%.*s,%d,",
			(int)W->WireNumberTok->TextLength, W->WireNumberTok->Text,
			(int)W->NameTok->TextLength, W->NameTok->Text,
			W->Gauge,
			(int)W->TypeTok->TextLength, W->TypeTok->Text,
			(int)W->ColorTok->TextLength, W->ColorTok->Text,
			W->Connection->Len);
		printConnection(W);
		printf("\n");

	}
	return 0;
}