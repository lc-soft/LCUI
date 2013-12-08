#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H

#ifdef USE_LIBPNG
#include <png.h>
#endif

#define PNG_BYTES_TO_CHECK 4

/* 载入png图片文件 */
LCUI_API int Graph_LoadPNG( const char *filepath, LCUI_Graph *out )
{
#ifdef USE_LIBPNG
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	char buf[PNG_BYTES_TO_CHECK];
	int i, j, temp, pos, color_type, channels;
	png_bytep* row_pointers;

	fp = fopen(filepath, "rb");
	if(fp == NULL) { 
		return FILE_ERROR_OPEN_ERROR;
	}
	
	png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);
	
	setjmp(png_jmpbuf(png_ptr)); /* 这句很重要 */
	
	temp = fread(buf,1,PNG_BYTES_TO_CHECK,fp);
	temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
	
	if (temp != 0) {/* 如果不是PNG图片文件 */
		return FILE_ERROR_UNKNOWN_FORMAT;
	}
	
	rewind(fp);
	/* 开始读文件 */
	png_init_io(png_ptr, fp); 
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	channels = png_get_channels(png_ptr, info_ptr); /* 通道数 */
	color_type = png_get_color_type(png_ptr, info_ptr); /* 色彩类型 */
	/* row_pointers里边就是rgba数据 */
	row_pointers = png_get_rows(png_ptr, info_ptr);
	//printf("channels: %d, colortype: %d\n", channels, color_type);
	
	/*如果是RGB+alpha通道，或者RGB+其它字节*/
	if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		/* 开始分配内存 */
		out->color_type = COLOR_TYPE_RGBA; 
		temp = Graph_Create( out, 
				png_get_image_width(png_ptr, info_ptr), 
				png_get_image_height(png_ptr, info_ptr) );
		if(temp != 0) {
			fclose(fp);
			_DEBUG_MSG("error: %s", MALLOC_ERROR);
			return 1;
		}
		
		temp = (4 * out->w);
		for(pos=0,i=0; i < out->h; i++) 
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
		out->color_type = COLOR_TYPE_RGB;
		temp = Graph_Create( out, 
				png_get_image_width(png_ptr, info_ptr), 
				png_get_image_height(png_ptr, info_ptr) );
		if(temp != 0) {
			fclose(fp);
			_DEBUG_MSG("error: %s", MALLOC_ERROR);
			return 1;
		}
		
		temp = (3 * out->w);
		for(pos=0,i=0; i < out->h; i++)
		for(j=0; j < temp; j += 3) {
			out->rgba[0][pos] = row_pointers[i][j]; // red
			out->rgba[1][pos] = row_pointers[i][j+1];// green
			out->rgba[2][pos] = row_pointers[i][j+2];// blue
			++pos;
		} 
	} else {
		fclose(fp);
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
	int j, i, row_size, pos;
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
		goto error_exit;
	}
	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) {
		_DEBUG_MSG("png_create_write_struct failed\n");
		goto error_exit;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		_DEBUG_MSG("png_create_info_struct failed\n");
		goto error_exit;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during init_io\n");
		goto error_exit;
	}
	png_init_io(png_ptr, fp);

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during writing header\n");
		goto error_exit;
	}
	Graph_Lock(graph);
	if(Graph_HaveAlpha(graph)) {
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	} else {
		color_type = PNG_COLOR_TYPE_RGB;
	}
	
	png_set_IHDR(png_ptr, info_ptr, graph->w, graph->h,
		8, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during writing bytes\n");
		Graph_Unlock(graph);
		goto error_exit;
	}
	if(Graph_HaveAlpha(graph)) {
		row_size = sizeof(uchar_t) * 4 * graph->w;
	} else {
		row_size = sizeof(uchar_t) * 3 * graph->w;
	}
	
	row_pointers = (png_bytep*)malloc(graph->h*sizeof(png_bytep));
	for(i=0,pos=0; i < graph->h; i++) {
		row_pointers[i] = (png_bytep)malloc(row_size);
		for(j=0; j < row_size; ++pos) {
			row_pointers[i][j++] = graph->rgba[0][pos]; // red
			row_pointers[i][j++] = graph->rgba[1][pos]; // green
			row_pointers[i][j++] = graph->rgba[2][pos];   // blue
			if( graph->color_type == COLOR_TYPE_RGBA ) {
				row_pointers[i][j++] = graph->rgba[3][pos]; // alpha 
			}
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) {
		_DEBUG_MSG("error during end of write\n");
		Graph_Unlock(graph);
		goto error_exit;
	}
	png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
	for (j=0; j<graph->h; j++) {
		free(row_pointers[j]);
	}
	free(row_pointers);
	
	fclose(fp);
	Graph_Unlock(graph);
#else
	printf("warning: not PNG support!"); 
#endif
	return 0;
error_exit:
	fclose(fp);
	return -1;
}

