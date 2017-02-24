#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include "test.h"

int test_image_reader( void )
{
	LCUI_Graph img;
	int ret, width, height;

	width = height = 0;
	Graph_Init( &img );
	_DEBUG_MSG("test png image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.png", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	ret = LCUI_GetImageSize( "test_image_reader.png", &width, &height );
	_DEBUG_MSG("png image size is: (%d, %d)\n", width, height);
	assert( ret == 0 && width == 91 && height == 69 );
	Graph_Free( &img );

	width = height = 0;
	_DEBUG_MSG("test jpeg image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.jpg", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	ret = LCUI_GetImageSize( "test_image_reader.jpg", &width, &height );
	_DEBUG_MSG("jpeg image size is: (%d, %d)\n", width, height);
	assert( ret == 0 && width == 91 && height == 69 );
	Graph_Free( &img );

	width = height = 0;
	_DEBUG_MSG("test bmp image reader...\n");
	ret = LCUI_ReadImageFile( "test_image_reader.bmp", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	ret = LCUI_GetImageSize( "test_image_reader.bmp", &width, &height );
	_DEBUG_MSG("bmp image size is: (%d, %d)\n", width, height);
	assert( ret == 0 && width == 91 && height == 69 );
	Graph_Free( &img );
	return 0;
}

