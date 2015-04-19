/* ***************************************************************************
 * background.c -- graph background image draw support.
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
 * ***************************************************************************/

/* ****************************************************************************
 * background.c -- 背景图像绘制支持
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

/** 初始化背景绘制参数 */
void Background_Init( LCUI_Background *bg )
{
	bg->color = RGB( 255, 255, 255 );
	Graph_Init( &bg->image );
	bg->size.using_value = TRUE;
	bg->size.value = SV_AUTO;
	bg->position.using_value = FALSE;
	bg->position.x.px = 0;
	bg->position.y.px = 0;
	bg->position.x.type = SVT_PX;
	bg->position.y.type = SVT_PX;
}

/** 
 * 在图层上绘制背景 
 * @param graph		需进行绘制的图层
 * @param bg		背景样式数据
 * @param box_size	容器尺寸
 * @param rect		背景中实际需要绘制的区域，如果为NULL，则绘制整个背景
 */
void Graph_DrawBackground(
	LCUI_Graph		*graph,
	const LCUI_Background	*bg,
	const LCUI_Size		*box_size,
	const LCUI_Rect		*rect
)
{
	LCUI_Graph image;
	LCUI_Size image_size;
	LCUI_Pos image_pos;

	Graph_FillColor( graph, bg->color );
	/* 计算背景图应有的尺寸 */
	if( bg->size.using_value ) {
		switch( bg->size.value ) {
		case SV_CONTAIN:
			/* 取宽和高里最大的一个来计算缩放后的图形尺寸 */
			if( bg->image.width > bg->image.height ) {
				image_size.w = box_size->w;
				image_size.h = bg->image.height;
				image_size.h *= (bg->image.width / box_size->w);
			} else {
				image_size.h = box_size->h;
				image_size.w = bg->image.width;
				image_size.w *= (bg->image.height / box_size->h);
			}
			break;
		case SV_COVER:
			/* 取宽和高里最小的一个来计算缩放后的图形尺寸 */
			if( bg->image.width < bg->image.height ) {
				image_size.w = box_size->w;
				image_size.h = bg->image.height;
				image_size.h *= (bg->image.width / box_size->w);
			} else {
				image_size.h = box_size->h;
				image_size.w = bg->image.width;
				image_size.w *= (bg->image.height / box_size->h);
			}
			break;
		case SV_AUTO:
		default:
			Graph_GetSize( &bg->image, &image_size );
			break;
		}
	} else {
		switch( bg->size.w.type ) {
		case SVT_SCALE:
			image_size.w = box_size->w * bg->size.w.scale;
			break;
		case SVT_PX:
			image_size.w = bg->size.w.px;
			break;
		default:
			image_size.w = 0;
			break;
		}
		switch( bg->size.h.type ) {
		case SVT_SCALE:
			image_size.h = box_size->h * bg->size.h.scale;
			break;
		case SVT_PX:
			image_size.h = bg->size.h.px;
			break;
		default:
			image_size.h = 0;
			break;
		}
	}
	/* 计算背景图的像素坐标 */
	if( bg->position.using_value ) {
		switch( bg->position.value ) {
		case SV_TOP:
		case SV_TOP_CENTER:
			image_pos.x = (box_size->w - image_size.w) / 2;
			image_pos.y = 0;
			break;
		case SV_CENTER_LEFT:
			image_pos.x = 0;
			image_pos.y = (box_size->h - image_size.h) / 2;
			break;
		case SV_CENTER:
		case SV_CENTER_CENTER:
			image_pos.x = (box_size->w - image_size.w) / 2;
			image_pos.y = (box_size->h - image_size.h) / 2;
			break;
		case SV_CENTER_RIGHT:
			image_pos.x = box_size->w - image_size.w;
			image_pos.y = (box_size->h - image_size.h) / 2;
			break;
		case SV_BOTTOM_LEFT:
			image_pos.x = 0;
			image_pos.y = box_size->h - image_size.h;
			break;
		case SV_BOTTOM_CENTER:
			image_pos.x = (box_size->w - image_size.w) / 2;
			image_pos.y = box_size->h - image_size.h;
			break;
		case SV_BOTTOM_RIGHT:
			image_pos.x = box_size->w - image_size.w;
			image_pos.y = box_size->h - image_size.h;
			break;
		case SV_TOP_RIGHT:
		default:
			image_pos.x = 0;
			image_pos.y = 0;
			break;
		}
	} else {
		switch( bg->position.x.type ) {
		case SVT_SCALE:
			image_pos.x = box_size->w - image_size.w;
			image_pos.x *= bg->position.x.scale;
			break;
		case SVT_PX:
			image_pos.x = bg->position.x.px;
			break;
		default:
			image_pos.x = 0;
			break;
		}
		switch( bg->position.y.type ) {
		case SVT_SCALE:
			image_pos.y = box_size->h - image_size.h;
			image_pos.y *= bg->position.y.scale;
			break;
		case SVT_PX:
			image_pos.y = bg->position.y.px;
			break;
		default:
			image_pos.y = 0;
			break;
		}
	}
	// 绘制 ...
	// code ...
	Graph_WritePNG( "debug_image.png", graph );
}
