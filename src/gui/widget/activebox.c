/* ***************************************************************************
 * activebox.c -- ActiveBox widget, play simple dynamic picture
 *
 * Copyright (C) 2012-2013 by
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
 * activebox.c -- ActiveBox部件, 播放简单的动态图像
 *
 * 版权所有 (C) 2012-2013 归属于
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_ACTIVEBOX_H
#include LC_RES_H

#include <time.h>

typedef struct AnimationRec_ {
	int id;
	AnimationData *animation;
} AnimationRec;

/*********************** Frames Process *******************************/
/* 动画库，用于记录所有动画的信息 */
static LCUI_Queue animation_database;
/* 动画流，以流水线的形式处理每个动画的每帧图像的更新 */
static LCUI_Queue animation_stream;

static int database_init = FALSE;
static int __timer_id = -1;

/** 获取当前帧 */
LCUI_API AnimationFrameData* Animation_GetFrame( AnimationData *src )
{
	return (AnimationFrameData*)Queue_Get(&src->frame, src->current-1);
}

/** 获取当前帧的图像 */
LCUI_API LCUI_Graph* Animation_GetGraphSlot( AnimationData *src )
{
	if( !src ) {
		return NULL;
	}
	return &src->slot;
}

static void Animation_CallFunc( AnimationData *animation )
{
	AppTasks_CustomAdd( ADD_MODE_REPLACE | AND_ARG_S, &animation->func );
}

/* 获取指定帧在整个动画容器中的位置 */
LCUI_API LCUI_Pos Animation_GetFrameMixPos(	AnimationData *animation,
						AnimationFrameData *frame )
{
	LCUI_Pos pos;
	pos = GetPosByAlign(	animation->size,
				Graph_GetSize(&frame->graph),
				ALIGN_MIDDLE_CENTER );
	return Pos_Add(pos, frame->offset);
}

/* 将动画当前帧的图像写入至槽中 */
static int Animation_UpdateGraphSlot( AnimationData *animation, int num )
{
	LCUI_Pos pos;
	AnimationFrameData *frame;

	frame = (AnimationFrameData*)Queue_Get( &animation->frame, num );
	DEBUG_MSG("animation: %p, frame: %p\n", animation, frame);
	if( !frame ) {
		return -1;
	}
	if(!Graph_IsValid( &animation->slot) ) {
		return -2;
	}
	Graph_FillAlpha( &animation->slot, 0 );
	if(0 < Queue_GetTotal( &animation->frame )) {
		pos = Animation_GetFrameMixPos( animation, frame );
		Graph_Replace( &animation->slot, &frame->graph, pos );
	}
	return 0;
}

/** 销毁动画数据 */
static void AnimationData_Destroy( void *arg )
{
	AnimationData *animation;
	animation = (AnimationData*)arg;
	Queue_Destroy( &animation->frame );
	Graph_Free( &animation->slot );
}


/**
 * 创建一个动画
 * 创建的动画将记录至动画库中
 * @param size
 *	动画的尺寸
 * @returns
 *	正常则返回指向动画库中的该动画的指针，失败则返回NULL
 */
LCUI_API AnimationData* Animation_Create( LCUI_Size size )
{
	int pos;
	AnimationData *p, animation;

	Queue_Init( &animation.frame, sizeof(AnimationFrameData), NULL );
	animation.func.func = NULL;
	animation.func.id = 0;
	Graph_Init( &animation.slot );
	animation.slot.color_type = COLOR_TYPE_RGBA;
	animation.current = 0;
	animation.state = 0;
	animation.size = size;
	
	if( !database_init ) {
		Queue_Init(	&animation_database,
				sizeof(AnimationData),
				AnimationData_Destroy );
		database_init = TRUE;
	}
	Queue_Lock( &animation_database );
	/* 记录该动画至库中 */
	pos = Queue_Add( &animation_database, &animation );
	p = (AnimationData*)Queue_Get( &animation_database, pos );
	Queue_Unlock( &animation_database );
	DEBUG_MSG("create animation: %p\n", p);
	Animation_Resize( p, size );
	return p;
}

static int AnimationStream_Delete( AnimationData *animation )
{
	int n;
	AnimationData* tmp;

	Queue_Lock( &animation_stream );
	n = Queue_GetTotal( &animation_stream );
	/* 查询该动画是否在动画流中存在 */
	for(n; n>=0; --n) {
		tmp = (AnimationData*)Queue_Get( &animation_stream, n );
		/* 如果存在则删除它 */
		if( tmp == animation ) {
			Queue_Delete( &animation_stream, n );
			break;
		}
	}
	Queue_Unlock( &animation_stream );
	if( n < 0 ) {
		return -1;
	}
	return 0;
}

/**
 * 删除一个动画
 * 从动画库中删除指定的动画
 * @param animation
 *	需删除的动画
 * @returns
 *	正常则返回0，失败则返回-1
 */
LCUI_API int Animation_Delete( AnimationData* animation )
{
	int n;
	AnimationData* tmp;

	/* 先从动画流中删除该动画的记录 */
	AnimationStream_Delete( animation );

	Queue_Lock( &animation_database );
	n = Queue_GetTotal( &animation_database );
	/* 查询该动画是否在动画库中存在 */
	for(n; n>=0; --n) {
		tmp = (AnimationData*)Queue_Get( &animation_database, n );
		/* 如果存在则删除它 */
		if( tmp == animation ) {
			Queue_Delete( &animation_database, n );
			break;
		}
	}
	Queue_Unlock( &animation_database );
	if( n < 0 ) {
		return -1;
	}
	return 0;
}


/* 功能：调整动画的容器尺寸 */
LCUI_API int Animation_Resize( AnimationData *p, LCUI_Size new_size )
{
	int i, total;
	LCUI_Pos pos;
	AnimationFrameData *frame;
	LCUI_Graph *graph;
	LCUI_Size size;

	if(new_size.w <= 0 || new_size.h <= 0)	{
		return -1;
	}
	if( !p ) {
		return -2;
	}

	p->size = new_size;
	total = Queue_GetTotal(&p->frame);
	for(i=0; i<total; ++i){
		frame = (AnimationFrameData *)Queue_Get(&p->frame, i);
		graph = Graph_GetQuote( &frame->graph );
		size = Graph_GetSize( graph );
		pos = Animation_GetFrameMixPos( p, frame );
		/* 判断当前帧图像是否超出动画尺寸 */
		if(pos.x+size.w > new_size.w){
			size.w = new_size.w - pos.x;
			size.w<0 ? size.w=0 :1;
		}
		if(pos.y+size.h > new_size.h){
			size.h = new_size.h - pos.y;
			size.h<0 ? size.h=0 :1;
		}
		Graph_Quote( &frame->graph, graph, Rect(0,0,size.w, size.h) );
	}
	/* 调整动画槽的尺寸 */
	Graph_Create( &p->slot, new_size.w, new_size.h );
	/* 更新动画槽中的图像 */
	Animation_UpdateGraphSlot( p, p->current );
	Animation_CallFunc( p );
	return 0;
}

/**
 * 为动画添加一帧图像
 * @param des
 *	目标动画
 * @param pic
 *	新增帧的图像
 * @param offset
 *	该帧图像的坐标偏移量，用于条该帧图像的显示位置
 * @param sleep_time
 *	该帧的显示时长（单位：毫秒）
 * */
LCUI_API int Animation_AddFrame(	AnimationData *des,
					LCUI_Graph *pic,
					LCUI_Pos offset,
					int sleep_time )
{
	AnimationFrameData *p, frame;
	if( !des ) {
		return -1;
	}
	if(!Graph_IsValid(pic)) {
		return -2;
	}

	frame.offset = offset;
	frame.sleep_time = sleep_time;
	frame.graph = *pic;
	frame.current_time = frame.sleep_time;
	p = Queue_Get( &des->frame, Queue_Add( &des->frame, &frame ) );
	DEBUG_MSG("animation: %p, frame: %p\n", des, p);
	return 0;
}

/**
 * 为动画关联回调函数
 * 关联回调函数后，动画每更新一帧都会调用该函数
 * @param des
 *	目标动画
 * @param func
 *	指向回调函数的函数指针
 * @param arg
 *	需传递给回调函数的第二个参数
 * */
LCUI_API int Animation_Connect(	AnimationData *des,
				void (*func)(AnimationData*, void*),
				void *arg )
{
	if( !des ) {
		return -1;
	}

	des->func.func = (CallBackFunc)func;
	des->func.id = LCUIApp_GetSelfID();
	des->func.arg[0] = NULL;
	des->func.arg[1] = arg;
	/* 如果不将该标志置为FALSE，会是确定的值，导致在执行任务后的销毁参数时出现段错误 */
	des->func.destroy_arg[0] = FALSE;
	des->func.destroy_arg[1] = FALSE;
	return 0;
}


/* 对动画流进行排序 */
static void AnimationStream_Sort(void)
{
	int i, j, pos, total;
	AnimationData *temp;
	AnimationFrameData *p, *q;
	/* 为动画流锁上互斥锁 */
	Queue_Lock( &animation_stream );
	total = Queue_GetTotal( &animation_stream );
	/* 使用选择排序法进行排序 */
	for(i=0; i<total; ++i) {
		/* 获取一个动画 */
		temp = (AnimationData*)Queue_Get( &animation_stream, i );
		if( !temp ) {
			continue;
		}
		/* 若该动画当前帧序号大于0 */
		if(temp->current > 0) {
			pos = temp->current-1;
		} else {
			pos = 0;
		}
		/* 获取当前帧 */
		p = (AnimationFrameData*)Queue_Get( &temp->frame, pos );
		if( !p ) {
			continue;
		}

		for(j=i+1; j<total; ++j) {
		/* 获取下一个动画 */
		temp = (AnimationData*)Queue_Get( &animation_stream, j );
			if( !temp ) {
				continue;
			}
			if(temp->current > 0) {
				pos = temp->current-1;
			} else {
				pos = 0;
			}
			/* 获取该动画的当前帧 */
			q =(AnimationFrameData*) Queue_Get(&temp->frame, pos);
			if( !q ) {
				continue;
			}
			/* 对比两个动画的当前帧的剩余等待时间 */
			if( q->current_time < p->current_time ) {
				/* 交换两个动画在动画流中的位置 */
				Queue_Swap( &animation_stream, j, i );
			}
		}
	}
	/* 解锁动画流 */
	Queue_Unlock(&animation_stream);
}

/* 将各个动画的当前帧的等待时间与指定时间相减 */
static void AnimationStream_TimeSub( int time )
{
	AnimationFrameData *frame;
	AnimationData *animation;
	int i, total, pos;

	Queue_Lock(&animation_stream);
	total = Queue_GetTotal(&animation_stream);
	DEBUG_MSG("start\n");
	for(i=0; i<total; ++i) {
		animation = (AnimationData*)Queue_Get(&animation_stream, i);
		if( !animation || animation->state == 0 ) {
			continue;
		}
		if(animation->current > 0) {
			pos = animation->current-1;
		} else {
			pos = 0;
		}
		frame = (AnimationFrameData*)Queue_Get(&animation->frame, pos);
		if( !frame ) {
			continue;
		}
		frame->current_time -= time;
		DEBUG_MSG("fames: %p, current: %d, time:%ld, sub:%d\n",
			animation, pos, frame->current_time, time);
	}
	DEBUG_MSG("end\n");
	Queue_Unlock(&animation_stream);
}

/** 更新流中的动画至下一帧，并获取当前动画和当前帧的等待时间 */
static AnimationData * AnimationStream_Update( int *sleep_time )
{
	int i, total;
	AnimationFrameData *frame = NULL;
	AnimationData *animation = NULL, *temp = NULL;
	clock_t used_time;

	DEBUG_MSG("start\n");
	total = Queue_GetTotal(&animation_stream);
	for(i=0; i<total; ++i){
		animation = (AnimationData*)Queue_Get(&animation_stream, i);
		if(animation->state == 1) {
			break;
		}
	}
	if(i >= total || !animation ) {
		return NULL;
	}
	/*
	 * 由于有些动画还未更新第一帧图像，动画槽里的图像也未载入第一帧的图像，因此，
	 * 需要优先处理帧序号为0的动画。
	 * */
	for(i=0; i<total; ++i){
		temp = (AnimationData*)Queue_Get( &animation_stream, i );
		if( animation->state == 1 && temp->current == 0 ) {
			animation = temp;
			break;
		}
	}
	if( animation && animation->current > 0 ) {
		frame = (AnimationFrameData*)Queue_Get( &animation->frame,
							animation->current-1 );
		if( !frame ) {
			return NULL;
		}
		DEBUG_MSG("current time: %ld\n", frame->current_time);
		if(frame->current_time > 0) {
			*sleep_time = frame->current_time;
			AnimationStream_TimeSub( frame->current_time );
		}

		frame->current_time = frame->sleep_time;
		++animation->current;
		total = Queue_GetTotal(&animation->frame);
		if(animation->current > total) {
			animation->current = 1;
		}
		frame = (AnimationFrameData*)Queue_Get(	&animation->frame,
							animation->current-1);
		if( !frame ) {
			return NULL;
		}
	} else {
		animation->current = 1;
		frame = (AnimationFrameData*)Queue_Get(&animation->frame, 0);
	}

	used_time = clock();/* 开始计时 */
	/* 将该动画当前帧的图像写入至槽中 */
	Animation_UpdateGraphSlot( animation, animation->current-1 );
	used_time = clock()-used_time;
	if(used_time > 0) {
		AnimationStream_TimeSub(used_time);
	}
	AnimationStream_Sort(); /* 重新排序 */
	DEBUG_MSG("current frame: %d\n", animation->current);
	DEBUG_MSG("end\n");
	return animation;
}

/* 响应定时器，处理动画的每一帧的更新 */
static void Process_Frames( void )
{
	int sleep_time = 10;
	AnimationData *animation;

	while(!LCUI_Active()) {
		LCUI_MSleep(10);
	}
	animation = AnimationStream_Update( &sleep_time );
	LCUITimer_Reset( __timer_id, sleep_time );
	if( animation ) {
		Animation_CallFunc( animation );
	}
}

/* 播放动画 */
LCUI_API int Animation_Play(AnimationData *animation)
{
	int i, total;
	AnimationData *tmp_ptr;
	if( !animation ) {
		return -1;
	}
	animation->state = 1;
	if(__timer_id == -1){
		Queue_Init( &animation_stream, sizeof(AnimationData), NULL );
		Queue_UsingPointer( &animation_stream );
		__timer_id = LCUITimer_Set( 50, Process_Frames, TRUE );
	}
	/* 检查该动画是否已存在 */
	Queue_Lock( &animation_stream );
	total = Queue_GetTotal( &animation_stream );
	for( i=0; i<total; ++i ) {
		tmp_ptr = Queue_Get( &animation_stream, i );
		if( tmp_ptr == animation ) {
			break;
		}
	}
	Queue_Unlock( &animation_stream );
	/* 添加至动画更新队列中 */
	if( i>=total ) {
		return Queue_AddPointer(&animation_stream, animation);
	}
	return 1;
}

/* 暂停动画 */
LCUI_API int Animation_Pause(AnimationData *animation)
{
	if( !animation ) {
		return -1;
	}
	animation->state = 0;
	return 0;
}


LCUI_API AnimationData* ActiveBox_GetCurrentAnimation( LCUI_Widget *widget )
{
	LCUI_ActiveBox *actbox;
	actbox = (LCUI_ActiveBox *)Widget_GetPrivData(widget);
	return actbox->current;
}

/* 刷新动画当前帧的显示 */
static void ActiveBox_RefreshFrame( AnimationData *unused, void *arg )
{
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	DEBUG_MSG("refresh\n");
	Widget_Draw(widget);
}


/**
 * 向ActiveBox部件添加一个动画
 * @param widget
 *	目标ActiveBox部件
 * @param animation
 *	要添加的动画
 * @param id
 *	该动画的标识号，用于区分各个动画
 * @return
 *	正常返回0，失败返回-1
 * @note
 *	添加的动画需要手动释放，ActiveBox部件只负责记录、引用动画
 */
LCUI_API int ActiveBox_AddAnimation(	LCUI_Widget *widget,
					AnimationData *animation,
					int id )
{
	AnimationRec rec;
	LCUI_ActiveBox *actbox;

	rec.animation = animation;
	rec.id = id;
	Animation_Connect( animation, ActiveBox_RefreshFrame, widget );
	actbox = (LCUI_ActiveBox*)Widget_GetPrivData( widget );
	if( 0 <= Queue_Add( &actbox->animation_list, &rec ) ) {
		return 0;
	}
	return -1;
}

/**
 * 切换ActiveBox部件播放的动画
 * @param widget
 *	目标ActiveBox部件
 * @param id
 *	切换至的新动画的标识号
 * @return
 *	切换成功则返回0，未找到指定ID的动画记录，则返回-1
 */
LCUI_API int ActiveBox_SwitchAnimation(	LCUI_Widget *widget,
					int id )
{
	int i, n;
	AnimationRec *p_rec;
	LCUI_ActiveBox *actbox;
	actbox = (LCUI_ActiveBox*)Widget_GetPrivData( widget );
	n = Queue_GetTotal( &actbox->animation_list );
	for(i=0; i<n; ++i) {
		p_rec = (AnimationRec*)Queue_Get( &actbox->animation_list, i );
		if( !p_rec ) {
			continue;
		}
		if( p_rec->id == id ) {
			if( actbox->current ) {
				Animation_Pause( actbox->current );
				/* 若当前动画处于播放状态，则暂停它
				 * 并使切换的新动画处于播放状态 */
				if( actbox->current->state == 1 ) {
					Animation_Pause( actbox->current );
					Animation_Play( p_rec->animation );
				} else {
					Animation_Pause( p_rec->animation );
				}
			}
			actbox->current = p_rec->animation;
			return 0;
		}
	}
	return -1;
}

/* 播放动画 */
LCUI_API int ActiveBox_Play( LCUI_Widget *widget )
{
	AnimationData *animation = ActiveBox_GetCurrentAnimation(widget);
	return Animation_Play(animation);
}

/* 暂停动画 */
LCUI_API int ActiveBox_Pause( LCUI_Widget *widget )
{
	AnimationData *animation = ActiveBox_GetCurrentAnimation(widget);
	return Animation_Pause(animation);
}

/* 初始化ActiveBox部件 */
static void ActiveBox_ExecInit(LCUI_Widget *widget)
{
	LCUI_ActiveBox *actbox;
	actbox = (LCUI_ActiveBox*)WidgetPrivData_New(widget, sizeof(LCUI_ActiveBox));
	actbox->current = NULL;
	Queue_Init( &actbox->animation_list, sizeof(AnimationRec), NULL );
	Widget_SetBackgroundLayout( widget, LAYOUT_CENTER );
}

/* 更新ActiveBox部件内显示的图像 */
static void ActiveBox_ExecUpdate(LCUI_Widget *widget)
{
	LCUI_Rect rect;
	AnimationData *animation;
	LCUI_Graph *frame_graph;
	LCUI_Pos pos;
	DEBUG_MSG("update\n");
	animation = ActiveBox_GetCurrentAnimation( widget );
	frame_graph = Animation_GetGraphSlot( animation );
	pos = GetPosByAlign( Widget_GetSize(widget),
				animation->size, ALIGN_MIDDLE_CENTER);

	Widget_SetBackgroundTransparent( widget, TRUE );
	Widget_SetBackgroundImage( widget, frame_graph );
	rect.x = pos.x;
	rect.y = pos.y;
	rect.width = frame_graph->w;
	rect.height = frame_graph->h;
	Widget_InvalidArea( widget, rect );
}

static void ActiveBox_ExecDestroy(LCUI_Widget *widget)
{

}

LCUI_API void Register_ActiveBox(void)
{
	WidgetType_Add("active_box");
	WidgetFunc_Add("active_box", ActiveBox_ExecInit, FUNC_TYPE_INIT);
	WidgetFunc_Add("active_box", ActiveBox_ExecUpdate, FUNC_TYPE_UPDATE);
	WidgetFunc_Add("active_box", ActiveBox_ExecDestroy, FUNC_TYPE_DESTROY);
}

/************************** End ActiveBox *****************************/
