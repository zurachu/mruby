#include <piece.h>

#include "mruby.h"

static mrb_value pce_font_put_str(mrb_state *mrb, mrb_value self)
{
	char* str;
	mrb_get_args(mrb, "z", &str);
	pceFontPutStr(str);
	return mrb_nil_value();
}

void setup_module(mrb_state *mrb)
{
	struct RClass* pce = mrb_define_module(mrb, "Pce");
	struct RClass* pce_font = mrb_define_module_under(mrb, pce, "Font");
	mrb_define_module_function(mrb, pce_font, "put_str", pce_font_put_str, MRB_ARGS_REQ(1));
}
