/*
 * css_library.c -- CSS library operation module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

#define MAX_NAME_LEN	256
#define LEN(A)		sizeof( A ) / sizeof( *A )

enum SelectorRank {
	GENERAL_RANK = 0,
	TYPE_RANK = 1,
	CLASS_RANK = 10,
	PCLASS_RANK = 10,
	ID_RANK = 100
};

enum SelectorFinderLevel {
	LEVEL_NONE,
	LEVEL_TYPE,
	LEVEL_ID,
	LEVEL_CLASS,
	LEVEL_CLASS_2,
	LEVEL_STATUS,
	LEVEL_STATUS_2,
	LEVEL_TOTAL_NUM
};

/* 样式表查找器的上下文数据结构 */
typedef struct NamesFinderRec_ {
	int level;			/**< 当前选择器层级 */
	int class_i;			/**< 当前处理到第几个类名 */
	int status_i;			/**< 当前处理到第几个状态名（伪类名） */
	int name_i;			/**< 选择器名称从第几个字符开始 */
	char name[MAX_NAME_LEN];	/**< 选择器名称缓存 */
	LCUI_SelectorNode node;		/**< 针对的选择器结点 */
} NamesFinderRec, *NamesFinder;

/** 样式链接记录组 */
typedef struct StyleLinkGroupRec_ {
	Dict *links;			/**< 样式链接表 */
	char *name;			/**< 选择器名称 */
	LCUI_SelectorNode snode;	/**< 选择器结点 */
} StyleLinkGroupRec, *StyleLinkGroup;

/** 样式结点记录 */
typedef struct StyleNodeRec_ {
	int rank;		/**< 权值，决定优先级 */
	int batch_num;		/**< 批次号 */
	char *space;		/**< 所属的空间 */
	char *selector;		/**< 选择器 */
	LCUI_StyleSheet sheet;	/**< 样式表 */
} StyleNodeRec, *StyleNode;

/** 样式链接记录 */
typedef struct StyleLinkRec_ {
	char *selector;		/**< 选择器 */
	StyleLinkGroup group;	/**< 所属组 */
	LinkedList styles;	/**< 作用于当前选择器的样式 */
	Dict *parents;		/**< 父级节点 */
} StyleLinkRec, *StyleLink;

static struct {
	LCUI_BOOL is_inited;
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LinkedList groups;		/**< 样式组列表 */
	Dict *cache;			/**< 样式表缓存，以选择器的 hash 值索引 */
	Dict *names;			/**< 样式属性名称表，以值的名称索引 */
	Dict *value_keys;		/**< 样式属性值表，以值的名称索引 */
	Dict *value_names;		/**< 样式属性值名称表，以值索引 */
	size_t count;			/**< 当前记录的属性数量 */
} library;

/** 样式字符串值与标识码 */
typedef struct KeyNameGroupRec_ {
	int key;
	char *name;
} KeyNameGroupRec, *KeyNameGroup;

static KeyNameGroupRec style_name_map[] = {
	{ key_visible, "visible"},
	{ key_width, "width" },
	{ key_height, "height" },
	{ key_min_width, "min-width" },
	{ key_min_height, "min-height" },
	{ key_max_width, "max-width" },
	{ key_max_height, "max-height" },
	{ key_display, "display" },
	{ key_z_index, "z-index" },
	{ key_top, "top" },
	{ key_right, "right" },
	{ key_left, "left" },
	{ key_bottom, "bottom" },
	{ key_position, "position" },
	{ key_opacity, "opacity" },
	{ key_vertical_align, "vertical-align" },
	{ key_background_color, "background-color" },
	{ key_background_position, "background-position" },
	{ key_background_size, "background-size" },
	{ key_background_image, "background-image" },
	{ key_padding_left, "padding-left" },
	{ key_padding_right, "padding-right" },
	{ key_padding_top, "padding-top" },
	{ key_padding_bottom, "padding-bottom" },
	{ key_margin_left, "margin-left" },
	{ key_margin_right, "margin-right" },
	{ key_margin_top, "margin-top" },
	{ key_margin_bottom, "margin-bottom" },
	{ key_border_top_color, "border-top-color" },
	{ key_border_right_color, "border-right-color" },
	{ key_border_bottom_color, "border-bottom-color" },
	{ key_border_left_color, "border-left-color" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_style, "border-top-style" },
	{ key_border_right_style, "border-right-style" },
	{ key_border_bottom_style, "border-bottom-style" },
	{ key_border_left_style, "border-left-style" },
	{ key_box_shadow_x, "box-shadow-x" },
	{ key_box_shadow_y, "box-shadow-y" },
	{ key_box_shadow_blur, "box-shadow-blur" },
	{ key_box_shadow_spread, "box-shadow-spread" },
	{ key_box_shadow_color, "box-shadow-color" },
	{ key_pointer_events, "pointer-events" },
	{ key_focusable, "focusable" },
	{ key_box_sizing, "box-sizing" },
	{ key_justify_content, "justify-content" }
};

/** 样式字符串与标识码的映射表 */
static KeyNameGroupRec style_value_map[] = {
	{ SV_NONE, "none" },
	{ SV_AUTO, "auto" },
	{ SV_CONTAIN, "contain" },
	{ SV_COVER, "cover" },
	{ SV_LEFT, "left" },
	{ SV_CENTER, "center" },
	{ SV_RIGHT, "right" },
	{ SV_TOP, "top" },
	{ SV_TOP_LEFT, "top left" },
	{ SV_TOP_CENTER, "top center" },
	{ SV_TOP_RIGHT, "top right" },
	{ SV_MIDDLE, "middle" },
	{ SV_CENTER_LEFT, "center left" },
	{ SV_CENTER_CENTER, "center center" },
	{ SV_CENTER_RIGHT, "center right" },
	{ SV_BOTTOM, "bottom" },
	{ SV_BOTTOM_LEFT, "bottom left" },
	{ SV_BOTTOM_CENTER, "bottom center" },
	{ SV_BOTTOM_RIGHT, "bottom right" },
	{ SV_SOLID, "solid" },
	{ SV_DOTTED, "dotted" },
	{ SV_DOUBLE, "double" },
	{ SV_DASHED, "dashed" },
	{ SV_CONTENT_BOX, "content-box" },
	{ SV_PADDING_BOX, "padding-box" },
	{ SV_BORDER_BOX, "border-box" },
	{ SV_GRAPH_BOX, "graph-box" },
	{ SV_STATIC, "static" },
	{ SV_RELATIVE, "relative" },
	{ SV_ABSOLUTE, "absolute" },
	{ SV_BLOCK, "block" },
	{ SV_INLINE_BLOCK, "inline-block" },
	{ SV_FLEX, "flex" },
	{ SV_FLEX_START, "flex-start" },
	{ SV_FLEX_END, "flex-end" },
	{ SV_NOWRAP, "nowrap" }
};

static int LCUI_DirectAddStyleName( int key, const char *name )
{
	return Dict_AddCopy( library.names, &key, name );
}

int LCUI_SetStyleName( int key, const char *name )
{
	char *newname;
	DictEntry *entry;
	LCUIMutex_Lock( &library.mutex );
	entry = Dict_Find( library.names, &key );
	if( entry ) {
		newname = strdup2( name );
		free( entry->v.val );
		entry->v.val = newname;
		LCUIMutex_Unlock( &library.mutex );
		return 0;
	}
	LCUIMutex_Unlock( &library.mutex );
	return -1;
}

int LCUI_AddCSSPropertyName( const char *name )
{
	int key;
	LCUIMutex_Lock( &library.mutex );
	key = library.count++;
	if( LCUI_DirectAddStyleName( key, name ) != 0 ) {
		--library.count;
		LCUIMutex_Unlock( &library.mutex );
		return -1;
	}
	LCUIMutex_Unlock( &library.mutex );
	return key;
}

const char *LCUI_GetStyleName( int key )
{
	return Dict_FetchValue( library.names, &key );
}

static KeyNameGroup CreateKeyNameGroup( int key, const char *name )
{
	KeyNameGroup group;
	group = malloc( sizeof( KeyNameGroupRec ) );
	group->name = strdup2( name );
	group->key = key;
	return group;
}

static void DestroyKeyNameGroup( void *data )
{
	KeyNameGroup group = data;
	free( group->name );
	free( group );
}

static void KeyNameGroupDestructor( void *privdata, void *data )
{
	DestroyKeyNameGroup( data );
}

int LCUI_AddStyleValue( int key, const char *name )
{
	KeyNameGroup group = CreateKeyNameGroup( key, name );
	if( Dict_Add( library.value_keys, group->name, group ) ) {
		DestroyKeyNameGroup( group );
		return -1;
	}
	if( Dict_Add( library.value_names, &group->key, group ) ) {
		DestroyKeyNameGroup( group );
		return -2;
	}
	return 0;
}

int LCUI_GetStyleValue( const char *str )
{
	KeyNameGroup group;
	group = Dict_FetchValue( library.value_keys, str );
	if( group ) {
		return group->key;
	}
	return -1;
}

const char *LCUI_GetStyleValueName( int val )
{
	KeyNameGroup group;
	group = Dict_FetchValue( library.value_names, &val );
	if( group ) {
		return group->name;
	}
	return NULL;
}

int LCUI_GetStyleTotal( void )
{
	return library.count;
}

LCUI_BOOL SelectorNode_Match( LCUI_SelectorNode sn1, 
			      LCUI_SelectorNode sn2 )
{
	int i, j;
	if( sn2->id ) {
		if( !sn1->id || strcmp( sn1->id, sn2->id ) != 0 ) {
			return FALSE;
		}
	}
	if( sn2->type && strcmp(sn2->type, "*") != 0 ) {
		if( !sn1->type || strcmp( sn1->type, sn2->type ) != 0 ) {
			return FALSE;
		}
	}
	if( sn2->classes ) {
		if( !sn1->classes ) {
			return FALSE;
		}
		for( i = 0; sn2->classes[i]; ++i ) {
			for( j = 0; sn1->classes[j]; ++j ) {
				if( strcmp( sn2->classes[i],
					    sn1->classes[i] ) == 0 ) {
					j = -1;
					break;
				}
			}
			if( j != -1 ) {
				return FALSE;
			}
		}
	}
	if( sn2->status ) {
		if( !sn1->status ) {
			return FALSE;
		}
		for( i = 0; sn2->status[i]; ++i ) {
			for( j = 0; sn1->status[j]; ++j ) {
				if( strcmp( sn2->status[i],
					    sn1->status[i] ) == 0 ) {
					j = -1;
					break;
				}
			}
			if( j != -1 ) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

static void SelectorNode_Copy( LCUI_SelectorNode dst, LCUI_SelectorNode src )
{
	int i;
	dst->id = src->id ? strdup2( src->id ) : NULL;
	dst->type = src->type ? strdup2( src->type ) : NULL;
	dst->fullname = src->fullname ? strdup2( src->fullname ) : NULL;
	if( src->classes ) {
		for( i = 0; src->classes[i]; ++i ) {
			sortedstrsadd( &dst->classes, src->classes[i] );
		}
	}
	if( src->status ) {
		for( i = 0; src->status[i]; ++i ) {
			sortedstrsadd( &dst->status, src->status[i] );
		}
	}
}

void SelectorNode_Delete( LCUI_SelectorNode node )
{
	if( node->type ) {
		free( node->type );
		node->type = NULL;
	}
	if( node->id ) {
		free( node->id );
		node->id = NULL;
	}
	if( node->classes ) {
		freestrs( node->classes );
		node->classes = NULL;
	}
	if( node->status ) {
		freestrs( node->status );
		node->status = NULL;
	}
	if( node->fullname ) {
		free( node->fullname );
		node->fullname = NULL;
	}
	free( node );
}

void Selector_Delete( LCUI_Selector s )
{
	int i;
	for( i = 0; i < MAX_SELECTOR_DEPTH; ++i ) {
		if( !s->nodes[i] ) {
			break;
		}
		SelectorNode_Delete( s->nodes[i] );
		s->nodes[i] = NULL;
	}
	s->rank = 0;
	s->length = 0;
	s->batch_num = 0;
	free( s->nodes );
	free( s );
}

LCUI_StyleSheet StyleSheet( void )
{
	LCUI_StyleSheet ss;
	ss = NEW( LCUI_StyleSheetRec, 1 );
	if( !ss ) {
		return ss;
	}
	ss->length = LCUI_GetStyleTotal();
	ss->sheet = NEW( LCUI_StyleRec, ss->length + 1 );
	return ss;
}

void StyleSheet_Clear( LCUI_StyleSheet ss )
{
	int i;
	LCUI_Style s;
	for( i = 0; i < ss->length; ++i ) {
		s = &ss->sheet[i];
		switch( s->type ) {
		case SVT_STRING:
		case SVT_WSTRING:
			if( s->is_valid && s->string ) {
				free( s->string );
			}
			s->string = NULL;
		default: break;
		}
		s->is_valid = FALSE;
	}
}

void StyleSheet_Delete( LCUI_StyleSheet ss )
{
	StyleSheet_Clear( ss );
	free( ss->sheet );
	free( ss );
}

int StyleSheet_Merge( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	LCUI_Style s;
	int i, count, size;
	if( src->length > dest->length ) {
		size = sizeof( LCUI_StyleRec )*src->length;
		s = realloc( dest->sheet, size );
		if( !s ) {
			return -1;
		}
		for( i = dest->length; i < src->length; ++i ) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for( count = 0, i = 0; i < src->length; ++i ) {
		s = &dest->sheet[i];
		if( !src->sheet[i].is_valid || s->is_valid ) {
			continue;
		}
		++count;
		switch( src->sheet[i].type ) {
		case SVT_STRING:
			s->string = strdup2( src->sheet[i].string );
			break;
		case SVT_WSTRING:
			size = wcslen( src->sheet[i].wstring ) + 1;
			s->wstring = malloc( size * sizeof( wchar_t ) );
			wcscpy( s->wstring, src->sheet[i].wstring );
			break;
		default:
			*s = src->sheet[i];
			break;
		}
		s->is_valid = TRUE;
		s->type = src->sheet[i].type;
	}
	return 0;
}

int StyleSheet_Replace( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	LCUI_Style s;
	int i, count, size;
	if( src->length > dest->length ) {
		size = sizeof( LCUI_StyleRec )*src->length;
		s = realloc( dest->sheet, size );
		if( !s ) {
			return -1;
		}
		for( i = dest->length; i < src->length; ++i ) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for( count = 0, i = 0; i < src->length; ++i ) {
		if( !src->sheet[i].is_valid ) {
			continue;
		}
		s = &dest->sheet[i];
		switch( src->sheet[i].type ) {
		case SVT_STRING:
			if( s->is_valid && s->string ) {
				free( s->string );
			}
			s->string = strdup2( src->sheet[i].string );
			break;
		case SVT_WSTRING:
			if( s->is_valid && s->string ) {
				free( s->wstring );
			}
			size = wcslen( src->sheet[i].wstring ) + 1;
			s->wstring = malloc( size * sizeof( wchar_t ) );
			wcscpy( s->wstring, src->sheet[i].wstring );
			break;
		default:
			*s = src->sheet[i];
			break;
		}
		s->is_valid = TRUE;
		s->type = src->sheet[i].type;
		++count;
	}
	return count;
}

/** 初始化样式表查找器 */
static void NamesFinder_Init( NamesFinder sfinder, LCUI_SelectorNode snode )
{
	sfinder->level = 0;
	sfinder->class_i = 0;
	sfinder->name_i = 0;
	sfinder->status_i = 0;
	sfinder->name[0] = 0;
	sfinder->node = snode;
}

/** 销毁样式表查找器 */
static void NamesFinder_Destroy( NamesFinder sfinder )
{
	sfinder->name_i = 0;
	sfinder->name[0] = 0;
	sfinder->class_i = 0;
	sfinder->status_i = 0;
	sfinder->node = NULL;
	sfinder->level = LEVEL_NONE;
}

/* 生成选择器全名列表 */
static int NamesFinder_Find( NamesFinder sfinder, LinkedList *list )
{
	int i, len, old_len, old_level, count = 0;
	char *fullname = sfinder->name + sfinder->name_i;
	old_len = len = strlen( fullname );
	old_level = sfinder->level;
	switch( sfinder->level ) {
	case LEVEL_TYPE: 
		/* 按类型选择器生成选择器全名 */
		if( !sfinder->node->type ) {
			return 0;
		}
		strcpy( fullname, sfinder->node->type );
		LinkedList_Append( list, strdup2( fullname ) );
		break;
	case LEVEL_ID: 
		/* 按ID选择器生成选择器全名 */
		if( !sfinder->node->id ) {
			return 0;
		}
		fullname[len++] = '#';
		fullname[len] = 0;
		strcpy( fullname + len, sfinder->node->id );
		LinkedList_Append( list, strdup2( fullname ) );
		break;
	case LEVEL_CLASS:
		if( !sfinder->node->classes ) {
			return 0;
		}
		/* 按类选择器生成选择器全名
		 * 假设当前选择器全名为：textview#main-btn-text，且有 .a .b .c 
		 * 这三个类，那么下面的处理将会拆分成以下三个选择器：
		 * textview#test-text.a
		 * textview#test-text.b
		 * textview#test-text.a
		 */
		fullname[len++] = '.';
		for( i = 0; sfinder->node->classes[i]; ++i ) {
			sfinder->level += 1;
			sfinder->class_i = i;
			strcpy( fullname + len, sfinder->node->classes[i] );
			LinkedList_Append( list, strdup2( fullname ) );
			/* 将当前选择器名与其它层级的选择器名组合 */
			while( sfinder->level < LEVEL_TOTAL_NUM ) {
				count += NamesFinder_Find( sfinder, list );
				sfinder->level += 1;
			}
			sfinder->level = LEVEL_CLASS;
		}
		sfinder->level = LEVEL_CLASS;
		fullname[old_len] = 0;
		sfinder->class_i = 0;
		return count;
	case LEVEL_CLASS_2:
		if( !sfinder->node->classes ) {
			return 0;
		}
		/* 按类选择器生成选择器全名，结果类似于这样： 
		 * textview#test-text.a.b
		 * textview#test-text.a.c
		 * textview#test-text.b.c
		 * textview#test-text.a.b.c
		 */
		fullname[len++] = '.';
		for( i = 0; sfinder->node->classes[i]; ++i ) {
			if( i <= sfinder->class_i ) {
				continue;
			}
			strcpy( fullname + len, sfinder->node->classes[i] );
			LinkedList_Append( list, strdup2( fullname ) );
			sfinder->class_i = i;
			count += NamesFinder_Find( sfinder, list );
			sfinder->class_i  = 0;
			sfinder->level = LEVEL_STATUS;
			/**
			 * 递归拼接伪类名，例如：
			 * textview#main-btn-text:active
			 */
			count += NamesFinder_Find( sfinder, list );
			sfinder->level = LEVEL_CLASS_2;
		}
		fullname[old_len] = 0;
		sfinder->level = LEVEL_CLASS_2;
		return count;
	case LEVEL_STATUS:
		if( !sfinder->node->status ) {
			return 0;
		}
		fullname[len++] = ':';
		sfinder->level = LEVEL_STATUS_2;
		/**
		 * 按伪类选择器生成选择器全名
		 * 假设当前选择器全名为：textview#main-btn-text:hover:focus:active
		 * 那么下面的循环会将它拆分为以下几个选择器：
		 * textview#main-btn-text:active
		 * textview#main-btn-text:active:focus
		 * textview#main-btn-text:active:focus:hover
		 * textview#main-btn-text:active:hover
		 * textview#main-btn-text:focus
		 * textview#main-btn-text:focus:hover
		 * textview#main-btn-text:hover
		 */
		for( i = 0; sfinder->node->status[i]; ++i ) {
			sfinder->status_i = i;
			strcpy( fullname + len, sfinder->node->status[i] );
			LinkedList_Append( list, strdup2( fullname ) );
			/**
			 * 递归调用，以一层层拼接出像下面这样的选择器：
			 * textview#main-btn-text:active:focus:hover
			 */
			count += NamesFinder_Find( sfinder, list );
		}
		sfinder->level = LEVEL_STATUS;
		fullname[old_len] = 0;
		sfinder->status_i = 0;
		return count;
	case LEVEL_STATUS_2:
		if( !sfinder->node->status ) {
			return 0;
		}
		/** 按伪类选择器生成选择器全名 */
		for( i = 0; sfinder->node->status[i]; ++i ) {
			if( i <= sfinder->status_i ) {
				continue;
			}
			fullname[len] = ':';
			strcpy( fullname + len + 1, sfinder->node->status[i] );
			LinkedList_Append( list, strdup2( fullname ) );
			sfinder->status_i = i;
			count += NamesFinder_Find( sfinder, list );
			sfinder->status_i = 0;
		}
		fullname[old_len] = 0;
		return count;
	default:break;
	}
	for( i = sfinder->level + 1; i < LEVEL_TOTAL_NUM; ++i ) {
		if( i == LEVEL_STATUS_2 || i == LEVEL_CLASS_2 ) {
			continue;
		}
		sfinder->level = i;
		count += NamesFinder_Find( sfinder, list );
	}
	fullname[old_len] = 0;
	sfinder->level = old_level;
	return count;
}

static int SelectorNode_Save( LCUI_SelectorNode node,
			      const char *name, int len, char type )
{
	char *str;
	if( len < 1 ) {
		return 0;
	}
	switch( type ) {
	case 0:
		if( node->type ) {
			break;
		}
		len += 1;
		str = malloc( sizeof( char ) * len );
		strncpy( str, name, len );
		node->type = str;
		return TYPE_RANK;
	case ':':
		if( sortedstrsadd( &node->status, name ) == 0 ) {
			return PCLASS_RANK;
		}
		break;
	case '.':
		if( sortedstrsadd( &node->classes, name ) == 0 ) {
			return CLASS_RANK;
		}
		break;
	case '#':
		if( node->id ) {
			break;
		}
		len += 1;
		str = malloc( sizeof( char ) * len );
		strncpy( str, name, len );
		node->id = str;
		return ID_RANK;
	default: break;
	}
	return 0;
}

int SelectorNode_GetNames( LCUI_SelectorNode sn, LinkedList *names )
{
	int count;
	NamesFinderRec sfinder;
	NamesFinder_Init( &sfinder, sn );
	count = NamesFinder_Find( &sfinder, names );
	NamesFinder_Destroy( &sfinder );
	return count;
}

int SelectorNode_Update( LCUI_SelectorNode node )
{
	int i, len = 0;
	char *fullname;

	node->rank = 0;
	if( node->id ) {
		len += strlen( node->id ) + 1;
		node->rank += ID_RANK;
	}
	if( node->type ) {
		len += strlen( node->type ) + 1;
		node->rank += TYPE_RANK;
	}
	if( node->classes ) {
		for( i = 0; node->classes[i]; ++i ) {
			len += strlen( node->classes[i] ) + 1;
			node->rank += CLASS_RANK;
		}
	}
	if( node->status ) {
		for( i = 0; node->status[i]; ++i ) {
			len += strlen( node->status[i] ) + 1;
			node->rank += PCLASS_RANK;
		}
	}
	if( len > 0 ) {
		fullname = malloc( sizeof( char ) * (len + 1) );
		if( !fullname ) {
			return -ENOMEM;
		}
		fullname[0] = 0;
		if( node->type ) {
			strcat( fullname, node->type );
		}
		if( node->id ) {
			strcat( fullname, "#" );
			strcat( fullname, node->id );
		}
		if( node->classes ) {
			for( i = 0; node->classes[i]; ++i ) {
				strcat( fullname, "." );
				strcat( fullname, node->classes[i] );
			}
			len += 1;
		}
		if( node->status ) {
			for( i = 0; node->status[i]; ++i ) {
				strcat( fullname, ":" );
				strcat( fullname, node->status[i] );
			}
			len += 1;
		}
	} else {
		fullname = NULL;
	}
	if( node->fullname ) {
		free( node->fullname );
	}
	node->fullname = fullname;
	return len;
}

void Selector_Update( LCUI_Selector s )
{
	int i;
	const unsigned char *p;
	unsigned int hash = 5381;
	for( i = 0; i < s->length; ++i ) {
		p = (unsigned char*)s->nodes[i]->fullname;
		while( *p ) {
			hash = ((hash << 5) + hash) + (*p++);
		}
	}
	s->hash = hash;
}

LCUI_Selector Selector( const char *selector )
{
	const char *p;
	int ni, si, rank;
	static int batch_num = 0;
	char type = 0, name[MAX_NAME_LEN];
	LCUI_BOOL is_saving = FALSE;
	LCUI_SelectorNode node = NULL;
	LCUI_Selector s = NEW( LCUI_SelectorRec, 1 );

	s->batch_num = ++batch_num;
	s->nodes = NEW( LCUI_SelectorNode, MAX_SELECTOR_DEPTH );
	if( !selector ) {
		s->length = 0;
		s->nodes[0] = NULL;
		return s;
	}
	for( ni = 0, si = 0, p = selector; *p; ++p ) {
		if( !node && is_saving ) {
			node = NEW( LCUI_SelectorNodeRec, 1 );
			if( si >= MAX_SELECTOR_DEPTH ) {
				_DEBUG_MSG( "%s: selector node list is too long.\n",
					    selector );
				return NULL;
			}
			s->nodes[si] = node;
		}
		switch( *p ) {
		case ':':
		case '.':
		case '#':
			if( !is_saving ) {
				is_saving = TRUE;
				type = *p;
				continue;
			}
			/* 保存上个结点 */
			rank = SelectorNode_Save( node, name, ni, type );
			is_saving = TRUE;
			type = *p;
			if( rank > 0 ) {
				s->rank += rank;
				ni = 0;
				continue;
			}
			_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
					selector, p - selector - ni );
			SelectorNode_Delete( node );
			node = NULL;
			ni = 0;
			continue;
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			if( !is_saving ) {
				ni = 0;
				node = NULL;
				continue;
			}
			is_saving = FALSE;
			rank = SelectorNode_Save( node, name, ni, type );
			if( rank > 0 ) {
				SelectorNode_Update( node );
				s->rank += rank;
				node = NULL;
				ni = 0;
				si++;
				continue;
			}
			_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
				    selector, p - selector - ni );
			SelectorNode_Delete( node );
			node = NULL;
			ni = 0;
			continue;
		default: break;
		}
		if( *p == '-' || *p == '_' || *p == '*'
		    || (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')
		    || (*p >= '0' && *p <= '9') ) {
			if( !is_saving ) {
				type = 0;
				is_saving = TRUE;
			}
			name[ni++] = *p;
			name[ni] = 0;
			continue;
		}
		_DEBUG_MSG( "%s: unknown char 0x%02x at %ld.\n",
			    selector, *p, p - selector );
		return NULL;
	}
	if( is_saving ) {
		if( !node ) {
			node = NEW( LCUI_SelectorNodeRec, 1 );
			if( si >= MAX_SELECTOR_DEPTH ) {
				_DEBUG_MSG( "%s: selector node list is too long.\n",
					    selector );
				return NULL;
			}
			s->nodes[si] = node;
		}
		rank = SelectorNode_Save( s->nodes[si], name, ni, type );
		if( rank > 0 ) {
			SelectorNode_Update( s->nodes[si] );
			s->rank += rank;
			si++;
		} else {
			SelectorNode_Delete( s->nodes[si] );
		}
	}
	s->nodes[si] = NULL;
	s->length = si;
	Selector_Update( s );
	return s;
}

static void DeleteStyleNode( StyleNode node )
{
	if( node->space ) {
		free( node->space );
		node->space = NULL;
	}
	if( node->selector ) {
		free( node->selector );
		node->selector = NULL;
	}
	StyleSheet_Delete( node->sheet );
	node->sheet = NULL;
	free( node );
}

static StyleLink CreateStyleLink( void )
{
	StyleLink link = NEW( StyleLinkRec, 1 );
	link->group = NULL;
	LinkedList_Init( &link->styles );
	link->parents = Dict_Create( &DictType_StringCopyKey, NULL );
	return link;
}

static void DeleteStyleLink( StyleLink link )
{
	Dict_Release( link->parents );
	LinkedList_Clear( &link->styles, (FuncPtr)DeleteStyleNode );
	free( link->selector );
	link->selector = NULL;
	link->parents = NULL;
	link->group = NULL;
	free( link );
}

static void StyleLinkDestructor( void *privdata, void *data )
{
	DeleteStyleLink( data );
}

static StyleLinkGroup CreateStyleLinkGroup( LCUI_SelectorNode snode )
{
	DictType *dtype = NEW( DictType, 1 );
	StyleLinkGroup group = NEW( StyleLinkGroupRec, 1 );
	group->snode = NEW( LCUI_SelectorNodeRec, 1 );
	SelectorNode_Copy( group->snode, snode );
	group->name = group->snode->fullname;
	*dtype = DictType_StringCopyKey;
	dtype->valDestructor = StyleLinkDestructor;
	group->links = Dict_Create( dtype, dtype );
	return group;
}

static void DeleteStyleLinkGroup( StyleLinkGroup group )
{
	DictType *dtype;
	dtype = group->links->privdata;
	SelectorNode_Delete( group->snode );
	Dict_Release( group->links );
	free( dtype );
	free( group );
}

static void StyleLinkGroupDestructor( void *privdata, void *data )
{
	DeleteStyleLinkGroup( data );
}

static Dict *CreateStyleGroup( void )
{
	Dict *dict;
	DictType *dtype = NEW( DictType, 1 );
	*dtype = DictType_StringCopyKey;
	dtype->valDestructor = StyleLinkGroupDestructor;
	dict = Dict_Create( dtype, dtype );
	return dict;
}

static void DeleteStyleGroup( Dict *dict )
{
	DictType *dtype;
	dtype = dict->privdata;
	Dict_Release( dict );
	free( dtype );
}

/** 根据选择器，选中匹配的样式表 */
static LCUI_StyleSheet LCUI_SelectStyleSheet( LCUI_Selector selector, 
					      const char *space )
{
	int i, right;
	StyleLink link;
	StyleNode snode;
	StyleLinkGroup slg;
	LCUI_SelectorNode sn;
	Dict *group, *parents;
	char buf[MAX_SELECTOR_LEN];
	char fullname[MAX_SELECTOR_LEN];

	link = NULL;
	parents = NULL;
	for( i = 0, right = selector->length - 1; right >= 0; --right, ++i ) {
		group = LinkedList_Get( &library.groups, i );
		if( !group ) {
			group = CreateStyleGroup();
			LinkedList_Append( &library.groups, group );
		}
		sn = selector->nodes[right];
		slg = Dict_FetchValue( group, sn->fullname );
		if( !slg ) {
			slg = CreateStyleLinkGroup( sn );
			Dict_Add( group, sn->fullname, slg );
		}
		if( i == 0 ) {
			strcpy( fullname, "*" );
		} else {
			strcpy( fullname, buf );
		}
		link = Dict_FetchValue( slg->links, fullname );
		if( !link ) {
			link = CreateStyleLink();
			link->group = slg;
			link->selector = strdup2( fullname );
			Dict_Add( slg->links, fullname, link );
		}
		if( i == 0 ) {
			strcpy( buf, sn->fullname );
			strcpy( fullname, buf );
		} else {
			strcpy( fullname, buf );
			sprintf( buf, "%s %s", sn->fullname, fullname );
		}
		/* 如果有上一级的父链接记录，则将当前链接添加进去 */
		if( parents ) {
			if( !Dict_FetchValue( parents, sn->fullname ) ) {
				Dict_Add( parents, sn->fullname, link );
			}
		}
		parents = link->parents;
	}
	if( !link ) {
		return NULL;
	}
	snode = NEW( StyleNodeRec, 1 );
	if( space ) {
		snode->space = NEW( char, strlen(space) + 1 );
		strcpy( snode->space, space );
	} else {
		snode->space = NULL;
	}
	snode->sheet = StyleSheet();
	snode->rank = selector->rank;
	snode->selector = strdup2( fullname );
	snode->batch_num = selector->batch_num;
	LinkedList_Append( &link->styles, snode );
	return snode->sheet;
}

int LCUI_PutStyleSheet( LCUI_Selector selector,
			LCUI_StyleSheet in_ss, const char *space )
{
	LCUI_StyleSheet ss;
	LCUIMutex_Lock( &library.mutex );
	Dict_Empty( library.cache );
	ss = LCUI_SelectStyleSheet( selector, space );
	if( ss ) {
		StyleSheet_Replace( ss, in_ss );
	}
	LCUIMutex_Unlock( &library.mutex );
	return 0;
}

static int StyleLink_GetStyleSheets( StyleLink link, LinkedList *outlist )
{
	StyleNode snode, out_snode;
	LinkedListNode *node, *out_node;

	if( !outlist ) {
		return link->styles.length;
	}
	for( LinkedList_Each( node, &link->styles ) ) {
		int pos = -1, i = 0;
		snode = node->data;
		for( LinkedList_Each( out_node, outlist ) ) {
			out_snode = out_node->data;
			if( snode->rank > out_snode->rank ) {
				pos = i;
				break;
			}
			if( snode->rank != out_snode->rank ) {
				i += 1;
				continue;
			}
			if( snode->batch_num > out_snode->batch_num ) {
				pos = i;
				break;
			}
			i += 1;
		}
		if( pos >= 0 ) {
			LinkedList_Insert( outlist, pos, snode );
		} else {
			LinkedList_Append( outlist, snode );
		}
	}
	return link->styles.length;
}

static int LCUI_FindStyleSheetFromLink( StyleLink link, LCUI_Selector s,
					int i, LinkedList *list )
{
	int count = 0;
	StyleLink parent;
	LinkedList names;
	LinkedListNode *node;
	LCUI_SelectorNode sn;

	LinkedList_Init( &names );
	count += StyleLink_GetStyleSheets( link, list );
	while( --i >= 0 ) {
		sn = s->nodes[i];
		SelectorNode_GetNames( sn, &names );
		for( LinkedList_Each( node, &names ) ) {
			parent = Dict_FetchValue( link->parents, node->data );
			if( !parent ) {
				continue;
			}
			count += LCUI_FindStyleSheetFromLink( parent, s,
							      i, list );
		}
		LinkedList_Clear( &names, free );
	}
	return count;
}

int LCUI_FindStyleSheetFromGroup( int group, const char *name, 
				  LCUI_Selector s, LinkedList *list )
{
	int i, count;
	Dict *groups;
	StyleLinkGroup slg;
	LinkedListNode *node;
	LinkedList names;

	groups = LinkedList_Get( &library.groups, group );
	if( !groups || s->length < 1 ) {
		return 0;
	}
	count = 0;
	i = s->length - 1;
	LinkedList_Init( &names );
	if( name ) {
		LinkedList_Append( &names, strdup2( name ) );
	} else {
		SelectorNode_GetNames( s->nodes[i], &names );
		LinkedList_Append( &names, strdup2( "*" ) );
	}
	for( LinkedList_Each( node, &names ) ) {
		DictEntry *entry;
		DictIterator *iter;
		char *name = node->data;
		slg = Dict_FetchValue( groups, name );
		if( !slg ) {
			continue;
		}
		iter = Dict_GetIterator( slg->links );
		while( (entry = Dict_Next( iter )) ) {
			StyleLink link = DictEntry_GetVal( entry );
			count += LCUI_FindStyleSheetFromLink( link, s, 
							      i, list );
		}
		Dict_ReleaseIterator( iter );
	}
	LinkedList_Clear( &names, free );
	return count;
}

void LCUI_PrintStyleSheet( LCUI_StyleSheet ss )
{
	int key;
	const char *name;
	for( key = 0; key < ss->length; ++key ) {
		LCUI_Style s = &ss->sheet[key];
		if( !s->is_valid ) {
			continue;
		}
		name = LCUI_GetStyleName( key );
		if( name ) {
			LOG( "\t%s", name );
		} else {
			LOG( "\t<unknown style %d>", key );
		}
		LOG( "%s: ", key > STYLE_KEY_TOTAL ? " (+)" : "" );
		switch( s->type ) {
		case SVT_AUTO:
			LOG( "auto" );
			break;
		case SVT_BOOL:
			LOG( "%s", s->val_bool ? "true" : "false" );
			break;
		case SVT_COLOR: {
			LCUI_Color *clr = &s->val_color;
			if( clr->alpha < 255 ) {
				LOG( "rgba(%d,%d,%d,%g)", clr->r,
					clr->g, clr->b, clr->a / 255.0 );
			} else {
				LOG( "#%02x%02x%02x", clr->r, clr->g, clr->b );
			}
			break;
		}
		case SVT_PX:
			LOG( "%gpx", s->val_px );
			break;
		case SVT_DIP:
			LOG( "%gdip", s->val_dip );
			break;
		case SVT_SP:
			LOG( "%gsp", s->val_sp );
			break;
		case SVT_STRING:
			LOG( "%s", s->val_string );
			break;
		case SVT_SCALE:
			LOG( "%g%%", s->val_scale * 100 );
			break;
		case SVT_STYLE:
			LOG( "%s", LCUI_GetStyleValueName( s->val_style ) );
			break;
		case SVT_VALUE:
			LOG( "%d", s->val_int );
			break;
		default:
			LOG( "%g", s->value );
			break;
		}
		LOG( ";\n" );
	}
}

void LCUI_PrintSelector( LCUI_Selector selector )
{
	char path[MAX_SELECTOR_LEN];
	LCUI_SelectorNode *sn;

	path[0] = 0;
	for( sn = selector->nodes; *sn; ++sn ) {
		strcat( path, (*sn)->fullname );
		strcat( path, " " );
	}
	LOG("path: %s (rank = %d, batch_num = %d)\n", path,
		selector->rank, selector->batch_num);
}

static void LCUI_PrintStyleLink( StyleLink link, const char *selector )
{
	DictEntry *entry;
	DictIterator *iter;
	LinkedListNode *node;
	char fullname[MAX_SELECTOR_LEN];
	if( selector ) {
		sprintf( fullname, "%s %s", link->group->name, selector );
	} else {
		strcpy( fullname, link->group->name );
	}
	for( LinkedList_Each( node, &link->styles ) ) {
		StyleNode snode = node->data;
		LOG( "\n[%s]", snode->space ? snode->space : "<none>" );
		LOG( "[rank: %d]\n%s {\n", snode->rank, fullname );
		LCUI_PrintStyleSheet( snode->sheet );
		LOG("}\n");
	}
	iter = Dict_GetIterator( link->parents );
	while( (entry = Dict_Next( iter )) ) {
		StyleLink parent = DictEntry_GetVal( entry );
		LCUI_PrintStyleLink( parent, fullname );
	}
	Dict_ReleaseIterator( iter );
}

void LCUI_PrintCSSLibrary( void )
{
	Dict *group;
	StyleLink link;
	StyleLinkGroup slg;
	DictIterator *iter;
	DictEntry *entry;

	link = NULL;
	LOG( "style library begin\n" );
	group = LinkedList_Get( &library.groups, 0 );
	iter = Dict_GetIterator( group );
	while( (entry = Dict_Next(iter)) ) {
		DictEntry *entry_slg;
		DictIterator *iter_slg;
		slg = DictEntry_GetVal( entry );
		iter_slg = Dict_GetIterator( slg->links );
		while( (entry_slg = Dict_Next(iter_slg)) ) {
			link = DictEntry_GetVal( entry_slg );
			LCUI_PrintStyleLink( link, NULL );
		}
		Dict_ReleaseIterator( iter_slg );
	}
	Dict_ReleaseIterator( iter );
	LOG( "style library end\n" );
}

void LCUI_GetStyleSheet( LCUI_Selector s, LCUI_StyleSheet out_ss )
{
	LinkedList list;
	LinkedListNode *node;
	LCUI_StyleSheet ss;
	LinkedList_Init( &list );
	StyleSheet_Clear( out_ss );
	LCUIMutex_Lock( &library.mutex );
	ss = Dict_FetchValue( library.cache, &s->hash );
	if( ss ) {
		StyleSheet_Replace( out_ss, ss );
		LCUIMutex_Unlock( &library.mutex );
		return;
	}
	ss = StyleSheet();
	LCUI_FindStyleSheet( s, &list );
	for( LinkedList_Each( node, &list ) ) {
		StyleNode sn = node->data;
		StyleSheet_Merge( ss, sn->sheet );
	}
	LinkedList_Clear( &list, NULL );
	Dict_Add( library.cache, &s->hash, ss );
	StyleSheet_Replace( out_ss, ss );
	LCUIMutex_Unlock( &library.mutex );
}

void LCUI_PrintStyleSheetsBySelector( LCUI_Selector s )
{
	LinkedList list;
	LinkedListNode *node;
	LCUI_StyleSheet ss;
	LinkedList_Init( &list );
	ss = StyleSheet();
	LCUI_FindStyleSheet( s, &list );
	LOG( "selector(%u) stylesheets begin\n", s->hash );
	for( LinkedList_Each( node, &list ) ) {
		StyleNode sn = node->data;
		LOG( "\n[%s]", sn->space ? sn->space : "<none>" );
		LOG( "[rank: %d]\n%s {\n", sn->rank, sn->selector );
		LCUI_PrintStyleSheet( sn->sheet );
		LOG("}\n");
		StyleSheet_Merge( ss, sn->sheet );
	}
	LinkedList_Clear( &list, NULL );
	LOG( "[selector(%u) final stylesheet] {\n", s->hash );
	LCUI_PrintStyleSheet( ss );
	LOG( "}\n");
	StyleSheet_Delete( ss );
	LOG( "selector(%u) stylesheets end\n", s->hash );
}

static void StyleSheetCacheDestructor( void *privdata, void *val )
{
	StyleSheet_Delete( val );
}

static void *DupStyleName( void *privdata, const void *val )
{
	return strdup2( val );
}

static void StyleNameDestructor( void *privdata, void *val )
{
	free( val );
}

static unsigned int IntKeyDict_HashFunction( const void *key )
{
	return Dict_IdentityHashFunction( *(unsigned int*)key );
}

static int IntKeyDict_KeyCompare( void *privdata,
				 const void *key1, const void *key2 )
{
	return *(unsigned int*)key1 == *(unsigned int*)key2;
}

static void IntKeyDict_KeyDestructor( void *privdata, void *key )
{
	free( key );
}

static void *IntKeyDict_KeyDup( void *privdata, const void *key )
{
	unsigned int *newkey = malloc( sizeof(unsigned int) );
	*newkey = *(unsigned int*)key;
	return newkey;
}

static void LCUI_InitStylesheetCache( void )
{
	static DictType cachedict;
	cachedict.valDup = NULL;
	cachedict.keyDup = IntKeyDict_KeyDup;
	cachedict.keyCompare = IntKeyDict_KeyCompare;
	cachedict.hashFunction = IntKeyDict_HashFunction;
	cachedict.keyDestructor = IntKeyDict_KeyDestructor;
	cachedict.valDestructor = StyleSheetCacheDestructor;
	cachedict.keyDestructor = IntKeyDict_KeyDestructor;
	library.cache = Dict_Create( &cachedict, NULL );
}

static void LCUI_DestroyStylesheetCache( void )
{
	Dict_Release( library.cache );
	library.cache = NULL;
}

static void LCUI_InitStyleNameLibrary( void )
{
	static DictType namedict = { 0 };
	namedict.valDup = DupStyleName;
	namedict.keyDup = IntKeyDict_KeyDup;
	namedict.keyCompare = IntKeyDict_KeyCompare;
	namedict.valDestructor = StyleNameDestructor;
	namedict.hashFunction = IntKeyDict_HashFunction;
	namedict.keyDestructor = IntKeyDict_KeyDestructor;
	library.names = Dict_Create( &namedict, NULL );
}

static void LCUI_DestroyStyleNameLibrary( void )
{
	Dict_Release( library.names );
	library.names = NULL;
}

static void LCUI_InitStyleValueLibrary( void )
{
	static DictType valdict, namedict = { 0 };
	valdict = DictType_StringKey;
	namedict.keyCompare = IntKeyDict_KeyCompare;
	namedict.hashFunction = IntKeyDict_HashFunction;
	valdict.valDestructor = KeyNameGroupDestructor;
	/* value_keys 表用于存放 key 和 name 数据 */
	library.value_keys = Dict_Create( &valdict, NULL );
	/* value_names 表仅引用 value_keys 里的数据  */
	library.value_names = Dict_Create( &namedict, NULL );
}

static void LCUI_DestroyStyleValueLibrary( void )
{
	Dict_Release( library.value_names );
	Dict_Release( library.value_keys );
	library.value_keys = NULL;
	library.value_names = NULL;
}

void LCUI_InitCSSLibrary( void )
{
	KeyNameGroup skn, skn_end;
	LCUI_InitStylesheetCache();
	LCUI_InitStyleNameLibrary();
	LCUI_InitStyleValueLibrary();
	LCUIMutex_Init( &library.mutex );
	LinkedList_Init( &library.groups );
	skn_end = style_name_map + LEN( style_name_map );
	for( skn = style_name_map; skn < skn_end; ++skn ) {
		LCUI_DirectAddStyleName( skn->key, skn->name );
	}
	skn_end = style_value_map + LEN( style_value_map );
	for( skn = style_value_map; skn < skn_end; ++skn ) {
		LCUI_AddStyleValue( skn->key, skn->name );
	}
	library.count = STYLE_KEY_TOTAL;
	library.is_inited = TRUE;
}

void LCUI_FreeCSSLibrary( void )
{
	library.is_inited = FALSE;
	LCUI_DestroyStylesheetCache();
	LCUI_DestroyStyleNameLibrary();
	LCUI_DestroyStyleValueLibrary();
	LCUIMutex_Destroy( &library.mutex );
	LinkedList_Clear( &library.groups, (FuncPtr)DeleteStyleGroup );
}
