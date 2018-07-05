#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <piece.h>

#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/proc.h"

unsigned char vbuff[128*88];
static unsigned char draw;
static unsigned char phase;

int errno;
void _exit( int status ) { while(1); }
void exit( int status ) { while(1); }

mrb_state* mrb;

void setup_module(mrb_state *mrb);

extern const uint8_t mrb_hello[];

static void* allocf(mrb_state* mrb, void* p, size_t size, void* ud)
{
	void* ret;
	if(size == 0)
	{
		if(p)
		{
			pceHeapFree(p);
		}
		return NULL;
	}
	ret = pceHeapAlloc(size);
	if(p)
	{
		memcpy(ret, p, size);
		pceHeapFree(p);		
	}
	return ret;
}

unsigned long prev_timer_get_count;
void put_timer_and_heap()
{
	unsigned long timer_get_count = pceTimerGetCount();
	pceFontPrintf( "%5d:%5d:", timer_get_count - prev_timer_get_count, pceHeapGetMaxFreeSize() );
	prev_timer_get_count = timer_get_count;
}

void pceAppInit( void )
{
	prev_timer_get_count = pceTimerGetCount();

	pceLCDDispStop();
	pceLCDSetBuffer( vbuff );
	pceAppSetProcPeriod( 80 );
	memset( vbuff, 0, 128*88 );

	pceFontSetType( 2 );
	pceFontSetPos( 0, 0 );
	put_timer_and_heap();
	pceFontPutStr("pceAppInit()\n\n");

	draw = 1;
	phase = 0;

	pceLCDDispStart();
}


void pceAppProc( int cnt )
{
	if ( pcePadGet() & PAD_D ) {
		phase = 0xFF;
		pceAppReqExit(0);
	}

	switch ( phase ) {
	case 0:
		draw = 1;
		phase++;
		break;

	case 1:
		mrb = mrb_open_allocf(allocf, NULL);
		if(mrb)
		{
			put_timer_and_heap();
			pceFontPutStr("mrb_open_allocf()\n\n");
			phase++;
		}
		else
		{
			pceFontPutStr("mrb_open_allocf failed.\n");
			phase = 0xFF;
		}
		draw = 1;
		break;

	case 2:
		setup_module(mrb);
		put_timer_and_heap();
		pceFontPutStr("setup_module()\n\n");
		draw = 1;
		phase++;
		break;

	case 3:
		mrb_load_irep(mrb, mrb_hello);
		pceFontPutStr("\n");
		put_timer_and_heap();
		pceFontPutStr("mrb_load_irep()\n");
		draw = 1;
		phase++;
		break;
	}

	if ( draw ) {
		pceLCDTrans();
		draw = 0;
	}
}


void pceAppExit( void )
{
	mrb_close(mrb);
}

