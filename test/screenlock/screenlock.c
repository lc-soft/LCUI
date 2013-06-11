// 一个屏幕滑动解锁的程序
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_PICBOX_H
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define PATH_FONTFILE "../../fonts/msyh.ttf"

/* 移动滑块 */
void move_pic_btn(LCUI_Widget *widget, LCUI_WidgetEvent *event)
{
	LCUI_Size size;
	LCUI_Rect des, rect;
	LCUI_Pos pos, parent;

	parent = Widget_GetGlobalPos(widget->parent);
	pos = Pos_Sub(event->drag.new_pos, parent);
	Widget_Move(widget, pos);
	if(event->drag.end_click) {/* 如果拖动已经结束 */
		//usleep(10000);
		rect = Widget_GetRect(widget);
		size = Widget_GetSize(widget->parent);
		des = Rect(size.w-10, 0, 30, 30);/* 目标区域 */
		 /* 如果部件区域与目标区域重叠，则退出程序 */
		if(LCUIRect_Overlay(rect, des)) {
			LCUI_MainLoop_Quit(NULL);
		} else {/* 否则，让部件回到起始位置，这个使用的是匀速移动 */
			Widget_MoveToPos(widget, Pos(0,0), 500);
		}
	}
}

/* 动态改变label部件的文本内容 */
void update_time(void *arg)
{
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100], str[256];
	LCUI_Widget *wday_label, *date_label;
	int l1=0,l2=0,r1=0,r2=0,old_l1=0, old_l2=0, old_r1=0,old_r2=0;
	char day[][20] = {"星期天","星期一","星期二","星期三","星期四","星期五","星期六"};
	LCUI_Widget	*pic_bg, *time_box, *pic_btn_line, *pic_btn,
			*win, *pic_l1, *pic_l2,
			 *pic_c, *pic_r1, *pic_r2;

	win = (LCUI_Widget *)arg; /* 转换类型 */
	/* 创建部件 */
	time_box	= Widget_New(NULL);
	pic_bg		= Widget_New("picture_box");
	pic_l1		= Widget_New("picture_box");
	pic_l2		= Widget_New("picture_box");
	pic_c		= Widget_New("picture_box");
	pic_btn		= Widget_New("picture_box");
	pic_btn_line	= Widget_New("picture_box");
	pic_btn		= Widget_New("picture_box");
	pic_r1		= Widget_New("picture_box");
	pic_r2		= Widget_New("picture_box");
	date_label	= Widget_New("label");
	wday_label	= Widget_New("label");

	/* 生成文件路径 */
	strcpy(filename, "drawable/time_0.png");
	/* 载入图片 */
	PictureBox_SetImageFile(pic_bg, "drawable/bg.png");
	PictureBox_SetImageFile(pic_btn_line, "drawable/btn_bg.png");
	PictureBox_SetImageFile(pic_btn, "drawable/btn.png");
	PictureBox_SetImageFile(pic_l1, filename);
	PictureBox_SetImageFile(pic_l2, filename);
	PictureBox_SetImageFile(pic_c,  "drawable/time_dot.png");
	PictureBox_SetImageFile(pic_r1, filename);
	PictureBox_SetImageFile(pic_r2, filename);

	PictureBox_SetSizeMode(pic_btn_line, SIZE_MODE_CENTER);

	/* 调整尺寸 */
	Widget_Resize(time_box,		Size(162, 38));
	Widget_Resize(pic_bg,		Size(300, 90));
	Widget_Resize(pic_btn_line,	Size(245, 66));
	Widget_Resize(pic_btn,		Size(46, 66));
	Widget_Resize(pic_l1,		Size(33, 36));
	Widget_Resize(pic_l2,		Size(33, 36));
	Widget_Resize(pic_c,		Size(30, 38));
	Widget_Resize(pic_r1,		Size(33, 36));
	Widget_Resize(pic_r2,		Size(33, 36));
	/* 调整布局 */
	Widget_SetAlign(pic_l1, ALIGN_MIDDLE_CENTER, Pos(-66,0));
	Widget_SetAlign(pic_l2, ALIGN_MIDDLE_CENTER, Pos(-33,0));
	Widget_SetAlign(pic_c, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_SetAlign(pic_r1, ALIGN_MIDDLE_CENTER, Pos(33,0));
	Widget_SetAlign(pic_r2, ALIGN_MIDDLE_CENTER, Pos(66,0));
	Widget_SetAlign(time_box, ALIGN_MIDDLE_CENTER, Pos(0,-50));
	Widget_SetAlign(pic_bg, ALIGN_MIDDLE_CENTER, Pos(0,5));
	Widget_SetAlign(date_label, ALIGN_MIDDLE_CENTER, Pos(0,35-50));
	Widget_SetAlign(wday_label, ALIGN_TOP_CENTER, Pos(0,5));
	Widget_SetAlign(pic_btn_line, ALIGN_BOTTOM_CENTER, Pos(0,-10));
	/* 放入容器 */
	Widget_Container_Add(time_box, pic_l1);
	Widget_Container_Add(time_box, pic_l2);
	Widget_Container_Add(time_box, pic_c);
	Widget_Container_Add(time_box, pic_r1);
	Widget_Container_Add(time_box, pic_r2);
	Widget_Container_Add(pic_btn_line, pic_btn);

	Window_ClientArea_Add(win, pic_bg);
	Window_ClientArea_Add(win, time_box);
	Window_ClientArea_Add(win, date_label);
	Window_ClientArea_Add(win, wday_label);
	Window_ClientArea_Add(win, pic_btn_line);
	/* 限制移动范围 */
	Widget_LimitPos(pic_btn, Pos(0,0), Pos(195,0));
	Widget_Event_Connect(pic_btn, EVENT_DRAG, move_pic_btn);

	Widget_Show(pic_bg);
	Widget_Show(pic_l1);
	Widget_Show(pic_l2);
	Widget_Show(pic_c);
	Widget_Show(pic_r1);
	Widget_Show(pic_r2);
	Widget_Show(time_box);
	Widget_Show(wday_label);
	Widget_Show(date_label);
	Widget_Show(pic_btn);
	Widget_Show(pic_btn_line);

	while(1) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */
		sprintf( str, "<color=40,165,45>%4d年%02d月%02d日</color>",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1,
			timeinfo->tm_mday );
		Label_Text( date_label, str );
		sprintf( str, "<color=40,165,45>%s</color>",
			day[(int) (timeinfo->tm_wday)] );
		Label_Text( wday_label, str );
		l1 = timeinfo->tm_hour/10;
		l2 = timeinfo->tm_hour%10;
		r1 = timeinfo->tm_min/10;
		r2 = timeinfo->tm_min%10;
		if(l1 != old_l1) {
			sprintf(filename, "drawable/time_%d.png", l1);
			PictureBox_SetImageFile(pic_l1, filename);
			old_l1 = l1;
		}
		if(l2 != old_l2) {
			sprintf(filename, "drawable/time_%d.png", l2);
			PictureBox_SetImageFile(pic_l2, filename);
			old_l2 = l2;
		}
		if(l1 != old_r1) {
			sprintf(filename, "drawable/time_%d.png", r1);
			PictureBox_SetImageFile(pic_r1, filename);
			old_r1 = r1;
		}
		if(r2 != old_r2) {
			sprintf(filename, "drawable/time_%d.png", r2);
			PictureBox_SetImageFile(pic_r2, filename);
			old_r2 = r2;
		}
		sleep(1);/* 暂停1秒 */
	}
	LCUIThread_Exit(NULL);
}

int main( int argc, char **argv )
{
	LCUI_Thread t;
	LCUI_Widget *window;

	LCUI_Init(320,240,0);
	window = Widget_New("window");
	Widget_SetBackgroundColor(window, RGB(255,255,255));
	Widget_Resize(window, Size(320, 240));
	/* 设置窗口为线条边框风格 */
	Widget_SetStyleID( window, WINDOW_STYLE_LINE );
	/* 创建线程，用于更新时间显示 */
	LCUIThread_Create(&t, update_time, (void*)window);
	Widget_Show( window );
	return LCUI_Main();
}

