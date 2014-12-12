/*
** Copyright (C) ARM Limited, 2001. All rights reserved.
*/

#include <stdio.h>
#include <string.h>
//#include <rt_misc.h>
#include <time.h>
#include "lib.h"

#undef DEBUG
//#define DEBUG
#ifdef DEBUG
#define DPRINTF	printf
#else
#define DPRINTF(...)
#endif

//#if (USE_YAFFS==1)

struct __FILE { int handle;   /* Add whatever you need here */};

//#define yaffs_Map2File(nfile)		((FILE*)(nfile+1))
//#define File_Map2yaffs(cfile)		((int)(cfile)-1)

//#else

//#endif

FILE __stdin, __stdout, __stderr;


int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here     */
	/* e.g. write a character to a UART, or to the */
	/* debugger console with SWI WriteC            */
	if(f == stdout)	// for console out put e.g. printf
		CONSOLE_PUTC(ch);

	return ch;
}


int ferror(FILE *f)
{   /* Your implementation of ferror */
    return EOF;
}


void _sys_exit(int return_code)
{
	for(;;);
}


int __raise(int signal, int argument)//void _ttywrch(int ch)
{
	return 0;
}
