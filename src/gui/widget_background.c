/* ***************************************************************************
* widget_background.c -- the widget background style processing module.
*
* Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
* widget_background.c -- 部件背景样式处理模块
*
* 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/gui/widget.h>

typedef struct ImageCacheRec_ {
	int ref_count;
	LCUI_Graph image;
	char *path;
} ImageCacheRec, *ImageCache;

typedef struct ImageRefRec_{
	LCUI_Widget widget;
	ImageCache cache;
} ImageRefRec, *ImageRef;

LCUI_BOOL is_inited = FALSE;
LCUI_RBTree images, refs;

static void AddRef( LCUI_Widget widget, ImageCache cache )
{
	RBTree_CustomInsert( &refs, widget, cache );
	cache->ref_count += 1;
}

static void DelRef( LCUI_Widget widget, ImageCache cache )
{
	cache->ref_count -= 1;
	RBTree_CustomErase( &refs, widget );
	if( cache->ref_count <= 0 ) {
		RBTree_CustomErase( &images, cache->path );
	}
}

static void ExecLoadImage( void *arg1, void *arg2 )
{
	char *path = arg2;
	LCUI_Graph image;
	LCUI_Widget widget = arg1;
	ImageCache cache;

	Graph_Init( &image );
	if( Graph_LoadImage( path, &image ) != 0 ) {
		return;
	}
	cache = NEW(ImageCacheRec, 1);
	cache->ref_count = 0;
	cache->image = image;
	cache->path = path;
	RBTree_CustomInsert( &images, path, cache );
	AddRef( widget, cache );
	Graph_Quote( &widget->computed_style.background.image,
		     &cache->image, NULL );
	Widget_AddTask( widget, WTT_BODY );
}

static int OnCompareWidget( void *data, const void *keydata )
{
	ImageRef ref = data;
	if( ref->widget == keydata ) {
		return 0;
	}
	if( (void*)ref->widget > keydata ) {
		return 1;
	}
	return -1;
}

static int OnComparePath( void *data, const void *keydata )
{
	return strcmp(((ImageCache)data)->path, (const char*)keydata);
}

static void OnDestroyCache( void *arg )
{
	ImageCache cache = arg;
	Graph_Free( &cache->image );
	free( cache->path );
	cache->path = NULL;
}

static void AsyncLoadImage( LCUI_Widget widget, const char *path )
{
	ImageCache cache;
	LCUI_Task task = {0};
	LCUI_Style s = &widget->cached_style->sheet[key_background_image];

	if( !is_inited ) {
		RBTree_Init( &images );
		RBTree_Init( &refs );
		RBTree_OnJudge( &refs, OnCompareWidget );
		RBTree_OnJudge( &images, OnComparePath );
		RBTree_OnDestroy( &refs, free );
		RBTree_OnDestroy( &images, OnDestroyCache );
		is_inited = TRUE;
	}
	if( s->is_valid && s->type == SVT_STRING ) {
		cache = RBTree_CustomGetData( &images, s->string );
		if( cache ) {
			DelRef( widget, cache );
		}
	}
	cache = RBTree_CustomGetData( &images, path );
	if( cache ) {
		AddRef( widget, cache );
		Graph_Quote( &widget->computed_style.background.image,
			     &cache->image, NULL );
		Widget_AddTask( widget, WTT_BODY );
		return;
	}
	task.func = ExecLoadImage;
	task.arg[0] = widget;
	task.arg[1] = strdup( path );
	LCUI_AddTask( &task );
}

/** 计算背景样式 */
void Widget_ComputeBackgroundStyle( LCUI_Widget widget )
{
	LCUI_Style s;
	LCUI_StyleSheet ss = widget->style;
	LCUI_Background *bg = &widget->computed_style.background;
	int key = key_background_start + 1;

	for( ; key < key_background_end; ++key ) {
		s = &ss->sheet[key];
		if( !s->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_background_color:
			bg->color = s->color;
			break;
		case key_background_image:
			switch( s->type ) {
			case SVT_IMAGE:
				if( !s->image ) {
					Graph_Init( &bg->image );
					break;
				}
				Graph_Quote( &bg->image, s->image, NULL );
				break;
			case SVT_STRING:
				AsyncLoadImage( widget, s->string );
			default: break;
			}
			break;
		case key_background_position:
			bg->position.using_value = TRUE;
			bg->position.value = s->value;
			break;
		case key_background_position_x:
			bg->position.using_value = FALSE;
			bg->position.x = *s;
			break;
		case key_background_position_y:
			bg->position.using_value = FALSE;
			bg->position.y = *s;
			break;
		case key_background_size:
			bg->size.using_value = TRUE;
			bg->position.value = s->value;
			break;
		case key_background_size_width:
			bg->size.using_value = FALSE;
			bg->size.w = *s;
			break;
		case key_background_size_height:
			bg->size.using_value = FALSE;
			bg->size.h = *s;
			break;
		default: break;
		}
	}
	Widget_AddTask( widget, WTT_BODY );
}
