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

void Background_Init( LCUI_Background *bg )
{
	bg->color = RGB( 255, 255, 255 );
	Graph_Init( &bg->image );
	bg->size.using_value = TRUE;
	bg->size.value = SV_AUTO;
	bg->position.using_value = TRUE;
	bg->position.value = SV_AUTO;
}

void Graph_DrawBackground( LCUI_PaintContext paint, const LCUI_Rect *box,
			   LCUI_Background *bg )
{
	float scale;
	LCUI_Graph graph;
	LCUI_BOOL with_alpha;
	LCUI_Rect read_rect, paint_rect;
	int image_x, image_y, image_w, image_h;

	/* 计算背景图应有的尺寸 */
	if( bg->size.using_value ) {
		switch( bg->size.value ) {
		case SV_CONTAIN:
			image_w = box->width;
			scale = 1.0 * bg->image.width / image_w;
			image_h = 1.0 * bg->image.height / scale;
			if( image_h > box->height ) {
				image_h = box->height;
				scale = 1.0 * bg->image.height / box->height;
				image_w = (int)(1.0 * bg->image.width / scale);
			}
			break;
		case SV_COVER:
			image_w = box->width;
			scale = 1.0 * bg->image.width / image_w;
			image_h = 1.0 * bg->image.height / scale;
			if( image_h < box->height ) {
				image_h = box->height;
				scale = 1.0 * bg->image.height / image_h;
				image_w = (int)(1.0 * bg->image.width / scale);
			}
			break;
		case SV_AUTO:
		default:
			image_w = bg->image.width;
			image_h = bg->image.height;
			break;
		}
	} else {
		switch( bg->size.w.type ) {
		case SVT_SCALE:
			image_w = box->w * bg->size.w.scale;
			break;
		case SVT_PX:
			image_w = bg->size.w.px;
			break;
		default:
			image_w = bg->image.width;
			break;
		}
		switch( bg->size.h.type ) {
		case SVT_SCALE:
			image_h = box->h * bg->size.h.scale;
			break;
		case SVT_PX:
			image_h = bg->size.h.px;
			break;
		default:
			image_h = bg->image.height;
			break;
		}
	}
	/* 计算背景图的像素坐标 */
	if( bg->position.using_value ) {
		switch( bg->position.value ) {
		case SV_TOP:
		case SV_TOP_CENTER:
			image_x = (box->w - image_w) / 2;
			image_y = 0;
			break;
		case SV_TOP_RIGHT:
			image_x = box->w - image_w;
			image_y = 0;
			break;
		case SV_CENTER_LEFT:
			image_x = 0;
			image_y = (box->h - image_h) / 2;
			break;
		case SV_CENTER:
		case SV_CENTER_CENTER:
			image_x = (box->w - image_w) / 2;
			image_y = (box->h - image_h) / 2;
			break;
		case SV_CENTER_RIGHT:
			image_x = box->w - image_w;
			image_y = (box->h - image_h) / 2;
			break;
		case SV_BOTTOM_LEFT:
			image_x = 0;
			image_y = box->h - image_h;
			break;
		case SV_BOTTOM_CENTER:
			image_x = (box->w - image_w) / 2;
			image_y = box->h - image_h;
			break;
		case SV_BOTTOM_RIGHT:
			image_x = box->w - image_w;
			image_y = box->h - image_h;
			break;
		case SV_TOP_LEFT:
		default:image_x = image_y = 0; break;
		}
	} else {
		switch( bg->position.x.type ) {
		case SVT_SCALE:
			image_x = box->w - image_w;
			image_x *= bg->position.x.scale;
			break;
		case SVT_PX:
			image_x = bg->position.x.px;
			break;
		default:image_x = 0; break;
		}
		switch( bg->position.y.type ) {
		case SVT_SCALE:
			image_y = box->h - image_h;
			image_y *= bg->position.y.scale;
			break;
		case SVT_PX:
			image_y = bg->position.y.px;
			break;
		default:image_y = 0; break;
		}
	}
	/* 获取当前绘制区域与背景内容框的重叠区域 */
	if( !LCUIRect_GetOverlayRect( box, &paint->rect, &paint_rect ) ) {
		return;
	}
	with_alpha = bg->color.alpha < 255;
	paint_rect.x -= paint->rect.x;
	paint_rect.y -= paint->rect.y;
	Graph_Quote( &graph, &paint->canvas, &paint_rect );
	Graph_FillRect( &graph, bg->color, NULL, TRUE );
	/* 将坐标转换为相对于背景内容框 */
	paint_rect.x += paint->rect.x - box->x;
	paint_rect.y += paint->rect.y - box->y;
	/* 保存背景图像区域 */
	read_rect.x = image_x;
	read_rect.y = image_y;
	read_rect.width = image_w;
	read_rect.height = image_h;
	/* 获取当前绘制区域与背景图像的重叠区域 */
	if( !LCUIRect_GetOverlayRect( &read_rect, &paint_rect, &read_rect ) ) {
		return;
	}
	/* 转换成相对于图像的坐标 */
	read_rect.x -= image_x;
	read_rect.y -= image_y;
	/* 如果尺寸没有变化则直接引用 */
	if( image_w == bg->image.w && image_h == bg->image.h ) {
		Graph_Quote( &graph, &bg->image, &read_rect );
		/* 转换成相对于当前绘制区域的坐标 */
		image_x = image_x + box->x - paint->rect.x;
		image_y = image_y + box->y - paint->rect.y;
		image_x += read_rect.x;
		image_y += read_rect.y;
		Graph_Mix( &paint->canvas, &graph, image_x,
			   image_y, with_alpha );
	} else {
		float scale;
		LCUI_Graph buffer;
		LCUI_Rect quote_rect;

		Graph_Init( &buffer );
		quote_rect = read_rect;
		/* 根据宽高的缩放比例，计算实际需要引用的区域 */
		if( image_w != bg->image.w ) {
			scale = 1.0 * bg->image.width / image_w;
			quote_rect.x *= scale;
			quote_rect.width *= scale;
		}
		if( image_h != bg->image.h ) {
			scale = 1.0 * bg->image.height / image_h;
			quote_rect.y *= scale;
			quote_rect.height *= scale;
		}
		/* 引用源背景图像的一块区域 */
		Graph_Quote( &graph, &bg->image, &quote_rect );
		image_w = read_rect.width;
		image_h = read_rect.height;
		/* 计算相对于绘制区域的坐标 */
		image_x = read_rect.x + image_x;
		image_x = image_x + box->x - paint->rect.x;
		image_y = read_rect.y + image_y;
		image_y = image_y + box->y - paint->rect.y;
		/* 按比例进行缩放 */
		Graph_Zoom( &graph, &buffer, FALSE, image_w, image_h );
		Graph_Mix( &paint->canvas, &buffer, image_x, 
			   image_y, with_alpha );
		Graph_Free( &buffer );
	}
}
