#include "test.h"
#include "libtest.h"
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/scrollbar.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/widget/textview.h>

/* clang-format off */

const char *test_css = CodeToString(

.container {
	width: 300px;
	height: 240px;
	padding: 10px;
	margin: 20px auto 0 auto;
	border: 1px solid #eee;
}

textview {
	white-space: nowrap;
	display: inline-block;
}

);

/* clang-format on */

const char *test_content = "\n\
/* ***************************************************************************\n\
 * test_scrollbar.c -- test scrollbar\n\
 *\n\
 * Copyright (c) 2021, Liu chao <lc-soft@live.cn> All rights reserved.\n\
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
	LCUI_Widget vscrollbar = LCUIWidget_New("scrollbar");
	LCUI_Widget hscrollbar = LCUIWidget_New("scrollbar");

	Widget_SetId(content, "license_content");
	TextView_SetText(content, test_content);
	ScrollBar_SetDirection(hscrollbar, LCUI_SCROLLBAR_HORIZONTAL);
	ScrollBar_BindTarget(vscrollbar, content);
	ScrollBar_BindTarget(hscrollbar, content);
	Widget_AddClass(container, "container");
	Widget_Append(container, content);
	Widget_Append(container, vscrollbar);
	Widget_Append(container, hscrollbar);
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

void test_scrollbar(void)
{
	float left, top;
	LCUI_SysEventRec e;
	LCUI_Widget content;

	LCUI_Init();
	LCUIDisplay_SetSize(800, 640);
	LCUI_LoadCSSString(test_css, __FILE__);
	BuildContentView();
	LCUI_RunFrame();

	content = LCUIWidget_GetById("license_content");
	left = content->computed_style.left;
	top = content->computed_style.top;

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 300;
	e.motion.y = 275;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	e.type = LCUI_MOUSEDOWN;
	e.button.button = LCUI_KEY_LEFTBUTTON;
	e.button.x = 300;
	e.button.y = 275;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 600;
	e.motion.y = 275;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	it_b("content should be moved to the left",
	     content->computed_style.left < left &&
		 top == content->computed_style.top,
	     TRUE);

	left = content->computed_style.left;
	top = content->computed_style.top;

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 400;
	e.motion.y = 275;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	e.type = LCUI_MOUSEUP;
	e.button.button = LCUI_KEY_LEFTBUTTON;
	e.button.x = 400;
	e.button.y = 275;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	it_b("content should be moved to the right",
	     content->computed_style.left > left &&
		 top == content->computed_style.top,
	     TRUE);

	left = content->computed_style.left;
	top = content->computed_style.top;

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 555;
	e.motion.y = 45;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	e.type = LCUI_MOUSEDOWN;
	e.button.button = LCUI_KEY_LEFTBUTTON;
	e.button.x = 555;
	e.button.y = 45;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 555;
	e.motion.y = 200;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	it_b("content should be moved to the top",
	     content->computed_style.left == left &&
		 top > content->computed_style.top,
	     TRUE);

	left = content->computed_style.left;
	top = content->computed_style.top;

	e.type = LCUI_MOUSEMOVE;
	e.motion.x = 555;
	e.motion.y = 100;
	e.motion.xrel = 0;
	e.motion.yrel = 0;
	LCUI_TriggerEvent(&e, NULL);
	e.type = LCUI_MOUSEUP;
	e.button.button = LCUI_KEY_LEFTBUTTON;
	e.button.x = 555;
	e.button.y = 100;
	LCUI_TriggerEvent(&e, NULL);
	LCUI_RunFrame();

	it_b("the content should have scrolled to the bottom",
	     content->computed_style.left == left &&
		 top < content->computed_style.top,
	     TRUE);

	LCUI_Destroy();
}

#ifdef PREVIEW_MODE

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

#endif
