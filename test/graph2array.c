/*
 * 此程序用于将图形数据转换成数组，以实现图形数据内置于程序
 * */

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H

#define MAX_ROW_PIXEL_NUM 11

char help_text[]={
	"picture file to C code."
	"usage:\n"
	"%s image_file_path\n"
};

static void
get_filename(char *filepath, char *out_name)
/* 根据文件路径，获取文件名 */
{
	int m,n = 0;
	char *p;
	for(m=0; m<strlen(filepath); ++m) {
		if(filepath[m]=='/')
			n = m+1;
	}
	p = filepath + n;
	strcpy(out_name, p);
}

static int
graph_conv( LCUI_Graph *buff, char *out_filename )
{
	int i;
	FILE *fp;
	char file[1024], name[1024];

	get_filename( out_filename, name );
	sprintf( file, "%s.c", name );
	printf("output file: %s\n", file);

	fp = fopen( file, "w+" );
	if( !fp ) {
		return -1;
	}
	fprintf( fp,
		"/* this code generate from file %s */\n"
		"int load_my_img ( LCUI_Graph *buff )\n"
		"{\n"
		"	int ret;\n",
		name
	);
	/* red channel */
	fprintf( fp, "	unsigned char red[] = {" );
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n		" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[0][i] );
	}
	fprintf( fp, "\n	};\n" );

	/* green channel */
	fprintf( fp, "	unsigned char green[] = {" );
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n		" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[1][i] );
	}
	fprintf( fp, "\n	};\n" );

	/* blue channel */
	fprintf( fp, "	unsigned char blue[] = {" );
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n		" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[2][i] );
	}
	fprintf( fp, "\n	};\n" );

	/* alpha channel */
	if( buff->have_alpha ) {
		fprintf( fp, "	unsigned char alpha[] = {" );
		for( i=0; i<buff->width*buff->height; ++i ) {
			if( i%MAX_ROW_PIXEL_NUM == 0 ) {
				fprintf( fp, "\n		" );
			}
			fprintf( fp, "0x%02x,", buff->rgba[3][i] );
		}
		fprintf( fp, "\n	};\n" );
	}

	fprintf( fp,
		"	if( Graph_Valid( buff ) ) {\n"
		"		Graph_Free( buff );\n"
		"	}\n"
		"	Graph_Init( buff );\n"
		"	buff->have_alpha = %s;\n"
		"	buff->alpha = 255;\n"
		"	buff->type = %d;\n"
		"	ret = Graph_Create( buff, %d, %d );\n"
		"	if( ret == 0 ) {\n"
		"		memcpy( buff->rgba[0], red, sizeof(red) );\n"
		"		memcpy( buff->rgba[1], green, sizeof(green) );\n"
		"		memcpy( buff->rgba[2], blue, sizeof(blue) );\n"
		"		if( buff->have_alpha ) {\n"
		"			memcpy( buff->rgba[3], alpha, sizeof(alpha) );\n"
		"		}\n"
		"	}\n"
		"	return ret;\n"
		"}\n",
		buff->have_alpha?"TRUE":"FALSE",
		buff->type, buff->width, buff->height
	);
	fclose( fp );
	return 0;
}

int main( int argc, char *argv[] )
{
	LCUI_Graph graph;

	Graph_Init( &graph );

	if( argc == 2 ) {
		Graph_LoadImage( argv[1], &graph );
		graph_conv( &graph, argv[1] );
	} else {
		printf(help_text, argv[0]);
	}
	return 0;
}
