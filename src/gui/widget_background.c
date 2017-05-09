/* ***************************************************************************
 * widget_background.c -- the widget background style processing module.
 * 
 * Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_background.c -- 部件背景样式处理模块
 *
 * 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/image.h>
#include <LCUI/gui/metrics.h>
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
RBTree images, refs;

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
	if( LCUI_ReadImageFile( path, &image ) != 0 ) {
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
	LCUI_AppTaskRec task = {0};
	LCUI_Style s = &widget->style->sheet[key_background_image];

	if( !is_inited ) {
		RBTree_Init( &images );
		RBTree_Init( &refs );
		RBTree_OnCompare( &refs, OnCompareWidget );
		RBTree_OnCompare( &images, OnComparePath );
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
	task.arg[1] = strdup2( path );
	task.destroy_arg[1] = free;
	LCUI_PostTask( &task );
}

void Widget_InitBackground( LCUI_Widget w )
{
	LCUI_BackgroundStyle *bg;
	bg = &w->computed_style.background;
	bg->color = RGB( 255, 255, 255 );
	Graph_Init( &bg->image );
	bg->size.using_value = TRUE;
	bg->size.value = SV_AUTO;
	bg->position.using_value = TRUE;
	bg->position.value = SV_AUTO;
}

void Widget_UpdateBackground( LCUI_Widget widget )
{
	LCUI_Style s;
	LCUI_StyleSheet ss = widget->style;
	LCUI_BackgroundStyle *bg = &widget->computed_style.background;
	int key = key_background_start;

	for( ; key <= key_background_end; ++key ) {
		s = &ss->sheet[key];
		switch( key ) {
		case key_background_color:
			if( s->is_valid ) {
				bg->color = s->color;
			} else {
				bg->color.value = 0;
			}
			break;
		case key_background_image:
			if( !s->is_valid ) {
				Graph_Init( &bg->image );
				break;
			}
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
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->position.using_value = TRUE;
				bg->position.value = s->value;
			} else {
				bg->position.using_value = FALSE;
				bg->position.value = 0;
			}
			break;
		case key_background_position_x:
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->position.using_value = FALSE;
				bg->position.x = *s;
			}
			break;
		case key_background_position_y:
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->position.using_value = FALSE;
				bg->position.y = *s;
			}
			break;
		case key_background_size:
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->size.using_value = TRUE;
				bg->size.value = s->value;
			} else {
				bg->size.using_value = FALSE;
				bg->size.value = 0;
			}
			break;
		case key_background_size_width:
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->size.using_value = FALSE;
				bg->size.width = *s;
			}
			break;
		case key_background_size_height:
			if( s->is_valid && s->type != SVT_NONE ) {
				bg->size.using_value = FALSE;
				bg->size.height = *s;
			}
			break;
		default: break;
		}
	}
	Widget_AddTask( widget, WTT_BODY );
}

/** 计算部件背景样式的实际值 */
void Widget_ComputeBackground( LCUI_Widget w, LCUI_Background *out )
{
	LCUI_StyleType type;
	LCUI_RectF *box = &w->box.border;
	LCUI_BackgroundStyle *bg = &w->computed_style.background;
	float scale, x = 0, y = 0, width, height;

	/* 计算背景图应有的尺寸 */
	if( bg->size.using_value ) {
		switch( bg->size.value ) {
		case SV_CONTAIN:
			width = box->width;
			scale = 1.0f * bg->image.width / width;
			height = bg->image.height / scale;
			if( height > box->height ) {
				height = box->height;
				scale = 1.0f * bg->image.height / box->height;
				width = bg->image.width / scale;
			}
			break;
		case SV_COVER:
			width = box->width;
			scale = 1.0f * bg->image.width / width;
			height = bg->image.height / scale;
			if( height < box->height ) {
				height = box->height;
				scale = 1.0 * bg->image.height / height;
				width = bg->image.width / scale;
			}
			x = (box->width - width) / 2;
			y = (box->height - height) / 2;
			break;
		case SV_AUTO:
		default:
			width = bg->image.width;
			height = bg->image.height;
			break;
		}
		out->position.x = LCUIMetrics_Compute( x, SVT_PX );
		out->position.y = LCUIMetrics_Compute( y, SVT_PX );
		out->size.width = LCUIMetrics_Compute( width, SVT_PX );
		out->size.height = LCUIMetrics_Compute( height, SVT_PX );
	} else {
		type = SVT_PX;
		switch( type ) {
		case SVT_SCALE:
			width = box->width * bg->size.width.scale;
			break;
		case SVT_NONE:
		case SVT_AUTO:
			width = bg->image.width;
			break;
		default:
			width = bg->size.width.value;
			type = bg->size.width.type;
			break;
		}
		out->size.width = LCUIMetrics_Compute( width, type );
		type = SVT_PX;
		switch( type ) {
		case SVT_SCALE:
			height = box->height * bg->size.height.scale;
			break;
		case SVT_NONE:
		case SVT_AUTO:
			height = bg->image.height;
			break;
		default:
			height = bg->size.height.value;
			break;
		}
		out->size.height = LCUIMetrics_Compute( height, type );
	}
	/* 计算背景图的像素坐标 */
	if( bg->position.using_value ) {
		switch( bg->position.value ) {
		case SV_TOP:
		case SV_TOP_CENTER:
			x = (box->width - width) / 2;
			y = 0;
			break;
		case SV_TOP_RIGHT:
			x = box->width - width;
			y = 0;
			break;
		case SV_CENTER_LEFT:
			x = 0;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER:
		case SV_CENTER_CENTER:
			x = (box->width - width) / 2;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER_RIGHT:
			x = box->width - width;
			y = (box->height - height) / 2;
			break;
		case SV_BOTTOM_LEFT:
			x = 0;
			y = box->height - height;
			break;
		case SV_BOTTOM_CENTER:
			x = (box->width - width) / 2;
			y = box->height - height;
			break;
		case SV_BOTTOM_RIGHT:
			x = box->width - width;
			y = box->height - height;
			break;
		case SV_TOP_LEFT:
		default: break;
		}
	} else {
		type = SVT_PX;
		switch( bg->position.x.type ) {
		case SVT_SCALE:
			x = box->width - width;
			x = x * bg->position.x.scale;
			break;
		case SVT_NONE:
		case SVT_AUTO:
			break;
		default:
			x = bg->position.x.value;
			type = bg->position.x.type;
			break;
		}
		out->position.x = LCUIMetrics_Compute( x, type );
		type = SVT_PX;
		switch( bg->position.y.type ) {
		case SVT_SCALE:
			y = box->height - height;
			y = y * bg->position.y.scale;
			break;
		case SVT_NONE:
		case SVT_AUTO:
			break;
		default:
			y = bg->position.y.value;
			type = bg->position.y.type;
			break;
		}
		out->position.y = LCUIMetrics_Compute( y, type );
	}
	out->color = bg->color;
	out->image = &bg->image;
	out->repeat.x = bg->repeat.x;
	out->repeat.y = bg->repeat.y;
}

void Widget_PaintBakcground( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_Rect box;
	LCUI_RectF fbox;
	LCUI_Background background;
	Widget_ComputeBackground( w, &background );
	fbox.x = w->box.padding.x - w->box.graph.x;
	fbox.y = w->box.padding.y - w->box.graph.y;
	fbox.width = w->box.padding.width;
	fbox.height = w->box.padding.height;
	box.x = LCUIMetrics_ComputeActual( fbox.x, SVT_PX );
	box.y = LCUIMetrics_ComputeActual( fbox.y, SVT_PX );
	box.width = LCUIMetrics_ComputeActual( fbox.width, SVT_PX );
	box.height = LCUIMetrics_ComputeActual( fbox.height, SVT_PX );
	Graph_DrawBackground( paint, &box, &background );
}
