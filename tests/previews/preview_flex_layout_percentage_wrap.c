/*
 * tests/previews/preview_flex_layout_percentage_wrap.c
 *
 * Standalone preview for percentage flex wrap layout debugging.
 * Run with: xmake run preview-flex-layout-percentage-wrap
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>

void test_flex_layout_percentage_wrap(void);

int main(int argc, char **argv)
{
        test_flex_layout_percentage_wrap();
        return lcui_main();
}
