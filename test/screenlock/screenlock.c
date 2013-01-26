// 一个屏幕滑动解锁的程序
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

void move_pic_btn(LCUI_Widget *widget, LCUI_DragEvent *event)
/* 功能：移动滑块 */
{
	LCUI_Pos pos, parent;

	parent = Get_Widget_Global_Pos(widget->parent);
	pos = Pos_Sub(event->new_pos, parent);
	Move_Widget(widget, pos);
	if(event->end_click) {/* 如果拖动已经结束 */
		LCUI_Size size;
		LCUI_Rect des, rect;
		usleep(10000);
		rect = Get_Widget_Rect(widget);
		size = Get_Widget_Size(widget->parent);
		des = Rect(size.w-10, 0, 30, 30);/* 目标区域 */
		 /* 如果部件区域与目标区域重叠，则退出程序 */
		if(Rect_Is_Overlay(rect, des)) {
			LCUI_StopMainLoop(); 
		} else {/* 否则，让部件回到起始位置，这个使用的是匀速移动 */
			Move_Widget_To_Pos(widget, Pos(0,0), 500);
		}
	}
}

void *update_time(void *arg)
/* 功能：动态改变label部件的文本内容 */
{
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	LCUI_Widget *wday_label, *date_label;
	int l1=0,l2=0,r1=0,r2=0,old_l1=0, old_l2=0, old_r1=0,old_r2=0;
	char day[][20] = {"星期天","星期一","星期二","星期三","星期四","星期五","星期六"};
	LCUI_Widget	*pic_bg, *time_box, *pic_btn_line, *pic_btn,
			*win, *pic_l1, *pic_l2,
			 *pic_c, *pic_r1, *pic_r2;
	
	win = (LCUI_Widget *)arg; /* 转换类型 */
	/* 创建部件 */
	time_box	= Create_Widget(NULL);
	pic_bg		= Create_Widget("picture_box");
	pic_l1		= Create_Widget("picture_box");
	pic_l2		= Create_Widget("picture_box");
	pic_c		= Create_Widget("picture_box");
	pic_btn		= Create_Widget("picture_box");
	pic_btn_line	= Create_Widget("picture_box");
	pic_btn		= Create_Widget("picture_box");
	pic_r1		= Create_Widget("picture_box");
	pic_r2		= Create_Widget("picture_box");
	date_label	= Create_Widget("label");
	wday_label	= Create_Widget("label");
	
	/* 生成文件路径 */
	strcpy(filename, "drawable/time_0.png");
	/* 载入图片 */
	Set_PictureBox_Image_From_File(pic_bg, "drawable/bg.png"); 
	Set_PictureBox_Image_From_File(pic_btn_line, "drawable/btn_bg.png"); 
	Set_PictureBox_Image_From_File(pic_btn, "drawable/btn.png"); 
	Set_PictureBox_Image_From_File(pic_l1, filename); 
	Set_PictureBox_Image_From_File(pic_l2, filename); 
	Set_PictureBox_Image_From_File(pic_c,  "drawable/time_dot.png"); 
	Set_PictureBox_Image_From_File(pic_r1, filename); 
	Set_PictureBox_Image_From_File(pic_r2, filename); 
	
	Set_PictureBox_Size_Mode(pic_btn_line, SIZE_MODE_CENTER);
	
	/* 调整尺寸 */
	Resize_Widget(time_box,		Size(162, 38));
	Resize_Widget(pic_bg,		Size(300, 90));
	Resize_Widget(pic_btn_line,	Size(245, 66));
	Resize_Widget(pic_btn,		Size(46, 66));
	Resize_Widget(pic_l1,		Size(33, 36));
	Resize_Widget(pic_l2,		Size(33, 36));
	Resize_Widget(pic_c,		Size(30, 38));
	Resize_Widget(pic_r1,		Size(33, 36));
	Resize_Widget(pic_r2,		Size(33, 36));
	/* 调整布局 */
	Set_Widget_Align(pic_l1, ALIGN_MIDDLE_CENTER, Pos(-66,0));
	Set_Widget_Align(pic_l2, ALIGN_MIDDLE_CENTER, Pos(-33,0));
	Set_Widget_Align(pic_c, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Set_Widget_Align(pic_r1, ALIGN_MIDDLE_CENTER, Pos(33,0));
	Set_Widget_Align(pic_r2, ALIGN_MIDDLE_CENTER, Pos(66,0));
	Set_Widget_Align(time_box, ALIGN_MIDDLE_CENTER, Pos(0,-50));
	Set_Widget_Align(pic_bg, ALIGN_MIDDLE_CENTER, Pos(0,5));
	Set_Widget_Align(date_label, ALIGN_MIDDLE_CENTER, Pos(0,35-50));
	Set_Widget_Align(wday_label, ALIGN_TOP_CENTER, Pos(0,5));
	Set_Widget_Align(pic_btn_line, ALIGN_BOTTOM_CENTER, Pos(0,-10));
	/* 放入容器 */
	Widget_Container_Add(time_box, pic_l1);
	Widget_Container_Add(time_box, pic_l2);
	Widget_Container_Add(time_box, pic_c);
	Widget_Container_Add(time_box, pic_r1);
	Widget_Container_Add(time_box, pic_r2);
	Widget_Container_Add(pic_btn_line, pic_btn);
	
	Window_Client_Area_Add(win, pic_bg);
	Window_Client_Area_Add(win, time_box);
	Window_Client_Area_Add(win, date_label);
	Window_Client_Area_Add(win, wday_label);
	Window_Client_Area_Add(win, pic_btn_line);
	/* 限制移动范围 */
	Limit_Widget_Pos(pic_btn, Pos(0,0), Pos(195,0));
	Widget_Drag_Event_Connect(pic_btn, move_pic_btn);
	
	Show_Widget(pic_bg);
	Show_Widget(pic_l1);
	Show_Widget(pic_l2);
	Show_Widget(pic_c);
	Show_Widget(pic_r1);
	Show_Widget(pic_r2);
	Show_Widget(time_box);  
	Show_Widget(wday_label);
	Show_Widget(date_label);
	Show_Widget(pic_btn);
	Show_Widget(pic_btn_line);
	
	while(1) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */ 
		/* 更改文本内容 */
		Label_Text(
			date_label, "<color=40,165,45>%4d年%02d月%02d日</color>",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday
		);
		Label_Text(
			wday_label, 
			"<color=40,165,45>%s</color>", 
			day[(int) (timeinfo->tm_wday)]
		);
		l1 = timeinfo->tm_hour/10;
		l2 = timeinfo->tm_hour%10;
		r1 = timeinfo->tm_min/10;
		r2 = timeinfo->tm_min%10;
		if(l1 != old_l1) {
			sprintf(filename, "drawable/time_%d.png", l1);
			Set_PictureBox_Image_From_File(pic_l1, filename); 
			old_l1 = l1;
		}
		if(l2 != old_l2) {
			sprintf(filename, "drawable/time_%d.png", l2);
			Set_PictureBox_Image_From_File(pic_l2, filename); 
			old_l2 = l2;
		}
		if(l1 != old_r1) {
			sprintf(filename, "drawable/time_%d.png", r1);
			Set_PictureBox_Image_From_File(pic_r1, filename); 
			old_r1 = r1;
		}
		if(r2 != old_r2) {
			sprintf(filename, "drawable/time_%d.png", r2);
			Set_PictureBox_Image_From_File(pic_r2, filename); 
			old_r2 = r2;
		}
		sleep(1);/* 暂停1秒 */
	} 
	LCUI_Thread_Exit(NULL);
}

int main(int argc, char*argv[]) 
{
	thread_t t;
	LCUI_Widget *window;
	
	setenv( "LCUI_FONTFILE", PATH_FONTFILE, FALSE );
	
	LCUI_Init(argc, argv);
	/* 创建窗口 */
	window = Create_Widget("window");
	/* 创建线程，用于更新时间显示 */
	LCUI_Thread_Create(&t, NULL, update_time, (void*)window);
	/* 设置窗口背景色为白色 */
	Set_Widget_Backcolor(window, RGB(255,255,255));
	/* 调整窗口尺寸 */
	Resize_Widget(window, Size(320, 240)); 
	/* 设置窗口为线条边框风格 */
	Set_Widget_StyleID( window, WINDOW_STYLE_LINE );
	Show_Widget( window );
	return LCUI_Main();
}

