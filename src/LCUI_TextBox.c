/*
 * 文本框部件，正在实现中，只添加了数据结构以及部分函数接口
 * 
 * 
 * 
 * 
 * */
 
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MEM_H
#include LC_WIDGET_H
#include LC_GRAPHICS_H
#include LC_FONT_H
#include <stdint.h>
#include <wchar.h>

typedef struct _LCUI_FontData	LCUI_FontData;
typedef struct _Special_KeyWord	Special_KeyWord;
typedef struct _LCUI_TextBox 	LCUI_TextBox;
typedef struct _LCUI_CharData	LCUI_CharData;
typedef struct _Text_Row_Data	Text_Row_Data; 
typedef struct _tag_style_data	tag_style_data;

typedef enum _font_style		enum_font_style;
typedef enum _font_weight		enum_font_weight;
typedef enum _font_decoration	enum_font_decoration;
typedef enum _tag_id			enum_tag_id;

enum _font_style
{ 
	FONT_STYLE_NORMAL = 0, 
	FONT_STYLE_ITALIC = 1, 
	FONT_STYLE_OBIQUE = 2 
};

enum _tag_id
{
	TAG_ID_FAMILY = 0,
	TAG_ID_STYLE = 1,
	TAG_ID_WIEGHT = 2,
	TAG_ID_DECORATION = 3,
	TAG_ID_SIZE = 4,
	TAG_ID_COLOR = 5
};

enum _font_weight
{
	FONT_WEIGHT_NORMAL	= 0,
	FONT_WEIGHT_BOLD	= 1 
}; 

enum _font_decoration
{
	FONT_DECORATION_NONE		= 0,	/* 无装饰 */
	FONT_DECORATION_BLINK		= 1,	/* 闪烁 */
	FONT_DECORATION_UNDERLINE	= 2,	/* 下划线 */
	FONT_DECORATION_LINE_THROUGH	= 3,	/* 贯穿线 */
	FONT_DECORATION_OVERLINE	= 4	/* 上划线 */
};

/******************* 字体相关数据 **********************/
struct _LCUI_FontData
{
	LCUI_String family;
	
	enum_font_style	style		:3;
	enum_font_weight	weight		:3;
	enum_font_decoration	decoration	:4;
	
	int need_back_color:1;
	int need_fore_color:1;
	LCUI_RGB fore_color;
	LCUI_RGB back_color;
	
	int pixel_size;		/* 当它等于-1时，将使用全局尺寸 */
};
/******************************************************/

/********* 保存字体相关数据以及位图 ********/
struct _LCUI_CharData
{
	wchar_t char_code;	/* 字符码 */
	LCUI_Bitmap bitmap;	/* 字体位图 */
	BOOL display:2;		/* 标志，是否需要显示该字 */
	BOOL need_update:2;	/* 标志，表示是否需要刷新该字的字体位图数据 */
	//BOOL using_quote:2;	/* 标志，表示字体数据是否引用了另一处的数据 */
	LCUI_FontData *data;	/* 字体相关数据 */
};
/***************************************/

/********* 保存一行的文本数据 *************/
struct _Text_Row_Data
{
	LCUI_Size max_size;	/* 记录最大尺寸 */
	LCUI_Queue string;	/* 这个队列中的成员用于引用源文本的字体数据 */
};
/***************************************/

/*************************** 特殊关键词 *****************************/
struct _Special_KeyWord
{
	LCUI_String keyword;		/* 关键词 */
	LCUI_FontData *data;		/* 该关键词使用的字体数据 */
	LCUI_Queue *text_source_data;	/* 关键词所属的源文本 */
};
/******************************************************************/


struct _tag_style_data 
{
	enum_tag_id tag;
	void *style;
};

struct _LCUI_TextBox
{
	BOOL using_code_mode	:2;	/* 指示是否开启代码模式 */
	BOOL using_style_tags	:2;	/* 指示是否处理样式标签 */
	BOOL enable_word_wrap	:2;	/* 指示是否自动换行 */
	BOOL enable_multiline	:2;	/* 指示是否为多行文本框部件 */ 
	
	BOOL have_select : 2;	/* 标记，指示是否在文本框中选择了文本 */
	uint32_t start, end;	/* 被选中的文本的范围 */ 
	
	LCUI_Queue color_keyword;	/* 记录需要使用指定风格的关键字 */
	LCUI_Queue text_source_data;	/* 储存文本相关数据 */
	LCUI_Queue rows_data;		/* 储存每一行文本的数据 */
	LCUI_Queue style_data;		/* 用于保存控制符中表达的字体属性 */
	
	uint32_t current_src_pos;	/* 当前光标在源文本中位置 */
	LCUI_Pos current_des_pos;	/* 当前光标在分段后的文本中的位置 */
	uint32_t max_text_len;		/* 最大文本长度 */ 
	
	LCUI_FontData default_data;
};


/***************************** 私有函数 *********************************/
static void FontData_Init ( LCUI_FontData *data )
/* 初始化字体样式数据 */
{
	String_Init( &data->family );
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->decoration = FONT_DECORATION_NONE;
	data->need_back_color = IS_FALSE;
	data->need_fore_color = IS_FALSE;
	data->pixel_size = -1;
}

static void destroy_tag_style_data( tag_style_data *data )
{ 
	free( data->style );
}

static void Destroy_Special_KeyWord(Special_KeyWord *key)
{
	/* 在该关键词所属的原文本中查找组成该关键词的字，并修改字所使用的字体数据 */
}

static void Destroy_CharData(LCUI_CharData *data)
{ 
	Free_Bitmap( &data->bitmap );
	free( data->data );
	//if( data->using_quote == IS_FALSE ) {
		//free( data->data );
	//}
}

static void Destroy_Text_Row_Data(Text_Row_Data *data)
{
	Destroy_Queue ( &data->string );
}


static void TextBox_TagStyle_Add( LCUI_Widget *widget, tag_style_data *data )
/* 将字体样式数据加入队列 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	Queue_Add( &textbox->style_data, data );
}

#define MAX_TAG_NUM 1

static LCUI_FontData *TextBox_Get_Current_FontData ( LCUI_Widget *widget )
/* 获取当前的字体样式数据 */
{
	int i, total, equal = 0,flags[MAX_TAG_NUM];
	LCUI_FontData *data;
	tag_style_data *p;
	LCUI_TextBox *textbox;
	
	textbox = Get_Widget_Private_Data( widget );
	data = (LCUI_FontData*) malloc (sizeof(LCUI_FontData));
	FontData_Init( data );
	
	total = Queue_Get_Total( &textbox->style_data );
	if(total > 0) {
		/* 从样式数据队列中获取字体样式数据 */
		for(equal=0,i=total-1; i>=0; --i) {
			p = Queue_Get( &textbox->style_data, i );
			switch(p->tag) {
			    case TAG_ID_COLOR: 
				if( flags[0] == 0 ) {
					data->need_fore_color = IS_TRUE;
					data->fore_color = *((LCUI_RGB*)p->style);
					++equal;
				}
				break;
			    default: break;
			}
			if(equal == MAX_TAG_NUM) {
				break;
			}
		}
	}
	if( equal != MAX_TAG_NUM ) {
		free( data );
		return NULL;
	}
	return data;
}

static void TextBox_TagStyle_Delete( LCUI_Widget *widget, enum_tag_id tag)
/* 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
{
	int i, total;
	tag_style_data *p;
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	total = Queue_Get_Total( &textbox->style_data );
	if(total > 0) {
		for(i=total-1; i>=0; --i) {
			p = Queue_Get( &textbox->style_data, i );
			if( p->tag == tag ) {
				Queue_Delete( &textbox->style_data, i );
			}
		}
	}
}

void clear_space(char *in, char *out)
/* 清除字符串中的空格 */
{
	int j, i, len = strlen(in);
	for(j=i=0; i<len; ++i) {
		if(in[i] == ' ') {
			continue;
		}
		out[j] = in[i];
		++j;
	}
	out[j] = 0;
}

static wchar_t *get_style_endtag ( wchar_t *str, char *out_tag_name )
/* 在字符串中获取样式的结束标签，输出的是标签名 */
{
	wchar_t *p;
	int i, j, len, tag_found = 0;
	
	len = wcslen ( str );
	if(str[0] != '<' && str[1] != '/') {
		return NULL;
	} 
	/* 匹配标签,获取标签名 */
	for(j=0,i=2; i<len; ++i) {
		switch(str[i]) {
		    case ' ': 
			if(  tag_found == 0 ) {
				break;
			}
			return NULL;
		    case '>': goto end_tag_search;
		    default: out_tag_name[j] = str[i]; ++j; break;
		}
	}
	
end_tag_search:;

	out_tag_name[j] = 0;
	if( j < 1 ) {
		return NULL;
	}
	p = &str[i];
	return p;
}

static wchar_t *get_style_tag( wchar_t *str, const char *tag, char *out_tag_data )
/* 在字符串中获取指定样式标签中的数据 */
{
	wchar_t *p;
	int i, j, len, tag_len; 
	
	len = wcslen ( str );
	tag_len = strlen ( tag );
	if(str[0] != '<') {
		return NULL;
	} 
	/* 匹配标签前半部分 */
	for(j=0,i=1; i<len; ++i) {
		if( str[i] == ' ' ) { 
			if( j == 0 || j >= tag_len ) { 
				continue;
			}
			return NULL;
		}
		else if( str[i] == tag[j] ) { 
			++j;
		} else { 
			/* 如果标签名部分已经匹配完 */
			if( j>= tag_len ) { 
				if( str[i] == '=' ) {
					++i; 
					break;
				}
			}
			/* 否则，有误 */
			return NULL;
		}
	}
	/* 获取标签后半部分 */
	for(j=0; i<len; ++i) {
		if( str[i] == ' ' ) {
			continue; 
		} else {
			/* 标签结束，退出 */
			if( str[i] == '>' ) {
				break;
			}
			/* 保存标签内的数据 */
			out_tag_data[j] = str[i];
			++j;
		}
	}
	out_tag_data[j] = 0;
	if(i >= len ) {
		return NULL;
	}
	p = &str[i];
	return p;
}

static wchar_t *covernt_tag_to_font_data (wchar_t *str, tag_style_data *out_data)
/* 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
{
	wchar_t *p, *q; 
	char tag_data[256];
	
	p = str; 
	if( (q = get_style_tag ( p, "color", tag_data)) ) {
		p = q;
		LCUI_RGB rgb;
		sscanf( tag_data, "%c,%c,%c", &rgb.red, &rgb.green, &rgb.blue ); 
		out_data->tag = TAG_ID_COLOR;
		out_data->style = malloc( sizeof(LCUI_RGB) );
		memcpy( out_data->style, &rgb, sizeof(LCUI_RGB) );
	} else {
		return NULL;
	}
	return p;
}


static wchar_t *handle_style_tag(LCUI_Widget *widget, wchar_t *str)
{
	wchar_t *q;
	tag_style_data data;
	
	/* 开始处理样式标签 */
	q = covernt_tag_to_font_data ( str, &data );
	if( q != NULL ) {
		/* 将标签样式数据加入队列 */
		TextBox_TagStyle_Add( widget, &data ); 
	}
	return q;
}

static wchar_t *handle_style_endtag(LCUI_Widget *widget, wchar_t *str)
/* 处理样式的结束标签 */
{
	wchar_t *p;
	char tag_name[256];
	/* 获取标签名 */
	p = get_style_endtag( str, tag_name );
	if( strcasecmp(tag_name, "color") == 0 ) {
		/* 消除该标签添加的字体样式 */
		TextBox_TagStyle_Delete ( widget, TAG_ID_COLOR );
	}
	return p;
}

static void TextBox_Get_Char_Bitmap ( LCUI_CharData *data )
/* 获取字体位图，字体的样式由文本框中记录的字体样式决定 */
{
	LCUI_Font font;
	font = LCUI_Sys.default_font;
	if(data->data != NULL) {
		if(data->data->pixel_size != -1) {
			font.size = data->data->pixel_size;
		}
	}
	Get_WChar_Bitmap( &font, data->char_code, &data->bitmap );
}

static int TextBox_Text_Add_NewRow ( LCUI_Queue *rows_data )
/* 添加新行 */
{
	Text_Row_Data data;
	
	Queue_Init( &data.string, sizeof(LCUI_CharData), NULL );
	/* 使用链表模式，方便数据的插入 */
	Queue_Set_DataMode( &data.string, QUEUE_DATA_MODE_LINKED_LIST );
	/* 队列成员使用指针，主要是引用text_source_data里面的数据 */
	Queue_Using_Pointer( &data.string );
	return Queue_Add( rows_data, &data );
}

static Text_Row_Data *TextBox_Get_Current_RowData ( LCUI_TextBox *textbox )
/* 获取指向当前行的指针 */
{
	return Queue_Get( &textbox->rows_data, textbox->current_des_pos.y );
}

/**********************************************************************/


/************************* 基本的部件处理 ********************************/
static void TextBox_Init(LCUI_Widget *widget)
/* 初始化文本框相关数据 */
{
	LCUI_TextBox *textbox;
	textbox = Malloc_Widget_Private(widget, sizeof(LCUI_TextBox));
	textbox->using_code_mode = IS_FALSE; 
	textbox->using_style_tags = IS_FALSE; 
	textbox->enable_word_wrap = IS_FALSE; 
	textbox->enable_multiline = IS_FALSE; 
	
	textbox->have_select = IS_FALSE;
	textbox->start = 0;
	textbox->end = 0;
	
	Queue_Init( &textbox->color_keyword, sizeof(Special_KeyWord), 
					Destroy_Special_KeyWord );
	/* 队列中使用链表储存这些数据 */
	Queue_Init( &textbox->text_source_data, sizeof(LCUI_CharData),
					Destroy_CharData );
	Queue_Set_DataMode( &textbox->text_source_data, 
					QUEUE_DATA_MODE_LINKED_LIST );
			
	Queue_Init( &textbox->rows_data, sizeof(Text_Row_Data), 
					Destroy_Text_Row_Data );
	/* 添加新行 */
	TextBox_Text_Add_NewRow ( &textbox->rows_data );
	Queue_Init( &textbox->style_data, sizeof(tag_style_data), 
			destroy_tag_style_data );
	FontData_Init ( &textbox->default_data );
	
	textbox->default_data.pixel_size = 12;
	textbox->current_src_pos = 0;
	textbox->current_des_pos = Pos(0,0);
	textbox->max_text_len = 5000; 
}

static void Destroy_TextBox(LCUI_Widget *widget)
/* 销毁文本框占用的资源 */
{
	
}

extern int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height);
static void TextBox_Draw(LCUI_Widget *widget)
/* 处理文本框的图形渲染 */
{
	LCUI_Pos pos;
	int i, j, n, rows;
	LCUI_RGB color;
	LCUI_Size size;
	LCUI_TextBox *textbox;
	LCUI_CharData *p_data;
	Text_Row_Data *p_row;
	
	textbox = Get_Widget_Private_Data( widget );
	if( widget->auto_size ) {
		
		/* 如果设置了自动调整尺寸，那就计算适应文本位图的最佳尺寸 */
		size.w = size.h = 0;
		rows = Queue_Get_Total( &textbox->rows_data );
		for(i=0; i<rows; ++i) {
			p_row = Queue_Get( &textbox->rows_data, i ); 
			size.w = size.w < p_row->max_size.w ? p_row->max_size.w: size.w;
			size.h += p_row->max_size.h;
		}
	} else {
		size = widget->size;
	}
	/* 绘制文本框背景图 */
	Draw_Empty_Slot( &widget->graph, size.w, size.h );
	printf("size: %d, %d\n", size.w, size.h); 
	/* 开始粘贴文本位图 */
	rows = Queue_Get_Total( &textbox->rows_data );
	for(pos.y=2,i=0; i<rows; ++i) {
		p_row = Queue_Get( &textbox->rows_data, i );
		n = Queue_Get_Total( &p_row->string );
		printf("char num: %d\n", n);
		for(pos.x=3,j=0; j<n; ++j) {
			p_data = Queue_Get( &p_row->string, j );
			printf("j:%d, p:%p, data: %p\n", j, p_data, p_data->data);
			if( p_data->data != NULL 
			 && p_data->data->need_fore_color ) {
				color = p_data->data->fore_color;
			} else {
				color = widget->fore_color;
			}
			Mix_Fonts_Bitmap( &widget->graph, pos.x, pos.y,
					&p_data->bitmap, color, 
					GRAPH_MIX_FLAG_OVERLAY );
			pos.x = pos.x + p_data->bitmap.width + 1;
		}
	}
}

void Process_TextBox_Drag(LCUI_Widget *widget, LCUI_DragEvent *event)
/* 处理鼠标对文本框的拖动事件 */
{
	
}

void Process_TextBox_Clicked(LCUI_Widget *widget, LCUI_Event *event)
/* 处理鼠标对文本框的点击事件 */
{
	
}

void Register_TextBox()
/* 注册文本框部件 */
{
	WidgetType_Add ( "text_box" );
	WidgetFunc_Add ( "text_box", TextBox_Init, FUNC_TYPE_INIT );
	WidgetFunc_Add ( "text_box", TextBox_Draw, FUNC_TYPE_UPDATE );
	WidgetFunc_Add ( "text_box", Destroy_TextBox, FUNC_TYPE_DESTROY );
}

/**********************************************************************/

/************************ 文本框部件的扩展功能 ****************************/
/* 剪切板 */
//static LCUI_String clip_board;

int TextBox_Text_Add(LCUI_Widget *widget, char *new_text)
/* 在光标处添加自定义样式的文本 */
{
	int i, total; 
	uint32_t rows, n_ignore = 0;
	wchar_t *buff, *p, *q;
	LCUI_TextBox *textbox;
	LCUI_CharData char_data; 
	Text_Row_Data *current_row_data;
	
	/* 如果有选中的文本，那就删除 */
	//...... 
	
	total = Char_To_Wchar_T( new_text, &buff );
	textbox = Get_Widget_Private_Data( widget );
	
	current_row_data = TextBox_Get_Current_RowData ( textbox );
	/* 根据样式标签生成对应的样式数据 */
	for(p=buff, i=0; i<total; ++i, ++p) { 
		if( textbox->using_style_tags ) {
			/* 处理样式的结束标签 */
			q = handle_style_endtag ( widget, p );
			if(q != NULL) { 
				/* 计算需忽略的字符数 */
				n_ignore = (q-p)/sizeof(wchar_t); 
			} else {
				/* 处理样式标签 */
				q = handle_style_tag ( widget, p ); 
				if( q != NULL ) {
					n_ignore = (q-p)/sizeof(wchar_t);
				}
			}
		}
		if(*p == '\n') { 
			/* 计算需要忽略的换行符的数量 */
			for( n_ignore=0,q=p; *q == '\n'; ++q,++n_ignore);
		}
		if(n_ignore > 0) {
			/* 被忽略的字符的属性都一样，所以只需赋一次值 */
			char_data.data = NULL;
			char_data.display = IS_FALSE; 
			char_data.need_update = IS_FALSE; 
			Bitmap_Init( &char_data.bitmap );
			while(n_ignore--) { 
				char_data.char_code = *p++;
				++textbox->current_src_pos;
				Queue_Insert( &textbox->text_source_data, 
					textbox->current_src_pos, &char_data );
				/* 遇到换行符，那就增加新行 */
				if(char_data.char_code == '\n') {
					rows = TextBox_Text_Add_NewRow( &textbox->rows_data );
					current_row_data = Queue_Get( &textbox->rows_data, rows );
					textbox->current_des_pos.x = 0;
					textbox->current_des_pos.y = rows;
				}
			}
			n_ignore = 0;
		}
		
		LCUI_CharData *pp;
		char_data.char_code = *p;
		char_data.display = IS_TRUE; 
		char_data.need_update = IS_TRUE;
		/* 获取当前字体样式属性 */
		char_data.data = TextBox_Get_Current_FontData( widget );
		printf("char: %c, style data:%p\n", *p, char_data.data);
		/* 获取字体位图 */
		Bitmap_Init( &char_data.bitmap );
		TextBox_Get_Char_Bitmap ( &char_data );
		/* 插入队列 */
		Queue_Insert( &textbox->text_source_data, 
				textbox->current_src_pos, &char_data );
		pp = Queue_Get( &textbox->text_source_data, textbox->current_src_pos );
		printf("1, at queue, pos: %d, char: %c, data: %p\n", 
		textbox->current_src_pos, pp->char_code, pp->data);
		
		/* 添加该字的数据的引用指针至队列 */
		Queue_Insert( &current_row_data->string, 
				textbox->current_des_pos.x, &char_data );
		
		pp = Queue_Get( &current_row_data->string, textbox->current_des_pos.x );
		printf("2, at queue, pos: %d, char: %c, data: %p\n", 
		textbox->current_des_pos.x, pp->char_code, pp->data);
		
		/* 记录这一行文本的宽度和高度 */
		current_row_data->max_size.w += char_data.bitmap.width;
		if(current_row_data->max_size.h < char_data.bitmap.height) {
			current_row_data->max_size.h = char_data.bitmap.height;
		}
		++textbox->current_src_pos;
		++textbox->current_des_pos.x;
	}
	return 0;
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

uint32_t TextBox_Get_Char_Pos(LCUI_Widget *widget, LCUI_Pos pixel_pos)
/* 根据文本框的相对坐标，获取该坐标对应于源文本中的字符 */
{
	return 0;
}

int TextBox_Get_Select_Text(LCUI_Widget *widget, char *out_text)
/* 获取文本框内被选中的文本 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	/* 如果选择了文本 */
	if( textbox->have_select ) {
		/* 获取选中的文本内容 */
		//......
		return 0;
	}
	return -1;
}

int TextBox_Copy_Select_Text(LCUI_Widget *widget)
/* 复制文本框内被选中的文本 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	/* 如果选择了文本 */
	if( textbox->have_select ) {
		/* 将选中的文本内容添加至剪切版 */
		//......
		return 0;
	}
	return -1;
}

int TextBox_Cut_Select_Text(LCUI_Widget *widget)
/* 剪切文本框内被选中的文本 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	/* 如果选择了文本 */
	if( textbox->have_select ) {
		/* 将选中的文本内容添加至剪切版，并删除被选中的文本 */
		//......
		return 0;
	}
	return -1;
}

void TextBox_Using_StyleTags(LCUI_Widget *widget, BOOL flag)
/* 指定文本框是否处理控制符 */
{
	LCUI_TextBox *textbox;
	textbox = Get_Widget_Private_Data( widget );
	textbox->using_style_tags = flag;
}

/**********************************************************************/
