#include <stdio.h>
#include "ig_util.cpp"
#include "ig_tokenizer.cpp"


struct edge
{
	int Id;
	token *NameTok;
	int Len;
	vertex *From;
	vertex *To;
	token *NotesTok;
}

struct vertex
{
	int Id;
	//char Name[5];
	//char Description[80];
	token *NameTok;
	token *DescriptionTok;
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

		token *Tok = (token*)malloc(sizeof(token));
		token *LineTok = (token*)malloc(sizeof(token));
		*LineTok = GetToken(T); 
		*Tok = {};
		
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
	}	
	free(tokenizer);
	free(lineTok);
	free(Tok);
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
		*Tok = {};
		
		printf("Type: %s\nTok: %.*s\n", 
			token_strings[(int)Tok->Type], 
			Tok->TextLength, 
			Tok->Text);

		//process verticies	
		int FoundField = 0;
		char *SepPos[4] = {};
		b32 FoundHeading = FALSE;

		edge *E = NULL;
		while((PrevTok->Type != Token_EndOfStream) && (Tok->Type != Token_EndOfStream))
		{	
			if(!E)
			{
				//Allocate Temporary Edge to Store Fields
				E = (edge*)malloc(sizeof(edge));
				*E = {};
				E->NotesTok = (token*)malloc(sizeof(token));
			}

			*Tok = GetToken(T);
			
			// if(!FoundHeading)
			// {
			// 	//no validation - should add
			// 	FoundHeading = TRUE;
			// 	FoundField = 0;
			// 	SepPos = {};  				
			// 	*LineTok = GetToken(T);		//start next cycle
			// }

			if (Tok->Type == Token_Comma)
			{
				switch(FoundField)
				{
					case 0:
						{
							if (PrevTok.Type == Token_Number)
							{
								E->
							}
						} break;
					
					default:
						break;
				}
				else
				{
					printf("Error on this line, skipping edge.\n");
				}
			}
			
			if (Tok->Type == Token_EndOfLine)
			{
				{
					// if(!FoundHeading)
					// {
					// 	//no validation - should add
					// 	FoundHeading = TRUE;
					// 	FoundSep = FALSE;
					// 	SepPos = NULL;				
					// 	*LineTok = GetToken(T);		//start next cycle
					// }
					if(FoundSep)
					{

						//Fill Vertex
						V->Id = Graph->VertexCount;

						V->NameTok->Type = Token_Identifier;
						V->NameTok->Text = LineTok->Text;
						V->NameTok->TextLength = SepPos - LineTok->Text;

						V->DescriptionTok->Type = Token_Identifier;
						V->DescriptionTok->Text = SepPos + 1;
						V->DescriptionTok->TextLength = Tok->Text + Tok->TextLength - SepPos - 2;	//Don't include newline or comma
					}
				}
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