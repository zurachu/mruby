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

static mrb_value pce_font_put_str(mrb_state *mrb, mrb_value self)
{
	char* str;
	mrb_get_args(mrb, "z", &str);
	pceFontPutStr(str);
	return mrb_nil_value();
}

static void setup_module()
{
	struct RClass* pce = mrb_define_module(mrb, "Pce");
	struct RClass* pce_font = mrb_define_module_under(mrb, pce, "Font");
	mrb_define_module_function(mrb, pce_font, "put_str", pce_font_put_str, MRB_ARGS_REQ(1));
}

void pceAppInit( void )
{
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

	setup_module();
	pceFontPutStr("setup_module: ");
	pceFontPrintf("%d\n", pceHeapGetMaxFreeSize());

	mrb_load_irep(mrb, mrb_hello);
	pceFontPutStr("mrb_load_irep: ");
	pceFontPrintf("%d\n", pceHeapGetMaxFreeSize());

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

