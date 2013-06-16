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

static int
graph_conv( LCUI_Graph *buff, char *out_filepath )
{
	int i;
	FILE *fp;
	char file[1024];

	sprintf( file, "%s.c", out_filepath );
	printf("output file: %s\n", file);

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
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[0][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* green channel */
	fprintf( fp, "static unsigned char graph_green[] = {" );
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[1][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* blue channel */
	fprintf( fp, "static unsigned char graph_blue[] = {" );
	for( i=0; i<buff->width*buff->height; ++i ) {
		if( i%MAX_ROW_PIXEL_NUM == 0 ) {
			fprintf( fp, "\n\t" );
		}
		fprintf( fp, "0x%02x,", buff->rgba[2][i] );
	}
	fprintf( fp, "\n};\n\n" );

	/* alpha channel */
	if( buff->have_alpha ) {
		fprintf( fp, "static unsigned char graph_alpha[] = {" );
		for( i=0; i<buff->width*buff->height; ++i ) {
			if( i%MAX_ROW_PIXEL_NUM == 0 ) {
				fprintf( fp, "\n\t" );
			}
			fprintf( fp, "0x%02x,", buff->rgba[3][i] );
		}
		fprintf( fp, "\n};\n\n" );
	}

	fprintf( fp,
		"int InCoreIMG_LoadMyIIMG ( LCUI_Graph *buff )\n"
		"{\n"
		"	int ret;\n"
		"	if( Graph_IsValid( buff ) ) {\n"
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
		"		memcpy( buff->rgba[2], blue, sizeof(blue) );\n",
		buff->have_alpha?"TRUE":"FALSE",
		buff->type, buff->width, buff->height
	);
	if( buff->have_alpha ) {
		fprintf( fp,
			"		if( buff->have_alpha ) {\n"
			"			memcpy( buff->rgba[3], alpha, sizeof(alpha) );\n"
			"		}\n" 
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
