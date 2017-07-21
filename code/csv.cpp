#include <stdio.h>
#include "ig_util.cpp"
#include "ig_tokenizer.cpp"


struct vertex
{
	int Id;
	//char Name[5];
	//char Description[80];
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

struct gr
{
	int VertexCapacity;
	int VertexCount;
	int EdgeCapacity;
	int EdgeCount;
	vertex **Verticies;
	edge **Edges;
};

char*
processCSVVerticies(gr *Graph, char *VertCSVPath)
{
	char *verticies = NULL;
	verticies = ReadEntireFileIntoMemoryAndNullTerminate(VertCSVPath);
	
	if(verticies)
	{
		tokenizer *T = (tokenizer*)malloc(sizeof(tokenizer));
		T->At = verticies;

		token *Tok = (token*)calloc(0, sizeof(token));
		token *LineTok = (token*)malloc(sizeof(token));
		*LineTok = GetToken(T); 
		
		// printf("Type: %s\nTok: %.*s\n", 
		// 		token_strings[(int)Tok->Type], 
		// 		Tok->TextLength, 
		// 		Tok->Text);

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
						vertex *V = (vertex*)malloc(sizeof(vertex));
						V->NameTok = (token*)malloc(sizeof(token));
						V->DescriptionTok = (token*)malloc(sizeof(token));
						
						//Fill Vertex
						V->Id = Graph->VertexCount;

						V->NameTok->Type = Token_Identifier;
						V->NameTok->Text = LineTok->Text;
						V->NameTok->TextLength = SepPos - LineTok->Text;

						V->DescriptionTok->Type = Token_Identifier;
						V->DescriptionTok->Text = SepPos + 1;
						V->DescriptionTok->TextLength = Tok->Text + Tok->TextLength - SepPos - 2;	//Don't include newline or comma

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

char*
processCSVEdges(gr *Graph, char *EdgeCSVPath)
{
	char *edges = NULL;
	edges = ReadEntireFileIntoMemoryAndNullTerminate(EdgeCSVPath);
	if(edges)
	{
		tokenizer *T = (tokenizer*)malloc(sizeof(tokenizer));
		T->At = edges;
		
		token *Tok = (token*)malloc(sizeof(token));
		token *PrevTok = (token*)malloc(sizeof(token));
		*PrevTok = GetToken(T); 
		
		int FoundField = 0;

		edge *E = NULL;
		while((PrevTok->Type != Token_EndOfStream) && (Tok->Type != Token_EndOfStream))
		{	
			if(!E)
			{
				//Allocate Temporary Edge to Store Fields
				E = (edge*)calloc(0, sizeof(edge));
				E->NameTok = (token*)malloc(sizeof(token));
				E->NotesTok = (token*)malloc(sizeof(token));
			}
			*Tok = GetToken(T);

			
			if (Tok->Type == Token_Comma)
			{
				switch(FoundField)
				{
					case 0:
						{
							//Name
							E->NameTok->Text = PrevTok->Text;
							E->NameTok->TextLength = Tok->Text - 1 - PrevTok->Text;
							*PrevTok = GetToken(T);	
							
							
						} break;

					case 1:
						{	//Len
							if (PrevTok->Type == Token_Number)
							{
								E->Len = NumberTokenToInt(*PrevTok);
							}
							*PrevTok = GetToken(T);	

						}break;

					case 2:
						{	//From
							printf("Not gonna From\n");	
							*PrevTok = GetToken(T);		

						}break;

					case 3:	
						{	//To
							printf("Not Gonna To\n");
							*PrevTok = GetToken(T);	

						}break;

					case 4:
						{	//Notes
							E->NotesTok->Text = PrevTok->Text;	
							E->NotesTok->TextLength = Tok->Text - 1 - PrevTok->Text;
							*PrevTok = GetToken(T);	

						}break;

					default:
						break;
				}
				++FoundField;
			}
			
			if (Tok->Type == Token_EndOfLine)
			{
				//check if found field ==5;

				//Add Edge To Graph
				Graph->Edges[Graph->EdgeCount++] = E;
				E = NULL;
				FoundField = 0;			
				*PrevTok = GetToken(T);		//start next cycle
			}
		}	
	}
	return edges;
}

int
main(int argc, char **args)
{
	gr *Graph;
	Graph = (gr*)malloc(sizeof(gr));
	
	Graph->VertexCapacity = 50;
	Graph->Verticies = (vertex**)malloc(Graph->VertexCapacity * sizeof(vertex));
	Graph->VertexCount = 0;

	Graph->EdgeCapacity = 200;
	Graph->Edges = (edge**)malloc(Graph->EdgeCapacity * sizeof(edge));
	Graph->EdgeCount = 0;

	char *verticies = processCSVVerticies(Graph, "Verticies.csv");
	char *edges = processCSVEdges(Graph, "Edges.csv");

	printf("Created %d verticies\n", Graph->VertexCount);
	for (int i=0; i<Graph->VertexCount; ++i)
	{
		printf("Vertex:%d\t Name:%.*s\t Description:%.*s\n",
				Graph->Verticies[i]->Id,
				Graph->Verticies[i]->NameTok->TextLength, Graph->Verticies[i]->NameTok->Text,
				Graph->Verticies[i]->DescriptionTok->TextLength, Graph->Verticies[i]->DescriptionTok->Text);
	}
	
	return 0;
}