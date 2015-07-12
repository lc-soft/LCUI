#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/widget.h>
#include <LCUI/display.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget/textview.h>

#include <io.h>
#include <fcntl.h>

/* 在运行程序时会打开控制台，以查看打印的调试信息 */
static void InitConsoleWindow(void)
{
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT );
	hf=_fdopen( hCrt, "w" );
	*stdout=*hf;
	setvbuf (stdout, NULL, _IONBF, 0);
	// test code
	printf ("InitConsoleWindow OK!\n");
}

void onTimer( void *arg )
{
	static int mode = LDM_SEAMLESS;
	_DEBUG_MSG("tip\n");
	LCUIDisplay_SetMode( mode );
	if( mode == LDM_SEAMLESS ) {
		mode = LDM_WINDOWED;
	} else {
		mode = LDM_SEAMLESS;
	}
}

void test_gen_font_code(void)
{
	LCUI_Graph font_img;
	uchar_t *bp, *src_row_bp, *row_bp;
	int i, ch, x, y, base_y;
	char str[64];
	LCUI_Rect rect;
	LCUI_FontBitmap bitmap;
	struct {
		char name[64];
		int is_solid;
		int width;
		int size;
		int line_height;
	} config[7] = {
		{ "font_inconsolata_12px", 1, 6, 12, 20 },
		{ "font_inconsolata_13px", 1, 7, 13, 20 },
		{ "font_inconsolata_14px", 1, 7, 14, 20 },
		{ "font_inconsolata_15px", 1, 8, 15, 22 },
		{ "font_inconsolata_16px", 1, 8, 16, 24 },
		{ "font_inconsolata_17px", 1, 9, 17, 26 },
		{ "font_inconsolata_18px", 1, 9, 18, 26 }
	};
	Graph_Init(&font_img);
	for( i=0; i<7; ++i ) {
		LCUI_BOOL has_pixel;
		int ch_left, ch_right, ch_top, ch_bottom;
		FILE *font_index_fp, *font_data_fp;
		uchar_t *byte_cursor;

		sprintf(str, "%s.png", config[i].name);
		if( Graph_LoadImage(str, &font_img) != 0 ) {
			continue;
		}
		byte_cursor = NULL;
		base_y = (int)(config[i].line_height * 4.0/5.0);
		sprintf(str, "%s-index.c", config[i].name);
		font_index_fp = fopen(str, "wt+");
		sprintf(str, "%s-data.c", config[i].name);
		font_data_fp = fopen(str, "wt+");
		src_row_bp = font_img.bytes;
		fprintf( font_index_fp, 
			 "#include <LCUI_Build.h>\n"
			 "#include <LCUI/LCUI.h>\n"
			 "#include <LCUI/font.h>\n\n"
			 "/* %s */\n"
			 "static const LCUI_FontBMP font_bitmap_info[] = {\n", config[i].name);
		fprintf(font_data_fp, "/* %s */\n", config[i].name);
		fprintf(font_data_fp, "static const unsigned char font_bitmap_bytes[] = {\n");
		for( ch = ' '; ch <= '~'; ++ch ) {
			bitmap.top = -1000;
			bitmap.left = 0;
			bitmap.rows = 0;
			bitmap.width = 0;
			row_bp = src_row_bp;
			ch_top = ch_bottom = -1;
			for( y=0; y<config[i].line_height; ++y ) {
				bp = row_bp;
				has_pixel = FALSE;
				ch_right = ch_left = -1;
				for( x = 0; x <= config[i].width; ++x ) {
					if( *bp == 255 ) {
						DEBUG_MSG( "     ");
					} else {
						if( ch_left == -1 ) {
							ch_left = x;
						} else {
							ch_right = x;
						}
						has_pixel = TRUE;
						DEBUG_MSG( "0x%02x,", 255 - *bp );
					}
					bp += font_img.bytes_per_pixel;
				}
				DEBUG_MSG("%d\n", y);
				if( ch_left >= 0 && ch_right > ch_left && ch_right - ch_left >= bitmap.width ) {
					bitmap.width = ch_right - ch_left + 1;
					bitmap.left = ch_left;
				}
				if( has_pixel ) {
					if( ch_top == -1 ) {
						ch_top = y;
					} else {
						ch_bottom = y;
					}
				}
				row_bp += font_img.bytes_per_row;
			}
			if( ch_top >= 0 && ch_bottom > ch_top ) {
				bitmap.top = base_y - ch_top;
				 bitmap.rows = ch_bottom - ch_top + 1;
			}
			if( bitmap.top == -1000 ) {
				bitmap.top = base_y;
			}
			// 如果是等宽字体
			if( config[i].is_solid ) {
				bitmap.advance.x = config[i].width;
				bitmap.advance.y = config[i].size;
			} else {
				bitmap.left = 0;
				bitmap.advance.x = bitmap.width;
				bitmap.advance.y = config[i].size;
			}
			rect.x = 0;
			rect.y = (ch - ' ') * config[i].line_height;
			rect.width = 12;
			rect.height = config[i].line_height;
			//Graph_Quote(&ch_img, &font_img, &rect);
			//sprintf(str, "%s-%d.png", config[i].name, ch);
			//Graph_WritePNG(str, &ch_img);
			row_bp = src_row_bp;
			bitmap.pitch = bitmap.rows;
			DEBUG_MSG( "%c: {%d,%d,%d,%d,%d,%p,8,0,{%d,%d}},\n", 
				 ch, bitmap.top, bitmap.left, bitmap.width, bitmap.rows,
				 bitmap.pitch,byte_cursor, bitmap.advance.x, bitmap.advance.y);
			fprintf( font_index_fp, "\t{%d,%d,%d,%d,%d,(unsigned char*)0x%p,8,0,{%d,%d}},\n", 
				 bitmap.top, bitmap.left, bitmap.width, bitmap.rows,
				 bitmap.pitch,byte_cursor, bitmap.advance.x, bitmap.advance.y);
			fprintf(font_data_fp, "\t/* ch = '%c' */\n", ch);
			ch_top = base_y - bitmap.top;
			for( y=0; y<config[i].line_height; ++y ) {
				bp = row_bp;
				if( y < ch_top || y >= ch_top + bitmap.rows ) {
					row_bp += font_img.bytes_per_row;
					continue;
				}
				fputc( '\t', font_data_fp );
				for( x = 0; x <= config[i].width; ++x ) {
					if( x >= bitmap.left && x < bitmap.left + bitmap.width ) {
						fprintf(font_data_fp, "0x%02x,", 255 - *bp);
						++byte_cursor;
					}
					bp += font_img.bytes_per_pixel;
				}
				fputc( '\n', font_data_fp );
				row_bp += font_img.bytes_per_row;
			}
			src_row_bp += font_img.bytes_per_row * config[i].line_height;
		}
		fprintf(font_index_fp, "};\n");
		fprintf(font_data_fp, "};\n");
		fclose(font_index_fp);
		fclose(font_data_fp);
		Graph_Free(&font_img);
	}
}

const wchar_t test_str[] = L"[size=12px]12px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=13px]13px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=14px]14px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=15px]15px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=16px]16px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=17px]17px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=18px]18px, 0123456789, hello,world! are you OK? I like this![/size]";

int main( int argc, char **argv )
{
	LCUI_Widget w, root, text, btn;
	LCUI_Graph *desktop_image = Graph_New();

	InitConsoleWindow();//test_gen_font_code();return 0;
	_DEBUG_MSG("test\n");
	LCUI_Init();
	LCUIDisplay_SetMode( LDM_WINDOWED );
	LCUIDisplay_SetSize( 960, 540 );
	w = LCUIWidget_New("debug-widget");
	text = LCUIWidget_New("textview");
	btn = LCUIWidget_New("button");
	Widget_Append( w, text );
	Widget_Append( w, btn );
	TextView_SetTextW( text, test_str);//测试文本内容，呵呵达！\nABCDEFG,abcdefg,[color=#ff0000]color font[/color]");
	Widget_Top( w );
	Widget_Show( w );
	Widget_Show( btn );
	Widget_Resize( w, 520, 240 );
	Widget_Resize( btn, 80, 40 );
	Widget_Move( w, 200, 200 );
	Widget_SetTitleW( w, L"测试" );
	Graph_LoadImage( "images/background-image.png", desktop_image );
	root = LCUIWidget_GetRoot();
	
	SetStyle( root->style, key_background_color, RGB(255,242,223), color );
	SetStyle( root->style, key_background_image, desktop_image, image );
	SetStyle( root->style, key_background_size, SV_COVER, style );
	Widget_Update( root, FALSE );

	SetStyle( w->style, key_background_color, ARGB(200,255,255,255), color );
	SetStyle( w->style, key_background_size_width, 0.50, scale ); 
	SetStyle( w->style, key_background_size_height, 200, px );
	SetStyle( w->style, key_background_position, SV_BOTTOM_CENTER, style );
	SetStyle( w->style, key_box_shadow_color, ARGB(200,0,122,204), color );
	SetStyle( w->style, key_box_shadow_x, 2, px );
	SetStyle( w->style, key_box_shadow_y, 2, px );
	SetStyle( w->style, key_box_shadow_spread, 0, px );
	SetStyle( w->style, key_box_shadow_blur, 8, px );
	SetStyle( w->style, key_border_top_width, 1, px );
	SetStyle( w->style, key_border_right_width, 1, px );
	SetStyle( w->style, key_border_bottom_width, 1, px );
	SetStyle( w->style, key_border_left_width, 1, px );
	SetStyle( w->style, key_border_top_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_right_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_bottom_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_left_color, RGB(0,122,204), color );
	Widget_Update( w, FALSE );
	return LCUI_Main();
}
