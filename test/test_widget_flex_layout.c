#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <stdio.h>
#include "test.h"

#define SCREEN_WIDTH	680
#define SCREEN_HEIGHT	480
#define BLOCK_SIZE	74

static int check_flex_box_1( void )
{
	char str[256];
	int ret = 0, i = 0;
	LinkedListNode *node;
	LCUI_Widget child, box;
	float simples[][2] = {
		{ 0, 0 },
		{ BLOCK_SIZE, 0 },
		{ BLOCK_SIZE * 2, 0 },
		{ 0, BLOCK_SIZE },
		{ 0, BLOCK_SIZE * 2 },
		{ BLOCK_SIZE, BLOCK_SIZE * 2 }
	};
	box = LCUIWidget_GetById( "flex-box-1" );
	CHECK( box->x == 0 && box->y == 0 );
	CHECK( box->height == BLOCK_SIZE * 3 + 1 );
	for( LinkedList_Each( node, &box->children ) ) {
		child = node->data;
		sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
			 child->x, child->y, simples[i][0], simples[i][1] );
		CHECK_WITH_TEXT( str, child->x == simples[i][0] &&
				 child->y == simples[i][1] );
		i += 1;
	}
	return ret;
}

static int check_flex_box_2( void )
{
	char str[256];
	int ret = 0, i = 0;
	LinkedListNode *node;
	LCUI_Widget child, box;
	const float offset_x1 = (SCREEN_WIDTH - BLOCK_SIZE * 3) / 2.0;
	const float offset_x2 = (SCREEN_WIDTH - BLOCK_SIZE * 2) / 2.0;
	float simples[][2] = {
		{ offset_x1, 0 },
		{ offset_x1 + BLOCK_SIZE, 0 },
		{ offset_x1 + BLOCK_SIZE * 2, 0 },
		{ 0, BLOCK_SIZE },
		{ offset_x2, BLOCK_SIZE * 2 },
		{ offset_x2 + BLOCK_SIZE, BLOCK_SIZE * 2 }
	};
	box = LCUIWidget_GetById( "flex-box-2" );
	CHECK( box->x == 0 && box->y == BLOCK_SIZE * 3 + 1 );
	CHECK( box->height == BLOCK_SIZE * 3 + 1 );
	for( LinkedList_Each( node, &box->children ) ) {
		child = node->data;
		sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
			 child->x, child->y, simples[i][0], simples[i][1] );
		CHECK_WITH_TEXT( str, child->x == simples[i][0] &&
				 child->y == simples[i][1] );
		i += 1;
	}
	return ret;
}

static int check_flex_box_3( void )
{
	char str[256];
	int ret = 0, i = 0;
	LinkedListNode *node;
	LCUI_Widget child, box;
	const float offset_x1 = SCREEN_WIDTH - BLOCK_SIZE * 3;
	const float offset_x2 = SCREEN_WIDTH - BLOCK_SIZE * 2;
	float simples[][2] = {
		{ offset_x1, 0 },
		{ offset_x1 + BLOCK_SIZE, 0 },
		{ offset_x1 + BLOCK_SIZE * 2, 0 },
		{ 0, BLOCK_SIZE },
		{ offset_x2, BLOCK_SIZE * 2 },
		{ offset_x2 + BLOCK_SIZE, BLOCK_SIZE * 2 }
	};
	box = LCUIWidget_GetById( "flex-box-3" );
	CHECK( box->x == 0 && box->y == BLOCK_SIZE * 3 * 2 + 2 );
	CHECK( box->height == BLOCK_SIZE * 3 + 1 );
	for( LinkedList_Each( node, &box->children ) ) {
		child = node->data;
		sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
			 child->x, child->y, simples[i][0], simples[i][1] );
		CHECK_WITH_TEXT( str, child->x == simples[i][0] &&
				 child->y == simples[i][1] );
		i += 1;
	}
	return ret;
}

static int check_flex_box_4( void )
{
	char str[256];
	int ret = 0, i = 0;
	LinkedListNode *node;
	LCUI_Widget child, box;
	const float offset_x = 480 - BLOCK_SIZE * 3;
	float simples[][2] = {
		{ 0, 0 },
		{ offset_x, BLOCK_SIZE },
		{ offset_x + BLOCK_SIZE, BLOCK_SIZE },
		{ offset_x + BLOCK_SIZE * 2, BLOCK_SIZE }
	};
	box = LCUIWidget_GetById( "flex-box-4" );
	CHECK( box->height == BLOCK_SIZE * 2 );
	for( LinkedList_Each( node, &box->children ) ) {
		child = node->data;
		sprintf( str, "[%d] (%g, %g) == (%g, %g)", child->index,
			 child->x, child->y, simples[i][0], simples[i][1] );
		CHECK_WITH_TEXT( str, child->x == simples[i][0] &&
				 child->y == simples[i][1] );
		i += 1;
	}
	return ret;
}

static int check_layout( void )
{
	int ret = 0;
	ret += check_flex_box_1();
	ret += check_flex_box_2();
	ret += check_flex_box_3();
	ret += check_flex_box_4();
	return ret;
}

int test_widget_flex_layout( void )
{
	int ret = 0;
	LCUI_Widget root, pack;

	LCUI_Init();
	TEST_LOG( "test widget flex layout\n" );
	LCUIDisplay_SetSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	root = LCUIWidget_GetRoot();
	CHECK( pack = LCUIBuilder_LoadFile( "test_widget_flex_layout.xml" ) );
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

#ifdef PREVIEW_MODE
#include <errno.h>

int tests_count = 0;

int main( void )
{
	LCUI_Widget root, pack;

	LCUI_Init();
	LCUIDisplay_SetSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile( "test_widget_flex_layout.xml" );
	if( !pack ) {
		LCUI_Destroy();
		return -ENOENT;
	}
	Widget_Append( root, pack );
	Widget_Unwrap( pack );
	return LCUI_Main();
}
#endif
