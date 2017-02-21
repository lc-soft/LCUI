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
	ret = LCUI_ReadImageFile( "test_image_reader.png", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	ret = LCUI_ReadImageFile( "test_image_reader.jpg", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	ret = LCUI_ReadImageFile( "test_image_reader.bmp", &img );
	assert( ret == 0 && img.width == 91 && img.height == 69 );
	Graph_Free( &img );
	return 0;
}
