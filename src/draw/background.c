/* ***************************************************************************
 * background.c -- graph background image draw support.
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
 * background.c -- 背景图像绘制支持
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

void Graph_DrawBackground( LCUI_PaintContext paint,
			   const LCUI_Rect *box,
			   const LCUI_Background *bg )
{
	double scale;
	LCUI_Graph graph, buffer;
	LCUI_Rect rect, read_rect;
	int x, y, width, height;
	/* 获取当前绘制区域与背景内容框的重叠区域 */
	if( !LCUIRect_GetOverlayRect( box, &paint->rect, &rect ) ) {
		return;
	}
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	Graph_Init( &buffer );
	Graph_Quote( &graph, &paint->canvas, &rect );
	Graph_FillRect( &graph, bg->color, NULL, TRUE );
	/* 将坐标转换为相对于背景内容框 */
	rect.x += paint->rect.x - box->x;
	rect.y += paint->rect.y - box->y;
	/* 保存背景图像区域 */
	read_rect.x = x = bg->position.x;
	read_rect.y = y = bg->position.y;
	read_rect.width = width = bg->size.width;
	read_rect.height = height = bg->size.height;
	/* 获取当前绘制区域与背景图像的重叠区域 */
	if( !LCUIRect_GetOverlayRect( &read_rect, &rect, &read_rect ) ) {
		return;
	}
	/* 转换成相对于图像的坐标 */
	read_rect.x -= bg->position.x;
	read_rect.y -= bg->position.y;
	/* 如果尺寸没有变化则直接引用 */
	if( bg->size.width == bg->image->width &&
	    bg->size.height == bg->image->height ) {
		Graph_QuoteReadOnly( &graph, bg->image, &read_rect );
		/* 转换成相对于当前绘制区域的坐标 */
		x += box->x - paint->rect.x + read_rect.x;
		y += box->y - paint->rect.y + read_rect.y;
	} else {
		rect = read_rect;
		/* 根据宽高的缩放比例，计算实际需要引用的区域 */
		if( width != bg->image->width ) {
			scale = 1.0 * bg->image->width / width;
			rect.x = roundi( rect.x * scale );
			rect.width = roundi( rect.width * scale );
		}
		if( height != bg->image->height ) {
			scale = 1.0 * bg->image->height / height;
			rect.y = roundi( rect.y * scale );
			rect.height = roundi( rect.height * scale );
		}
		/* 引用源背景图像的一块区域 */
		Graph_QuoteReadOnly( &graph, bg->image, &rect );
		width = read_rect.width;
		height = read_rect.height;
		/* 按比例进行缩放 */
		Graph_Zoom( &graph, &buffer, FALSE, width, height );
		Graph_QuoteReadOnly( &graph, &buffer, NULL );
	}
	/* 计算相对于绘制区域的坐标 */
	x += read_rect.x + box->x - paint->rect.x;
	y += read_rect.y + box->y - paint->rect.y;
	Graph_Mix( &paint->canvas, &graph, x, y, bg->color.alpha < 255 );
	Graph_Free( &buffer );
}
