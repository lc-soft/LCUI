#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/scrollbar.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/widget/textview.h>

const char *test_css = CodeToString(

.container {
	width: 640px;
	height: 240px;
	padding: 10px;
	margin: 20px auto 0 auto;
	border: 1px solid #eee;
}

);

const char *test_content = "\n\
/* ***************************************************************************\n\
 * test_scrollbar.c -- test scrollbar\n\
 *\n\
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.\n\
 *\n\
 * Redistribution and use in source and binary forms, with or without\n\
 * modification, are permitted provided that the following conditions are met:\n\
 *\n\
 *   * Redistributions of source code must retain the above copyright notice,\n\
 *     this list of conditions and the following disclaimer.\n\
 *   * Redistributions in binary form must reproduce the above copyright\n\
 *     notice, this list of conditions and the following disclaimer in the\n\
 *     documentation and/or other materials provided with the distribution.\n\
 *   * Neither the name of LCUI nor the names of its contributors may be used\n\
 *     to endorse or promote products derived from this software without\n\
 *     specific prior written permission.\n\
 *\n\
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n\
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n\
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE\n\
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n\
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n\
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n\
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n\
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n\
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n\
 * POSSIBILITY OF SUCH DAMAGE.\n\
 */\n\
";

/* Build content view with native C code */
void BuildContentView(void)
{
        LCUI_Widget root = LCUIWidget_GetRoot();
	LCUI_Widget container = LCUIWidget_New(NULL);
	LCUI_Widget content = LCUIWidget_New("textview");
	LCUI_Widget scrollbar = LCUIWidget_New("scrollbar");

	TextView_SetText(content, test_content);
	ScrollBar_BindTarget(scrollbar, content);
	Widget_AddClass(container, "container");
	Widget_Append(container, content);
	Widget_Append(container, scrollbar);
	Widget_Append(root, container);
}

/* Build content view with the XML code in test_scrollbar.xml */
int BuildContentViewFromXML(void)
{
        LCUI_Widget root = LCUIWidget_GetRoot();
        LCUI_Widget pack = LCUIBuilder_LoadFile("test_scrollbar.xml");

        if (!pack) {
                return -1;
        }
        Widget_Append(root, pack);
        Widget_Unwrap(pack);
	return 0;
}

int main(int argc, char **argv)
{
        LCUI_Init();
	LCUIDisplay_SetSize(800, 640);
	LCUI_LoadCSSString(test_css, __FILE__);
	/* We have two ways to build content view */
	BuildContentViewFromXML();
	BuildContentView();
        return LCUI_Main();
}
