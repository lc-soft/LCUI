#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H

#ifdef USE_LIBPNG
/* 需要将png头文件中使用的__restrict改为restrict才能在VS2012下编译通过 */
#define __restrict restrict
#include <png.h>
#endif

#define PNG_BYTES_TO_CHECK 4

/* 载入png图片文件 */
LCUI_API int Graph_LoadPNG( const char *filepath, LCUI_Graph *out )
{
#ifdef USE_LIBPNG
	FILE *pic_fp;
	png_structp png_ptr;
	png_infop   info_ptr;
	char buf[PNG_BYTES_TO_CHECK];
	int i, j, temp, pos, color_type, channels;
	png_bytep* row_pointers;

	pic_fp = fopen(filepath, "rb");
	if(pic_fp == NULL) { 
		return FILE_ERROR_OPEN_ERROR;
	}
	
	png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);
	
	setjmp(png_jmpbuf(png_ptr)); /* 这句很重要 */
	
	temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);
	temp = png_sig_cmp((void*)buf,(png_size_t)0,PNG_BYTES_TO_CHECK);
	
	if (temp != 0) {/* 如果不是PNG图片文件 */
		return FILE_ERROR_UNKNOWN_FORMAT;
	}
	
	rewind(pic_fp);
	/* 开始读文件 */
	png_init_io(png_ptr, pic_fp); 
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	channels       = png_get_channels(png_ptr, info_ptr);/*获取通道数*/
	color_type     = png_get_color_type(png_ptr, info_ptr);/*颜色类型*/
	out->bit_depth = png_get_bit_depth(png_ptr, info_ptr);/*获取位深*/
	/* row_pointers里边就是rgba数据 */
	row_pointers = png_get_rows(png_ptr, info_ptr);
	//printf("channels: %d, colortype: %d\n", channels, color_type);
	
	out->type = TYPE_PNG;  /* 图片类型为png */
	/*如果是RGB+alpha通道，或者RGB+其它字节*/
	if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		/* 开始分配内存 */
		out->have_alpha = TRUE; 
		temp = Graph_Create( out, 
				png_get_image_width(png_ptr, info_ptr), 
				png_get_image_height(png_ptr, info_ptr)
			);
		if(temp != 0) {
			fclose(pic_fp);
			_DEBUG_MSG("error: %s", MALLOC_ERROR);
			return 1;
		}
		
		temp = (4 * out->width);
		for(pos=0,i=0; i < out->height; i++) 
		for(j=0; j < temp; j += 4) {
			out->rgba[0][pos] = row_pointers[i][j];// red
			out->rgba[1][pos] = row_pointers[i][j+1];// green
			out->rgba[2][pos] = row_pointers[i][j+2];// blue
			out->rgba[3][pos] = row_pointers[i][j+3];// alpha
			++pos;
		} 
	}
	else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
	/*如果是RGB通道*/
		out->have_alpha = FALSE;
		temp = Graph_Create( out, 
				png_get_image_width(png_ptr, info_ptr), 
				png_get_image_height(png_ptr, info_ptr)
			);
		if(temp != 0) {
			fclose(pic_fp);
			_DEBUG_MSG("error: %s", MALLOC_ERROR);
			return 1;
		}
		
		temp = (3 * out->width);
		for(pos=0,i=0; i < out->height; i++)
		for(j=0; j < temp; j += 3) {
			out->rgba[0][pos] = row_pointers[i][j]; // red
			out->rgba[1][pos] = row_pointers[i][j+1];// green
			out->rgba[2][pos] = row_pointers[i][j+2];// blue
			++pos;
		} 
	} else {
		return FILE_ERROR_UNKNOWN_FORMAT;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
#else
	_DEBUG_MSG("warning: not PNG support!"); 
#endif
	return 0;
}

/* 将图像数据写入至png文件 */
LCUI_API int Graph_WritePNG( const char *file_name, LCUI_Graph *graph )
{
#ifdef USE_LIBPNG
	FILE *fp;
	int j, i, temp, pos;
	png_byte color_type; 
	png_structp png_ptr;
	png_infop info_ptr; 
	png_bytep * row_pointers;
	
	if(!Graph_IsValid(graph)) {
		_DEBUG_MSG("graph is not valid\n");
		return -1;
	}
	
	/* create file */
	fp = fopen(file_name, "wb");
	if (!fp) {
		_DEBUG_MSG("file %s could not be opened for writing\n", file_name);
		return -1;
	}
	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) {
		_DEBUG_MSG("png_create_write_struct failed\n");
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		_DEBUG_MSG("png_create_info_struct failed\n");
		return -1;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during init_io\n");
		return -1;
	}
	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during writing header\n");
		return -1;
	}
	Graph_Lock(graph);
	if(Graph_HaveAlpha(graph)) {
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	} else {
		color_type = PNG_COLOR_TYPE_RGB;
	}
	
	png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
		graph->bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		_DEBUG_MSG("error during writing bytes\n");
		Graph_Unlock(graph);
		return -1;
	}
	if(Graph_HaveAlpha(graph)) {
		temp = (4 * graph->width);
	} else {
		temp = (3 * graph->width);
	}
	
	row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
	for(i=0,pos=0; i < graph->height; i++) {
		row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
		for(j=0; j < temp; ++pos) {
			row_pointers[i][j++] = graph->rgba[0][pos]; // red
			row_pointers[i][j++] = graph->rgba[1][pos]; // green
			row_pointers[i][j++] = graph->rgba[2][pos];   // blue
			if(Graph_HaveAlpha(graph)) {
				row_pointers[i][j++] = graph->rgba[3][pos]; // alpha 
			}
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during end of write\n");
		Graph_Unlock(graph);
		return -1;
	}
	png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
	for (j=0; j<graph->height; j++) {
		free(row_pointers[j]);
	}
	free(row_pointers);
	
	fclose(fp);
	Graph_Unlock(graph);
#else
	printf("warning: not PNG support!"); 
#endif
	return 0;
}

