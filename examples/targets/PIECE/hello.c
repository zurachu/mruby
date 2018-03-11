#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <piece.h>

#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/proc.h"

unsigned char vbuff[128*88];
static unsigned char draw;

int errno;
void _exit( int status ) { while(1); }
void exit( int status ) { while(1); }

mrb_state* mrb;

extern const uint8_t mrb_hello[];

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
	mrb_irep* irep;

	pceLCDDispStop();
	pceLCDSetBuffer( vbuff );
	pceAppSetProcPeriod( 80 );
	memset( vbuff, 0, 128*88 );

	pceFontSetPos( 0, 0 );
	pceFontPutStr("pceAppInit: ");
	pceFontPrintf("%d\n", pceHeapGetMaxFreeSize());

	mrb = mrb_open_allocf(allocf, NULL);
	if(mrb)
	{
		pceFontPutStr("mrb_open_allocf: ");
		pceFontPrintf("%d\n", pceHeapGetMaxFreeSize());
	}
	else
	{
		pceFontPutStr("mrb_open_allocf failed.\n");		
	}

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

