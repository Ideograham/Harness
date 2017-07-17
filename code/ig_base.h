//ig_base.h
#ifndef IG_BASE_H
#define IG_BASE_H

#define internal static
#define local_persist static
#define global_variable static

#define TRUE 1
#define FALSE 0

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned long
#define u64 unsigned long long

#define s8 char
#define s16 short
#define s32 long
#define s64 long long

#define b8 u8
#define b16 u16
#define b32 u32
#define b64 u64

#define r32 float
#define r64 double

#define ArrayCount(A) (sizeof(A) / sizeof(A[0]))

//Byte order macros for Windows
#define htons(A)   ((((u16)(A) & 0xff00) >> 8) | \
					(((u16)(A) & 0x00ff) << 8))

#define htonl(A)   ((((u32)(A) & 0xff000000) >> 24) | \
					(((u32)(A) & 0x00ff0000) >>  8) | \
					(((u32)(A) & 0x0000ff00) <<  8) | \
					(((u32)(A) & 0x000000ff) << 24))

#define htonll(A) 	   ((((u64)(A) & 0x00000000000000FFULL) << 56) | \
						(((u64)(A) & 0x000000000000FF00ULL) << 40) | \
						(((u64)(A) & 0x0000000000FF0000ULL) << 24) | \
						(((u64)(A) & 0x00000000FF000000ULL) <<  8) | \
						(((u64)(A) & 0x000000FF00000000ULL) >>  8) | \
						(((u64)(A) & 0x0000FF0000000000ULL) >> 24) | \
						(((u64)(A) & 0x00FF000000000000ULL) >> 40) | \
						(((u64)(A) & 0xFF00000000000000ULL) >> 56))
 
#define ntohs htons
#define ntohl htonl
#define ntohll htonll


#define Assert(expression) if (!(expression)) {*(int*)(0) = 0;}

//end of IG_BASE_H
#endif