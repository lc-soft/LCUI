/* ***************************************************************************
 * rect.c -- Rectangle area handling
 *
 * Copyright (C) 2013 by
 * Liu Chao
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
 * rect.c -- 矩形区域处理
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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
#include LC_LCUI_H
#include LC_MISC_H

/* 初始化矩形区域的数据 */
LCUI_EXPORT(void)
Rect_Init( LCUI_Rect *rect )
{
	rect->x      = 0;
	rect->y      = 0;
	rect->width  = 0;
	rect->height = 0;
	rect->center_x = 0;
	rect->center_y = 0;
}

/*
 * 功能：检测两个矩形是否成十字架式叠加
 * 返回值：
 *  1 a竖，b横
 * -1 a衡，b竖
 *  0 不叠加
 **/
LCUI_EXPORT(int)
LCUIRect_IsCrossOverlay( LCUI_Rect a, LCUI_Rect b )
{
	if(a.x < b.x && a.y > b.y
	&& a.x + a.width  > b.x + b.width
	&& a.y + a.height < b.y + b.height) {
		return 1;
	}
	if(b.x < a.x && b.y > a.y
	&& b.x + b.width  > a.x + a.width
	&& b.y + b.height < a.y + a.height) {
		return -1;
	}
	return 0;
}

/*
 * 功能：获取需裁剪的区域
 * 说明：指定容器尺寸和容器中的区域位置及尺寸，即可得到该区域中需要进行裁剪区域
 *  */
LCUI_EXPORT(int)
LCUIRect_GetCutArea( LCUI_Size container, LCUI_Rect rect, LCUI_Rect *cut )
{
	int result = 0;
	cut->x = 0;
	cut->y = 0;
	cut->width = rect.width;
	cut->height = rect.height;
	/* 获取需裁剪的区域 */
	if(rect.x < 0) {
		cut->width += rect.x;
		cut->x = 0 - rect.x;
		result = 1;
	}
	if(rect.x + rect.width > container.w) {
		cut->width -= (rect.x + rect.width - container.w);
		result = 1;
	}

	if(rect.y < 0) {
		cut->height += rect.y;
		cut->y = 0 - rect.y;
		result = 1;
	}
	if(rect.y + rect.height > container.h) {
		cut->height -= (rect.y + rect.height - container.h);
		result = 1;
	}
	/* 有效化处理 */
	if( cut->width < 0 ) {
		cut->width = 0;
	}
	if( cut->height < 0 ) {
		cut->height = 0;
	}
	return result;
}

/*
 * 功能：获取指定区域在容器中的有效显示区域
 * 说明：指定容器的区域大小，再指定容器中的区域位置及大小，就能得到该容器实际能显示
 * 出的该区域范围。
 * */
LCUI_EXPORT(LCUI_Rect)
LCUIRect_ValidArea( LCUI_Size container, LCUI_Rect rect )
{
	if (rect.x < 0) {
		rect.width += rect.x;
		rect.x = 0;
	}
	if (rect.y < 0) {
		rect.height += rect.y;
		rect.y = 0;
	}

	if (rect.x + rect.width > container.w) {
		if(rect.x < container.w) {
			rect.width = container.w - rect.x;
		} else {
			rect.width = 0;
		}
	}
	if (rect.y + rect.height > container.h) {
		if(rect.y < container.h) {
			rect.height = container.h - rect.y;
		} else {
			rect.height = 0;
		}
	}
	return rect;
}

/*
 * 功能：检测两个矩形中，A矩形是否包含B矩形
 * 返回值：两不矩形属于包含关系返回1，否则返回0。
 * */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_IncludeRect( LCUI_Rect a, LCUI_Rect b )
{
	int count = 0, m, n = 0, x[4], y[4];
	b.width -= 1;
	b.height -= 1;
	/* 得出矩形b的4个点的坐标 */
	x[0] = b.x;
	y[0] = b.y;
	x[1] = b.x + b.width;
	y[1] = b.y;
	x[2] = b.x;
	y[2] = b.y + b.height;
	x[3] = b.x + b.width;
	y[3] = b.y + b.height;
	for (m = 0; m < 4; ++m) {
		/*printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n",
			m, x[m], a.x, x[m], a.x, a.width,
			y[m], a.y, y[m], a.y, a.height
		);*/
		if ((x[m] >= a.x && x[m] < a.x + a.width)
		 && (y[m] >= a.y && y[m] < a.y + a.height)) {
		/* 如果点(x[m],y[m])在矩形A内 */
			//printf("yes\n");
			count += 1;
		}
	}

	if (count == 4) {
		n = 1;
	} else {
		n = 0;
	}
	return n;
}

/*
 * 功能：将有重叠部分的两个矩形，进行分割，并得到分割后的矩形
 * 说明：主要用于局部区域刷新里，添加的需刷新的区域有可能会与已添加的区域重叠，为避免
 * 重复刷新同一块区域，需要在添加时对矩形进行分割，得到完全重叠和不重叠的矩形。
 * 参数说明：
 * old ： 已存在的矩形区域
 * new ： 将要添加的矩形区域
 * rq  ： 指向矩形的队列的指针
 * 注意！传递参数时，请勿颠倒old和new位置。
 **/
LCUI_EXPORT(int)
LCUIRect_Cut(	LCUI_Rect	old_rect,
		LCUI_Rect	new_rect,
		LCUI_Queue	*rects_buff )
{
	int i;
	LCUI_Rect r[5];

	for(i=0; i<5; ++i) {
		Rect_Init(&r[i]);
	}

	/* 计算各个矩形的x轴坐标和宽度 */
	r[0].x = new_rect.x;
	r[0].y = new_rect.y;
	//printf("old,pos(%d,%d), size(%d,%d)\n", old_rect.x, old_rect.y, old_rect.width, old_rect.height);
	//printf("new,pos(%d,%d), size(%d,%d)\n", new_rect.x, new_rect.y, new_rect.width, new_rect.height);
	/* 如果前景矩形在背景矩形的左边 */
	if(new_rect.x < old_rect.x) {
		/* 如果X轴上与背景矩形不重叠 */
		if(new_rect.x + new_rect.width <= old_rect.x) {
			return -1;
		}
		r[0].width = old_rect.x - new_rect.x;
		r[1].x = old_rect.x;
		r[2].x = r[1].x;
		r[4].x = r[2].x;
		/* 如果前景矩形在X轴上包含背景矩形 */
		if(new_rect.x + new_rect.width > old_rect.x + old_rect.width) {
			r[1].width = old_rect.width;

			r[3].x = old_rect.x + old_rect.width;
			r[3].width = new_rect.x + new_rect.width - r[3].x;
		} else { /* 得出矩形2的宽度 */
			r[1].width = new_rect.x + new_rect.width - old_rect.x;
		}
		/* 得出矩形3和5的宽度 */
		r[2].width = r[1].width;
		r[4].width = r[2].width;
	} else {
		if(old_rect.x + old_rect.width <= new_rect.x) {
			return -1;
		}
		r[1].x = new_rect.x;
		r[2].x = r[1].x;
		r[4].x = r[2].x;

		if(new_rect.x + new_rect.width > old_rect.x + old_rect.width) {
			r[1].width = old_rect.x + old_rect.width - r[1].x;
			r[3].x = old_rect.x + old_rect.width;
			r[3].width = new_rect.x + new_rect.width - r[3].x;
		} else {
			r[1].width = new_rect.width;
		}

		r[2].width = r[1].width;
		r[4].width = r[2].width;
	}

	/* 计算各个矩形的y轴坐标和高度 */
	r[0].height = new_rect.height;
	r[3].y = new_rect.y;
	r[3].height = r[0].height;
	r[4].y = old_rect.y + old_rect.height;
	if(new_rect.y < old_rect.y) {
		if(new_rect.y + new_rect.height <= old_rect.y) {
			return -1;
		}
		r[1].y = new_rect.y;
		r[1].height = old_rect.y - new_rect.y;
		r[2].y = old_rect.y;
		/* 如果前景矩形在Y轴上包含背景矩形 */
		if(new_rect.y + new_rect.height > old_rect.y + old_rect.height) {
			r[2].height = old_rect.height;
			r[4].height = new_rect.y + new_rect.height - r[4].y;
		} else {
			r[2].height = new_rect.y + new_rect.height - old_rect.y;
		}
	} else {
		if(new_rect.y >= old_rect.y + old_rect.height) {
			return -1;
		}
		r[2].y = new_rect.y;

		if(new_rect.y + new_rect.height > old_rect.y + old_rect.height) {
			r[2].height = old_rect.y + old_rect.height - r[2].y;
			r[4].height = new_rect.y + new_rect.height - r[4].y;
		} else r[2].height = new_rect.y + new_rect.height - r[2].y;
	}

	//r[0].width -= 1;
	//r[1].height -= 1;
	//r[3].x += 1;
	//r[3].width -= 1;
	//r[4].y += 1;
	//r[4].height -= 1;

	for(i=0; i<5; i++) {
		//if(debug_mark)
		//	printf("slip rect[%d]: %d,%d, %d,%d\n", i, r[i].x, r[i].y, r[i].width, r[i].height);
		Queue_Add(rects_buff, &r[i]);
	}
	return 0;
}

/* 获取两矩形重叠部分的矩形 */
LCUI_EXPORT(int)
LCUIRect_GetOverlay( LCUI_Rect a, LCUI_Rect b, LCUI_Rect *out )
{
	int x = 0,y = 0,w = 0,h = 0;

	if(b.x > a.x && b.x+b.width < a.x+a.width) {
		x = a.x;
		w = b.width;
	} else if(b.x <= a.x && b.x+b.width >= a.x+a.width) {
		x = a.x;
		w = a.width;
	} else if(b.x+b.width > a.x && b.x+b.width <= a.x+a.width) {
		x = a.x;
		w = b.x+b.width - a.x;
	} else if(b.x >= a.x && b.x < a.x+a.width) {
	   x = b.x;
	   w = a.x+a.width - b.x;
	}

	if(b.y > a.y && b.y+b.height < a.y+a.height) {
		y = b.y;
		h = b.height;
	} else if(b.y <= a.y && b.y+b.height >= a.y+a.height) {
	   y = a.y;
	   h = a.height;
	} else if(b.y+b.height > a.y && b.y+b.height <= a.y+a.height) {
		y = a.y;
		h = b.y+b.height - a.y;
	} else if(b.y >= a.y && b.y < a.y+a.height) {
		y = b.y;
		h = a.y+a.height - b.y;
	}

	*out = Rect(x, y, w, h);
	if (x + w == 0 || y + h == 0) {
		return 0;
	}
	return 1;
}

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_EXPORT(LCUI_Rect)
Rect( int x, int y, int width, int height )
{
	LCUI_Rect s;

	Rect_Init (&s);
	s.x = x;
	s.y = y;
	s.width = width;
	s.height = height;
	return s;
}

/* 检测一个点是否被矩形包含 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_IncludePoint( LCUI_Pos pos, LCUI_Rect rect )
{
	if (pos.x >= rect.x && pos.x < rect.x + rect.width-1
	 && pos.y >= rect.y && pos.y < rect.y + rect.height-1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 * 功能：检测两个矩形是否重叠
 * 参数说明：
 * ax ：矩形A的左上角起点所在x轴的坐标
 * ay ：矩形A的左上角起点所在y轴的坐标
 * aw ：矩形A的宽度
 * ah ：矩形A的高度
 * bx ：矩形B的左上角起点所在x轴的坐标
 * by ：矩形B的左上角起点所在y轴的坐标
 * bw ：矩形B的宽度
 * bh ：矩形B的高度
 * 返回值：两不重叠返回0，重叠则返回1。
 */
static LCUI_BOOL
_LCUIRect_Overlay (	int ax, int ay, int aw, int ah,
			int bx, int by, int bw, int bh )
{
	int m, n = 0, x[4], y[4];
	/* 检测两个矩形是否成十字架式叠加 */
	if(ax < bx && ay > by && ax + aw > bx + bw && ay + ah < by + bh) {
		return TRUE;
	}
	if(bx < ax && by > ay && bx + bw > ax + aw && by + bh < ay + ah) {
		return TRUE;
	}
	/*
	 * 哪怕是一丁点的误差，都有可能造成图形显示上的问题
	 * 因为局部区域刷新就是靠这个函数得知区域是否与部件
	 * 矩形是否重叠。
	 * */
	x[0] = ax;
	y[0] = ay;
	x[1] = ax + aw - 1;
	y[1] = ay;
	x[2] = ax;
	y[2] = ay + ah - 1;
	x[3] = ax + aw - 1; /* 因为长和宽包括了左上角点的长宽，要-1 */
	y[3] = ay + ah - 1;
	/* 这只是检测四个角是否在矩形内 */
	for (m = 0; m < 4; ++m) {
		if ((x[m] >= bx && x[m] < bx + bw)
		 && (y[m] >= by && y[m] < by + bh)) {
			/*printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n",
			m, x[m], bx,x[m], bx,bw, y[m],by, y[m], by,bh);
			*/
			n++;
		}
	}
	if(n == 0) {
		//printf("n == 0\n");
		x[0] = bx;
		y[0] = by;
		x[1] = bx + bw - 1;
		y[1] = by;
		x[2] = bx;
		y[2] = by + bh - 1;
		x[3] = bx + bw - 1;
		y[3] = by + bh - 1;
		for (m = 0; m < 4; ++m) {
			if ((x[m] >= ax && x[m] < ax + aw)
			 && (y[m] >= ay && y[m] < ay + ah)) {
				//printf("[%d](%d>=%d && %d<%d+%d) && (%d>=%d && %d<%d+%d)\n", m, x[m],bx,x[m],ax,aw,y[m],ay,y[m],ay,ah);
				n++;
			}
		}
	}
	if( n > 0 ) {
		return TRUE;
	}
	//printf("1111\n");
	return FALSE;
}

/* 检测两个矩形是否重叠 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_Overlay( LCUI_Rect a, LCUI_Rect b )
{
	return _LCUIRect_Overlay(	a.x, a.y, a.width, a.height,
					b.x, b.y, b.width, b.height);
}

/* 判断两个矩形是否相等 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_Equal( LCUI_Rect a, LCUI_Rect b )
{
	if(a.x == b.x && a.y == b.y
	&& a.width == b.width && a.height == b.height) {
		return TRUE;
	}
	return FALSE;
}

/* 判断矩形是否有效 */
LCUI_EXPORT(int)
LCUIRect_IsValid( LCUI_Rect r )
{
	if(r.width <= 0 || r.height <= 0) {
		return 0;
	}
	return 1;
}


/*----------------------------- RectQueue ------------------------------*/

/* 切换队列 */
LCUI_EXPORT(void)
RectQueue_Switch( LCUI_RectQueue *queue )
{
	if( queue->number == 0 ) {
		queue->number = 1;
	} else {
		queue->number = 0;
	}
}

/* 初始化储存矩形数据的队列 */
LCUI_EXPORT(void)
RectQueue_Init( LCUI_RectQueue *queue )
{
	queue->number = 0;
	Queue_Init( &queue->queue[0], sizeof(LCUI_Rect), NULL );
	Queue_Init( &queue->queue[1], sizeof(LCUI_Rect), NULL );
	/* 采用链表来记录数据 */
	Queue_SetDataMode( &queue->queue[0], QUEUE_DATA_MODE_LINKED_LIST );
	Queue_SetDataMode( &queue->queue[1], QUEUE_DATA_MODE_LINKED_LIST );
}

/* 销毁储存矩形数据的队列 */
LCUI_EXPORT(void)
RectQueue_Destroy( LCUI_RectQueue *queue )
{
	Queue_Destroy( &queue->queue[0] );
	Queue_Destroy( &queue->queue[1] );
}


/* 将矩形数据追加至队列 */
static int
RectQueue_Add( LCUI_Queue* queue, LCUI_Rect rect )
{
	int i, flag = 0;
	LCUI_Rect *rect_ptr, *cur_rect_ptr;
	LCUI_Queue rect_buff;

	if( rect.width <= 0 || rect.height <= 0 ) {
		return -1;
	}
	_DEBUG_MSG("add new rect: %d,%d,%d,%d, current total: %d\n",
	 rect.x, rect.y, rect.width, rect.height, queue->total_num);
	Queue_Init( &rect_buff, sizeof(LCUI_Rect), NULL );

	for (i=0; i<queue->total_num; ++i) {
		cur_rect_ptr = (LCUI_Rect*)Queue_Get( queue, i );
		if( !cur_rect_ptr ) {
			break;
		}
		_DEBUG_MSG("[%d] rect: %d,%d,%d,%d\n", i,
		 cur_rect_ptr->x, cur_rect_ptr->y,
		 cur_rect_ptr->width, cur_rect_ptr->height);
		/* 如果矩形无效，或者被新增的矩形区域包含，则删除 */
		if ( cur_rect_ptr->width <= 0 || cur_rect_ptr->height <= 0
		 || LCUIRect_IncludeRect( rect, *cur_rect_ptr) ) {
			Queue_Delete ( queue, i );
			continue;
		}

		/* 如果与现有的矩形相同，或被现有的矩形包含 */
		if( LCUIRect_Equal( rect, *cur_rect_ptr)
		 || LCUIRect_IncludeRect( *cur_rect_ptr, rect ) ) {
			flag = 1;
			break;
		}

		/* 如果新增的矩形与队列中的矩形不重叠 */
		if( !LCUIRect_Overlay(rect, *cur_rect_ptr) ) {
			continue;
		}
		continue;
		_DEBUG_MSG("[%d] rect overlay, start cut\n", i);
		/* 根据当前区域，分割新区域 */
		LCUIRect_Cut( *cur_rect_ptr, rect, &rect_buff );
		for( i=0; i<rect_buff.total_num; ++i ) {
			rect_ptr = (LCUI_Rect*)Queue_Get( &rect_buff, i );
			if( !rect_ptr ) {
				break;
			}
			_DEBUG_MSG("[%d] add child rect: %d,%d,%d,%d\n", i,
				 rect_ptr->x, rect_ptr->y,
				 rect_ptr->width, rect_ptr->height);
			RectQueue_Add( queue, *rect_ptr );
		}
		flag = 1;
		break;
	}

	/* 销毁队列 */
	Queue_Destroy( &rect_buff );
	if ( flag == 0 ) {
		return Queue_Add( queue, &rect );
	}
	return -1;
}

/* 添加矩形区域至可用的队列 */
LCUI_EXPORT(int)
RectQueue_AddToValid( LCUI_RectQueue *queue, LCUI_Rect rect )
{
	if( queue->number == 0 ) {
		return RectQueue_Add( &queue->queue[1], rect );
	} else {
		return RectQueue_Add( &queue->queue[0], rect );
	}
}

/* 添加矩形区域至当前占用的队列 */
LCUI_EXPORT(int)
RectQueue_AddToCurrent( LCUI_RectQueue *queue, LCUI_Rect rect )
{
	if( queue->number == 0 ) {
		return RectQueue_Add( &queue->queue[0], rect );
	} else {
		return RectQueue_Add( &queue->queue[1], rect );
	}
}


/* 从可用的队列中取出一个矩形区域 */
LCUI_EXPORT(LCUI_BOOL)
RectQueue_GetFromValid( LCUI_RectQueue *queue, LCUI_Rect *rect_buff )
{
	LCUI_Rect *rect_ptr;
	LCUI_Queue *q;
	if( queue->number == 0 ) {
		q = &queue->queue[1];
	} else {
		q = &queue->queue[0];
	}
	rect_ptr = (LCUI_Rect*)Queue_Get( q, 0 );
	if( rect_ptr == NULL ) {
		Queue_Delete( q, 0 );
		return FALSE;
	}
	*rect_buff = *rect_ptr;
	Queue_Delete( q, 0 );
	return TRUE;
}

/* 从当前占用的队列中取出一个矩形区域 */
LCUI_EXPORT(LCUI_BOOL)
RectQueue_GetFromCurrent( LCUI_RectQueue *queue, LCUI_Rect *rect_buff )
{
	LCUI_Rect *rect_ptr;
	LCUI_Queue *q;
	if( queue->number == 0 ) {
		q = &queue->queue[0];
	} else {
		q = &queue->queue[1];
	}
	rect_ptr = (LCUI_Rect*)Queue_Get( q, 0 );
	if( rect_ptr == NULL ) {
		Queue_Delete( q, 0 );
		return FALSE;
	}
	*rect_buff = *rect_ptr;
	Queue_Delete( q, 0 );
	return TRUE;
}

/*--------------------------- End RectQueue ----------------------------*/
