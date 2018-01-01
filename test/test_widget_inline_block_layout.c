#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <stdio.h>
#include "test.h"

#define BOX_SIZE	64.0
#define SCREEN_WIDTH	680
#define SCREEN_HEIGHT	480

static int check_dropdown_menu( void )
{
	int ret = 0;
	LCUI_Widget menu;
	float height = 34 * 3 + 2 + 8 * 2;
	menu = LCUIWidget_GetById( "test-dropdown" );
	CHECK( menu->x == 20 && menu->y == SCREEN_HEIGHT - height - 20 );
	CHECK( menu->width > 220 && menu->width < 280 );
	CHECK( menu->height == height );
	return ret;
}

static int check_layout( void )
{
	int ret = 0;
	char str[256];
	float width, block_size;
	LCUI_Widget child, box;

	block_size = BOX_SIZE + (5 + 1) * 2;
	box = LCUIWidget_GetById( "box-container" );
	CHECK( box->x == 0 && box->y == 0 );
	CHECK( box->width > block_size * 3 );
	CHECK( box->width < block_size * 5 );
	CHECK( box->height == BOX_SIZE + (5 + 1) * 2 * 3 );

	child = LinkedList_Get( &box->children, 0 );
	sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
		 child->x, child->y, 0., 0. );
	CHECK_WITH_TEXT( str, child->x == 5. && child->y == 5. );

	child = LinkedList_Get( &box->children, 1 );
	width = (BOX_SIZE + (5 + 1) * 2) * 2 + (5 + 1) * 2 + 5;
	sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
		 child->x, child->y, width, 5. );
	CHECK_WITH_TEXT( str, child->x == width && child->y == 5. );

	ret += check_dropdown_menu();
	return ret;
}

int test_widget_inline_block_layout( void )
{
	int ret = 0;
	LCUI_Widget root, pack;

	LCUI_Init();
	TEST_LOG( "test widget inline block layout\n" );
	LCUIDisplay_SetSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	root = LCUIWidget_GetRoot();
	CHECK( pack = LCUIBuilder_LoadFile( "test_widget_inline_block_layout.xml" ) );
	if( !pack ) {
		LCUI_Destroy();
		return ret;
	}
	Widget_UpdateStyle( root, TRUE );
	Widget_Append( root, pack );
	Widget_Unwrap( pack );
	LCUIWidget_Update();
	ret += check_layout();
	LCUI_Destroy();
	return ret;
}
