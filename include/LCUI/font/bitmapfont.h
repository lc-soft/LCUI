extern uchar_t const *in_core_font_8x8();

BOOL FontBMP_Valid(LCUI_FontBMP *bitmap);
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回帧，无效返回假
 */

void Print_FontBMP_Info(LCUI_FontBMP *bitmap);
/* 功能：打印位图的信息 */

void FontBMP_Init(LCUI_FontBMP *bitmap);
/* 初始化字体位图 */

void FontBMP_Free(LCUI_FontBMP *bitmap);
/* 释放字体位图占用的资源 */

int FontBMP_Create(LCUI_FontBMP *bitmap, int width, int rows);
/* 功能：创建字体位图 */

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap);
/* 功能：根据字符编码，获取已内置的字体位图 */

void Set_Default_Font(char *fontfile);
/* 
 * 功能：设定默认的字体文件路径
 * 说明：需要在LCUI初始化前使用，因为LCUI初始化时会打开默认的字体文件
 *  */

/* 初始化字体处理模块 */
void LCUIModule_Font_Init( void );

/* 停用字体处理模块 */
void LCUIModule_Font_End( void );

void Font_Init(LCUI_Font *in);
/* 
 * 功能：初始化Font结构体数据
 * 说明：默认是继承系统的字体数据
 * */

void Font_Free(LCUI_Font *in);
/* 功能：释放Font结构体数据占用的内存资源 */

void LCUI_Font_Free();
/* 功能：释放LCUI默认的Font结构体数据占用的内存资源 */

int Show_FontBMP(LCUI_FontBMP *fontbmp);
/* 功能：在屏幕打印以0和1表示字体位图 */

int FontBMP_Mix( LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag );
/* 功能：将字体位图绘制到背景图形上 */

int Open_Fontfile(LCUI_Font *font_data, char *fontfile);
/* 打开指定路径中的字体文件，并保存数据至LCUI_Font结构体中 */

/* 获取现有的字体位图数据 */
LCUI_FontBMP *
Get_ExistFontBMP( LCUI_Font *font_data, wchar_t ch, int pixel_size );
