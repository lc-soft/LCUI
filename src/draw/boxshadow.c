/* ****************************************************************************
 * boxshadow.c -- draw box shadow.
 * 
 * Copyright (C) 2014 by
 * Liu Chao <lc-soft@live.cn>
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
 * boxshadow.c -- 矩形阴影
 *
 * 版权所有 (C) 2014 归属于
 * 刘超 <lc-soft@live.cn>
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
#include LC_LCUI_H
#include LC_GRAPH_H

static void Graph_DrawTopShadow( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow )
{
        int x, y, bound_x, bound_y;
        LCUI_Color color;
        LCUI_Rect shadow_area, box_area;

        box_area.x = BoxShadow_GetBoxX( &shadow );
        box_area.y = BoxShadow_GetBoxY( &shadow );
        box_area.w = BoxShadow_GetWidth( &shadow, graph->w );
        box_area.h = BoxShadow_GetHeight( &shadow, graph->h );
        /* 计算需要绘制上边阴影的区域 */
        shadow_area.x = box_area.x;
        shadow_area.y = BoxShadow_GetY( &shadow );
        shadow_area.w = graph->w - shadow.size*2;
        shadow_area.h = shadow.size;
        color = shadow.top_color;
        bound_x = shadow_area.x + shadow_area.w;
        bound_y = shadow_area.y + shadow_area.h;

        for( y=shadow_area.y; y<bound_y; ++y ) {
                /* 忽略不在有效区域内的像素 */
                if( y < area.y || y >= area.y + area.h 
                 || y >= box_area.y || y <= box_area.y + box_area.h ) {
                        continue;
                }
                /* 计算当前行阴影的透明度 */
                color.alpha = 255*(y-shadow_area.y)/shadow_area.h;
                for( x=shadow_area.x; x<bound_x; ++x ) {
                        if( x < area.x || x >= area.x + area.w
                         || x >= box_area.x || x <= box_area.x + box_area.w ) {
                                continue;
                        }
                        Graph_SetPixel( graph, x, y, shadow.top_color );
                }
        }
}

static void Graph_DrawBottomShadow( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow )
{
        int x, y, bound_x, bound_y;
        LCUI_Color color;
        LCUI_Rect shadow_area, box_area;

        box_area.x = BoxShadow_GetBoxX( &shadow );
        box_area.y = BoxShadow_GetBoxY( &shadow );
        box_area.w = BoxShadow_GetWidth( &shadow, graph->w );
        box_area.h = BoxShadow_GetHeight( &shadow, graph->h );
        shadow_area.x = box_area.x;
        shadow_area.y = BoxShadow_GetY( &shadow );
        shadow_area.y += (box_area.y + box_area.h);
        shadow_area.w = graph->w - shadow.size*2;
        shadow_area.h = shadow.size;
        color = shadow.top_color;
        bound_x = shadow_area.x + shadow_area.w;
        bound_y = shadow_area.y + shadow_area.h;

        for( y=shadow_area.y; y<bound_y; ++y ) {
                if( y < area.y || y >= area.y + area.h 
                 || y >= box_area.y || y <= box_area.y + box_area.h ) {
                        continue;
                }
                color.alpha = 255*(bound_y-y)/shadow_area.h;
                for( x=shadow_area.x; x<bound_x; ++x ) {
                        if( x < area.x || x >= area.x + area.w
                         || x >= box_area.x || x <= box_area.x + box_area.w ) {
                                continue;
                        }
                        Graph_SetPixel( graph, x, y, shadow.bottom_color );
                }
        }
}

static void Graph_DrawLeftShadow( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow )
{
        int x, y, bound_x, bound_y;
        LCUI_Color color;
        LCUI_Rect shadow_area, box_area;

        box_area.x = BoxShadow_GetBoxX( &shadow );
        box_area.y = BoxShadow_GetBoxY( &shadow );
        box_area.w = BoxShadow_GetWidth( &shadow, graph->w );
        box_area.h = BoxShadow_GetHeight( &shadow, graph->h );
        shadow_area.x = BoxShadow_GetX( &shadow );
        shadow_area.y = box_area.y;
        shadow_area.w = shadow.size;
        shadow_area.h = graph->h - shadow.size*2;
        color = shadow.top_color;
        bound_x = shadow_area.x + shadow_area.w;
        bound_y = shadow_area.y + shadow_area.h;

        for( x=shadow_area.x; x<bound_x; ++x ) {
                if( x < area.x || x >= area.x + area.w
                 || x >= box_area.x || x <= box_area.x + box_area.w ) {
                        continue;
                }
                color.alpha = 255*(x-shadow_area.x)/shadow_area.w;
                for( y=shadow_area.y; y<bound_y; ++y ) {
                        if( y < area.y || y >= area.y + area.h 
                         || y >= box_area.y || y <= box_area.y + box_area.h ) {
                                continue;
                        }
                        Graph_SetPixel( graph, x, y, shadow.left_color );
                }
        }
}

static void Graph_DrawRightShadow( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow )
{
        int x, y, bound_x, bound_y;
        LCUI_Color color;
        LCUI_Rect shadow_area, box_area;

        box_area.x = BoxShadow_GetBoxX( &shadow );
        box_area.y = BoxShadow_GetBoxY( &shadow );
        box_area.w = BoxShadow_GetWidth( &shadow, graph->w );
        box_area.h = BoxShadow_GetHeight( &shadow, graph->h );
        shadow_area.x = BoxShadow_GetX( &shadow );
        shadow_area.x += (box_area.x + box_area.w);
        shadow_area.y = BoxShadow_GetY( &shadow );
        shadow_area.y += (box_area.y + box_area.h);
        shadow_area.w = shadow.size;
        shadow_area.h = graph->h - shadow.size*2;
        color = shadow.top_color;
        bound_x = shadow_area.x + shadow_area.w;
        bound_y = shadow_area.y + shadow_area.h;

        for( x=shadow_area.x; x<bound_x; ++x ) {
                if( x < area.x || x >= area.x + area.w
                 || x >= box_area.x || x <= box_area.x + box_area.w ) {
                        continue;
                }
                color.alpha = 255*(bound_x-x)/shadow_area.w;
                for( y=shadow_area.y; y<bound_y; ++y ) {
                        if( y < area.y || y >= area.y + area.h 
                         || y >= box_area.y || y <= box_area.y + box_area.h ) {
                                continue;
                        }
                        Graph_SetPixel( graph, x, y, shadow.left_color );
                }
        }
}

int Graph_DrawBoxShadowEx( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow )
{
        /* 判断图像尺寸是否低于阴影占用的最小尺寸 */
        if( BoxShadow_GetWidth(&shadow, 0) < graph->w
         || BoxShadow_GetHeight(&shadow, 0) < graph->h ) {
                return -1;
        }

        Graph_DrawTopShadow( graph, area, shadow );
        Graph_DrawBottomShadow( graph, area, shadow );
        Graph_DrawLeftShadow( graph, area, shadow );
        Graph_DrawRightShadow( graph, area, shadow );
        return 0;
}
