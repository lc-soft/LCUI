#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/font.h>

int main( void )
{
	int ret;
	LCUI_Graph img;
	LCUI_Pos pos = {0, 80};
	LCUI_Rect area = {0, 0, 320, 240};
	LCUI_TextLayer txt = TextLayer_New();
	LCUI_TextStyleRec txtstyle;

	/* 初始化字体处理功能 */
	LCUI_InitFontLibrary();

	/* 创建一个图像，并使用灰色填充 */
	Graph_Init( &img );
	Graph_Create( &img, 320, 240 );
	Graph_FillRect( &img, RGB( 240, 240, 240 ), NULL, FALSE );

	/* 设置文本的字体大小 */
	TextStyle_Init( &txtstyle );
	txtstyle.pixel_size = 24;
	txtstyle.has_pixel_size = TRUE;

	/* 设置文本图层的固定尺寸、文本样式、文本内容、对齐方式 */
	TextLayer_SetFixedSize( txt, 320, 240 );
	TextLayer_SetTextStyle( txt, &txtstyle );
	TextLayer_SetTextAlign( txt, SV_CENTER );
	TextLayer_SetTextW( txt, L"这是一段测试文本\nHello, World!", NULL );
	TextLayer_Update( txt, NULL );

	/* 将文本图层绘制到图像中，然后将图像写入至 png 文件中 */
	TextLayer_RenderTo( txt, area, pos, &img );
	ret = LCUI_WritePNGFile( "test_string_render.png", &img );
	Graph_Free( &img );

	/* 释放字体处理功能相关资源 */
	LCUI_FreeFontLibrary();
	return ret;
}
