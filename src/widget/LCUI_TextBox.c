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
#include LC_TEXTBOX_H
#include LC_LABEL_H
#include LC_SCROLLBAR_H
#include LC_INPUT_H
#include LC_ERROR_H 

typedef struct _LCUI_TextBox
{
	LCUI_Widget *text;
	LCUI_Widget *cursor;
	LCUI_Widget *scrollbar[2];
}
LCUI_TextBox;

/*----------------------------- Private ------------------------------*/
static LCUI_Widget *active_textbox = NULL; 

static void
_put_textbox_cursor( LCUI_Widget *widget, void *arg )
{
	active_textbox = widget;
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

static LCUI_TextLayer *
TextBox_Get_TextLayer( LCUI_Widget *widget )
/* 获取文本框部件内的文本图层指针 */
{
	LCUI_Widget *label;
	label = TextBox_Get_Label( widget );
	return Label_Get_TextLayer( label );
}

static void 
blink_cursor()
/* 闪烁文本框中的光标 */
{
	static int cur_status = 0;
	if(cur_status == 0) {
		show_textbox_cursor();
		cur_status = 1;
	} else {
		hide_textbox_cursor();
		cur_status = 0;
	}
}

static void 
TextBox_TextLayer_Click( LCUI_Widget *widget, LCUI_DragEvent *event )
{
	static LCUI_Pos pos;
	static LCUI_TextBox *tb;
	static LCUI_TextLayer *layer;
	
	/* 保存当前已获得焦点的部件 */
	active_textbox = widget;
	layer = TextBox_Get_TextLayer( widget );
	tb = Get_Widget_PrivData( active_textbox );
	/* 全局坐标转换成相对坐标 */
	pos = GlobalPos_ConvTo_RelativePos( tb->text, event->cursor_pos );
	//printf("pos: %d,%d\n", pos.x, pos.y);
	/* 根据像素坐标，设定文本光标的位置 */
	TextLayer_Set_Cursor_PixelPos( layer, pos );
	/* 获取光标的当前位置 */
	pos = TextLayer_Get_Cursor_Pos( layer );
	/* 主要是调用该函数更新当前文本浏览区域，以使光标处于显示区域内 */
	TextBox_Cursor_Move( widget, pos );
}

static void
TextBox_Input( LCUI_Widget *widget, LCUI_Key *key )
{
	static char buff[5];
	static int cols, rows;
	static LCUI_Pos cur_pos;
	static LCUI_TextLayer *layer;
	
	//printf("you input %d\n", key->code);
	layer = TextBox_Get_TextLayer( widget );
	cur_pos = TextLayer_Get_Cursor_Pos( layer );
	cols = TextLayer_Get_RowLen( layer, cur_pos.y );
	rows = TextLayer_Get_Rows( layer ); 
	switch( key->code ) {
	    case KEY_HOMEPAGE: //home键移动光标至行首
		cur_pos.x = 0;
		goto mv_cur_pos;
		
	    case KEY_END: //end键移动光标至行尾
		cur_pos.x = cols;
		goto mv_cur_pos;
		
	    case KEY_LEFT:
		if( cur_pos.x > 0 ) {
			cur_pos.x--;
		} else if( cur_pos.y > 0 ) {
			cur_pos.y--;
			cur_pos.x = TextLayer_Get_RowLen( layer, cur_pos.y );
		}
		goto mv_cur_pos;
		
	    case KEY_RIGHT:
		if( cur_pos.x < cols ) {
			cur_pos.x++;
		} else if( cur_pos.y < rows-1 ) {
			cur_pos.y++;
			cur_pos.x = 0;
		}
		goto mv_cur_pos;
		
	    case KEY_UP:
		if( cur_pos.y > 0 ) {
			cur_pos.y--;
		}
		goto mv_cur_pos;
		
	    case KEY_DOWN:
		if( cur_pos.y < rows-1 ) {
			cur_pos.y++;
		}
		/* 移动光标位置 */
mv_cur_pos:;
		TextBox_Cursor_Move( widget,cur_pos );
		break;
		
	    case KEY_BACKSPACE: //删除光标左边的字符
		TextBox_Text_Backspace( widget, 1 );
		break;
		
	    case KEY_DELETE:
		//删除光标右边的字符
		
		break;
		
	    default:;
		/* 如果该ASCII码代表的字符是可见的 */
		if( key->code == 10 || (key->code > 31 && key->code < 126) ) {
			//wchar_t *text;
			buff[0] = key->code;
			buff[1] = 0;
			TextBox_Text_Add( widget, buff);
			//text = TextLayer_Get_Text( layer );
			//free( text );
		}
		
	    //向文本框中添加字符
		break;
	}
}

static void 
TextBox_Scroll_TextLayer( ScrollBar_Data data, void *arg )
/* 滚动文本框内的文本图层 */
{
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg;
	printf("data: size: %d / %d, num: %d / %d\n", 
	data.current_size, data.max_size, data.current_num, data.max_num);
	LCUI_Pos pos;
	
	pos.x = 0;
	pos.y = data.current_num;
	pos.y = 0 - pos.y;
	printf("offset.y: %d\n", pos.y);
	TextBox_TextLayer_Set_Offset( widget, pos );
	Update_Widget( widget );
}

static void 
TextBox_Init( LCUI_Widget *widget )
/* 初始化文本框相关数据 */
{
	LCUI_TextBox *textbox;
	
	textbox = Widget_Create_PrivData(widget, sizeof(LCUI_TextBox));
	
	textbox->text = Create_Widget( "label" );
	textbox->cursor = Create_Widget( NULL );
	textbox->scrollbar[0] = Create_Widget( "scrollbar" );
	textbox->scrollbar[1] = Create_Widget( "scrollbar" );
	/* 不可获得焦点 */
	textbox->text->focus = FALSE;
	textbox->cursor->focus = FALSE;
	textbox->scrollbar[0]->focus = FALSE;
	textbox->scrollbar[1]->focus = FALSE;
	
	Label_AutoSize( textbox->text, FALSE, 0 );
	Set_Widget_Dock( textbox->text, DOCK_TYPE_FILL );
	
	/* 添加至相应的容器 */
	Widget_Container_Add( textbox->text, textbox->cursor ); 
	Widget_Container_Add( widget, textbox->text ); 
	Widget_Container_Add( widget, textbox->scrollbar[0] ); 
	Widget_Container_Add( widget, textbox->scrollbar[1] ); 
	/* 设置滚动条的尺寸 */
	Set_Widget_Size( textbox->scrollbar[0], "10px", NULL );
	Set_Widget_Dock( textbox->scrollbar[0], DOCK_TYPE_RIGHT );
	/* 将回调函数与滚动条连接 */
	ScrollBar_Connect( textbox->scrollbar[0], TextBox_Scroll_TextLayer, widget );
	
	Show_Widget( textbox->text );
	
	TextLayer_Using_StyleTags( Label_Get_TextLayer(textbox->text), FALSE );
	Set_Widget_Padding( widget, Padding(2,2,2,2) );
	Set_Widget_Backcolor( textbox->cursor, RGB(0,0,0) );
	Set_Widget_BG_Mode( textbox->cursor, BG_MODE_FILL_BACKCOLOR );
	
	Resize_Widget( textbox->cursor, Size(1, 14) );
	/* 设置可点击区域的alpha值要满足的条件 */
	Set_Widget_ClickableAlpha( textbox->cursor, 0, 1 );
	Set_Widget_ClickableAlpha( textbox->text, 0, 1 );
	/* 设定定时器，每1秒闪烁一次 */
	set_timer( 500, blink_cursor, TRUE );
	Widget_Drag_Event_Connect( widget, TextBox_TextLayer_Click );
	/* 关联FOCUS_IN 事件 */
	Widget_FocusIn_Event_Connect( widget, _put_textbox_cursor, NULL );
	Widget_Keyboard_Event_Connect( widget, TextBox_Input );
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

static LCUI_Widget *
TextBox_Get_Scrollbar( LCUI_Widget *widget, int which )
{
	LCUI_TextBox *tb;
	tb = Get_Widget_PrivData( widget );
	if( which == 0 ) {
		return tb->scrollbar[0];
	}
	return tb->scrollbar[1];
}

static void 
TextBox_ScrollBar_Update( LCUI_Widget *widget )
/* 更新文本框的滚动条 */
{
	LCUI_Size area_size, layer_size;
	LCUI_Widget *scrollbar;
	LCUI_TextLayer *layer;
	
	/* 获取文本图层 */
	layer = TextBox_Get_TextLayer( widget );
	/* 获取文本图层和文本框区域的尺寸 */
	layer_size = TextLayer_Get_Size( layer );
	area_size = Get_Container_Size( widget );
	/* 获取纵向滚动条 */
	scrollbar = TextBox_Get_Scrollbar( widget, 0 );
	//printf("area: %d,%d, layer: %d,%d\n", 
	//area_size.w, area_size.h, layer_size.w, layer_size.h);
	/* 如果文本图层高度超过显示区域 */
	if( area_size.h > 0 && layer_size.h > area_size.h ) {
		/* 修改滚动条中记录的最大值和当前值，让滚动条在更新后有相应的长度 */
		ScrollBar_Set_MaxSize( scrollbar, layer_size.h );
		ScrollBar_Set_CurrentSize( scrollbar, area_size.h );
		Show_Widget( scrollbar );
	} else {
		/* 不需要显示滚动条 */
		Hide_Widget( scrollbar );
	}
}

static void
Exec_TextBox_Update( LCUI_Widget *widget )
/* 更新文本框的文本图层 */
{
	printf("Exec_TextBox_Update(): enter\n");
	Exec_Update_Widget( TextBox_Get_Label( widget ) );
	TextBox_ScrollBar_Update( widget );
	TextBox_Cursor_Update( widget );
	printf("Exec_TextBox_Update(): quit\n");
}

static void
Exec_TextBox_Resize( LCUI_Widget *widget )
/* 在文本框改变尺寸后，进行附加处理 */
{
	/* 更新文本框内的文本图层 */
	Exec_Update_Widget( TextBox_Get_Label( widget ) );
	/* 更新文本框的滚动条 */
	TextBox_ScrollBar_Update( widget );
	/* 更新文本框的显示区域 */
	TextBox_ViewArea_Update( widget );
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
/*--------------------------- End Private ----------------------------*/


/*----------------------------- Public -------------------------------*/
/* 剪切板 */
//static LCUI_String clip_board;
void Register_TextBox()
/* 注册文本框部件 */
{
	WidgetType_Add ( "text_box" );
	WidgetFunc_Add ( "text_box", TextBox_Init, FUNC_TYPE_INIT );
	WidgetFunc_Add ( "text_box", Exec_TextBox_Draw, FUNC_TYPE_DRAW );
	WidgetFunc_Add ( "text_box", Exec_TextBox_Update, FUNC_TYPE_UPDATE );
	WidgetFunc_Add ( "text_box", Exec_TextBox_Resize, FUNC_TYPE_RESIZE );
	WidgetFunc_Add ( "text_box", Destroy_TextBox, FUNC_TYPE_DESTROY );
}

LCUI_Pos
TextBox_ViewArea_Get_Pos( LCUI_Widget *widget )
/* 获取文本显示区域的位置 */
{
	LCUI_Pos pos;
	LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	pos = layer->offset_pos;
	pos.x = -pos.x;
	pos.y = -pos.y;
	return pos;
}

int
TextBox_ViewArea_Update( LCUI_Widget *widget )
/* 更新文本框的文本显示区域 */
{
	static int cursor_h;
	static ScrollBar_Data scrollbar_data;
	static LCUI_Pos cursor_pos, area_pos;
	static LCUI_Size layer_size, area_size;
	static LCUI_Widget *scrollbar;
	static LCUI_TextLayer *layer;
	printf("TextBox_ViewArea_Update(): enter\n");
	/* 获取显示区域的尺寸 */
	area_size = Get_Container_Size( widget );
	if( area_size.h <= 0 || area_size.w <= 0 ) {
		return -1;
	}
	
	layer = TextBox_Get_TextLayer( widget );
	layer_size = TextLayer_Get_Size( layer );
	if( layer_size.h <= 0 || layer_size.w <= 0 ) {
		return -2;
	}
	/* 获取显示区域的位置 */
	area_pos = TextBox_ViewArea_Get_Pos( widget );
	/* 获取光标的坐标 */
	cursor_pos = TextLayer_Get_Cursor_FixedPixelPos( layer );
	/* 获取当前行的最大高度作为光标的高度 */
	cursor_h = TextLayer_CurRow_Get_MaxHeight( layer );
	
	/* 准备调整纵向滚动条的位置，先获取文本框的纵向滚动条 */
	scrollbar = TextBox_Get_Scrollbar( widget, 0 );
	/* 设定滚动条的数据中的最大值 */
	ScrollBar_Set_MaxNum( scrollbar, layer_size.h - area_size.h );
	
	printf("cursor_pos: %d,%d\n", cursor_pos.x, cursor_pos.y);
	printf("area_rect: %d,%d,%d,%d\n", area_pos.x, area_pos.y,
	area_size.w, area_size.h );
	printf("layer_size: %d,%d\n", layer_size.w, layer_size.h);
	/* 检测光标Y轴坐标是否超出显示区域的范围 */
	if( cursor_pos.y < area_pos.y ) {
		area_pos.y = cursor_pos.y;
	}
	if(cursor_pos.y + cursor_h > area_pos.y + area_size.h ) {
		printf("outrange\n");
		area_pos.y = cursor_pos.y + cursor_h - area_size.h;
		printf("area_pos.y: %d\n", area_pos.y);
	}
	/* 如果显示区域在Y轴上超过文本图层的范围 */
	if( area_pos.y + area_size.h > layer_size.h ) {
		area_pos.y = layer_size.h - area_size.h;
	}
	if( area_pos.y < 0 ) {
		area_pos.y = 0;
	}
	/* 设定滚动条的数据中的当前值 */
	ScrollBar_Set_CurrentNum( scrollbar, area_pos.y );
	/* 获取滚动条的数据，供滚动文本层利用 */
	scrollbar_data = ScrollBar_Get_Data( scrollbar );
	
	printf("scrollbar_data: size: %d / %d, num: %d / %d\n", 
	scrollbar_data.current_size, scrollbar_data.max_size, 
	scrollbar_data.current_num, scrollbar_data.max_num);
	/* 根据数据，滚动文本图层至响应的位置，也就是移动文本显示区域 */
	TextBox_Scroll_TextLayer( scrollbar_data, widget );
	printf("TextBox_ViewArea_Update(): quit\n");
	return 0;
}

LCUI_Widget*
TextBox_Get_Label( LCUI_Widget *widget )
/* 获取文本框部件内的label部件指针 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_PrivData( widget );
	return textbox->text;
}

LCUI_Widget *
TextBox_Get_Cursor( LCUI_Widget *widget )
/* 获取文本框部件内的光标 */
{
	LCUI_TextBox *tb;
	tb = Get_Widget_PrivData( widget );
	return tb->cursor;
}

void TextBox_Text(LCUI_Widget *widget, char *new_text)
/* 设定文本框显示的文本 */
{
	LCUI_Widget *label;
	label = TextBox_Get_Label( widget );
	Set_Label_Text( label, new_text );
	TextBox_ViewArea_Update( widget );
}

void TextBox_TextLayer_Set_Offset( LCUI_Widget *widget, LCUI_Pos offset_pos )
/* 为文本框内的文本图层设置偏移 */
{
	LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Set_Offset( layer, offset_pos );
	/* 需要更新光标的显示位置 */
	TextBox_Cursor_Update( widget );
}

void TextBox_Text_Add(LCUI_Widget *widget, char *new_text)
/* 在光标处添加文本 */
{
	static LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Add( layer, new_text );
	Update_Widget( widget );
	TextBox_ViewArea_Update( widget );
}

int TextBox_Text_Paste(LCUI_Widget *widget)
/* 将剪切板的内容粘贴至文本框 */
{
	return 0;
}


int TextBox_Text_Backspace(LCUI_Widget *widget, int n)
/* 删除光标左边处n个字符 */
{
	LCUI_Pos cur_pos;
	LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Backspace( layer, n );
	cur_pos = TextLayer_Get_Cursor_Pos( layer );
	TextBox_Cursor_Move( widget, cur_pos );
	Update_Widget( widget );
	return 0;
}

int TextBox_Text_Delete(LCUI_Widget *widget, int n)
/* 删除光标右边处n个字符 */
{
	return 0;
}


LCUI_Pos 
TextBox_Cursor_Update( LCUI_Widget *widget )
/* 更新文本框的光标，返回该光标的像素坐标 */
{
	LCUI_Pos pixel_pos;
	LCUI_Widget *cursor;
	LCUI_TextLayer *layer;
	LCUI_Size size;
	
	layer = TextBox_Get_TextLayer( widget );
	cursor = TextBox_Get_Cursor( widget );
	size.w = 1;
	size.h = TextLayer_CurRow_Get_MaxHeight( layer );
	pixel_pos = TextLayer_Get_Cursor_PixelPos( layer );
	Move_Widget( cursor, pixel_pos );
	Resize_Widget( cursor, size );
	Show_Widget( cursor ); /* 让光标在更新时显示 */
	return pixel_pos;
}

LCUI_Pos
TextBox_Cursor_Move( LCUI_Widget *widget, LCUI_Pos new_pos )
/* 移动文本框内的光标 */
{
	LCUI_Pos pixel_pos;
	LCUI_Widget *cursor;
	LCUI_TextLayer *layer;
	LCUI_Size size;
	
	layer = TextBox_Get_TextLayer( widget );
	cursor = TextBox_Get_Cursor( widget );
	size.w = 1;
	size.h = TextLayer_CurRow_Get_MaxHeight( layer );
	pixel_pos = TextLayer_Set_Cursor_Pos( layer, new_pos );
	Move_Widget( cursor, pixel_pos );
	Resize_Widget( cursor, size );
	/* 让光标在更新时显示 */
	Show_Widget( cursor ); 
	/* 更新文本显示区域 */
	TextBox_ViewArea_Update( widget );
	return pixel_pos;
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

/*--------------------------- End Public -----------------------------*/
