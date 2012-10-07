/*
 * 文本框部件，正在实现中，只添加了数据结构以及部分函数接口
 * 
 * 
 * 
 * 
 * */
#define DEBUG

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_DRAW_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_FONT_H
#include LC_ERROR_H 
#include <wchar.h>

typedef struct _LCUI_TextBox		LCUI_TextBox;

struct _LCUI_TextBox
{
	LCUI_Widget *text;
};

/************************* 基本的部件处理 ********************************/
static void 
TextBox_Init( LCUI_Widget *widget )
/* 初始化文本框相关数据 */
{
	LCUI_TextBox *textbox;
	
	textbox = Widget_Create_PrivData(widget, sizeof(LCUI_TextBox));
	textbox->text = Create_Widget( "label" );
	Widget_Container_Add( widget, textbox->text ); 
}

static void 
Destroy_TextBox(LCUI_Widget *widget)
/* 销毁文本框占用的资源 */
{
	
}

static void 
Exec_TextBox_Update( LCUI_Widget *widget )
/* 处理文本框的图形渲染 */
{
	Draw_Empty_Slot( &widget->graph, widget->size.w, widget->size.h );
}

static LCUI_TextLayer *
TextBox_Get_TextLayer( LCUI_Widget *widget )
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_PrivData( widget );
	return Get_Widget_PrivData( textbox->text );
}

void 
Process_TextBox_Drag(LCUI_Widget *widget, LCUI_DragEvent *event)
/* 处理鼠标对文本框的拖动事件 */
{
	
}

void 
Process_TextBox_Clicked(LCUI_Widget *widget, LCUI_Event *event)
/* 处理鼠标对文本框的点击事件 */
{
	
}

void Register_TextBox()
/* 注册文本框部件 */
{
	WidgetType_Add ( "text_box" );
	WidgetFunc_Add ( "text_box", TextBox_Init, FUNC_TYPE_INIT );
	WidgetFunc_Add ( "text_box", Exec_TextBox_Update, FUNC_TYPE_UPDATE );
	WidgetFunc_Add ( "text_box", Destroy_TextBox, FUNC_TYPE_DESTROY );
}

/**********************************************************************/

/************************ 文本框部件的扩展功能 ****************************/
/* 剪切板 */
//static LCUI_String clip_board;

int TextBox_Text_Add(LCUI_Widget *widget, char *new_text)
/* 在光标处添加文本 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	return TextLayer_Text_Add( layer, new_text );
}

int TextBox_Text_Paste(LCUI_Widget *widget)
/* 将剪切板的内容粘贴至文本框 */
{
	return 0;
}


int TextBox_Text_Backspace(LCUI_Widget *widget, int n)
/* 删除光标左边处n个字符 */
{
	return 0;
}

int TextBox_Text_Delete(LCUI_Widget *widget, int n)
/* 删除光标右边处n个字符 */
{
	return 0;
}


LCUI_Pos TextBox_Get_Pixel_Pos(LCUI_Widget *widget, uint32_t char_pos)
/* 根据源文本中的位置，获取该位置的字符相对于文本框的坐标 */
{
	LCUI_Pos pos;
	pos.x = pos.y = 0;
	return pos;
}

uint32_t TextBox_Get_Char_Pos( LCUI_Widget *widget, LCUI_Pos pixel_pos )
/* 根据文本框的相对坐标，获取该坐标对应于源文本中的字符 */
{
	return 0;
}

int TextBox_Get_Select_Text( LCUI_Widget *widget, char *out_text )
/* 获取文本框内被选中的文本 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	return TextLayer_Get_Select_Text( layer, out_text );
}

int TextBox_Copy_Select_Text(LCUI_Widget *widget)
/* 复制文本框内被选中的文本 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	return TextLayer_Copy_Select_Text( layer );
}

int TextBox_Cut_Select_Text(LCUI_Widget *widget)
/* 剪切文本框内被选中的文本 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	return TextLayer_Cut_Select_Text( layer );
}

void TextBox_Using_StyleTags(LCUI_Widget *widget, BOOL flag)
/* 指定文本框是否处理控制符 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Using_StyleTags( layer, flag );
}

/**********************************************************************/
