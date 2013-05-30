
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H 

#include <stdio.h>
#include <stdlib.h>

/* 检测图片格式，并解码图片 */
static int Graph_DetectImage(const char *filepath, LCUI_Graph *out)
{
	int result = FILE_ERROR_UNKNOWN_FORMAT;
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = Graph_LoadPNG(filepath, out);  
	}
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = Graph_LoadJPEG(filepath, out);  
	}
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = Graph_LoadBMP(filepath, out); 
	}
	return result;
}

/* 载入指定图片文件的图像数据 */
LCUI_API int Graph_LoadImage( const char *filepath, LCUI_Graph *out )
{
	FILE *fp;
	int result = 0;
	
	Graph_Init(out); 
	out->have_alpha = FALSE;
	fp = fopen(filepath,"r");
	if ( fp== NULL ) {
		result = FILE_ERROR_OPEN_ERROR; 
	} else {
		fgetc(fp);
		if (!ferror (fp)) { /* 如果没出错 */
			fseek(fp,0,SEEK_END);
			if (ftell(fp)>4) {
				fclose(fp);
				result = Graph_DetectImage(filepath, out); 
			} else {
				result = FILE_ERROR_SHORT_FILE; /* 文件过小 */
				fclose(fp);
			}
		}
	}
	switch(result) {
	case FILE_ERROR_SHORT_FILE:
		printf("error: file is too short!\n");
		break;
	case FILE_ERROR_OPEN_ERROR:
		perror(filepath);
		break;
	case FILE_ERROR_UNKNOWN_FORMAT:
		printf("error: unknown format!\n");
		break;
	}
	return result;
}
