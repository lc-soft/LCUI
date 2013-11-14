/*
 * 此程序用于将图形数据转换成数组，以实现图形数据内置于程序
 * */

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H

#define MAX_ROW_PIXEL_NUM 11

static char help_text[]={
	"picture file to C code."
	"usage:\n"
	"%s image_file_path\n"
};

/** Converte image data to C code, and write to the source code file */
static int GraphConvertToCode( LCUI_Graph *buff, char *out_filepath )
{
	int i;
	FILE *fp;
	char file[1024];

	sprintf( file, "%s.c", out_filepath );
	printf("output file: %s\n", file);
	/* open output file */
	fp = fopen( file, "w+" );
	if( !fp ) {
		return -1;
	}
	fprintf( fp, "/* this code generate from file %s */\n", out_filepath );
	fprintf( fp, 
		"#include <LCUI_Build.h>\n"
		"#include LC_LCUI_H\n"
		"#include LC_GRAPH_H\n\n"
	);
	/* red channel */
	fprintf( fp, "static unsigned char graph_red[] = {" );
	for( i=0; i<buff->w*buff->h; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[0][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* green channel */
	fprintf( fp, "static unsigned char graph_green[] = {" );
	for( i=0; i<buff->w*buff->h; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[1][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* blue channel */
	fprintf( fp, "static unsigned char graph_blue[] = {" );
	for( i=0; i<buff->w*buff->h; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[2][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* alpha channel */
	if( buff->color_type == COLOR_TYPE_RGBA ) {
		fprintf( fp, "static unsigned char graph_alpha[] = {" );
		for( i=0; i<buff->w*buff->h; ++i ) {
			if( i%MAX_ROW_PIXEL_NUM == 0 ) {
				fprintf( fp, "\n\t" );
			}
			fprintf( fp, "0x%02x,", buff->rgba[3][i] );
		}
		fprintf( fp, "\n};\n\n" );
	}

	fprintf( fp,
		"int InCoreIMG_LoadMyIIMG( LCUI_Graph *buff )\n"
		"{\n"
		"	int ret;\n"
		"	if( Graph_IsValid( buff ) ) {\n"
		"		Graph_Free( buff );\n"
		"	}\n"
		"	Graph_Init( buff );\n"
		"	buff->color_type = %s;\n"
		"	buff->alpha = 255;\n"
		"	ret = Graph_Create( buff, %d, %d );\n"
		"	if( ret == 0 ) {\n"
		"		memcpy( buff->rgba[0], red, sizeof(red) );\n"
		"		memcpy( buff->rgba[1], green, sizeof(green) );\n"
		"		memcpy( buff->rgba[2], blue, sizeof(blue) );\n",
		buff->color_type == COLOR_TYPE_RGBA? "COLOR_TYPE_RGBA":"COLOR_TYPE_RGB",
		buff->w, buff->h
	);
	if( buff->color_type == COLOR_TYPE_RGBA ) {
		fprintf( fp,
			"		memcpy( buff->rgba[3], alpha, sizeof(alpha) );\n"
		);
	}
	fprintf( fp,
		"	}\n"
		"	return ret;\n"
		"}\n"
	);
	fclose( fp );
	return 0;
}

int main( int argc, char *argv[] )
{
	int ret;
	if( argc == 2 ) {
		LCUI_Graph graph;
		Graph_Init( &graph );
		ret = Graph_LoadImage( argv[1], &graph );
		if( ret != 0 ) {
			return -1;
		}
		ret = GraphConvertToCode( &graph, argv[1] );
		if( ret != 0 ) {
			return -2;
		}
		Graph_Free( &graph );
	} else {
		printf( help_text, argv[0] );
	}
	return 0;
}
