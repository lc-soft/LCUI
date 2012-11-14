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
#include LC_LABEL_H
#include LC_INPUT_H
#include LC_ERROR_H 

typedef struct _LCUI_TextBox
{
	LCUI_Widget *text;
	LCUI_Widget *cursor;
}
LCUI_TextBox;

static LCUI_Widget *active_textbox = NULL; 
/************************* 基本的部件处理 ********************************/
static void
_put_textbox_cursor( LCUI_Widget *widget, void *arg )
{
	active_textbox = widget;
}

static void
set_textbox_cursor_despos( LCUI_Pos pos )
{
	LCUI_Pos pixel_pos;
	LCUI_TextBox *tb;
	LCUI_TextLayer *layer;
	Text_RowData *row_ptr;
	
	tb = Get_Widget_PrivData( active_textbox ); 
	layer = Label_Get_TextLayer( tb->text );
	pixel_pos = TextLayer_Set_Pixel_Pos( layer, pos );
	pixel_pos.y += 2;
	pos = layer->current_des_pos;
	row_ptr = Queue_Get( &layer->rows_data, pos.y );
	Move_Widget( tb->cursor, pixel_pos );
	Resize_Widget( tb->cursor, Size(1, row_ptr->max_size.h) );
}

static void 
hide_textbox_cursor( )
{
	if( !active_textbox ) {
		return;
	}
	
	LCUI_TextBox *tb;
	tb = Get_Widget_PrivData( active_textbox );
	Hide_Widget( tb->cursor );
}

static void 
show_textbox_cursor( )
{
	if( !active_textbox ) {
		return;
	}
	
	LCUI_TextBox *tb;
	tb = Get_Widget_PrivData( active_textbox );
	Show_Widget( tb->cursor );
}

static void 
blink_cursor()
/* 闪烁文本框中的光标 */
{
	static int status = 0;
	if(status == 0) {
		show_textbox_cursor();
		status = 1;
	} else {
		hide_textbox_cursor();
		status = 0;
	}
}

static void 
TextBox_TextLayer_Click( LCUI_Widget *widget, LCUI_DragEvent *event )
{
	LCUI_Pos pos;
	LCUI_TextBox *tb;
	active_textbox = widget;
	tb = Get_Widget_PrivData( active_textbox );
	pos = GlobalPos_ConvTo_RelativePos( tb->text, event->cursor_pos );
	//printf("pos: %d,%d\n", pos.x, pos.y);
	set_textbox_cursor_despos( pos );
}

static void
TextBox_Input( LCUI_Widget *widget, LCUI_Key *key )
{
	printf("you input %d\n", key->code);
	switch( key->code ) {
	    case KEY_LEFT:
		break;
	    case KEY_RIGHT:
		break;
	    case KEY_UP:
		break;
	    case KEY_DOWN:
		break;
	    default:break;
	}
}

static void 
TextBox_Init( LCUI_Widget *widget )
/* 初始化文本框相关数据 */
{
	LCUI_TextBox *textbox;
	
	Set_Widget_Padding( widget, Padding(2,2,2,2) );
	textbox = Widget_Create_PrivData(widget, sizeof(LCUI_TextBox));
	textbox->text = Create_Widget( "label" );
	textbox->cursor = Create_Widget( NULL );
	textbox->text->focus = FALSE;
	textbox->cursor->focus = FALSE;
	TextLayer_Using_StyleTags( Label_Get_TextLayer(textbox->text), FALSE );
	Widget_Container_Add( textbox->text, textbox->cursor ); 
	Widget_Container_Add( widget, textbox->text ); 
	Show_Widget( textbox->text );
	Set_Widget_BG_Mode( textbox->cursor, BG_MODE_FILL_BACKCOLOR );
	Set_Widget_Backcolor( textbox->cursor, RGB(0,0,0) );
	Resize_Widget( textbox->cursor, Size(1, 14) );
	Set_Widget_ClickableAlpha( textbox->cursor, 0, 1 );
	Set_Widget_ClickableAlpha( textbox->text, 0, 1 );
	/* 设定定时器，每1秒闪烁一次 */
	set_timer( 500, blink_cursor, TRUE );
	Widget_Drag_Event_Connect( widget, TextBox_TextLayer_Click );
	Widget_FocusIn_Event_Connect( widget, _put_textbox_cursor, NULL );
	Widget_Keyboard_Event_Connect( widget, TextBox_Input );
}

static LCUI_Widget*
TextBox_Get_Label( LCUI_Widget *widget )
/* 获取文本框部件内的label部件指针 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_PrivData( widget );
	return textbox->text;
}

static void 
Destroy_TextBox( LCUI_Widget *widget )
/* 销毁文本框占用的资源 */
{
	
}

static void 
Exec_TextBox_Draw( LCUI_Widget *widget )
/* 处理文本框的图形渲染 */
{
	Draw_Empty_Slot( &widget->graph, widget->size.w, widget->size.h );
}

static LCUI_TextLayer *
TextBox_Get_TextLayer( LCUI_Widget *widget )
{
	LCUI_Widget *label;
	label = TextBox_Get_Label( widget );
	return Label_Get_TextLayer( label );
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
	WidgetFunc_Add ( "text_box", Exec_TextBox_Draw, FUNC_TYPE_DRAW );
	WidgetFunc_Add ( "text_box", Destroy_TextBox, FUNC_TYPE_DESTROY );
}

/**********************************************************************/

/************************ 文本框部件的扩展功能 ****************************/
/* 剪切板 */
//static LCUI_String clip_board;

void TextBox_Text_Add(LCUI_Widget *widget, char *new_text)
/* 在光标处添加文本 */
{
	LCUI_Widget *label;
	label = TextBox_Get_Label( widget );
	Set_Label_Text( label, new_text );
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
