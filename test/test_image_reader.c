#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include "test.h"

int test_image_reader( void )
{
	int ret;
	LCUI_Graph img;
	Graph_Init( &img );
	_DEBUG_MSG("test png image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.png", &img );
	_DEBUG_MSG("png image size is: (%d, %d)\n", img.width, img.height);
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	_DEBUG_MSG("test jpeg image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.jpg", &img );
	_DEBUG_MSG("jpeg image size is: (%d, %d)\n", img.width, img.height);
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	_DEBUG_MSG("test bmp image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.bmp", &img );
	_DEBUG_MSG("bmp image size is: (%d, %d)\n", img.width, img.height);
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	return 0;
}
