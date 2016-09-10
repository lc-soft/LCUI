
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h> 

/** 检测图片格式，并解码图片 */
static int Graph_DetectImage(const char *filepath, LCUI_Graph *out)
{
	int ret = -1;
	if (ret == -1) {
		ret = Graph_LoadPNG(filepath, out);  
	}
	if (ret == -1) {
		ret = Graph_LoadJPEG(filepath, out);  
	}
	if (ret == -1) {
		ret = Graph_LoadBMP(filepath, out); 
	}
	return ret;
}

int Graph_LoadImage( const char *filepath, LCUI_Graph *out )
{
	FILE *fp;
	int ret = 0;
	Graph_Init( out );
	out->color_type = COLOR_TYPE_RGB;
	fp = fopen( filepath, "rb" );
	if( fp == NULL ) {
		return ENOENT;
	}
	fgetc( fp );
	if( !ferror( fp ) ) {
		fseek( fp, 0, SEEK_END );
		if( ftell( fp ) > 4 ) {
			fclose( fp );
			ret = Graph_DetectImage( filepath, out );
		} else {
			ret = -1;
			fclose( fp );
		}
	}
	return ret;
}

int Graph_GetImageSize( const char *filepath, int *width, int *height )
{
	int ret = Graph_GetPNGSize( filepath, width, height );
	if( ret != 0 ) {
		ret = Graph_GetJPEGSize( filepath, width, height );
	}
	if( ret != 0 ) {
		ret = Graph_GetBMPSize( filepath, width, height );
	}
	return ret;
}
