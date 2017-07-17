#ifndef HARNESS_H
#define HARNESS_H
#include "ig_base.h"


typedef struct cavity 
{
	u32 id;
	char name[32];
	cavity *next;
} cavity;

typedef struct {
	u32 id;
	u32 pinTotal;
	u32 pinUsed;
	b32 endPoint;
	cavity *conductorList;
	char name[80];
} node;

typedef struct 
{
	u32 id;
	double length;
	node *from;
	node *to;
	char notes[160];
} connection;

typedef struct 
{
	u32 id;
	node *start;
	node *end;

	char name[80];
	char gauge[32];
	char material[32];
	char color[16];
	char stripe[16];

} wire;

#endif