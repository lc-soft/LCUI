#include <wchar.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/font/textlayer.h>
#include "test.h"

static int test_textlayer_wordbreak(void)
{
	LCUI_TextLayer layer;

	layer = TextLayer_New();
	TextLayer_SetTextW(layer,
			   L"hello, world! "
			   L"long long long texttexttext, test word break",
			   NULL);
	TextLayer_SetFixedSize(layer, )
	TextLayer_Update(layer, NULL);
}

int test_textlayer(void)
{
	return test_textlayer_wordbreak();
}
