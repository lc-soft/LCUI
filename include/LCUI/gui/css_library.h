/* ***************************************************************************
 * css_library.h -- css library operation module.
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * css_library.h -- CSS 样式库操作模块
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_CSS_LIBRARY_H
#define LCUI_CSS_LIBRARY_H

LCUI_BEGIN_HEADER

#define MAX_SELECTOR_LEN	1024
#define MAX_SELECTOR_DEPTH	32

 /** 样式属性名 */
enum LCUI_StyleKeyName {
	// position start
	key_left,
	key_right,
	key_top,
	key_bottom,
	key_position,
	// position end

	// display start
	key_visible,
	key_display,
	// display end

	key_z_index,
	key_opacity,
	key_box_sizing,
	key_width,
	key_min_width,
	key_max_width,
	key_height,
	key_min_height,
	key_max_height,
	
	// margin start
	key_margin_top,
	key_margin_right,
	key_margin_bottom,
	key_margin_left,
	// margin end

	// padding start
	key_padding_top,
	key_padding_right,
	key_padding_bottom,
	key_padding_left,
	// padding end

	key_vertical_align,

	// border start
	key_border_top_width,
	key_border_top_style,
	key_border_top_color,
	key_border_right_width,
	key_border_right_style,
	key_border_right_color,
	key_border_bottom_width,
	key_border_bottom_style,
	key_border_bottom_color,
	key_border_left_width,
	key_border_left_style,
	key_border_left_color,
	key_border_top_left_radius,
	key_border_top_right_radius,
	key_border_bottom_left_radius,
	key_border_bottom_right_radius,
	// border end

	// background start
	key_background_color,
	key_background_image,
	key_background_size,
	key_background_size_width,
	key_background_size_height,
	key_background_repeat,
	key_background_repeat_x,
	key_background_repeat_y,
	key_background_position,
	key_background_position_x,
	key_background_position_y,
	key_background_origin,
	// background end

	// box shadow start
	key_box_shadow_x,
	key_box_shadow_y,
	key_box_shadow_spread,
	key_box_shadow_blur,
	key_box_shadow_color,
	// box shadow end

	key_pointer_events,
	key_focusable,
	STYLE_KEY_TOTAL
};

#define key_display_start	key_visible
#define key_display_end		key_display
#define key_position_start	key_left
#define key_position_end	key_position
#define key_margin_start	key_margin_top
#define key_margin_end		key_margin_left
#define key_padding_start	key_padding_top
#define key_padding_end		key_padding_left
#define key_border_start	key_border_top_width
#define key_border_end		key_border_bottom_right_radius
#define key_background_start	key_background_color
#define key_background_end	key_background_origin
#define key_box_shadow_start	key_box_shadow_x
#define key_box_shadow_end	key_box_shadow_color

typedef struct LCUI_StyleSheetRec_ {
	LCUI_Style sheet;
	int length;
} LCUI_StyleSheetRec, *LCUI_StyleSheet;

/** 选择器结点结构 */
typedef struct LCUI_SelectorNodeRec_ {
	char *id;			/**< ID */
	char *type;			/**< 类型名称 */
	char **classes;			/**< 样式类列表 */
	char **status;			/**< 状态列表 */
	char *fullname;			/**< 全名，由 id、type、classes、status 组合而成 */
	int rank;			/**< 权值 */
} LCUI_SelectorNodeRec, *LCUI_SelectorNode;

/** 选择器结构 */
typedef struct LCUI_SelectorRec_ {
	int rank;			/**< 权值，决定优先级 */
	int batch_num;			/**< 批次号 */
	int length;			/**< 选择器结点长度 */
	unsigned int hash;		/**< 哈希值 */
	LCUI_SelectorNode *nodes;	/**< 选择器结点列表 */
} LCUI_SelectorRec, *LCUI_Selector;


#define CheckStyleType(S, K, T) (S[K].is_valid && S[K].type == SVT_##T)
#define CheckStyleValue(S, K, V) (S[K].is_valid && S[K].type == SV_##V)

#define SetStyle(S, NAME, VAL, TYPE)	S->sheet[NAME].is_valid = TRUE, \
					S->sheet[NAME].type = SVT_##TYPE, \
					S->sheet[NAME].val_##TYPE = VAL

#define UnsetStyle(S, NAME)	S->sheet[NAME].is_valid = FALSE, \
				S->sheet[NAME].type = SVT_NONE, \
				S->sheet[NAME].val_int = 0

#define LCUI_FindStyleSheet(S, L) LCUI_FindStyleSheetFromGroup(0, NULL, S, L)

LCUI_API LCUI_StyleSheet StyleSheet( void );

LCUI_API void StyleSheet_Clear( LCUI_StyleSheet ss );

LCUI_API void StyleSheet_Delete( LCUI_StyleSheet ss );

LCUI_API int StyleSheet_Merge( LCUI_StyleSheet dest, LCUI_StyleSheet src );

LCUI_API int StyleSheet_Replace( LCUI_StyleSheet dest, LCUI_StyleSheet src );

LCUI_API LCUI_Selector Selector( const char *selector );

LCUI_API void Selector_Update( LCUI_Selector s );

LCUI_API void Selector_Delete( LCUI_Selector s );

LCUI_API int SelectorNode_GetNames( LCUI_SelectorNode sn, LinkedList *names );

LCUI_API int SelectorNode_Update( LCUI_SelectorNode node );

LCUI_API void SelectorNode_Delete( LCUI_SelectorNode node );

/** 
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LCUI_API LCUI_BOOL SelectorNode_Match( LCUI_SelectorNode sn1,
				       LCUI_SelectorNode sn2 );

LCUI_API int LCUI_PutStyleSheet( LCUI_Selector selector,
				 LCUI_StyleSheet in_ss, const char *space );

/**
 * 从指定组中查找样式表
 * @param[in] group 组号
 * @param[in] name 选择器结点名称，若为 NULL，则根据选择器结点生成名称
 * @param[in] s 选择器
 * @param[out] list 找到的样式表列表
 */
LCUI_API int LCUI_FindStyleSheetFromGroup( int group, const char *name,
					   LCUI_Selector s, LinkedList *list );

LCUI_API void LCUI_GetStyleSheet( LCUI_Selector s, LCUI_StyleSheet out_ss );

LCUI_API void LCUI_PrintStyleSheetsBySelector( LCUI_Selector s );

LCUI_API int LCUI_SetStyleName( int key, const char *name );

LCUI_API int LCUI_AddStyleName( const char *name );

LCUI_API int LCUI_AddStyleValue( int key, const char *name );

LCUI_API int LCUI_GetStyleValue( const char *str );

LCUI_API const char *LCUI_GetStyleValueName( int val );

LCUI_API const char *LCUI_GetStyleName( int key );

LCUI_API int LCUI_GetStyleTotal( void );

LCUI_API void LCUI_PrintStyleSheet( LCUI_StyleSheet ss );

LCUI_API void LCUI_PrintSelector( LCUI_Selector selector );

LCUI_API void LCUI_PrintCSSLibrary( void );

LCUI_API void LCUI_InitCSSLibrary( void );

LCUI_API void LCUI_ExitCSSLibrary( void );

LCUI_END_HEADER

#endif
