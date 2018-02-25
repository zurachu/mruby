#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <piece.h>

#include "mruby.h"

unsigned char vbuff[128*88];
static unsigned char draw;

int errno;
void _exit( int status ) { while(1); }
void exit( int status ) { while(1); }

mrb_state* mrb;

static void* allocf(mrb_state* mrb, void* p, size_t size, void* ud)
{
	void* ret;
	if(size == 0)
	{
		pceHeapFree(p);
		return NULL;
	}
	ret = pceHeapAlloc(size);
	if(p)
	{
		memcpy(ret, p, size);
	}
	return ret;
}

void pceAppInit( void )
{
	pceLCDDispStop();
	pceLCDSetBuffer( vbuff );
	pceAppSetProcPeriod( 80 );
	memset( vbuff, 0, 128*88 );

	mrb = mrb_open_allocf(allocf, NULL);

	pceFontSetPos( 0, 0);
	pceFontPrintf("Hello, World");

	draw = 1;

	pceLCDDispStart();
}


void pceAppProc( int cnt )
{
	if ( draw ) {
		pceLCDTrans();
		draw = 0;
	}
}


void pceAppExit( void )
{
	mrb_close(mrb);
}

