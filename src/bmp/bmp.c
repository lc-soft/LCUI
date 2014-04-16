#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_ERROR_H

/* 这个结构体用于存储bmp文件的文件头的信息 */
typedef struct bmp_head {
	short int BMPsyg;
	short int nic[8];
	short int ix;
	short int nic2;
	short int iy;
	short int nic3[2];
	short int depth;
	short int rle;
	short int nic4[11];
} bmp_head;

/* 载入BMP图片文件 */
LCUI_API int Graph_LoadBMP( const char *filepath, LCUI_Graph *out )
{
	FILE *fp;
	bmp_head bmp;
	uchar_t *bytep;
	int  x, y, tempi, pocz, omin;

	fp = fopen(filepath,"rb");
	if(fp == NULL) {
		return FILE_ERROR_OPEN_ERROR;
	}

	/* 检测是否为bmp图片 */
	tempi = fread(&bmp, 1, sizeof(bmp_head),fp);
	if (tempi < sizeof(bmp_head) || bmp.BMPsyg != 19778) {
		return FILE_ERROR_UNKNOWN_FORMAT;
	}

	pocz = bmp.nic[4];
	if ((bmp.depth != 32) && (bmp.depth != 24) ) {
		_DEBUG_MSG("can not support  %i bit-depth !\n", bmp.depth);
		return  FILE_ERROR_UNKNOWN_FORMAT;
	}
	out->color_type = COLOR_TYPE_RGB;
	tempi = Graph_Create(out, bmp.ix, bmp.iy);
	if(tempi != 0) {
		_DEBUG_MSG("can not alloc memory\n");
		return 1;
	}

	fseek(fp,0,SEEK_END);
	omin = ftell(fp);
	omin = omin-pocz;
	omin = omin-((out->w*out->h)*(bmp.depth/8));
	omin = omin/(out->h);
	fseek(fp,pocz,SEEK_SET);

	switch( bmp.depth ) {
	case 32:
		for (y=0; y<out->h; ++y) {
			/* 从最后一行开始写入像素数据 */
			bytep = out->bytes + ((out->h-y-1)*out->w)*3;
			for (x=0; x<out->w; ++x) {
				*bytep++ = fgetc(fp);
				*bytep++ = fgetc(fp);
				*bytep++ = fgetc(fp);
				tempi = fgetc(fp);
			}
			/* 略过填充字符 */
			if (omin>0) {
				for (tempi=0;tempi<omin;tempi++) {
					fgetc(fp);
				}
			}
		}
		break;
	case 24:
		for (y=0; y<out->h; ++y) {
			bytep = out->bytes + ((out->h-y-1)*out->w)*3;
			for (x=0; x<out->w; ++x) {
				*bytep++ = fgetc(fp);
				*bytep++ = fgetc(fp);
				*bytep++ = fgetc(fp);
			}
			if (omin>0) {
				for (tempi=0;tempi<omin;tempi++) {
					fgetc(fp);
				}
			}
		}
		break;
	}
	fclose(fp);
	return 0;
}
