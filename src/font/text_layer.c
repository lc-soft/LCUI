
//#define DEBUG

#include <LCUI_Build.h>

#include LC_LCUI_H 
#include LC_GRAPH_H
#include LC_WIDGET_H
#include LC_FONT_H

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Special_KeyWord	Special_KeyWord;
typedef struct _LCUI_CharData	LCUI_CharData;
typedef struct _Text_RowData		Text_RowData; 
typedef struct _tag_style_data	tag_style_data;

typedef enum _tag_id	enum_tag_id;

enum _tag_id
{
	TAG_ID_FAMILY = 0,
	TAG_ID_STYLE = 1,
	TAG_ID_WIEGHT = 2,
	TAG_ID_DECORATION = 3,
	TAG_ID_SIZE = 4,
	TAG_ID_COLOR = 5
}; 

/********* 保存字体相关数据以及位图 ********/
struct _LCUI_CharData
{
	wchar_t char_code;	/* 字符码 */
	LCUI_FontBMP bitmap;	/* 字体位图 */
	BOOL display:2;		/* 标志，是否需要显示该字 */
	BOOL need_update:2;	/* 标志，表示是否需要刷新该字的字体位图数据 */
	//BOOL using_quote:2;	/* 标志，表示是否引用了现成的文本样式 */
	LCUI_TextStyle *data;	/* 文本样式数据 */
};
/***************************************/

/********* 保存一行的文本数据 *************/
struct _Text_RowData
{
	LCUI_Size max_size;	/* 记录最大尺寸 */
	LCUI_Pos pos;		/* 当前行所在的位置 */
	LCUI_Queue string;	/* 这个队列中的成员用于引用源文本的字体数据 */
};
/***************************************/

/*************************** 特殊关键词 *****************************/
struct _Special_KeyWord
{
	LCUI_String keyword;		/* 关键词 */
	LCUI_TextStyle *data;		/* 该关键词使用的字体数据 */
	LCUI_Queue *text_source_data;	/* 关键词所属的源文本 */
};
/******************************************************************/

struct _tag_style_data 
{
	enum_tag_id tag;
	void *style;
};

/**************************** TextStyle *******************************/
void 
TextStyle_Init ( LCUI_TextStyle *data )
/* 初始化字体样式数据 */
{
	memset( data->family, 0, sizeof(data->family) );
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->decoration = FONT_DECORATION_NONE;
	data->need_back_color = IS_FALSE;
	data->need_fore_color = IS_FALSE;
	data->fore_color = RGB(0,0,0);
	data->back_color = RGB(0,0,0);
	data->pixel_size = 12;
}

void
TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily )
{
	strncpy( style->family, fontfamily, sizeof(style->family) );
}

void
TextStyle_FontSize( LCUI_TextStyle *style, int fontsize )
{
	style->pixel_size = fontsize;
}

void
TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->fore_color = color;
}

void
TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->back_color = color;
}

void
TextStyle_FontStyle( LCUI_TextStyle *style, enum_font_style fontstyle )
{
	style->style = fontstyle;
}

void
TextStyle_FontWeight( LCUI_TextStyle *style, enum_font_weight fontweight )
{
	style->weight = fontweight;
}

void
TextStyle_FontDecoration( LCUI_TextStyle *style, enum_font_decoration decoration )
{
	style->decoration = decoration;
}

int 
TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b )
{
	return 0;
}

/************************** End TextStyle *****************************/


/***************************** 私有函数 *********************************/

static void 
destroy_tag_style_data( tag_style_data *data )
{ 
	//free( data->style );
}

static void 
Destroy_Special_KeyWord(Special_KeyWord *key)
{
	/* 在该关键词所属的原文本中查找组成该关键词的字，并修改字所使用的字体数据 */
}

static void 
Destroy_CharData(LCUI_CharData *data)
{ 
	FontBMP_Free( &data->bitmap );
	free( data->data );
	//if( data->using_quote == IS_FALSE ) {
		//free( data->data );
	//}
}

static void 
Destroy_Text_RowData(Text_RowData *data)
{
	Destroy_Queue ( &data->string );
}


static void 
TextLayer_TagStyle_Add( LCUI_TextLayer *layer, tag_style_data *data )
/* 将字体样式数据加入队列 */
{
	Queue_Add( &layer->tag_buff, data );
}

#define MAX_TAG_NUM 1

static LCUI_TextStyle *
TextLayer_Get_Current_TextStyle ( LCUI_TextLayer *layer )
/* 获取当前的字体样式数据 */
{
	int i, total, equal = 0,flags[MAX_TAG_NUM];
	LCUI_TextStyle *data;
	tag_style_data *p; 
	
	data = (LCUI_TextStyle*) malloc (sizeof(LCUI_TextStyle));
	TextStyle_Init( data );
	
	total = Queue_Get_Total( &layer->tag_buff );
	//DEBUG_MSG("total tag: %d\n", total);
	if(total <= 0) {
		free( data );
		return NULL;
	}
	/* 从样式数据队列中获取字体样式数据 */
	for(equal=0,i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		DEBUG_MSG("tag id: %d\n", p->tag);
		switch( p->tag ) {
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
	if( equal != MAX_TAG_NUM ) {
		free( data );
		return NULL;
	}
	return data;
}

static void 
TextLayer_TagStyle_Delete( LCUI_TextLayer *layer, enum_tag_id tag)
/* 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
{
	int i, total;
	tag_style_data *p; 
	 
	total = Queue_Get_Total( &layer->tag_buff );
	DEBUG_MSG("delete start, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
	if(total <= 0) {
		return;
	}
	for(i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		if( p->tag == tag ) {
			Queue_Delete( &layer->tag_buff, i );
			break;
		}
	} 
	DEBUG_MSG("delete end, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
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

static wchar_t *
get_style_endtag ( wchar_t *str, char *out_tag_name )
/* 在字符串中获取样式的结束标签，输出的是标签名 */
{
	wchar_t *p;
	int i, j, len, tag_found = 0;
	
	len = wcslen ( str );
	//printf("string: %S\n", str);
	if(str[0] != '<' || str[1] != '/') { 
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

static wchar_t *
get_style_tag( wchar_t *str, const char *tag, char *out_tag_data )
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

static wchar_t *
covernt_tag_to_style_data (wchar_t *str, tag_style_data *out_data)
/* 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
{
	wchar_t *p, *q; 
	char tag_data[256];
	
	p = str; 
	//DEBUG_MSG("covernt_tag_to_style_data(): enter\n");
	if( (q = get_style_tag ( p, "color", tag_data)) ) {
		int r,g,b, len, i, j;
		LCUI_RGB rgb;
		
		p = q;
		//DEBUG_MSG("is color style tag, data: %s\n", tag_data);
		len = strlen(tag_data); 
		for(j=0,i=0; i<len; ++i) {
			if(tag_data[i] == ',') {
				++j;
			}
		}
		if(j == 2) {
			sscanf( tag_data, "%d,%d,%d", &r, &g, &b ); 
		}
		else if(tag_data[0] == '#') {
			switch(len) {
			    case 4: 
				sscanf( tag_data, "#%1X%1X%1X", &r, &g, &b );
				r<<=4; g<<=4; b<<=4;
				break;
			    case 7:
				sscanf( tag_data, "#%2X%2X%2X", &r, &g, &b ); 
				break;
			    default:
				r=0; g=0; b=0;
				break;
			}
		} else {
			r=0; g=0; b=0;
		}
		DEBUG_MSG("color: %d,%d,%d\n", r,g,b);
		rgb = RGB(r, g, b);
		out_data->tag = TAG_ID_COLOR;
		out_data->style = malloc( sizeof(LCUI_RGB) );
		memcpy( out_data->style, &rgb, sizeof(LCUI_RGB) );
	} else {
		p = NULL;
	}
	//DEBUG_MSG("covernt_tag_to_style_data(): quit\n");
	return p;
}

static wchar_t *
handle_style_tag( LCUI_TextLayer *layer, wchar_t *str )
{
	wchar_t *q;
	tag_style_data data;
	
	/* 开始处理样式标签 */
	q = covernt_tag_to_style_data ( str, &data );
	//DEBUG_MSG("handle_style_tag():%p\n", q);
	if( q != NULL ) {
		//DEBUG_MSG("add style data\n");
		/* 将标签样式数据加入队列 */
		TextLayer_TagStyle_Add( layer, &data ); 
	}
	return q;
}

static wchar_t *
handle_style_endtag( LCUI_TextLayer *layer, wchar_t *str )
/* 处理样式的结束标签 */
{
	wchar_t *p;
	char tag_name[256];
	/* 获取标签名 */
	p = get_style_endtag( str, tag_name );
	if( p == NULL ) {
		return NULL;
	}
	if( strcasecmp(tag_name, "color") == 0 ) {
		/* 消除该标签添加的字体样式 */
		TextLayer_TagStyle_Delete ( layer, TAG_ID_COLOR );
	}
	return p;
}

static void 
TextLayer_Get_Char_BMP ( LCUI_CharData *data )
/* 获取字体位图，字体的样式由文本图层中记录的字体样式决定 */
{
	LCUI_Font font;
	
	Font_Init( &font );
	if(data->data != NULL) {
		if(data->data->pixel_size != -1) {
			font.size = data->data->pixel_size;
		}
	}
	Get_FontBMP( &font, data->char_code, &data->bitmap );
}

static int 
TextLayer_Text_Add_NewRow ( LCUI_TextLayer *layer )
/* 添加新行 */
{
	Text_RowData data;
	
	data.pos = Pos(0,0);
	data.max_size = Size(0,0);
	Queue_Init( &data.string, sizeof(LCUI_CharData), NULL );
	/* 使用链表模式，方便数据的插入 */
	Queue_Set_DataMode( &data.string, QUEUE_DATA_MODE_LINKED_LIST );
	/* 队列成员使用指针，主要是引用text_source_data里面的数据 */
	Queue_Using_Pointer( &data.string );
	return Queue_Add( &layer->rows_data, &data );
}

static Text_RowData *
TextLayer_Get_Current_RowData ( LCUI_TextLayer *layer )
/* 获取指向当前行的指针 */
{
	return Queue_Get( &layer->rows_data, layer->current_des_pos.y );
}

static void
TextLayer_Update_RowSize (LCUI_TextLayer *layer, int row )
/* 更新指定行文本位图的尺寸 */
{
	int total, i;
	LCUI_Size size;
	LCUI_CharData *char_data;
	Text_RowData *row_data;
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string );
	for( size.w=0,size.h=0,i=0; i<total; ++i ) {
		char_data = Queue_Get( &row_data->string, i );
		size.w += char_data->bitmap.width;
		size.w += char_data->bitmap.left;
		//height = char_data->bitmap.top;
		if( char_data->data != NULL ) {
			if( char_data->data->pixel_size != -1 ) {
				if( size.h < char_data->data->pixel_size + 2) {
					size.h = char_data->data->pixel_size + 2;
				}
			} else {
				if( size.h < 14) {
					size.h = 14;
				}
			}
		} else {
			if( size.h < 14) {
				size.h = 14;
			}
		}
	}
	row_data->max_size = size;
}
/**********************************************************************/


/*************************** 基本的处理 *********************************/
void 
TextLayer_Init( LCUI_TextLayer *layer )
/* 初始化文本图层相关数据 */
{
	layer->using_code_mode = IS_FALSE; 
	layer->using_style_tags = IS_FALSE; 
	layer->enable_word_wrap = IS_FALSE; 
	layer->enable_multiline = IS_FALSE; 
	
	layer->have_select = IS_FALSE;
	layer->start = 0;
	layer->end = 0;
	
	Queue_Init( &layer->color_keyword, sizeof(Special_KeyWord), Destroy_Special_KeyWord );
	/* 队列中使用链表储存这些数据 */
	Queue_Init( &layer->text_source_data, sizeof(LCUI_CharData), Destroy_CharData );
	Queue_Set_DataMode( &layer->text_source_data, QUEUE_DATA_MODE_LINKED_LIST ); 
	Queue_Init( &layer->rows_data, sizeof(Text_RowData), Destroy_Text_RowData ); 
	TextLayer_Text_Add_NewRow ( layer );/* 添加新行 */
	Queue_Init( &layer->tag_buff, sizeof(tag_style_data), destroy_tag_style_data );
	Queue_Init( &layer->style_data, sizeof(LCUI_TextStyle), NULL );
	RectQueue_Init( &layer->refresh_area );
	layer->default_data.pixel_size = 12;
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
	layer->max_text_len = 5000; 
	TextStyle_Init ( &layer->default_data );
}

void 
Destroy_TextLayer( LCUI_TextLayer *layer )
/* 销毁文本图层占用的资源 */
{
	
}

void 
TextLayer_Draw( LCUI_Widget *widget, LCUI_TextLayer *layer, int mode )
/* 绘制文本图层 */
{
	LCUI_Rect area;
	LCUI_Pos pos;
	int i, j, n, rows, size;
	LCUI_RGB color;
	LCUI_Graph slot;
	LCUI_CharData *p_data;
	Text_RowData *p_row;
	
	Graph_Init( &slot );
	/* 开始粘贴文本位图 */
	rows = Queue_Get_Total( &layer->rows_data ); 
	for(pos.y=0,i=0; i<rows; ++i) {
		p_row = Queue_Get( &layer->rows_data, i );
		n = Queue_Get_Total( &p_row->string );
		pos.x = 0;
		//
		//if( p_row->pos.y != pos.y ) { 
		//	RectQueue_Add( &layer->refresh_area, Rect(pos.x, pos.y, 
		//		p_row->max_size.w, p_row->max_size.h) );
		//	p_row->pos.y = pos.y;
		//} 
		for(j=0; j<n; ++j) {
			p_data = Queue_Get( &p_row->string, j ); 
			if( p_data->data != NULL ) {
				if( p_data->data->pixel_size > 0 ) {
					size = p_data->data->pixel_size;
				} else { 
					size = layer->default_data.pixel_size;
				}
				size += 2;
				
				if( p_data->data->need_fore_color ) {
					color = p_data->data->fore_color;
				} else {
					color = layer->default_data.fore_color;
				}
			} else {
				size = layer->default_data.pixel_size + 2; 
				color = layer->default_data.fore_color;
			} 
			if( p_data->need_update ) { 
				p_data->need_update = FALSE;
				/* 引用区域 */
				area = Rect(pos.x, pos.y, size, size);
				RectQueue_Add( &layer->refresh_area, area );
				Quote_Graph( &slot, &widget->graph, area );
				/* 将该区域的alpha通道填充为0 */
				Graph_Fill_Alpha( &slot, 0 );
				FontBMP_Mix( &widget->graph, Pos( pos.x, 
					pos.y + size - p_data->bitmap.top),
					&p_data->bitmap, color, mode );
			}
			pos.x += p_data->bitmap.left;
			pos.x += p_data->bitmap.width;
		}
		pos.y += p_row->max_size.h;
	} 
	n = Queue_Get_Total( &layer->refresh_area ); 
	for(i=0; i<n; ++i) { 
		RectQueue_Get( &area, 0 , &layer->refresh_area ); 
		Add_Widget_Refresh_Area( widget, area ); 
		Queue_Delete( &layer->refresh_area, 0 ); 
	} 
}

void
TextLayer_Refresh( LCUI_TextLayer *layer )
/* 标记文本图层中每个字的位图，等待绘制文本图层时进行更新 */
{
	uint_t i, j;
	int rows, len;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	rows = Queue_Get_Total( &layer->rows_data );
	for(i=0; i<rows; ++i) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		len = Queue_Get_Total( &row_ptr->string );
		for(j=0; j<len; ++j) {
			char_ptr = Queue_Get( &row_ptr->string, j );
			if( char_ptr ) {
				char_ptr->need_update = TRUE;
			}
		}
	}
}
/**********************************************************************/

/************************ 文本图层部件的扩展功能 ****************************/
/* 剪切板 */
//static LCUI_String clip_board;

LCUI_Size 
TextLayer_Get_Size ( LCUI_TextLayer *layer )
/* 获取文本图层的实际尺寸 */
{
	int i, rows;
	LCUI_Size size;
	Text_RowData *p_row; 
	
	rows = Queue_Get_Total( &layer->rows_data );
	for(size.w=0,size.h=0,i=0; i<rows; ++i,++size.h) {
		p_row = Queue_Get( &layer->rows_data, i );
		if( size.w < p_row->max_size.w ) {
			size.w = p_row->max_size.w;
		}
		size.h += p_row->max_size.h;
	}
	return size;
}

void
TextLayer_Text_Set_Default_Style( LCUI_TextLayer *layer, LCUI_TextStyle style )
/* 设定默认的文本样式 */
{
	memcpy( &layer->default_data, &style, sizeof(LCUI_TextStyle) );
}

void
TextLayer_Clear( LCUI_TextLayer *layer )
/* 清空文本内容 */
{
	Destroy_Queue( &layer->text_source_data );
	Destroy_Queue( &layer->rows_data );
	Destroy_Queue( &layer->style_data );
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
}

void
TextLayer_Row_Refresh( LCUI_TextLayer *layer, uint_t row, uint_t start_cols )
/* 记录第row行的第start_cols及后面所有的字所在的区域，以便刷新 */
{
	uint_t total, i;
	Text_RowData *row_data;
	LCUI_CharData *p;
	LCUI_Rect area;
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string );
	area.y = row_data->pos.y;
	for(area.x=0,i=0; i<total; ++i) {
		p = Queue_Get( &row_data->string, i );
		if( p->data != NULL ) {
			if(p->data->pixel_size > 0) {
				area.width = area.height = p->data->pixel_size + 2;
			} else {
				area.width = area.height = 14;
			}
		} else {
			area.width = area.height = 14;
		}
		/* 记录该区域 */
		RectQueue_Add( &layer->refresh_area, area );
		area.x += p->bitmap.left;
		area.x += p->bitmap.width;
	}
}

void
TextLayer_Row_Set_End( LCUI_TextLayer *layer, uint_t row, uint_t start_cols )
/* 为指定行设定结束点，结束点及后面的数据将被删除，但不记录残余文本位图区域 */
{
	uint_t total, i;
	Text_RowData *row_data; 
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string );
	/* 移除多余的数据 */
	for(i=start_cols; i<total; ++i) {
		Queue_Delete( &row_data->string, start_cols ); 
	}
}

int
TextLayer_Text_Length( LCUI_TextLayer *layer )
/* 获取文本位图中的文本长度 */
{
	return Queue_Get_Total( &layer->text_source_data );
}

void
TextLayer_Text_Process( LCUI_TextLayer *layer, char *new_text )
/* 对文本进行预处理，处理后的数据保存至layer里 */ 
{
	int total; 
	uint32_t rows, n_ignore = 0;
	wchar_t *finish, *buff, *p, *q;
	
	DEBUG_MSG("%s\n", new_text);
	
	LCUI_CharData char_data; 
	Text_RowData *current_row_data;
	
	/* 如果有选中的文本，那就删除 */
	//......  
	total = Char_To_Wchar_T( new_text, &buff );
	current_row_data = TextLayer_Get_Current_RowData ( layer );
	if( !current_row_data ) {
		TextLayer_Text_Add_NewRow( layer );
		current_row_data = TextLayer_Get_Current_RowData ( layer );
	}
	
	FontBMP_Init( &char_data.bitmap );
	/* 根据样式标签生成对应的样式数据 */
	for(p=buff, finish=buff+total; p<finish; ++p) { 
		if( layer->using_style_tags ) {
			/* 处理样式的结束标签 */ 
			q = handle_style_endtag ( layer, p );
			if(q != NULL) {
				/* 计算需忽略的字符数 */
				n_ignore = q-p+1;
			} else {
				/* 处理样式标签 */
				q = handle_style_tag ( layer, p ); 
				if( q != NULL ) {
					n_ignore = q-p+1;
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
			FontBMP_Init( &char_data.bitmap ); 
		}
		while(n_ignore > 0) { 
			char_data.char_code = *p++;
			Queue_Insert( &layer->text_source_data, 
				layer->current_src_pos, &char_data );
			/* 遇到换行符，那就增加新行 */
			if(char_data.char_code == '\n') {
				rows = TextLayer_Text_Add_NewRow( layer );
				current_row_data = Queue_Get( &layer->rows_data, rows );
				layer->current_des_pos.x = 0;
				layer->current_des_pos.y = rows;
			} else {
				++layer->current_src_pos; 
			}
			--n_ignore;
			if(n_ignore == 0) {
				n_ignore = -1;
				break;
			}
		}
		if(n_ignore == -1) {
			--p; n_ignore = 0;
			continue;
		}
		
		char_data.char_code = *p;
		char_data.display = IS_TRUE; 
		char_data.need_update = IS_TRUE; 
		char_data.data = TextLayer_Get_Current_TextStyle( layer );
		Queue_Insert( &layer->text_source_data, layer->current_src_pos, &char_data ); 
		Queue_Insert( &current_row_data->string, layer->current_des_pos.x, &char_data ); 
		
		++layer->current_src_pos;
		++layer->current_des_pos.x;
	}
}

void
TextLayer_Text_GenerateBMP( LCUI_TextLayer *layer )
/* 为文本图层中的文本生成位图，已存在位图的文字将不重新生成 */
{
	uint_t i, j, len, rows;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	
	rows = Queue_Get_Total( &layer->rows_data );
	for(j=0; j<rows; ++j) {
		row_ptr = Queue_Get( &layer->rows_data, j );
		len = Queue_Get_Total( &row_ptr->string );
		for(i=0; i<len; ++i) {
			char_ptr = Queue_Get( &row_ptr->string, i );
			if( char_ptr->display 
			 && FontBMP_Valid( &char_ptr->bitmap ) ) {
				continue;
			}
			DEBUG_MSG( "generate FontBMP, char code: %d\n", char_ptr->char_code );
			TextLayer_Get_Char_BMP ( char_ptr );
		}
	}
}

void
TextLayer_Merge( LCUI_TextLayer *des, LCUI_TextLayer *src )
/* 合并两个文本图层 */
{
	LCUI_Rect area;
	uint_t i, j, old_size, new_size;
	LCUI_CharData *p, *q;
	Text_RowData *src_row_ptr, *des_row_ptr;
	uint_t old_rows, new_rows, max_rows, min_rows;
	int32_t src_len, des_len, max_len, min_len;
	
	old_rows = Queue_Get_Total( &des->rows_data );
	new_rows = Queue_Get_Total( &src->rows_data );
	DEBUG_MSG("old_rows: %d, new_rows: %d\n", old_rows, new_rows);
	
	if(old_rows > new_rows) {
		max_rows = old_rows;
		min_rows = new_rows;
	} else {
		max_rows = new_rows;
		min_rows = old_rows;
	} 
	for(i=0; i<min_rows; ++i) {
		area.x = 0;
		src_row_ptr = Queue_Get( &src->rows_data, i );
		des_row_ptr = Queue_Get( &des->rows_data, i );
		src_len = Queue_Get_Total( &src_row_ptr->string );
		des_len = Queue_Get_Total( &des_row_ptr->string );
		if(src_len > des_len) {
			max_len = src_len;
			min_len = des_len;
		} else {
			max_len = des_len;
			min_len = src_len;
		}
		for(j=0; j<min_len; ++j) {
			p = Queue_Get( &src_row_ptr->string, j );
			q = Queue_Get( &des_row_ptr->string, j );
			if(p->char_code == q->char_code) {
				/* 转移字体位图 */
				DEBUG_MSG("char_code: %d == %d\n", q->char_code, p->char_code);
				p->bitmap = q->bitmap;
				p->need_update = FALSE;
				FontBMP_Init( &q->bitmap );
			} else {
				p->need_update = TRUE;
				DEBUG_MSG("char_code: %d != %d\n", q->char_code, p->char_code);
			}
			/* 获取字体大小 */
			if(p->data == NULL) {
				new_size = 12;
			} else {
				if(p->data->pixel_size > 0) {
					new_size = p->data->pixel_size;
				} else {
					new_size = 12;
				}
			}
			if(q->data == NULL) {
				old_size = 12;
			} else {
				if(q->data->pixel_size > 0) {
					old_size = q->data->pixel_size;
				} else {
					old_size = 12;
				}
			}
			//文本样式也要对比
			if(new_size != old_size) {
				area.width = area.height = old_size+2;
				RectQueue_Add( &des->refresh_area, area );
				if(q->data != NULL) {
					free(q->data);
				}
				free( q->data );
				q->data = NULL;
				p->need_update = TRUE; 
			}
			if(p->need_update) {
				FontBMP_Free(&q->bitmap); 
			}
			area.x += q->bitmap.left;
			area.x += q->bitmap.width;
		}
		if( src_len < max_len ) {
			/* 如果这一行删减了几个字，则记录区域 */
			for(j=min_len; j<max_len; ++j) {
				q = Queue_Get( &des_row_ptr->string, j );
				if(q->data == NULL) {
					old_size = 12;
				} else {
					if(q->data->pixel_size > 0) {
						old_size = q->data->pixel_size;
					} else {
						old_size = 12;
					}
				}
				area.width = area.height = old_size+2;
				RectQueue_Add( &des->refresh_area, area );
				area.x += q->bitmap.left;
				area.x += q->bitmap.width;
			}
		} 
		area.y += des_row_ptr->max_size.h; 
	} 
	area.x = 0; 
	if(new_rows < max_rows) {/* 如果是删减几行文本,则需要记录被删文本的区域 */ 
		for(i=min_rows; i<max_rows; ++i) {
			des_row_ptr = Queue_Get( &des->rows_data, min_rows );
			area.width = des_row_ptr->max_size.w;
			area.height = des_row_ptr->max_size.h;
			RectQueue_Add( &des->refresh_area, area ); 
		}
	} 
	/* 转移数据 */
	Destroy_Queue( &des->text_source_data );
	des->text_source_data = src->text_source_data; 
	Destroy_Queue( &des->rows_data );
	des->rows_data = src->rows_data;
}

int
TextLayer_Text( LCUI_TextLayer *layer, char *new_text )
/* 设定整个文本图层中需显示的文本，原有选中文本被删除 */
{
	DEBUG_MSG("enter\n"); 
	uint_t i, rows;
	LCUI_TextLayer new_layer;

	TextLayer_Init( &new_layer );
	TextLayer_Text_Set_Default_Style( &new_layer, layer->default_data);
	TextLayer_Using_StyleTags( &new_layer, layer->using_style_tags );
	TextLayer_Text_Process( &new_layer, new_text );
	/* 合并两个文本图层，记录不同字所在区域，等待处理刷新 */
	TextLayer_Merge( layer, &new_layer ); 
	TextLayer_Text_GenerateBMP( layer ); 
	/* 更新每一行文本位图的尺寸 */
	rows = Queue_Get_Total( &layer->rows_data );
	for(i=0; i<rows; ++i) {
		TextLayer_Update_RowSize( layer, i );
	}
	Destroy_TextLayer( &new_layer );  
	DEBUG_MSG("quit\n");
	return 0;
}

int 
TextLayer_Text_Append( LCUI_TextLayer *layer, char *new_text )
/* 在文本末尾追加文本，不移动光标，不删除原有选中文本 */
{
	return 0;
}

int 
TextLayer_Text_Add( LCUI_TextLayer *layer, char *new_text )
/* 在光标处添加文本，如有选中文本，将被删除 */
{
	uint_t i, rows;
	TextLayer_Text_Process( layer, new_text );
	TextLayer_Text_GenerateBMP( layer );
	/* 更新每一行文本位图的尺寸 */
	rows = Queue_Get_Total( &layer->rows_data );
	for(i=0; i<rows; ++i) {
		TextLayer_Update_RowSize( layer, i );
	}
	return 0;
}

int 
TextLayer_Text_Paste( LCUI_TextLayer *layer )
/* 将剪切板的内容粘贴至文本图层 */
{
	return 0;
}


int 
TextLayer_Text_Backspace( LCUI_TextLayer *layer, int n )
/* 删除光标左边处n个字符 */
{
	return 0;
}

int 
TextLayer_Text_Delete( LCUI_TextLayer *layer, int n )
/* 删除光标右边处n个字符 */
{
	return 0;
}

LCUI_Pos 
TextLayer_Get_Pixel_Pos( LCUI_TextLayer *layer, uint32_t char_pos )
/* 根据源文本中的位置，获取该位置的字符相对于文本图层的坐标 */
{
	LCUI_Pos pos;
	pos.x = pos.y = 0;
	return pos;
}

uint32_t 
TextLayer_Get_Char_Pos( LCUI_TextLayer *layer, LCUI_Pos pixel_pos )
/* 根据文本图层的相对坐标，获取该坐标对应于源文本中的字符 */
{
	return 0;
}

int 
TextLayer_Get_Select_Text( LCUI_TextLayer *layer, char *out_text )
/* 获取文本图层内被选中的文本 */
{ 
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 获取选中的文本内容 */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Copy_Select_Text( LCUI_TextLayer *layer )
/* 复制文本图层内被选中的文本 */
{
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 将选中的文本内容添加至剪切版 */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Cut_Select_Text( LCUI_TextLayer *layer )
/* 剪切文本图层内被选中的文本 */
{
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 将选中的文本内容添加至剪切版，并删除被选中的文本 */
		//......
		return 0;
	}
	return -1;
}

void 
TextLayer_Using_StyleTags( LCUI_TextLayer *layer, BOOL flag )
/* 指定文本图层是否处理样式标签 */
{
	layer->using_style_tags = flag;
}

/**********************************************************************/
