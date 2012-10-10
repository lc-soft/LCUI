
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
	int result = 1;
	if (result == 1) {
		result = load_png(filepath, out);  
	}
	if (result == 1) {
		result = load_jpeg(filepath, out);  
	}
	if (result == 1) {
		result = load_bmp(filepath, out); 
	}
	return result;
}

int Load_Image(const char *filepath, LCUI_Graph *out)
/* 
 * 功能：载入指定图片文件的图形数据
 * 说明：打开图片文件，并解码至内存，打开的图片文件越大，占用的内存也就越大 
 * */
{
	FILE *fp;
	int result = 0;   /* 错误代号为0 */
	
	Graph_Init(out); 
	out->have_alpha = IS_FALSE;
	/*fp是全局变量，其它函数会用到它*/
	if ((fp = fopen(filepath,"r")) == NULL) {
		perror(filepath);
		result = OPEN_ERROR; 
	} else {
		fgetc(fp);
		if (!ferror (fp)) {/*r如果没出错*/
			fseek(fp,0,SEEK_END);
			if (ftell(fp)>4) {
				fclose(fp);
				result = detect_image(filepath, out); 
			} else {
				result = SHORT_FILE;//文件过小 
				fclose(fp);
			}
		}
	}
	return result;   /* 返回错误代码 */
}

