#include <stdio.h>
#include "ig_util.cpp"
#include "ig_tokenizer.cpp"

struct vertex
{
	int Id;
	char Name[5];
	char Description[80];
};

struct gr
{
	int Capacity;
	int VertexCount;
	vertex **Verticies;
};

int
main(int argc, char **args)
{
	char *verticies;
	char *edges;
	verticies = ReadEntireFileIntoMemoryAndNullTerminate(
		"../misc/Verticies.csv");
	edges = ReadEntireFileIntoMemoryAndNullTerminate(
		"../misc/Edges.csv");

	//printf("%s\n", verticies );
	tokenizer T;
	T.At = verticies;
	
	gr *Graph;
	Graph = (gr*)malloc(sizeof(gr));
	Graph->Capacity = 50;
	Graph->Verticies = (vertex**)malloc(Graph->Capacity * sizeof(vertex));
	Graph->VertexCount = 0;

	vertex *V = (vertex*)malloc(sizeof(vertex));

	*V = {};
	token Tok = {}; 

	GetToken(&T);
	while(Tok.Type != Token_EndOfStream)
	{
		//printf("Tok %s:", token_strings[(int)Tok.Type]);
		// printf("Type: %s\nTok: %.*s\n", 
		// 	token_strings[(int)Tok.Type], 
		// 	(size_t)Tok.TextLength, 
		// 	Tok.Text);
		
		V->Id = Graph->VertexCount; 
		Tok = GetToken(&T);
		if (Tok.Type == Token_Identifier)
		{

		}
	}

	return 0;
}