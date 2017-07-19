#include "ig_base.h"
#include <stdlib.h>

//For TEXT files since it will null terminate.
static char *
ReadEntireFileIntoMemoryAndNullTerminate(char *FileName)
{
    char *Result = 0;
    
    FILE *File = fopen(FileName, "r");
    if(File)
    {
		//Kill UTF Header
		size_t pos = 0;
		int ch = 0;
		while (File)
		{
			ch = fgetc(File);
			if (ch == 0xef ||
				ch == 0xbb ||
				ch == 0xbf)
			{
				++pos;
			}
			else
			{
				break;
			}
		}
		
		fseek(File, 0, SEEK_END);
		size_t FileSize = ftell(File);
        fseek(File, pos, SEEK_SET);

		size_t allocSize = FileSize + 1 - pos;
		if (allocSize > 0)
		{
			Result = (char *)malloc(allocSize);
			fread(Result, FileSize - pos, 1, File);
			Result[FileSize] = 0;
		}
        
        
        fclose(File);
    }
    return(Result);
}

static void
copyStringLiteral(char *dst, char *src, int len)
{
    for (int i = 0; i < len; ++i)
    {
        *dst++ = *src++;
    }
    *dst = 0; // null terminate
}