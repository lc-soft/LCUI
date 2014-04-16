#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_GRAPH_H

#ifdef USE_LIBPNG
#include <png.h>
#endif

#define PNG_BYTES_TO_CHECK 4

/* 载入png图片文件 */
LCUI_API int Graph_LoadPNG( const char *filepath, LCUI_Graph *graph )
{
#ifdef USE_LIBPNG
        FILE *fp;
        png_structp png_ptr;
        png_infop info_ptr;
	png_bytep* row_pointers;
        char buf[PNG_BYTES_TO_CHECK];
	uchar_t *pPixel;
        int w, h, x, y, temp, color_type;

        fp = fopen(filepath, "rb");
        if( fp == NULL ) { 
                return FILE_ERROR_OPEN_ERROR;
        }
        
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        info_ptr = png_create_info_struct(png_ptr);

        setjmp(png_jmpbuf(png_ptr)); 
        /* 读取PNG_BYTES_TO_CHECK个字节的数据 */
        temp = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
        /* 若读到的数据并没有PNG_BYTES_TO_CHECK个字节 */
        if( temp < PNG_BYTES_TO_CHECK ) {
                fclose(fp);
		png_destroy_read_struct( &png_ptr, &info_ptr, 0);
                return FILE_ERROR_UNKNOWN_FORMAT;
        }
        /* 检测数据是否为PNG的签名 */
        temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
        /* 如果不是PNG的签名，则说明该文件不是PNG文件 */
        if( temp != 0 ) {
                fclose(fp);
		png_destroy_read_struct( &png_ptr, &info_ptr, 0);
                return FILE_ERROR_UNKNOWN_FORMAT;
        }
        /* 复位文件指针 */
        rewind(fp);
        /* 开始读文件 */
        png_init_io(png_ptr, fp); 
        /* 读取PNG图片信息 */
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
        /* 获取图像的色彩类型 */
        color_type = png_get_color_type(png_ptr, info_ptr);
	/* 获取图像的宽高 */
	w = png_get_image_width( png_ptr, info_ptr );
	h = png_get_image_height( png_ptr, info_ptr );
	/* 获取所有行像素数据，row_pointers里边就是rgba数据 */
        row_pointers = png_get_rows( png_ptr, info_ptr );
	/* 根据不同的色彩类型进行相应处理 */
        switch( color_type ) {
	case PNG_COLOR_TYPE_RGB_ALPHA:
                graph->color_type = COLOR_TYPE_ARGB; 
                temp = Graph_Create( graph, w, h );
		if( temp != 0 ) {
			fclose(fp);
			png_destroy_read_struct( &png_ptr, &info_ptr, 0);
			return FILE_ERROR_MALLOC_ERROR;
		}
		
		pPixel = graph->bytes;
                for( y=0; y<h; ++y ) {
			/*
			 * Graph的像素数据存储格式是BGRA，而PNG库
			 * 提供像素数据的是RGBA格式的，因此需要调整写入顺序
			 */
			for( x=0; x<w*4; x+=4 ) {
				*pPixel++ = row_pointers[y][x+2];
				*pPixel++ = row_pointers[y][x+1];
				*pPixel++ = row_pointers[y][x];
				*pPixel++ = row_pointers[y][x+3];
			}
		}
		break;

        case PNG_COLOR_TYPE_RGB:
                graph->color_type = COLOR_TYPE_RGB; 
                temp = Graph_Create( graph, w, h );
		if( temp != 0 ) {
			fclose(fp);
			png_destroy_read_struct( &png_ptr, &info_ptr, 0);
			return FILE_ERROR_MALLOC_ERROR;
		}
		
		pPixel = graph->bytes;
                for( y=0; y<h; ++y ) {
			for( x=0; x<w*3; x+=3 ) {
				*pPixel++ = row_pointers[y][x+2];
				*pPixel++ = row_pointers[y][x+1];
				*pPixel++ = row_pointers[y][x];
			}
		}
		break;
	/* 其它色彩类型的图像就读了 */
        default:
                fclose(fp);
		png_destroy_read_struct( &png_ptr, &info_ptr, 0);
                return FILE_ERROR_UNKNOWN_FORMAT;
        }
        png_destroy_read_struct( &png_ptr, &info_ptr, 0);
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
        int x, y, row_size, pos;
        png_byte color_type; 
        png_structp png_ptr;
        png_infop info_ptr; 
        png_bytep *row_pointers;
        uchar_t *pPixel;

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
                goto error_exit;
        }
	if( graph->color_type == COLOR_TYPE_ARGB ) {
                row_size = sizeof(uchar_t) * 4 * graph->w;
        } else {
                row_size = sizeof(uchar_t) * 3 * graph->w;
        }
        
	pPixel = graph->bytes;
        row_pointers = (png_bytep*)malloc( graph->h*sizeof(png_bytep) );
        for(y=0,pos=0; y < graph->h; y++) {
                row_pointers[y] = (png_bytep)malloc(row_size);
                for(x=0; x < row_size; ) {
                        row_pointers[y][x+2] = *pPixel++; // blue
                        row_pointers[y][x+1] = *pPixel++; // green
                        row_pointers[y][x] = *pPixel++;   // red
                        if( graph->color_type == COLOR_TYPE_ARGB ) {
                                row_pointers[y][x+3] = *pPixel++; // alpha 
				x += 4;
			} else {
				x += 3;
			}
                }
        }
        png_write_image( png_ptr, row_pointers );

        /* end write */
        if (setjmp(png_jmpbuf(png_ptr))) {
                _DEBUG_MSG("error during end of write\n");
                goto error_exit;
        }
        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for( y=0; y<graph->h; ++y ) {
                free( row_pointers[y] );
        }
        free( row_pointers );
        
        fclose( fp );
#else
        printf("warning: not PNG support!"); 
#endif
        return 0;
error_exit:
        fclose(fp);
        return -1;
}
