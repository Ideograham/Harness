#include <stdio.h>
#include "ig_util.cpp"
#include "ig_tokenizer.cpp"


struct cavity
{
	int Id;
	token *NameTok;
	//how to link this in?  pointer to vertex and wire?
};

struct connector
{
	int Id;
	token *NameTok;
	token *NotesTok;
	int PinCount;
	token *PartNumberTok;
	token *MfgTok;
};

struct vertex
{
	int Id;
	token *NameTok;
	token *DescriptionTok;
};

struct edge
{
	int Id;
	token *NameTok;
	int Len;
	vertex *From;
	vertex *To;
	token *NotesTok;
};

struct wire
{
	int Id;
	token *WireNumberTok;
	token *NameTok;
	vertex *StartVertex;
	token *StartCavity;
	vertex *EndVertex;
	token *EndCavity;
};

struct gr
{
	int VertexCapacity;
	int VertexCount;
	int EdgeCapacity;
	int EdgeCount;
	int WireCapacity;
	int WireCount;
	vertex **Verticies;
	edge **Edges;
	wire **Wires;
};

wire*
allocateWire()
{
	wire *W = (wire*)calloc(1, sizeof(wire));
	W->WireNumberTok = (token*)calloc(1, sizeof(token));
	W->NameTok = (token*)calloc(1, sizeof(token));
	W->StartCavity = (token*)calloc(1, sizeof(token));
	W->EndCavity = (token*)calloc(1, sizeof(token));
	W->StartVertex = (vertex*)calloc(1, sizeof(vertex));
	W->EndVertex = (vertex*)calloc(1, sizeof(vertex));
	return W;
}


char*
processCSVVerticies(gr *Graph, char *VertCSVPath)
{
	char *verticies = NULL;
	verticies = ReadEntireFileIntoMemoryAndNullTerminate(VertCSVPath);
	
	if(verticies)
	{
		tokenizer *T = (tokenizer*)calloc(1, sizeof(tokenizer));
		T->At = verticies;

		token *Tok = (token*)calloc(1, sizeof(token));
		token *LineTok = (token*)calloc(1, sizeof(token));
		*LineTok = GetToken(T); 
		
		//printTok(LineTok);
		
		//process verticies	
		b32 FoundHeading = FALSE;
		b32 FoundSep = FALSE;
		char *SepPos = NULL;

		while((LineTok->Type != Token_EndOfStream) && (Tok->Type != Token_EndOfStream))
		{	
			*Tok = GetToken(T);
			
			if (Tok->Type == Token_Comma)
			{
				if(!FoundSep)
				{
					FoundSep = TRUE;
					SepPos = Tok->Text;	
				}
				else
				{
					printf("Error, already found a comma on this line.\n");
				}
			}
			
			if (Tok->Type == Token_EndOfLine)
			{
				{
					if(!FoundHeading)
					{
						//no validation - should add
						FoundHeading = TRUE;
						FoundSep = FALSE;
						SepPos = NULL;				
						*LineTok = GetToken(T);		//start next cycle
					}
					if(FoundSep)
					{
						//Allocate New Vertex
						vertex *V = (vertex*)calloc(1, sizeof(vertex));
						V->NameTok = (token*)calloc(1, sizeof(token));
						V->DescriptionTok = (token*)calloc(1, sizeof(token));
						
						//Fill Vertex
						V->Id = Graph->VertexCount;

						V->NameTok->Type = Token_Identifier;
						V->NameTok->Text = LineTok->Text;
						V->NameTok->TextLength = SepPos - LineTok->Text;

						V->DescriptionTok->Type = Token_Identifier;
						V->DescriptionTok->Text = SepPos + 1;
						V->DescriptionTok->TextLength = Tok->Text + Tok->TextLength - SepPos - 2;

						//Add Vertex To Graph
						Graph->Verticies[Graph->VertexCount++] = V;

						//FoundSep = FALSE
						FoundSep = FALSE;
						SepPos = NULL;				
						*LineTok = GetToken(T);		//start next cycle
					}
					else
					{
						//error line

					}
				}
			}
		}
		free(T);
		free(LineTok);
		free(Tok);
	}	
	return verticies;
}

vertex*
matchVertexInGraph(gr* Graph, token *Name)
{
	vertex *Result = NULL;
	b32 Found = FALSE;
	//printf("Matching %.*s with TextLength = %d\n", (int)Name->TextLength, Name->Text, Name->TextLength);

	if(Graph)
	{
		for(int i=0; i<Graph->VertexCount; ++i)
		{
			vertex *V = Graph->Verticies[i];
			if (V->NameTok->TextLength == Name->TextLength)
			{
				char *VItr = V->NameTok->Text;
				char *NItr = Name->Text;
				b32 NotFound = FALSE;
				for (int i=0; i<V->NameTok->TextLength; ++i)
				{
					if (VItr[i] != NItr[i])
					{
						NotFound = TRUE;
					}
				}
				if(!NotFound)
				{
					Found = TRUE;
					Result = V;
					break;
				}
			}
		}
	}
	return Result;
}


char*
processCSVEdges(gr *Graph, char *EdgeCSVPath)
{
	char *edges = NULL;
	edges = ReadEntireFileIntoMemoryAndNullTerminate(EdgeCSVPath);
	if(edges)
	{
		tokenizer *T = (tokenizer*)calloc(1, sizeof(tokenizer));
		T->At = edges;
		
		token *Tok = (token*)calloc(1, sizeof(token));
		token *PrevTok = (token*)calloc(1, sizeof(token));
		*PrevTok = GetToken(T); 
		
		int LineNumber = 0;
		int FoundField = 0;
		char *LastComma = edges;
		edge *E = NULL;

		while((PrevTok->Type != Token_EndOfStream) && (Tok->Type != Token_EndOfStream))
		{	
			if(!E)
			{
				//Allocate Temporary Edge to Store Fields
				//printf("Allocating new Edge for line %d\n", LineNumber);
				E = (edge*)calloc(1, sizeof(edge));
				E->NameTok = (token*)calloc(1, sizeof(token));
				E->NotesTok = (token*)calloc(1, sizeof(token));
			}
			*Tok = GetToken(T);
			
			if (Tok->Type == Token_Comma)
			{
				switch(FoundField)
				{
					case 0:
						{
							//Name
							E->NameTok->Text = LastComma;
							E->NameTok->TextLength = Tok->Text - LastComma;
							*PrevTok = GetToken(T);	
						}break;

					case 1:
						{	
							//Len
							if (PrevTok->Type == Token_Number)
							{
								E->Len = NumberTokenToInt(*PrevTok);
							}
							*PrevTok = GetToken(T);	
						}break;

					case 2:
						{	
							//From
							E->From = matchVertexInGraph(Graph, PrevTok);
							if (!E->From)
								printf("bad from created on line %d\n", LineNumber);
							*PrevTok = GetToken(T);		
						}break;

					case 3:	
						{	
							//To
							E->To = matchVertexInGraph(Graph, PrevTok);
							if (!E->To)
								printf("bad to created on line %d\n", LineNumber);
							*PrevTok = GetToken(T);	
						}break;

					case 4:
						{	
							//Notes - must handle in end of line

						}break;

					default:
						printf("Shouldn't get here\n");
						break;
				}
				LastComma = Tok->Text+1;
				++FoundField;
				continue;
			}
			
			if (Tok->Type == Token_EndOfLine)
			{
				//Maintain Debug Info
				++LineNumber;
				//printf("Found %d fields on line %d\n", FoundField+1, LineNumber);

				//Store out Edge
				if (FoundField == 4)
				{
					E->Id = Graph->EdgeCount;
					E->NotesTok->Text = LastComma;
					E->NotesTok->TextLength = Tok->Text - LastComma;

					Graph->Edges[Graph->EdgeCount] = E;
					++Graph->EdgeCount;
				}
				else
					printf("Not saving edge on line %d\n", LineNumber);
				
				//Setup Next Line, Eating multiple new line characters
				E = NULL;
				FoundField = 0;
				*PrevTok = *Tok;
				while(PrevTok->Type != Token_EndOfStream && PrevTok->Type == Token_EndOfLine)
				{
					*PrevTok = GetToken(T);		//start next cycle
					LastComma = PrevTok->Text;
				}			
			}
		}
		free(T);
		free(PrevTok);
		free(Tok);
	}
	return edges;
}

char*
processCSVWires(gr *Graph, char *wireCSVPath)
{
	char *wires = NULL;
	wires = ReadEntireFileIntoMemoryAndNullTerminate(wireCSVPath);
	if(wires)
	{
		tokenizer *T = (tokenizer*)calloc(1, sizeof(tokenizer));
		T->At = wires;
		
		token *Tok = (token*)calloc(1, sizeof(token));
		token *PrevTok = (token*)calloc(1, sizeof(token));
		*PrevTok = GetToken(T); 
		
		int LineNumber = 0;
		int FoundField = 0;
		char *LastComma = wires;
		wire *W = NULL;

		while((PrevTok->Type != Token_EndOfStream) && (Tok->Type != Token_EndOfStream))
		{	
			if(!W)
			{
				//printf("Allocating new wire for line %d\n", LineNumber);
				W = allocateWire();
			}
			*Tok = GetToken(T);
			
			if (Tok->Type == Token_Comma)
			{
				switch(FoundField)
				{
					case 0:
						{
							//Wire Number
							W->WireNumberTok->Text = LastComma;
							W->WireNumberTok->TextLength = Tok->Text - LastComma;
							*PrevTok = GetToken(T);	
						}break;

					case 1:
						{	
							//Name
							W->NameTok->Text = LastComma;
							W->NameTok->TextLength = Tok->Text - LastComma;
							*PrevTok = GetToken(T);	
						}break;

					case 2:
						{	
							//Start Vertex
							W->StartVertex = matchVertexInGraph(Graph, PrevTok);
							if (!W->StartVertex)
								printf("bad start verted created on line %d\n", LineNumber);
							*PrevTok = GetToken(T);		
						}break;
					
					case 3:
						{	
							//start cavity
							W->StartCavity->Text = LastComma;
							W->StartCavity->TextLength = Tok->Text - LastComma;
							*PrevTok = GetToken(T);	
						}break;

					case 4:	
						{	
							//end verted
							W->EndVertex = matchVertexInGraph(Graph, PrevTok);
							if (!W->EndVertex)
								printf("bad end vertex created on line %d\n", LineNumber);
							*PrevTok = GetToken(T);	
						}break;
					
					case 5:
						{	
							//End cavity - handle in end of line
							W->EndCavity->Text = LastComma;
							W->EndCavity->TextLength = Tok->Text - LastComma;
							*PrevTok = GetToken(T);	
						}break;

					default:
						printf("Shouldn't get here\n");
						break;
				}
				LastComma = Tok->Text+1;
				++FoundField;
				continue;
			}
			
			if (Tok->Type == Token_EndOfLine)
			{
				//Maintain Debug Info
				++LineNumber;
				//printf("Found %d fields on line %d\n", FoundField+1, LineNumber);

				//Store out wire
				if (FoundField == 5)
				{
					W->Id = Graph->WireCount;
					W->EndCavity->Text = LastComma;
					W->EndCavity->TextLength = Tok->Text - LastComma;

					Graph->Wires[Graph->WireCount] = W;
					++Graph->WireCount;
				}
				else
					printf("Not saving wire on line %d\n", LineNumber);
				
				//Setup Next Line, Eating multiple new line characters
				W = NULL;
				FoundField = 0;
				*PrevTok = *Tok;
				while(PrevTok->Type != Token_EndOfStream && PrevTok->Type == Token_EndOfLine)
				{
					*PrevTok = GetToken(T);		//start next cycle
					LastComma = PrevTok->Text;
				}			
			}
		}
		free(T);
		free(PrevTok);
		free(Tok);
	}
	return wires;
}

void
freeVertex(vertex *V)
{
	if(V)
	{
		if(V->NameTok)
			free(V->NameTok);
		if(V->DescriptionTok)
			free(V->DescriptionTok);	
		free(V);
	}
}

void
freeEdge(edge *E)
{
	if(E)
	{
		free(E->NameTok);
		free(E->NotesTok);
		freeVertex(E->From);
		freeVertex(E->To);
		free(E);
	}
}

void
freeWire(wire *W)
{
	if(W)
	{
		free(W->WireNumberTok);
		free(W->NameTok);
		free(W->StartVertex);
		free(W->StartCavity);
		free(W->EndVertex);
		free(W->EndCavity);
		free(W);
	}
}

void
freeGraph(gr *Graph)
{
	if(Graph)
		{
		for(int i=0; i<Graph->VertexCapacity; ++i)
		{
			freeVertex(Graph->Verticies[i]);		
		}

		for(int i=0; i<Graph->EdgeCapacity; ++i)
		{
			freeEdge(Graph->Edges[i]);
		}

		for(int i=0; i<Graph->WireCapacity; ++i)
		{
			freeWire(Graph->Wires[i]);
		}
		free(Graph->Edges);
		free(Graph->Verticies);
		free(Graph->Wires);
		free(Graph);	
	}
}

gr*
allocateGraph(int VertexCapacity = 50, int EdgeCapacity = 200, int WireCapacity = 50)
{
	gr *Graph;
	Graph = (gr*)calloc(1, sizeof(gr));
	
	Graph->VertexCapacity = VertexCapacity;
	Graph->Verticies = (vertex**)calloc((size_t)Graph->VertexCapacity, sizeof(vertex*));
	Graph->VertexCount = 0;

	Graph->EdgeCapacity = EdgeCapacity;
	Graph->Edges = (edge**)calloc((size_t)Graph->EdgeCapacity, sizeof(edge*));
	Graph->EdgeCount = 0;

	Graph->WireCapacity = WireCapacity;
	Graph->Wires = (wire**)calloc((size_t)Graph->WireCapacity, sizeof(wire*));
	Graph->WireCount = 0;

	return Graph;
}

/*
int
main(int argc, char **args)
{
	gr *Graph = allocateGraph();

	char *vertexCSV = processCSVVerticies(Graph, "Verticies.csv");
	char *edgeCSV = processCSVEdges(Graph, "Edges.csv");
	char *wireCSV = processCSVWires(Graph, "Wires.csv");
	
	printf("Created %d verticies\n", Graph->VertexCount);
	for (int i=0; i<Graph->VertexCount; ++i)
	{
		printf("Vertex:%d\t Name:%.*s\t Description:%.*s\n",
				Graph->Verticies[i]->Id,
				(int)Graph->Verticies[i]->NameTok->TextLength, Graph->Verticies[i]->NameTok->Text,
				(int)Graph->Verticies[i]->DescriptionTok->TextLength, Graph->Verticies[i]->DescriptionTok->Text);
	}

	printf("Created %d edges\n", Graph->EdgeCount);
	for (int i=0; i<Graph->EdgeCount; ++i)
	{
		printf("Edge:%d\tName:%.*s\tLength:%d\tFrom:%d\tTo:%d\tNotes:%.*s\n",
				Graph->Edges[i]->Id,
				(int)Graph->Edges[i]->NameTok->TextLength, Graph->Edges[i]->NameTok->Text,
				Graph->Edges[i]->Len,
				Graph->Edges[i]->From->Id,
				Graph->Edges[i]->To->Id,
				(int)Graph->Edges[i]->NotesTok->TextLength, Graph->Edges[i]->NotesTok->Text);
	}
	
	printf("Created %d wires\n", Graph->WireCount);
	for (int i=0; i<Graph->WireCount; ++i)
	{
		printf("Wire:%d\tNumber%.*s\tName:%.*s\tFrom:%d\tTo:%d\t\n",
				Graph->Wires[i]->Id,
				(int)Graph->Wires[i]->WireNumberTok->TextLength, Graph->Wires[i]->WireNumberTok->Text,
				(int)Graph->Wires[i]->NameTok->TextLength, Graph->Wires[i]->NameTok->Text,
				Graph->Wires[i]->StartVertex->Id,
				Graph->Wires[i]->EndVertex->Id);
	}
	//freeGraph(Graph);
	return 0;
}
*/
