/* ***************************************************************************
 * textbox.c -- LCUI's TextBox widget
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * textbox.c -- LCUI 的文本框部件
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

//#define DEBUG

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
	LCUI_Widget *text;		/* 文本显示层 */
	LCUI_Widget *cursor;		/* 光标 */
	LCUI_Widget *scrollbar[2];	/* 两个滚动条 */
	int limit_mode;			/* 限制模式 */
	int block_size;			/* 块大小 */
	LCUI_Queue text_block_buff;	/* 文本块缓冲区 */
}
LCUI_TextBox;

typedef struct _LCUI_TextBlock
{
	int pos_type;	/* 指定该文本块要添加至哪个位置 */
	char *text;	/* 指向文本所在内存空间的指针 */
}
LCUI_TextBlock;

/*----------------------------- Private ------------------------------*/
static LCUI_Widget *active_textbox = NULL; 
static int __timer_id = -1;

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
	static int cur_state = 0;
	if(cur_state == 0) {
		show_textbox_cursor();
		cur_state = 1;
	} else {
		hide_textbox_cursor();
		cur_state = 0;
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
TextBox_Input( LCUI_Widget *widget, LCUI_KeyboardEvent *event )
{
	static char buff[5];
	static int cols, flag, rows;
	static LCUI_Pos cur_pos;
	static LCUI_TextLayer *layer;
	static LCUI_TextBox *textbox;
	
	//_DEBUG_MSG("you input: %d\n", event->key_code);
	layer = TextBox_Get_TextLayer( widget );
	textbox = Get_Widget_PrivData( widget );
	cur_pos = TextLayer_Get_Cursor_Pos( layer );
	cols = TextLayer_Get_RowLen( layer, cur_pos.y );
	rows = TextLayer_Get_Rows( layer ); 
	switch( event->key_code ) {
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
		if( textbox->limit_mode == 0 ) {
			flag = 1;
		} else {
			flag = 0;
		}
		/* 处理文本框的字符输入限制 */
		if( Check_Option( textbox->limit_mode, ONLY_0_TO_9 ) ) {
			if( event->key_code >= '0' && event->key_code <= '9' ) {
				++flag;
			}
		}
		if( Check_Option( textbox->limit_mode, ONLY_a_TO_z ) ) {
			if( event->key_code >= 'a' && event->key_code <= 'z' ) {
				++flag;
			}
		}
		if( Check_Option( textbox->limit_mode, ONLY_A_TO_Z ) ) {
			if( event->key_code >= 'A' && event->key_code <= 'Z' ) {
				++flag;
			}
		}
		if( Check_Option( textbox->limit_mode, ONLY_UNDERLINE ) ) {
			if( event->key_code == '_' ) {
				++flag;
			}
		}
		//_DEBUG_MSG("input char: %c, %d\n", event->key_code, flag);
		/* 如果该ASCII码代表的字符是可见的 */
		if( flag == 1 && (event->key_code == 10 || 
			(event->key_code > 31 && event->key_code < 126)) ) {
			//wchar_t *text;
			buff[0] = event->key_code;
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
TextBox_HoriScroll_TextLayer( ScrollBar_Data data, void *arg )
/* 水平滚动文本框内的文本图层 */
{
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg;
	//printf("1, data: size: %d / %d, num: %d / %d\n", 
	//data.current_size, data.max_size, data.current_num, data.max_num);
	LCUI_Pos pos;
	
	pos = TextBox_ViewArea_Get_Pos( arg );
	pos.x = 0 - data.current_num;
	pos.y = 0 - pos.y;
	//printf("1, offset.x: %d\n", pos.x);
	TextBox_TextLayer_Set_Offset( widget, pos );
	Update_Widget( widget );
}

static void 
TextBox_VertScroll_TextLayer( ScrollBar_Data data, void *arg )
/* 垂直滚动文本框内的文本图层 */
{
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg;
	//printf("2, data: size: %d / %d, num: %d / %d\n", 
	//data.current_size, data.max_size, data.current_num, data.max_num);
	LCUI_Pos pos;
	
	pos = TextBox_ViewArea_Get_Pos( arg );
	pos.y = 0 - data.current_num;
	pos.x = 0 - pos.x;
	//printf("2, offset.y: %d\n", pos.y);
	TextBox_TextLayer_Set_Offset( widget, pos );
	Update_Widget( widget );
}

static void
destroy_textblock( LCUI_TextBlock *ptr )
{
	free( ptr->text );
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
	textbox->limit_mode = 0;
	textbox->block_size = 256;
	
	Label_AutoSize( textbox->text, FALSE, 0 );
	Set_Widget_Size( textbox->text, "100%", "100%" );
	
	/* 添加至相应的容器 */
	Widget_Container_Add( textbox->text, textbox->cursor ); 
	Widget_Container_Add( widget, textbox->text ); 
	Widget_Container_Add( widget, textbox->scrollbar[0] );
	Widget_Container_Add( widget, textbox->scrollbar[1] );
	/* 设置滚动条的尺寸 */
	Set_Widget_Size( textbox->scrollbar[0], "10px", NULL );
	Set_Widget_Size( textbox->scrollbar[1], NULL, "10px" );
	Set_Widget_Align( textbox->scrollbar[0], ALIGN_TOP_RIGHT, Pos(0,0) );
	Set_Widget_Align( textbox->scrollbar[1], ALIGN_BOTTOM_LEFT, Pos(0,0) );
	/* 滚动条设为横向 */
	ScrollBar_Set_Direction( textbox->scrollbar[1], 1 );
	/* 将回调函数与滚动条连接 */
	ScrollBar_Connect( textbox->scrollbar[0], TextBox_VertScroll_TextLayer, widget );
	ScrollBar_Connect( textbox->scrollbar[1], TextBox_HoriScroll_TextLayer, widget );
	Show_Widget( textbox->text );
	
	Queue_Init( &textbox->text_block_buff, sizeof(LCUI_TextBlock), destroy_textblock );
	
	TextLayer_Using_StyleTags( Label_Get_TextLayer(textbox->text), FALSE );
	Set_Widget_Padding( widget, Padding(2,2,2,2) );
	Set_Widget_Backcolor( textbox->cursor, RGB(0,0,0) );
	Set_Widget_BG_Mode( textbox->cursor, BG_MODE_FILL_BACKCOLOR );
	
	Resize_Widget( textbox->cursor, Size(1, 14) );
	/* 设置可点击区域的alpha值要满足的条件 */
	Set_Widget_ClickableAlpha( textbox->cursor, 0, 1 );
	Set_Widget_ClickableAlpha( textbox->text, 0, 1 );
	/* 设定定时器，每1秒闪烁一次 */
	if( __timer_id == -1 ) {
		__timer_id = set_timer( 500, blink_cursor, TRUE );
	}
	Widget_Drag_Event_Connect( widget, TextBox_TextLayer_Click );
	/* 关联 FOCUS_OUT 和 FOCUS_IN 事件 */
	Widget_FocusOut_Event_Connect( widget, hide_textbox_cursor, NULL );
	Widget_FocusIn_Event_Connect( widget, _put_textbox_cursor, NULL );
	/* 关联按键输入事件 */
	Widget_KeyboardEvent_Connect( widget, TextBox_Input );
	/* 默认不启用多行文本模式 */
	TextBox_Multiline( widget, FALSE );
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
	LCUI_Graph *graph;
	graph = Widget_GetSelfGraph( widget );
	Draw_Empty_Slot( graph, widget->size.w, widget->size.h );
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
TextBox_ScrollBar_Update_Size( LCUI_Widget *widget )
/* 更新滚动条的长度 */
{
	int tmp;
	char size_str[15];
	LCUI_Size area_size, layer_size;
	LCUI_Widget *scrollbar[2], *label;
	LCUI_TextLayer *layer;
	
	label = TextBox_Get_Label( widget );
	/* 获取文本图层 */
	layer = TextBox_Get_TextLayer( widget );
	
	/* 获取文本图层和文本框区域的尺寸 */
	layer_size = TextLayer_Get_Size( layer );
	area_size = Get_Container_Size( widget );
	/* 获取纵向和横向滚动条 */
	scrollbar[0] = TextBox_Get_Scrollbar( widget, 0 );
	scrollbar[1] = TextBox_Get_Scrollbar( widget, 1 );
	
	/* 如果文本图层高度超过显示区域 */
	if( layer->enable_multiline && area_size.h > 0 
	  && layer_size.h > area_size.h ) {
		tmp = area_size.w - Get_Widget_Width( scrollbar[0] );
		snprintf( size_str, sizeof(size_str)-1, "%dpx", tmp );
		Set_Widget_Size( label, size_str, NULL );
		
		/* 修改滚动条中记录的最大值和当前值，让滚动条在更新后有相应的长度 */
		ScrollBar_Set_MaxSize( scrollbar[0], layer_size.h );
		ScrollBar_Set_CurrentSize( scrollbar[0], area_size.h );
		Show_Widget( scrollbar[0] );
		/* 如果横向滚动条可见 */
		if( scrollbar[1]->visible ) {
			tmp = area_size.h - Get_Widget_Height( scrollbar[1] );
			snprintf( size_str, sizeof(size_str)-1, "%dpx", tmp );
			Set_Widget_Size( scrollbar[0], NULL, size_str );
		} else {
			Set_Widget_Size( scrollbar[0], NULL, "100%" );
		}
	} else {
		/* 不需要显示滚动条 */
		Hide_Widget( scrollbar[0] );
		Set_Widget_Size( label, "100%", NULL );
	}
	/* 和上面的处理基本一样，这个是处理横向滚动条 */
	if( layer->enable_multiline &&
	 area_size.w > 0 && layer_size.w > area_size.w ) {
		tmp = area_size.h - Get_Widget_Height( scrollbar[1] );
		snprintf( size_str, sizeof(size_str)-1, "%dpx", tmp );
		Set_Widget_Size( label, NULL, size_str );
		
		ScrollBar_Set_MaxSize( scrollbar[1], layer_size.w );
		ScrollBar_Set_CurrentSize( scrollbar[1], area_size.w );
		Show_Widget( scrollbar[1] );
		
		if( scrollbar[0]->visible ) {
			tmp = area_size.w - Get_Widget_Width( scrollbar[0] );
			snprintf( size_str, sizeof(size_str)-1, "%dpx", tmp );
			Set_Widget_Size( scrollbar[1], size_str, NULL );
		} else {
			Set_Widget_Size( scrollbar[1], "100%", NULL );
		}
	} else {
		Set_Widget_Size( label, NULL, "100%" );
		Hide_Widget( scrollbar[1] );
	}
}

static int
TextBox_ScrollBar_Update_Pos( LCUI_Widget *widget )
/* 更新滚动条的位置 */
{
	static ScrollBar_Data scrollbar_data;
	static LCUI_Pos area_pos;
	static LCUI_Size layer_size, area_size;
	static LCUI_Widget *scrollbar[2];
	static LCUI_TextLayer *layer;
	
	area_size = Get_Container_Size( widget ); 
	scrollbar[0] = TextBox_Get_Scrollbar( widget, 0 );
	scrollbar[1] = TextBox_Get_Scrollbar( widget, 1 ); 
	if( scrollbar[0]->visible ) {
		area_size.w -= Get_Widget_Width( scrollbar[0] );
	}
	if( scrollbar[1]->visible ) {
		area_size.h -= Get_Widget_Height( scrollbar[1] );
	}
	
	if( area_size.h <= 0 || area_size.w <= 0 ) {
		return -1;
	}
	
	layer = TextBox_Get_TextLayer( widget );
	if( !layer->enable_multiline ) {
		return 1;
	}
	layer_size = TextLayer_Get_Size( layer );
	if( layer_size.h <= 0 || layer_size.w <= 0 ) {
		return -2;
	}
	area_pos = TextBox_ViewArea_Get_Pos( widget );
	ScrollBar_Set_MaxNum( scrollbar[0], layer_size.h - area_size.h );
	
	if( area_pos.y + area_size.h > layer_size.h ) {
		area_pos.y = layer_size.h - area_size.h;
	}
	if( area_pos.x + area_size.w > layer_size.w ) {
		area_pos.x = layer_size.w - area_size.w;
	}
	if( area_pos.y < 0 ) {
		area_pos.y = 0;
	}
	if( area_pos.x < 0 ) {
		area_pos.x = 0;
	}
	ScrollBar_Set_CurrentNum( scrollbar[0], area_pos.y ); 
	scrollbar_data = ScrollBar_Get_Data( scrollbar[0] );
	TextBox_VertScroll_TextLayer( scrollbar_data, widget );
	
	ScrollBar_Set_MaxNum( scrollbar[1], layer_size.w - area_size.w );
	ScrollBar_Set_CurrentNum( scrollbar[1], area_pos.x );
	scrollbar_data = ScrollBar_Get_Data( scrollbar[1] );
	TextBox_HoriScroll_TextLayer( scrollbar_data, widget );
	return 0;
}

static void 
__TextBox_Text_Append(LCUI_Widget *widget, char *new_text)
/* 在文本末尾追加文本 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Append( layer, new_text );
}

static void 
__TextBox_Text_Add(LCUI_Widget *widget, char *new_text)
/* 在光标处添加文本 */
{
	LCUI_Pos cur_pos;
	LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Add( layer, new_text );
	cur_pos = TextLayer_Get_Cursor_Pos( layer );
	TextBox_Cursor_Move( widget, cur_pos );
}

static void
Exec_TextBox_Update( LCUI_Widget *widget )
/* 更新文本框的文本图层 */
{
	LCUI_TextBlock *text_ptr;
	LCUI_TextBox *textbox;
	
	textbox = Get_Widget_PrivData( widget );
	/* 如果缓冲区内有文本块 */
	if( Queue_Get_Total( &textbox->text_block_buff ) > 0 ) {
		/* 获取文本块 */
		text_ptr = Queue_Get( &textbox->text_block_buff, 0 );
		if( text_ptr ) {
			//_DEBUG_MSG("text block: %p, text: %p\n", 
			//	text_ptr, text_ptr->text);
			switch( text_ptr->pos_type ) {
			    case AT_TEXT_LAST:
				/* 将此文本块追加至文本末尾 */
				__TextBox_Text_Append( widget, text_ptr->text );
				break;
			    case AT_CURSOR_POS:
				/* 将此文本块插入至光标当前处 */
				__TextBox_Text_Add( widget, text_ptr->text );
				break;
			    default: break;
			}
		}
		/* 删除该文本块 */
		Queue_Delete( &textbox->text_block_buff, 0 );
		/* 更新滚动条的位置 */
		TextBox_ScrollBar_Update_Pos( widget );
		/* 标记下次继续更新 */
		__Update_Widget( widget );
	}
	/* 更新文本图层的内容 */
	Exec_Update_Widget( TextBox_Get_Label( widget ) );
	/* 更新滚动条的长度 */
	TextBox_ScrollBar_Update_Size( widget );
	/* 更新文本框内的光标 */
	TextBox_Cursor_Update( widget );
}

static void
Exec_TextBox_Resize( LCUI_Widget *widget )
/* 在文本框改变尺寸后，进行附加处理 */
{
	Exec_TextBox_Update( widget );
}

void 
Process_TextBox_Drag(LCUI_Widget *widget, LCUI_DragEvent *event)
/* 处理鼠标对文本框的拖动事件 */
{
	
}

void 
Process_TextBox_Clicked(LCUI_Widget *widget, LCUI_EventSlot *event)
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
	static LCUI_Widget *scrollbar[2];
	static LCUI_TextLayer *layer;
	//printf("TextBox_ViewArea_Update(): enter\n");
	/* 获取显示区域的尺寸 */
	area_size = Get_Container_Size( widget );
	/* 获取滚动条 */
	scrollbar[0] = TextBox_Get_Scrollbar( widget, 0 );
	scrollbar[1] = TextBox_Get_Scrollbar( widget, 1 );
	/* 如果滚动条可见，那么区域尺寸就需要减去滚动条占用的尺寸 */
	if( scrollbar[0]->visible ) {
		area_size.w -= Get_Widget_Width( scrollbar[0] );
	}
	if( scrollbar[1]->visible ) {
		area_size.h -= Get_Widget_Height( scrollbar[1] );
	}
	
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
	
	/* 设定滚动条的数据中的最大值 */
	ScrollBar_Set_MaxNum( scrollbar[0], layer_size.h - area_size.h );
	
	DEBUG_MSG("cursor_pos: %d,%d\n", cursor_pos.x, cursor_pos.y);
	DEBUG_MSG("area_rect: %d,%d,%d,%d\n", 
		area_pos.x, area_pos.y,area_size.w, area_size.h );
	DEBUG_MSG("layer_size: %d,%d\n", layer_size.w, layer_size.h);
	
	/* 检测光标Y轴坐标是否超出显示区域的范围 */
	if( cursor_pos.y < area_pos.y ) {
		area_pos.y = cursor_pos.y;
	}
	if(cursor_pos.y + cursor_h > area_pos.y + area_size.h ) {
		area_pos.y = cursor_pos.y + cursor_h - area_size.h;
	}
	/* 如果显示区域在Y轴上超过文本图层的范围 */
	if( area_pos.y + area_size.h > layer_size.h ) {
		area_pos.y = layer_size.h - area_size.h;
	}
	if( area_pos.y < 0 ) {
		area_pos.y = 0;
	}
	/* 设定滚动条的数据中的当前值 */
	ScrollBar_Set_CurrentNum( scrollbar[0], area_pos.y );
	/* 获取滚动条的数据，供滚动文本层利用 */
	scrollbar_data = ScrollBar_Get_Data( scrollbar[0] );
	
	//printf("scrollbar_data: size: %d / %d, num: %d / %d\n", 
	//scrollbar_data.current_size, scrollbar_data.max_size, 
	//scrollbar_data.current_num, scrollbar_data.max_num);
	/* 根据数据，滚动文本图层至响应的位置，也就是移动文本显示区域 */
	TextBox_VertScroll_TextLayer( scrollbar_data, widget );
	
	/* 设定滚动条的数据中的最大值 */
	ScrollBar_Set_MaxNum( scrollbar[1], layer_size.w - area_size.w );
	
	if( cursor_pos.x < area_pos.x ) {
		area_pos.x = cursor_pos.x;
	}
	if(cursor_pos.x +5 > area_pos.x + area_size.w ) {
		area_pos.x = cursor_pos.x+5 - area_size.w;
	}
	if( area_pos.x + area_size.w > layer_size.w ) {
		area_pos.x = layer_size.w - area_size.w;
	}
	if( area_pos.x < 0 ) {
		area_pos.x = 0;
	}
	/* 设定滚动条的数据中的当前值 */
	ScrollBar_Set_CurrentNum( scrollbar[1], area_pos.x );
	/* 获取滚动条的数据，供滚动文本层利用 */
	scrollbar_data = ScrollBar_Get_Data( scrollbar[1] );
	
	//printf("scrollbar_data: size: %d / %d, num: %d / %d\n", 
	//scrollbar_data.current_size, scrollbar_data.max_size, 
	//scrollbar_data.current_num, scrollbar_data.max_num);
	TextBox_HoriScroll_TextLayer( scrollbar_data, widget );
	//printf("TextBox_ViewArea_Update(): quit\n");
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

static int
textbuff_add_utf8_text( LCUI_TextBox* textbox, char *new_text, int pos_type )
/* 将UTF-8编码文本添加至缓冲区内 */
{
	unsigned char t;
	int i, j, count, len, size;
	char *text_buff;
	LCUI_TextBlock text_block;
	
	len = strlen( new_text );
	//_DEBUG_MSG("len = %d\n", len);
	switch( pos_type ) {
		case AT_TEXT_LAST: 
		case AT_CURSOR_POS:
		text_block.pos_type = pos_type;
		break;
		default: return -1;
	}
	for(i=0; i<len; ++i) {
		if( len-i > textbox->block_size ) {
			size = textbox->block_size;
		} else {
			size = len-i +1;
		}
		text_buff = malloc( sizeof(char) * size );
		if( !text_buff ) {
			return -2;
		}
		for( count=0,j=0; i<len; ++i, ++j ) {
			/* 如果大于当前块大小 */
			if( j >= textbox->block_size ) {
				if( count == 0 ) {
					break;
				} 
				text_buff = realloc( text_buff, 
						sizeof(char) * (j+count) );
				if( !text_buff ) {
					return -2;
				}
			}
			if(count > 0) {
				/* 保存一个字节 */
				text_buff[j] = new_text[i];
				--count;
				continue;
			}
			/* 需要转存为unsigned char类型，否则位移的结果会有问题 */
			t = new_text[i];
			/* 根据编码信息，判断该UTF-8字符还剩多少个字节 */
			if((t>>7) == 0); // 0xxxxxxx
			else if((t>>5) == 6) {// 110xxxxx 
				count = 1; 
			}
			else if((t>>4) == 14) {// 1110xxxx 
				count = 2; 
			}
			else if((t>>3) == 30) {// 11110xxx 
				count = 3; 
			}
			else if((t>>2) == 62) {// 111110xx 
				count = 4; 
			}
			else if((t>>1) == 126) {// 1111110x 
				count = 5; 
			}
			/* 保存一个字节 */
			text_buff[j] = new_text[i];
			//_DEBUG_MSG("char: %d, count: %d\n", new_text[i], count);
		}
		--i;
		text_buff[j] = 0;
		text_block.text = text_buff;
		/* 添加文本块至缓冲区 */
		Queue_Add( &textbox->text_block_buff, &text_block );
	}
	return 0;
}

static void
TextBox_TextBuff_Add( LCUI_Widget *widget, char *new_text, int pos_type )
/* 将文本添加至缓冲区内 */
{
	LCUI_TextBox *textbox;
	
	textbox = Get_Widget_PrivData( widget );
	/* 判断当前使用的文本编码方式 */
	switch(Get_EncodingType()) {
	    case ENCODEING_TYPE_UTF8 :
		textbuff_add_utf8_text( textbox, new_text, pos_type );
		break;
	    case ENCODEING_TYPE_GB2312 :
		break;
	    default: break;
	}
}

static void
TextBox_Text_Clear( LCUI_Widget *widget )
{
	LCUI_TextLayer *layer;
	
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Clear( layer );
}

void TextBox_Text(LCUI_Widget *widget, char *new_text)
/* 设定文本框显示的文本 */
{
	/* 清空显示的文本 */
	TextBox_Text_Clear( widget );
	/* 把文本分割成块，加入至缓冲队列，让文本框分段显示 */
	TextBox_TextBuff_Add( widget, new_text, AT_TEXT_LAST );
	Update_Widget( widget );
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
	/* 把文本分割成若干块，加入至缓冲队列，让文本框分段处理显示 */ 
	TextBox_TextBuff_Add( widget, new_text, AT_CURSOR_POS );
	Update_Widget( widget );
}

void TextBox_Text_Append(LCUI_Widget *widget, char *new_text)
/* 在文本末尾追加文本 */
{
	TextBox_TextBuff_Add( widget, new_text, AT_TEXT_LAST );
	Update_Widget( widget );
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

void 
TextBox_Multiline( LCUI_Widget *widget, BOOL flag )
/* 指定文本框是否启用多行文本显示 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Multiline( layer, flag );
}

void
TextBox_Text_Set_MaxLength( LCUI_Widget *widget, int max )
/* 设置文本框中能够输入的最大字符数 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Set_MaxLength( layer, max );
}

void
TextBox_Text_Set_PasswordChar( LCUI_Widget *widget, wchar_t ch )
/* 为文本框设置屏蔽字符 */
{
	LCUI_TextLayer *layer;
	layer = TextBox_Get_TextLayer( widget );
	TextLayer_Text_Set_PasswordChar( layer, ch );
	TextLayer_Refresh( layer );
	Update_Widget( widget );
}

void
TextBox_Text_Limit( LCUI_Widget *widget, int mode )
/* 
 * 功能：限制能对文本框输入的字符 
 * 说明：参数mode的取值可为：
 *      ONLY_0_9       //只能输入0至9范围内的字符
 *      ONLY_a_z       //只能输入a至z范围内的字符
 *      ONLY_A_Z       //只能输入A至Z范围内的字符
 *      ONLY_UNDERLINE //只能输入下划线
 * 上述值可同时使用，可以这样：
 * ONLY_0_TO_9 | ONLY_a_TO_z | ONLY_A_TO_Z
 * 设置文本框，只能输入数字和字母
 * */
{
	LCUI_TextBox *textbox;
	
	textbox = Get_Widget_PrivData( widget );
	textbox->limit_mode = mode;
}
/*--------------------------- End Public -----------------------------*/
