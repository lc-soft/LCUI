/* ***************************************************************************
 * LCUI_Graphics.c -- graphics handle
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Graphics.c -- 图形处理
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/ 
#include <png.h>
#include <jpeglib.h>
#include <math.h> 

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPHICS_H
#include LC_MEM_H
#include LC_MISC_H 
#include LC_WIDGET_H
#include LC_CURSOR_H

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


struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

LCUI_RGB
RGB (unsigned char red, unsigned char green,
	 unsigned char blue)

/* 功能：将三个颜色值转换成LCUI_RGB型数据 */
{
	LCUI_RGB color;

	color.red = red;
	color.green = green;
	color.blue = blue;
	return color;
}

int Get_Graph_Type(LCUI_Graph *pic)
/* 返回图形的类型 */
{
	return pic->type;
}

LCUI_Size Get_Graph_Size(LCUI_Graph *pic)
/* 功能：获取图形的尺寸 */
{
	if(NULL == pic)
		return Size(0,0);
	return Size(pic->width, pic->height);
}

int Graph_Is_PNG(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为png
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_PNG) return 1;
	else return 0;
}

int Graph_Is_BMP(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为bmp
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_BMP) return 1;
	else return 0;
}

int Graph_Is_JPG(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否为jpg 
 * 返回值：是则返回1，否则返回0
 * */
{
	if( Get_Graph_Type(pic) == TYPE_JPG) return 1;
	else return 0;
}

int Graph_Have_Alpha(LCUI_Graph *pic)
/* 
 * 功能：判断图像是否带有带alpha通道 
 * 返回值：是则返回1，否则返回0
 * */
{
	pic = Get_Quote_Graph(pic);
	if( pic->flag == HAVE_ALPHA) return 1;
	else return 0;
}

int Valid_Graph(LCUI_Graph *pic)
/*
 * 功能：检测图形数据是否有效
 * 返回值：有效返回1，无效返回0
 */
{
	LCUI_Graph *p;
	p = Get_Quote_Graph(pic);
	if(NULL != p && p->malloc == IS_TRUE 
	&& p->width > 0 && p->height > 0)
		return 1; 
	return 0;
}

int Valid_Bitmap(LCUI_Bitmap *bitmap)
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回1，无效返回0
 */
{
	if(NULL != bitmap && bitmap->malloc == IS_TRUE 
		&& bitmap->width * bitmap->height > 0)
		return 1; 
	return 0;
}

void Print_Bitmap_Info(LCUI_Bitmap *bitmap)
/* 功能：打印位图的信息 */
{
	printf("address:%p\n",bitmap);
	if(bitmap != NULL) 
		printf("width:%d,height:%d,alpha:%u,malloc = %d(%s)\n", 
		bitmap->width, bitmap->height, bitmap->alpha, bitmap->malloc, 
		bitmap->malloc == IS_TRUE?"is true":"is false");
}

void Print_Graph_Info(LCUI_Graph *pic)
/* 功能：打印图像的信息 */
{
	printf("address:%p\n",pic);
	if(pic == NULL) return;
	printf("width:%d, height:%d, alpha:%u,flag = %d(%s),malloc = %d(%s)\n", 
	pic->width, pic->height, pic->alpha, pic->flag, 
	pic->flag == HAVE_ALPHA ? "have alpha":"no alpha",
	pic->malloc, pic->malloc == IS_TRUE?"is true":"is false");
	
	if(pic->quote == IS_TRUE) {
		printf("graph src:");
		Print_Graph_Info(Get_Quote_Graph(pic));
	}
}

void Bitmap_Init(LCUI_Bitmap *in)
/* 功能：初始化LCUI_Bitmap结构体数据 */
{
	in->data = NULL;
	in->alpha = 255;
	in->width = 0;
	in->height = 0;
	in->malloc = IS_FALSE;
}

void Graph_Init(LCUI_Graph *pic)
/* 初始化图片数据结构体 */
{
	if(pic == NULL) return;
	pic->quote	= IS_FALSE;
	pic->malloc	= IS_FALSE;
	pic->flag	= NO_ALPHA;
	pic->rgba	= NULL;
	pic->alpha	= 255;
	pic->pos	= Pos(0, 0);
	pic->width	= 0;
	pic->height	= 0;
	pic->type	= DEFAULT;
	pic->bit_depth	= 8;    /* 位深 */
	thread_rwlock_init(&pic->lock);	/* 读/写/互斥锁 */
}


/************************ Graph Quote *********************************/
int Quote_Graph(LCUI_Graph *des, LCUI_Graph *src, LCUI_Rect area)
/* 
 * 功能：引用另一个图层中指定区域里的图形 
 * 说明：src是被引用的对象，des是引用者，area是引用的src中的图形所在的区域
 * */
{
	if(src == NULL || des == NULL) return -1;
	area = Get_Valid_Area(Size(src->width, src->height), area); 
	des->src = src;
	des->pos.x = area.x;
	des->pos.y = area.y;
	des->width = area.width;
	des->height= area.height;
	des->quote = IS_TRUE;
	return 0;
}

LCUI_Rect Get_Graph_Valid_Rect(LCUI_Graph *graph)
/* 功能：获取被引用的图形所在的有效区域 */
{
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect;
	
	pos = graph->pos; 
	cut_rect.x = pos.x;
	cut_rect.y = pos.y;
	cut_rect.width = graph->width;
	cut_rect.height = graph->height; 
	
	if(graph->quote == IS_FALSE) 
		return cut_rect; 
	else {
		w = graph->src->width;
		h = graph->src->height;
	}
	
	/* 获取需裁剪的区域 */
	if(pos.x < 0) {
		cut_rect.width += pos.x;
		cut_rect.x = 0 - pos.x; 
	}
	if(pos.x + graph->width > w)
		cut_rect.width -= (pos.x +  graph->width - w); 
	
	if(pos.y < 0) {
		cut_rect.height += pos.y;
		cut_rect.y = 0 - pos.y; 
	}
	if(pos.y + graph->height > h)
		cut_rect.height -= (pos.y +  graph->height - h); 
	
	LCUI_Rect rect;
	/* 获取父部件的有效显示范围 */
	rect = Get_Graph_Valid_Rect(graph->src);
	/* 如果父部件需要裁剪，那么，子部件根据情况，也需要进行裁剪 */
	if(rect.x > 0) { 
		temp = pos.x + cut_rect.x;
		if(temp < rect.x) { 
			temp = rect.x - pos.x; 
			cut_rect.width -= (temp - cut_rect.x); 
			cut_rect.x = temp; 
		}
	}
	if(rect.y > 0) {
		temp = pos.y + cut_rect.y;
		if(pos.y < rect.y) {
			temp = rect.y - pos.y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.width < w) { 
		temp = pos.x+cut_rect.x+cut_rect.width;
		if(temp > rect.x+rect.width)  
			cut_rect.width -= (temp-(rect.x+rect.width));
	}
	if(rect.height < h) {
		temp = pos.y+cut_rect.y+cut_rect.height;
		if(temp > rect.y+rect.height)
			cut_rect.height -= (temp-(rect.y+rect.height));
	} 
	
	return cut_rect;
}

LCUI_Graph *Get_Quote_Graph(LCUI_Graph *graph)
/* 
 * 功能：获取指向被引用的图形的指针 
 * 说明：如果当前图形引用了另一个图形，并且，该图形处于一条引用链中，那么，本函数会返
 * 回指向被引用的最终图形的指针。
 * */
{
	if(graph == NULL) return NULL;
	if(graph->quote == IS_FALSE)
		return graph; 
	return Get_Quote_Graph(graph->src);
}

/************************ End Graph Quote *****************************/



/********************* Image File Processing **************************/
static int detect_jpg(char *filepath, LCUI_Graph *out)
/* 功能：检测并解码jpg图片 */
{
	FILE *fp;
	fp = fopen(filepath,"r");
	if(fp == NULL) return -1;
	
	int x,y, m, n, k;
	short int JPsyg;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_stride,jaka;

	if(fread(&JPsyg,sizeof (short int),1,fp))
		if (JPsyg != -9985)   /* 如果不是jpg图片 */
			return 1; 
			
	rewind(fp);
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return 2;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,fp);
	(void) jpeg_read_header(&cinfo,IS_TRUE);
	(void) jpeg_start_decompress(&cinfo);    

	jaka = cinfo.num_components;
	
	//if (jaka==3) printf("color\n"); else printf("grayscale\n");
	out->flag = NO_ALPHA;
	Malloc_Graph(out,cinfo.output_width,cinfo.output_height);
	if(!out->rgba || !out->rgba[0] || !out->rgba[1] || !out->rgba[2]){
		printf("错误(jpg):无法分配足够的内存供存储数据!\n");
		return 1;
	}
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo,JPOOL_IMAGE,row_stride,1);
	y=0;
	for(y=0; cinfo.output_scanline <cinfo.output_height; ++y) {
		(void) jpeg_read_scanlines(&cinfo,buffer,1);
		m = y*out->width;
		if (jaka==3) for (x=0;x<out->width;x++) {
			n = x+m;
			k=x*3;
			out->rgba[0][n]=buffer[0][k++];
			out->rgba[1][n]=buffer[0][k++];
			out->rgba[2][n]=buffer[0][k++];
		} 
		else for (x=0;x<out->width;x++)
		{
			n = x+m;
			out->rgba[0][n]=buffer[0][x];
			out->rgba[1][n]=buffer[0][x];
			out->rgba[2][n]=buffer[0][x];
		} 
	}
	out->type = TYPE_JPG;//图片类型为jpg
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fp);
	return 0;	
}

static int detect_bmp(char *filepath, LCUI_Graph *out)
/* 功能：检测并解码bmp图片 */
{
	FILE *fp;
	fp = fopen(filepath,"r");
	if(fp == NULL) return -1;
	bmp_head bmp;    
	int m, n, x, y, temp, tempi, rle,pocz, omin;
	unsigned char rozp;
	unsigned char **bak_rgba;    /* 备份的图片数据 */
	
	temp = fread(&bmp, 1, sizeof(bmp_head),fp);
	if (temp < sizeof(bmp_head) || bmp.BMPsyg != 19778)/* 如果不是bmp图片 */ 
		return 1; 
	pocz = bmp.nic[4];
	out->bit_depth = bmp.depth;
	rle = bmp.rle;
	if ((out->bit_depth != 32) && (out->bit_depth != 24) 
		&& (out->bit_depth != 8) && (out->bit_depth !=4))
	{ 
		printf("错误(bmp):位深 %i 不支持!\n",out->bit_depth);
		return 2;
	}
	out->flag = NO_ALPHA;     /* 没有透明效果 */
	temp = Malloc_Graph(out, bmp.ix, bmp.iy);
	if(temp != 0)
	{
		printf("错误(bmp):无法分配足够的内存供存储数据!\n");
		return 1;
	}
	
	int size;
	size = out->width * out->height * sizeof(unsigned char);
	bak_rgba = (unsigned char**)malloc(3 * sizeof(unsigned char*));
	bak_rgba[0] = (unsigned char*)malloc(size);
	bak_rgba[1] = (unsigned char*)malloc(size);
	bak_rgba[2] = (unsigned char*)malloc(size);
	
	if(!bak_rgba || !bak_rgba[0] || !bak_rgba[1] || !bak_rgba[2]){
		printf("错误(bmp):无法分配足够的内存供存储数据!\n");
		return 1;
	}
	rozp = ftell(fp);
	fseek(fp,0,SEEK_END);
	omin = ftell(fp);
	omin = omin-pocz;
	omin = omin-((out->width*out->height)*(out->bit_depth/8));
	omin = omin/(out->height);
	fseek(fp,pocz,SEEK_SET);
	switch(out->bit_depth){ 
	case 32:
		for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				out->rgba[2][n]=fgetc(fp);
				out->rgba[1][n]=fgetc(fp);
				out->rgba[0][n]=fgetc(fp);
				tempi=fgetc(fp);
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) fgetc(fp);
		}
	break;
		 
	case 24:
		for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				out->rgba[2][n]=fgetc(fp);
				out->rgba[1][n]=fgetc(fp);
				out->rgba[0][n]=fgetc(fp);
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) fgetc(fp); 
		}
	break;
		
	case 8: 
		fseek(fp,-256*4,SEEK_CUR);
		for (tempi=0;tempi<256;tempi++) {
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		/* nonRLE */	
		if (rle==0) for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = x+m;
				if ((tempi=fgetc(fp))==-1) return 0;
				out->rgba[0][n]=bak_rgba[0][tempi];
				out->rgba[1][n]=bak_rgba[1][tempi];
				out->rgba[2][n]=bak_rgba[2][tempi];
			}
			if (omin>0) for (tempi=0;tempi<omin;tempi++) 
				getc(fp); 
		}   /* end nonRLE */
		else for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				if ((out->bit_depth=fgetc(fp))==-1) 
					return 0;
				n = m+x+rle;
				tempi=fgetc(fp);
				if (out->bit_depth==0) {
					for (rle=0;rle<tempi;rle++){
						out->bit_depth=fgetc(fp);
						out->rgba[0][n]=bak_rgba[0][out->bit_depth];
						out->rgba[1][n]=bak_rgba[1][out->bit_depth];
						out->rgba[2][n]=bak_rgba[2][out->bit_depth];
					}
					if (tempi!=((tempi/2)*2)) tempi=fgetc(fp);
				} else for (rle=0;rle<out->bit_depth;rle++){
					out->rgba[0][n]=bak_rgba[0][tempi];
					out->rgba[1][n]=bak_rgba[1][tempi];
					out->rgba[2][n]=bak_rgba[2][tempi];
				} 
				x+=rle-1;
			}	
		}
		/* end RLE  */  
		break;

	case 4: 
		fseek(fp,-16*4,SEEK_CUR);
		for (tempi=0;tempi<16;tempi++) {
			bak_rgba[2][tempi]=fgetc(fp);	
			bak_rgba[1][tempi]=fgetc(fp);	
			bak_rgba[0][tempi]=fgetc(fp);	
			x=fgetc(fp);
		}
		/* nonRLE */
		if (rle==0) for (y=0;y<out->height;y++) {
			m = (out->height-y-1)*out->width;
			for (x=0;x<out->width;x++) {
				n = m+x;
				tempi=fgetc(fp);
				rozp=tempi;
				rozp=(rozp & 0xf0)>>4;
				out->rgba[0][n]=bak_rgba[0][rozp];
				out->rgba[1][n]=bak_rgba[1][rozp];
				out->rgba[2][n]=bak_rgba[2][rozp];
				x++;
				rozp=tempi;
				rozp=(rozp & 0x0f);
				out->rgba[0][n]=bak_rgba[0][rozp];
				out->rgba[1][n]=bak_rgba[1][rozp];
				out->rgba[2][n]=bak_rgba[2][rozp];
			} 
		} /* end nonRLE */
		else { /* RLE */
			
		} /* end RLE */ 
	break;
	}
	out->type = TYPE_BMP;     /* 图片类型为bmp */
	free(bak_rgba[0]);
	free(bak_rgba[1]);
	free(bak_rgba[2]);
	free(bak_rgba);
	fclose(fp);
	return 0;	
}

int write_png_file(char *file_name , LCUI_Graph *graph)
/* 功能：将LCUI_Graph结构中的数据写入至png文件 */
{
	int j, i, temp, pos;
	png_byte color_type; 

	png_structp png_ptr;
	png_infop info_ptr; 
	png_bytep * row_pointers;
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
	{
		printf("[write_png_file] File %s could not be opened for writing", file_name);
		return -1;
	}
	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		printf("[write_png_file] png_create_write_struct failed");
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("[write_png_file] png_create_info_struct failed");
		return -1;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during init_io");
		return -1;
	}
	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing header");
		return -1;
	}
	Using_Graph(graph, 0);
	if(graph->flag == HAVE_ALPHA) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	else color_type = PNG_COLOR_TYPE_RGB;
	
	png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
		graph->bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing bytes");
		End_Use_Graph(graph);
		return -1;
	}
	if(graph->flag == HAVE_ALPHA) temp = (4 * graph->width);
	else temp = (3 * graph->width);
	
	row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
	for(i=0,pos=0; i < graph->height; i++)
	{
		row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
		for(j=0; j < temp; ++pos)
		{
			row_pointers[i][j++] = graph->rgba[0][pos]; // red
			row_pointers[i][j++] = graph->rgba[1][pos]; // green
			row_pointers[i][j++] = graph->rgba[2][pos];   // blue
			if(graph->flag == HAVE_ALPHA) 
				row_pointers[i][j++] = graph->rgba[3][pos]; // alpha 
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during end of write");
		End_Use_Graph(graph);
		return -1;
	}
	png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
	for (j=0; j<graph->height; j++)
		free(row_pointers[j]);
	free(row_pointers);

	fclose(fp);
	End_Use_Graph(graph);
    return 0;
}

int detect_png(char *filepath, LCUI_Graph *out)
/* 功能：检测并解码png图片 */
{
	FILE *pic_fp;
	pic_fp = fopen(filepath, "rb");
	if(pic_fp == NULL) /* 文件打开失败 */
		return -1;
	
	/* 初始化各种结构 */
	png_structp png_ptr;
	png_infop   info_ptr;
	char        buf[PNG_BYTES_TO_CHECK];
	int         temp;
	
	png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);
	
	setjmp(png_jmpbuf(png_ptr)); // 这句很重要
	
	temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);
	temp = png_sig_cmp((void*)buf,(png_size_t)0,PNG_BYTES_TO_CHECK);
	
	/*检测是否为png文件*/
	if (temp!=0) return 1;
	
	rewind(pic_fp);
	/*开始读文件*/
	png_init_io(png_ptr, pic_fp);/*pic_fp是全局变量*/
	// 读文件了
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	int color_type,channels;
	
	/*获取宽度，高度，位深，颜色类型*/
	channels       = png_get_channels(png_ptr, info_ptr);/*获取通道数*/
	out->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type     = png_get_color_type(png_ptr, info_ptr);/*颜色类型*/
	
	int pos = 0;
	/* row_pointers里边就是rgba数据 */
	png_bytep* row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	int i,j;
	if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{/*如果是RGB+alpha通道，或者RGB+其它字节*/
		//开始分配内存
		out->flag = HAVE_ALPHA;
		temp = Malloc_Graph(out, png_get_image_width(png_ptr, info_ptr), 
					png_get_image_height(png_ptr, info_ptr));
		if(temp != 0)
		{/* 如果分配内存失败 */
			fclose(pic_fp);
			printf("错误(png):无法分配足够的内存供存储数据!\n");
			return 1;
		}
		temp = (4 * out->width);
		for(i = 0; i < out->height; i++) 
		for(j = 0; j < temp; j += 4)
		{
			out->rgba[0][pos] = row_pointers[i][j]; // red
			out->rgba[1][pos] = row_pointers[i][j+1]; // green
			out->rgba[2][pos] = row_pointers[i][j+2];   // blue
			out->rgba[3][pos] = row_pointers[i][j+3]; // alpha
			++pos;
		} 
	}
	else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
	{/*如果是RGB通道*/
		out->flag = NO_ALPHA;
		temp = Malloc_Graph(out, png_get_image_width(png_ptr, info_ptr), 
					png_get_image_height(png_ptr, info_ptr));
		if(temp != 0)
		{
			fclose(pic_fp);
			printf("错误(png):无法分配足够的内存供存储数据!\n");
			return 1;
		}
		temp = (3 * out->width);
		for(i = 0; i < out->height; i++)
		for(j = 0; j < temp; j += 3)
		{
			out->rgba[0][pos] = row_pointers[i][j]; // red
			out->rgba[1][pos] = row_pointers[i][j+1]; // green
			out->rgba[2][pos] = row_pointers[i][j+2];   // blue
			++pos;
		} 
	}
	else return 1;
	out->type = TYPE_PNG;  /* 图片类型为png */
	/* 撤销数据占用的内存 */
	png_destroy_read_struct(&png_ptr, &info_ptr, 0); 
	return 0;
}

int detect_image(char *filepath, LCUI_Graph *out)
/* 功能：检测图片格式，并解码图片 */
{
	int result = 1;
	if (result == 1) result = detect_png(filepath, out);/*检测png*/  
	if (result == 1) result = detect_jpg(filepath, out);/*检测jpg*/ 
	if (result == 1) result = detect_bmp(filepath, out);/*检测bmp*/ 
	return result;
}

int Load_Image(char *filepath, LCUI_Graph *out)
/* 
 * 功能：载入指定图片文件的图形数据
 * 说明：打开图片文件，并解码至内存，打开的图片文件越大，占用的内存也就越大 
 * */
{
	FILE *fp;
	Graph_Init(out); 
	int result = 0;   /* 错误代号为0 */
	out->flag = NO_ALPHA;
	/*fp是全局变量，其它函数会用到它*/
	if ((fp = fopen(filepath,"r")) == NULL) {
		perror(filepath);
		result = OPEN_ERROR; 
	}
	else{
		fgetc(fp);
		if (!ferror (fp)) {/*r如果没出错*/
			fseek(fp,0,SEEK_END);
			if (ftell(fp)>4) {
				fclose(fp);
				result = detect_image(filepath, out);/*检测图片并解码*/
			} 
			else {
				result = SHORT_FILE;//文件过小 
				fclose(fp);
			}
		}
	}
	return result;   /* 返回错误代码 */
}
/******************* End Image File Processing ************************/


/************************ Graphics Processing *************************/
void Zoom_Graph(LCUI_Graph *in, LCUI_Graph *out, int flag, LCUI_Size size)
/* 功能：根据指定模式，对图像进行缩放 */
{
	LCUI_Graph *src;
	LCUI_Rect rect;
	LCUI_Pos pos; 
	int count, temp, x, y, k, m;
	double scale_x,scale_y;
	
	if(!Valid_Graph(in)) 
		return; 
	
	if(size.w <=0 || size.h <= 0) { 
		Free_Graph(out);
		return;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	rect = Get_Graph_Valid_Rect(in);
	src = Get_Quote_Graph(in);
	
	scale_x = (double)rect.width / size.w;
	scale_y = (double)rect.height / size.h;
	
	if(flag == DEFAULT) {/* 如果缩放方式为缺省，图片的宽和高的缩放比例将会一样 */
		if (scale_x<scale_y) scale_y = scale_x; 
		else scale_x = scale_y;
	}
	out->flag = in->flag;
	Malloc_Graph(out, size.w, size.h);/* 申请内存 */ 
	Using_Graph(out, 1);
	Using_Graph(in, 0); 
	
	for (y=0; y < size.h; ++y)  {
		pos.y = y*scale_y;
		k = y*size.w;
		m = (pos.y+rect.y)*src->width+rect.x;
		for (x = 0; x < size.w; ++x) {
			pos.x = x*scale_x; 
			temp  = k + x;
			count = m + pos.x;
			out->rgba[0][temp] = in->rgba[0][count];
			out->rgba[1][temp] = in->rgba[1][count];
			out->rgba[2][temp] = in->rgba[2][count];
			if(in->flag == HAVE_ALPHA)
				out->rgba[3][temp] = in->rgba[3][count];
		}
	} 
	
	End_Use_Graph(out);
	End_Use_Graph(in);
	out->width  = size.w;
	out->height = size.h;
	out->malloc = IS_TRUE;  /* 已经分配内存 */
}

int Cut_Graph(LCUI_Graph *src, LCUI_Rect rect, LCUI_Graph *out)
/* 
 * 功能：从源图形中截取出指定区域的图形 
 * 说明：裁剪出的图像数据，全局透明度和源图像一致
 * */
{
	int x, y;
	int temp,count = 0, k; 
	out->flag = src->flag;
	out->alpha = src->alpha;
	rect = Get_Valid_Area(Size(src->width, src->height), rect); 
	if(Valid_Graph(src) && rect.width * rect.height > 0) { 
		temp = Malloc_Graph(out, rect.width, rect.height);
		if(temp != 0) return -1; 
		Using_Graph(out, 1);
		Using_Graph(src, 0);
		/* 开始读取图片中的图形数组并写入窗口 */ 
		for (y=0;y< rect.height; ++y) {
			k = (rect.y+y)*src->width + rect.x;
			for (x=0;x<rect.width; ++x) {
				temp = k + x; /* 计算图片内需要读取的区域的各点坐标 */
				out->rgba[0][count] = src->rgba[0][temp];
				out->rgba[1][count] = src->rgba[1][temp];
				out->rgba[2][count] = src->rgba[2][temp];
				if(src->flag == HAVE_ALPHA)
					out->rgba[3][count] = src->rgba[3][temp];
				++count;
			}
		} 
		End_Use_Graph(out);
		End_Use_Graph(src);
		return 0;
	}
	return -1;
}

int Graph_Flip_Horizontal(LCUI_Graph *src, LCUI_Graph *out)  
/* 功能：将图像进行水平翻转 */  
{  
	int value = 0, x, y, pos, left, temp, count;  
	int width = src->width, height = src->height;  
	unsigned char buff;  
    
	if(!Valid_Graph(src)) value = -1;
	else {
		if(src->flag == HAVE_ALPHA) out->flag = HAVE_ALPHA;
		Malloc_Graph(out, width, height);
		/* 水平翻转其实也就是交换两边的数据 */  
		temp = width / 2.0;
		for (y = 0; y < height; ++y) {   
			pos = y * width;
			left = pos;
			for (x = 0; x <= temp; ++x)  {
				count = left + width - x - 1;
				
				buff = src->rgba[0][pos]; 
				out->rgba[0][pos] = src->rgba[0][count];  
				out->rgba[0][count] = buff;
                 
				buff = src->rgba[1][pos]; 
				out->rgba[1][pos] = src->rgba[1][count];  
				out->rgba[1][count] = buff;
                
				buff = src->rgba[2][pos]; 
				out->rgba[2][pos] = src->rgba[2][count];  
				out->rgba[2][count] = buff;
                
				if(src->flag == HAVE_ALPHA) {
					buff = src->rgba[3][pos]; 
					out->rgba[3][pos] = src->rgba[3][count];  
					out->rgba[3][count] = buff;
				}
				++pos;
			}
		}
	}
	return value;  
} 

int Get_Screen_Graph(LCUI_Graph *out)
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */
{
	unsigned char  *dest;
	int i, temp, h, w;
	if(LCUI_Sys.init != IS_TRUE) /* 如果没有初始化过 */
		return -1; 
	
	out->flag = NO_ALPHA;/* 无alpha通道 */
	out->type = TYPE_BMP;
	temp = Malloc_Graph(out, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h);
	if(temp != 0) 
		return -2; 
		
	i = 0; 
	dest = LCUI_Sys.screen.fb_mem; /* 指针指向帧缓冲的内存 */
	for (h = 0; h < LCUI_Sys.screen.size.h; ++h) {
		for (w = 0; w < LCUI_Sys.screen.size.w; ++w) {
			/* 读取帧缓冲的内容 */
			out->rgba[2][i] = *(dest++);
			out->rgba[1][i] = *(dest++);
			out->rgba[0][i] = *(dest++);
			dest++;
			++i; 
		}
	}
	return 0;
}

void Fill_Color(LCUI_Graph *pic, LCUI_RGB color)
/* 功能：为传入的图形填充颜色 */
{
	int size; 
	if(Valid_Graph(pic))
	{
		Using_Graph(pic, 1); 
		size = sizeof(unsigned char) * pic->height * pic->width;
		memset(pic->rgba[0], color.red,		size);
		memset(pic->rgba[1], color.green,	size);
		memset(pic->rgba[2], color.blue,	size);
		End_Use_Graph(pic);
	}
}


void Tile_Graph(LCUI_Graph *src, LCUI_Graph *out, int width, int height)
/* 
 * 功能：平铺图形
 * 参数说明：
 * src 原始图形
 * out 填充后的图形
 * width 填充的宽度
 * height 填充的高度
 **/
{
	if(!Valid_Graph(src) || out == NULL || width <=0 || height <= 0)
		return;
		
	int x,y,w,h,temp,count, m;
	temp = 0;
		
	Malloc_Graph(out, width, height);
	Using_Graph(out, 1);
	Using_Graph(src, 0);
	if(Graph_Have_Alpha(src)) {
		for(y = 0;y < height;++y) {
			h = y % src->height;
			m = h * src->width;
			for(x = 0;x < width;++x,++temp) {
				w = x % src->width;
				count = m + w;
				out->rgba[0][temp] = (src->rgba[0][count] * src->rgba[3][count] + out->rgba[0][temp] * (255 - src->rgba[3][count])) /255;
				out->rgba[1][temp] = (src->rgba[1][count] * src->rgba[3][count] + out->rgba[1][temp] * (255 - src->rgba[3][count])) /255;
				out->rgba[2][temp] = (src->rgba[2][count] * src->rgba[3][count] + out->rgba[2][temp] * (255 - src->rgba[3][count])) /255;
			} 
		}
	} else {
		for(y = 0;y < height;++y) {
			h = y%src->height;
			m = h * src->width;
			for(x = 0;x < width;++x) {
				w = x % src->width;
				count = m + w;
				out->rgba[0][temp] = src->rgba[0][count];
				out->rgba[1][temp] = src->rgba[1][count];
				out->rgba[2][temp] = src->rgba[2][count];
				out->rgba[3][temp] = 255;
				++temp;
			}
		}
	} 
	End_Use_Graph(out);
	End_Use_Graph(src); 
}

int Mix_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * 功能：将前景图与背景图混合叠加
 * 说明：back_graph是背景图像，fore_graph前景图像，混合后的数据保存在背景图中
 */
{
	unsigned char *r1, *g1, *a1, *b1, *r2, *g2, *b2; 

	unsigned int total, y = 0, m, n;
	unsigned char j;//, alpha; 
	float k;
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;
	
	src_rect = Get_Graph_Valid_Rect(fore_graph);
	des_rect = Get_Graph_Valid_Rect(back_graph);
	src = Get_Quote_Graph(fore_graph);
	des = Get_Quote_Graph(back_graph); 
	
	if(!Valid_Graph(des) || !Valid_Graph(src))
		return -1;
	if(des_pos.x > des->width) return -1;
	if(des_pos.y > des->height) return -1;
	
	/* 如果前景图尺寸超出背景图的范围，需要更改前景图的像素读取范围 */ 
	if( Get_Cut_Area(
		Size( des_rect.width, des_rect.height),
		Rect( des_pos.x, des_pos.y, 
			src_rect.width, src_rect.height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	Using_Graph(src, 0);
	Using_Graph(des, 1);
	/* 如果前景图形有alpha通道 */
	if(Graph_Have_Alpha(src)) {
		k = fore_graph->alpha / 255.0;
		if(fore_graph->alpha == 255){
			for (y = 0; y < cut.height; ++y) {
				m = (cut.y + y + src_rect.y) * src->width + cut.x + src_rect.x;
				n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
				/*
				 * 原来的代码是这样的：
				 * for (x = 0; x < cut.width; ++x) {
				 *   temp = m + x; //得出图片内需要读取的区域的各点坐标
				 *   count = n + x; //得出需填充至窗口的各点的坐标 
				 *   ......
				 * }
				 * 为了尽可能的提高效率，使用了以下简化的代码：
				 * */
				//
				total = n + cut.width;
				/* 根据alpha通道来混合像素点 */
				for (; n < total; ++n,++m) { 
					des->rgba[0][n] = (src->rgba[0][m]*src->rgba[3][m] + des->rgba[0][n]*(255-src->rgba[3][m]))/255;
					des->rgba[1][n] = (src->rgba[1][m]*src->rgba[3][m] + des->rgba[1][n]*(255-src->rgba[3][m]))/255;
					des->rgba[2][n] = (src->rgba[2][m]*src->rgba[3][m] + des->rgba[2][n]*(255-src->rgba[3][m]))/255; 
				}
				//
			}
		} else {
			for (y = 0; y < cut.height; ++y) {
				m = (cut.y + y + src_rect.y) * src->width + cut.x + src_rect.x;
				n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
				total = n + cut.width; 
				for (; n < total; ++n,++m) { 
					j = src->rgba[3][m] * k;
					des->rgba[0][n] = (src->rgba[0][m]*j + des->rgba[0][n]*(255-j))/255;
					des->rgba[1][n] = (src->rgba[1][m]*j + des->rgba[1][n]*(255-j))/255;
					des->rgba[2][n] = (src->rgba[2][m]*j + des->rgba[2][n]*(255-j))/255;
				}
			} 
		} 
	} else if(fore_graph->alpha < 255) {
		for (y = 0; y < cut.height; ++y) {
			m = (cut.y + y + src_rect.y) * src->width + cut.x + src_rect.x;
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			total = n + cut.width; 
			for (; n < total; ++n,++m) {
				des->rgba[0][n] = (src->rgba[0][m]*fore_graph->alpha + des->rgba[0][n]*(255-fore_graph->alpha))/255;
				des->rgba[1][n] = (src->rgba[1][m]*fore_graph->alpha + des->rgba[1][n]*(255-fore_graph->alpha))/255;
				des->rgba[2][n] = (src->rgba[2][m]*fore_graph->alpha + des->rgba[2][n]*(255-fore_graph->alpha))/255;
			} 
		}
	} else {/* 如果前景图形没有透明效果 */
		for (y = 0; y < cut.height; ++y) { 
			/* 计算前景图内需要读取的区域的各起点坐标 */
			m = (cut.y + y + src_rect.y) * src->width + cut.x + src_rect.x;
			/* 计算背景图内需要读取的区域的各起点坐标 */
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			/* 使用指针来引用 */
			r1 = des->rgba[0] + n;
			g1 = des->rgba[1] + n;
			b1 = des->rgba[2] + n;
			a1 = des->rgba[3] + n;
			
			r2 = src->rgba[0] + m;
			g2 = src->rgba[1] + m;
			b2 = src->rgba[2] + m;
			/* 拷贝 */
			memcpy(r1, r2, cut.width);
			memcpy(g1, g2, cut.width);
			memcpy(b1, b2, cut.width);
			/* 透明度取决于前景图的透明度 */
			if(Graph_Have_Alpha(des)) {
				memset(a1, src->alpha, sizeof(unsigned char)*cut.width);
			}
		}
	} 
	
	End_Use_Graph(des);
	End_Use_Graph(src); 
	return 0; 
}


int Replace_Graph(LCUI_Graph *back_graph, LCUI_Graph *fore_graph, LCUI_Pos des_pos)
/* 
 * 功能：替换原图中对应位置的区域中的图形
 * 说明：与Overlay_Graph函数不同，back_graph中的图形数据会被fore_graph中的图形数据覆盖
 */
{
	unsigned char *r1, *g1, *a1, *a2, *b1, *r2, *g2, *b2;

	int y = 0,total, m, n;
	unsigned char j, k; 
	
	LCUI_Graph *src, *des;
	LCUI_Rect cut, src_rect, des_rect;
	
	src_rect = Get_Graph_Valid_Rect(fore_graph);
	des_rect = Get_Graph_Valid_Rect(back_graph); 
	src = Get_Quote_Graph(fore_graph);
	des = Get_Quote_Graph(back_graph);
	
	if(!Valid_Graph(des) || !Valid_Graph(src))
		return -1;
	if(des_pos.x > des->width) return -1;
	if(des_pos.y > des->height) return -1;
	 
	if( Get_Cut_Area(
		Size( back_graph->width, back_graph->height),
		Rect( des_pos.x, des_pos.y, 
			fore_graph->width, fore_graph->height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	} 
	Using_Graph(src, 0);
	Using_Graph(des, 1);
	k = src->alpha / 255.0;
	if(Graph_Have_Alpha(src) && !Graph_Have_Alpha(des)) {
		for (y = 0; y < cut.height; ++y) {
			m = (cut.y + y + src_rect.y) * src->width + cut.x + src_rect.x;
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			total = n + cut.width; 
			for (; n < total; ++n,++m) {
				j = src->rgba[3][m] * k; 
				des->rgba[0][n] = (src->rgba[0][m]*j + 255*(255-j))/255;
				des->rgba[1][n] = (src->rgba[1][m]*j + 255*(255-j))/255;
				des->rgba[2][n] = (src->rgba[2][m]*j + 255*(255-j))/255; 
			}
		}
	} else {
		for (y = 0; y < cut.height; ++y) { 
			m = (cut.y + y + src_rect.y) *src->width + cut.x + src_rect.x;
			n = (des_pos.y + y + des_rect.y) * des->width + des_pos.x + des_rect.x;
			/* 使用指针来引用 */
			r1 = des->rgba[0] + n;
			g1 = des->rgba[1] + n;
			b1 = des->rgba[2] + n;
			
			r2 = src->rgba[0] + m;
			g2 = src->rgba[1] + m;
			b2 = src->rgba[2] + m; 
			/* 拷贝 */
			memcpy(r1, r2, cut.width);
			memcpy(g1, g2, cut.width);
			memcpy(b1, b2, cut.width);
			if(Graph_Have_Alpha(des)) {
				a1 = des->rgba[3] + n;
				if(!Graph_Have_Alpha(src)) {
					memset(a1, src->alpha, cut.width);
				} else {
					a2 = src->rgba[3] + m; 
					memcpy(a1, a2, cut.width);  
				}
			}
		}
	} 
	End_Use_Graph(des);
	End_Use_Graph(src);
	return 0; 
} 

int Align_Image(LCUI_Graph *graph, LCUI_Graph *image, int flag)
/* 功能：将图片以指定对齐方式粘贴至背景图上 */
{
	LCUI_Pos pos;
	pos.x = 0;
	pos.y = 0;
	if(!Valid_Graph(graph) || ! Valid_Graph(image))
	/* 如果图片无效 */
		return -1; 
		
	if((flag & ALIGN_TOP_LEFT) == ALIGN_TOP_LEFT);
	/* 向左上角对齐 */
	else if((flag & ALIGN_TOP_CENTER) == ALIGN_TOP_CENTER)
	/* 向上中间对齐 */
		pos.x = (graph->width - image->width) / 2;  
	else if((flag & ALIGN_TOP_RIGHT) == ALIGN_TOP_RIGHT)
	/* 向右上角对齐 */
		pos.x = graph->width - image->width; 
	else if((flag & ALIGN_MIDDLE_LEFT) == ALIGN_MIDDLE_LEFT)
	/* 向中央偏左对齐 */ 
		pos.y = (graph->height - image->height) / 2; 
	else if((flag & ALIGN_MIDDLE_CENTER) == ALIGN_MIDDLE_CENTER)
	{ /* 向正中央对齐 */
		pos.x = (graph->width - image->width) / 2;
		pos.y = (graph->height - image->height) / 2;
	}
	else if((flag & ALIGN_MIDDLE_RIGHT) == ALIGN_MIDDLE_RIGHT)
	{ /* 向中央偏由对齐 */
		pos.x = graph->width - image->width;
		pos.y = (graph->height - image->height) / 2;
	}
	else if((flag & ALIGN_BOTTOM_LEFT) == ALIGN_BOTTOM_LEFT)
	/* 向底部偏左对齐 */ 
		pos.y = graph->height - image->height; 
	else if((flag & ALIGN_BOTTOM_CENTER) == ALIGN_BOTTOM_CENTER)
	{ /* 向底部居中对齐 */
		pos.x = (graph->width - image->width) / 2;
		pos.y = graph->height - image->height;
	}
	else if((flag & ALIGN_BOTTOM_RIGHT) == ALIGN_BOTTOM_RIGHT)
	{ /* 向底部偏右对齐 */
		pos.x = graph->width - image->width;
		pos.y = graph->height - image->height;
	}
	
	if( Check_Option(flag, GRAPH_MIX_FLAG_OVERLAY) ) 
	/* 如果包含GRAPH_MIX_FLAG_OVERLAY选项 */
		Mix_Graph(graph, image, pos); 
	else if( Check_Option(flag, GRAPH_MIX_FLAG_REPLACE) ) 
	/* 如果包含GRAPH_MIX_FLAG_REPLACE选项 */
		Replace_Graph(graph, image, pos); 
	else Mix_Graph(graph, image, pos);
	
	return 0;
}


int Fill_Background_Image(LCUI_Graph *graph, LCUI_Graph *bg, int flag, LCUI_RGB color)
/* 功能：为指定图形填充背景图像 */
{
	Fill_Color(graph, color); /* 填充背景色 */
	if(!Valid_Graph(bg) || graph == NULL)
		return -1; 
		
	LCUI_Pos pos;
	LCUI_Graph temp_bg;
	pos.x = 0;
	pos.y = 0;
	Graph_Init(&temp_bg);
	switch(flag) {
	case LAYOUT_ZOOM:/* 缩放 */
		Zoom_Graph( bg, &temp_bg, DEFAULT,
		Size(graph->width, graph->height)
		);
		pos.x = (graph->width - temp_bg.width) / 2.0;
		pos.y = (graph->height - temp_bg.height) / 2.0;
		Mix_Graph(graph, &temp_bg, pos);
		break;
	case LAYOUT_STRETCH:/* 拉伸 */
		Zoom_Graph( bg,  &temp_bg, CUSTOM, 
		Size(graph->width, graph->height) 
		);
		Mix_Graph(graph, &temp_bg, pos);
		break;
	case LAYOUT_CENTER:/* 居中 */
		pos.x = (graph->width - bg->width) / 2;
		pos.y = (graph->height - bg->height) / 2;
		Mix_Graph(graph, bg, pos);
		break;
	case LAYOUT_TILE:/* 平铺 */
		Tile_Graph(bg, graph, graph->width, graph->height);
		break;
	default: 
		Mix_Graph(graph, bg, pos);
		break;
	}
	Free_Graph(&temp_bg);
	return 0; 
}

/* 以下是图像圆角化算法，暂未完成 */
#ifdef use_round_rect
int Set_Graph_To_Rounded_Rectangle(LCUI_Graph *graph, int radius, int mode, int line_size, LCUI_RGB *color)
/* 
 * 功能：将图形圆角化
* 参数说明：
* graph：需要处理的图形数据
* radius：圆的半径
* mode： 处理模式(draw_border_line 绘制边框线 / none 不绘制)
* line_size：线的直径
* color：线的颜色
*/
{
	Using_Graph(graph);/* 使用数据 */
	if(Valid_Graph(graph) && graph->flag == HAVE_ALPHA)
	{/* 如果图形数据有效,并且有alpha通道 */
		if(graph->width < radius*2 || graph->height < radius*2) 
			return 1; 
		int mini, count, x, y, temp, a, b, start_x, start_y;
		/* 圆的方程：(x－a)^2+(y－b)^2=r^2，学过数学的你懂的！ */
		/* 准备将左上角圆角化 */
		start_x = 0;
		start_y = 0;
		a = radius;
		b = radius;
		for(y = 0; y <= radius; ++y) {
			mini = -1;
			for(x = 0; x <= radius; ++x) {
				temp = radius * radius - (x - a) * (x - a) - (y - b) * (y - b) ;
				temp = abs(temp);
				if(x == 0) {
					mini = temp;
					count = y * graph->width + x;
					graph->rgba[3][count] = 0;/* 否则就让这个像素点透明 */
				} else {
					if(temp <= mini && temp != 0) {
						mini = temp;
						count = y * graph->width + x;
						graph->rgba[3][count] = 0;/* 否则就让这个像素点透明 */
					} else {
						//printf("draw line : y = %d, x = %d, temp = %d\n", y, x, temp);
						for(temp = x - 1; temp < x -1 + line_size; ++temp) 
						{/* 绘制边框线 */
							count = y * graph->width + temp;
							graph->rgba[0][count] = color->red;
							graph->rgba[1][count] = color->green;
							graph->rgba[2][count] = color->blue;
							graph->rgba[3][count] = 255;
							//printf("2222\n");
						}
						break;/* 如果这个点在圆上就退出本层for循环 */
					}
				}
			}
		}
	}
	End_Use_Graph(graph);
	return -1;
}

#endif

int Fill_Graph_Alpha(LCUI_Graph *src, unsigned char alpha)
/* 功能：填充图形的alpha通道的所有值 */
{
	if(Valid_Graph(src) && Graph_Have_Alpha(src)) {
		memset(src->rgba[3], alpha, sizeof(unsigned char) * src->width * src->height);
		return 0;
	}
	return -1;
}

int Draw_Graph_Border(LCUI_Graph *src,LCUI_RGB color, LCUI_Border border)
/* 功能：为图形边缘绘制矩形边框 */
{
	int x,y,count, k, start_x,start_y;
	if(Valid_Graph(src)) {
		/* 绘制上边的线 */
		Using_Graph(src, 1);
		for(y=0;y<border.top;++y) {
			k = y * src->width;
			for(x = 0; x < src->width; ++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		start_y = src->height - border.bottom;
		/* 绘制下边的线 */
		for(y=0;y<border.bottom;++y) {
			k = (start_y+y) * src->width;
			for(x=0;x<src->width;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* 绘制左边的线 */
		for(y=0;y<src->height;++y) {
			k = y * src->width;
			for(x=0;x<border.left;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* 绘制右边的线 */
		start_x = src->width - border.right;
		for(y=0;y<src->height;++y) {
			k = y * src->width + start_x;
			for(x=0;x<border.right;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(src->flag == HAVE_ALPHA) 
					src->rgba[3][count] = 255;
			}
		}
		/* 边框线绘制完成 */
		End_Use_Graph(src);
	}
	else return -1;
	return 0;
}

LCUI_RGBA RGBA_Mix(LCUI_RGBA back, LCUI_RGBA fore)
/* 功能：混合两个像素点的颜色 */
{
	if(fore.alpha == 255) {
		back.red	= fore.red;
		back.blue	= fore.blue;
		back.green	= fore.green;
	}
	else if(fore.alpha == 0);
	else {
		back.red	= (fore.red * fore.alpha + back.red * (255 - back.alpha)) /255;
		back.green	= (fore.green * fore.alpha + back.green * (255 - back.alpha)) /255;
		back.blue	= (fore.blue * fore.alpha + back.blue * (255 - back.alpha)) /255;
	}
	
	return back;
}

long max(long a, long b)
{
	return a > b ? a:b;
}

double radian(int angle) 
{
	return angle*3.1415926/180;
}

int Rotate_Graph(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des)
/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自：http://read.pudn.com/downloads154/sourcecode/graph/684994/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E5%9B%BE%E5%83%8F%E6%97%8B%E8%BD%AC/%E6%BA%90%E4%BB%A3%E7%A0%81/MyDIPView.cpp__.htm
 * 算法有待优化完善。
 */
{
	if(!Valid_Graph(src)) return -1;
    // 源图像的宽度和高度
    int    width, height;   
       
    // 旋转后图像的宽度和高度   
    int    new_width,new_height;

    // 旋转角度（弧度）   
    float   fRotateAngle;
    
    // 旋转角度的正弦和余弦   
    float   fSina, fCosa;
       
    // 源图四个角的坐标（以图像中心为坐标系原点）   
    float   fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;   
       
    // 旋转后四个角的坐标（以图像中心为坐标系原点）   
    float   fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
    
    // 两个中间常量   
    float   f1,f2;
    
    // 获取图像的"宽度"（4的倍数）   
    width = src->width;
       
    // 获取图像的高度   
    height = src->height;   
       
    // 将旋转角度从度转换到弧度   
    fRotateAngle = (float) radian(rotate_angle);   
       
    // 计算旋转角度的正弦   
    fSina = (float) sin((double)fRotateAngle);   
       
    // 计算旋转角度的余弦   
    fCosa = (float) cos((double)fRotateAngle);   
    
    // 计算原图的四个角的坐标（以图像中心为坐标系原点）   
    fSrcX1 = (float) (- (width  - 1) / 2);   
    fSrcY1 = (float) (  (height - 1) / 2);   
    fSrcX2 = (float) (  (width  - 1) / 2);   
    fSrcY2 = (float) (  (height - 1) / 2);   
    fSrcX3 = (float) (- (width  - 1) / 2);   
    fSrcY3 = (float) (- (height - 1) / 2);   
    fSrcX4 = (float) (  (width  - 1) / 2);   
    fSrcY4 = (float) (- (height - 1) / 2);   
       
    // 计算新图四个角的坐标（以图像中心为坐标系原点）   
    fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;   
    fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;   
    fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;   
    fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;   
    fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;   
    fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;   
    fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;   
    fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;   
       
    // 计算旋转后的图像实际宽度   
    new_width  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);   
    // 计算旋转后的图像高度   
    new_height = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);   
       
    // 两个常数，这样不用以后每次都计算了   
    f1 = (float) (-0.5 * (new_width - 1) * fCosa - 0.5 * (new_height - 1) * fSina   
        + 0.5 * (width  - 1));   
    f2 = (float) ( 0.5 * (new_width - 1) * fSina - 0.5 * (new_height - 1) * fCosa   
        + 0.5 * (height - 1));   
       
    if(Valid_Graph(des))
		Free_Graph(des);/* 先将这个内存释放 */
		
    des->flag = src->flag;
    // 分配内存，储存新的图形
    if(Malloc_Graph(des, new_width, new_height) != 0)
		return -1;

	long m, n, z;
	long src_x, src_y, des_x, des_y;
    // 针对图像每行进行操作
    Using_Graph(src, 0);
    Using_Graph(des, 1);
    for(des_y = 0; des_y < new_height; ++des_y)   
    {
		m = new_width * des_y;
        // 针对图像每列进行操作   
        for(des_x = 0; des_x < new_width; ++des_x)   
        {
			n = m + des_x;
            // 计算该象素在源图中的坐标   
            src_y = (long) (-((float) des_x) * fSina + ((float) des_y) * fCosa + f2 + 0.5);   
            src_x = (long) ( ((float) des_x) * fCosa + ((float) des_y) * fSina + f1 + 0.5);   
               
            // 判断是否在源图范围内   
            if( (src_x >= 0) && (src_x < width) && (src_y >= 0) && (src_y < height))   
            {
                // 指向源DIB第i0行，第j0个象素的指针
                z = width * src_y + src_x;
                des->rgba[0][n] = src->rgba[0][z];
                des->rgba[1][n] = src->rgba[1][z];
                des->rgba[2][n] = src->rgba[2][z];
                if(des->flag == HAVE_ALPHA)
					des->rgba[3][n] = src->rgba[3][z];
            }
            else   
            {
                // 对于源图中没有的象素，直接赋值为255   
                des->rgba[0][n] = 255;
                des->rgba[1][n] = 255;
                des->rgba[2][n] = 255;
                if(des->flag == HAVE_ALPHA)
					des->rgba[3][n] = 0;
            }
        }
    }
    End_Use_Graph(src);
    End_Use_Graph(des);
    return 0;
}
/********************** Graphics Processing End ***********************/

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue)
/* 功能：获取与指定区域重叠的部件 */
{
	int i, total;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_Widget *child; 
	LCUI_Queue *widget_list;
	 
	if(widget == NULL) 
		widget_list = &LCUI_Sys.widget_list; 
	else {
		if(widget->visible == IS_FALSE) 
			return;
		widget_list = &widget->child; 
	}
	
	total = Queue_Get_Total(widget_list); 
	
	for(i=total-1; i>=0; --i) {/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(widget_list, i); 
		if(child != NULL && child->visible == IS_TRUE) {
		/* 如果有可见的子部件 */ 
			tmp = Get_Widget_Valid_Rect(child); 
			pos = Get_Widget_Global_Pos(child);
			tmp.x += pos.x;
			tmp.y += pos.y;
			if(!Rect_Valid(tmp)) continue;
			if (Rect_Is_Overlay(tmp, rect)) { 
				/* 记录与该区域重叠的部件 */
				Queue_Add_Pointer(queue, child);
				/* 递归调用 */
				Get_Overlay_Widget(rect, child, queue);  
			} 
		}
	}
}

LCUI_RGBA Get_Graph_Pixel(LCUI_Graph *graph, LCUI_Pos pos)
/* 功能：获取图像中指定坐标的像素点的颜色 */
{
	int i;
	LCUI_RGBA rgba;
	
	i = graph->width*pos.y + pos.x; 
	
	rgba.red = graph->rgba[0][i];
	rgba.green = graph->rgba[1][i];
	rgba.blue = graph->rgba[2][i];
	
	if(Graph_Have_Alpha(graph))
		rgba.alpha = graph->rgba[3][i];
	else 
		rgba.alpha = 255;
		
	return rgba;
}

int Graph_Is_Opaque(LCUI_Graph *graph)
/* 
 * 功能：检测图形是否为不透明 
 * 说明：完全透明则返回-1，不透明则返回1，有透明效果则返回0
 * */
{ 
	if( !Graph_Have_Alpha(graph) )
		return 1; 
	if( graph->alpha == 0 )
		return -1;
	else if(graph->alpha < 255)
		return 0;
		
	unsigned char *p;
	unsigned int size;
	
	size = graph->width * graph->height;
	
	for(p=graph->rgba[3]; *p==255 && p<graph->rgba[3]+size; ++p);
	if(p == graph->rgba[3]+size) return 1; 
	else return 0; 
}


int Widget_Layer_Is_Opaque(LCUI_Widget *widget)
/* 功能：判断部件图形是否不透明 */
{ 
	return Graph_Is_Opaque(&widget->graph);
}

int Widget_Layer_Not_Visible(LCUI_Widget *widget)
/* 功能：检测部件图形是否完全透明 */
{
	if(Graph_Is_Opaque(&widget->graph) == -1)
		return 1;
	return 0;
}

extern void count_time();
extern void end_count_time();

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph)
/* 
 * 功能：获取屏幕中指定区域内实际要显示的图形 
 * 说明：指定的区域必须是与部件区域不部分重叠的
 * */
{ 
	LCUI_Pos pos;
	LCUI_Widget *widget; 
	LCUI_Queue widget_buff;
	LCUI_Rect valid_area;
	LCUI_Graph buff;
	
	Graph_Init(&buff);
	Queue_Init(&widget_buff, sizeof(LCUI_Widget), NULL);
	Queue_Using_Pointer(&widget_buff); /* 只用于存放指针 */
	/* 检测这个区域是否有效 */
	if (rect.x < 0) return -1; 
	if (rect.y < 0) return -1; 
	if (rect.x + rect.width > Get_Screen_Width ()) return -1;
	if (rect.y + rect.height > Get_Screen_Height ()) return -1;
	
	if (rect.width <= 0 && rect.height <= 0) return -2;
	
	int i, total; 
	/* 根据指定的尺寸，分配内存空间，用于储存图形数据 */
	Malloc_Graph(graph, rect.width, rect.height);
	graph->flag = NO_ALPHA; 
	/* 获取与该区域重叠的部件，并记录至队列widget_buff中 */
	Get_Overlay_Widget(rect, NULL, &widget_buff); 
	buff.flag = HAVE_ALPHA;
	total = Queue_Get_Total(&widget_buff); 
	if(total > 0) {
		//printf("rect(%d,%d,%d,%d), list cover widget:\n",
		//rect.x, rect.y, rect.width, rect.height
		//);
		//printf("list cover widget:\n");
		//for(i=total-1; i>=0; --i) {
			///* 队列最末端是最前端显示的部件，所以从尾至头遍历 */
			//widget = (LCUI_Widget*)Queue_Get(&widget_buff, i);
			//print_widget_info(widget); 
			///* 如果图层完全不可见，即完全透明 */
			//switch(Graph_Is_Opaque(&widget->graph)) {
				//case -1:
				//Queue_Delete_Pointer(&widget_buff, i);
				//case 0: break;
				//case 1: goto skip_loop;
			//} 
		//} 
		i = -1;
//skip_loop:
		//printf("list end\n");
		//nobuff_print("mix graph layer, use time:");
		//count_time();
		total = Queue_Get_Total(&widget_buff);
		if(i <= 0){
			i=0;
			Cut_Graph (&LCUI_Sys.screen.buff, rect, graph);
		}
		for(; i<total; ++i) {
			widget = (LCUI_Widget*)Queue_Get(&widget_buff, i); 
			pos = Get_Widget_Global_Pos(widget);
			/* 获取部件中有效显示区域 */
			valid_area = Get_Widget_Valid_Rect(widget);
			/* 引用部件中的有效区域中的图形 */
			Quote_Graph(&buff, &widget->graph, valid_area); 
			//print_widget_info(widget);
			//printf("src size: %d, %d\n", widget->graph.width, widget->graph.height);
			//printf("cut rect: %d, %d, %d, %d\n",
				//valid_area.x, valid_area.y, 
				//valid_area.width, valid_area.height);
			/* 获取相对坐标 */
			pos.x = pos.x - rect.x + valid_area.x;
			pos.y = pos.y - rect.y + valid_area.y;
			//Print_Graph_Info(&buff);
			/* 叠加 */ 
			Mix_Graph(graph, &buff, pos);
		} 
		//end_count_time();
	} else {/* 否则，直接贴背景图 */ 
		Cut_Graph (&LCUI_Sys.screen.buff, rect, graph);
	}
	
	if (LCUI_Sys.cursor.visible == IS_TRUE) { /* 如果游标可见 */
		/* 如果该区域与游标的图形区域重叠 */ 
		if (Rect_Is_Overlay( rect, Get_Cursor_Rect()) ) {
			pos.x = LCUI_Sys.cursor.pos.x - rect.x;
			pos.y = LCUI_Sys.cursor.pos.y - rect.y;
			/* 将图形合成 */ 
			Mix_Graph (graph, &LCUI_Sys.cursor.graph, pos);
		}
	} 
	/* 释放队列占用的内存空间 */
	Destroy_Queue(&widget_buff);
	return 0;
}

