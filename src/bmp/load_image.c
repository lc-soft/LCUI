
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H 
#include LC_DRAW_H

#include <stdio.h>
#include <stdlib.h>

static int detect_image(const char *filepath, LCUI_Graph *out)
/* 功能：检测图片格式，并解码图片 */
{
	int result = FILE_ERROR_UNKNOWN_FORMAT;
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = load_png(filepath, out);  
	}
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = load_jpeg(filepath, out);  
	}
	if (result == FILE_ERROR_UNKNOWN_FORMAT) {
		result = load_bmp(filepath, out); 
	}
	return result;
}

LCUI_EXPORT(int) Load_Image(const char *filepath, LCUI_Graph *out)
/* 
 * 功能：载入指定图片文件的图形数据
 * 说明：打开图片文件，并解码至内存，打开的图片文件越大，占用的内存也就越大 
 * */
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
				result = detect_image(filepath, out); 
			} else {
				result = FILE_ERROR_SHORT_FILE; /* 文件过小 */
				fclose(fp);
			}
		}
	}
	switch(result) {
	case FILE_ERROR_SHORT_FILE: printf("error: file is too short!\n");break;
	case FILE_ERROR_OPEN_ERROR: perror(filepath);break;
	case FILE_ERROR_UNKNOWN_FORMAT: printf("error: unknown format!\n");break;
	}
	return result;
}
