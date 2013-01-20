// 测试矩形裁剪的示例程序

#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_MISC_H
#include LC_LABEL_H 
#include <unistd.h>
#include <math.h>

static LCUI_Widget *window = NULL;

static void 
test_move_widget(
				LCUI_Widget *widget, 
				LCUI_Widget **child_area, 
				LCUI_Pos des_pos, 
				int speed,
				LCUI_Widget *bg)
/* 
 * 功能：将部件以指定的速度向指定位置移动
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * 此函数修改自Move_Widget_To_Pos函数
 * */
{
	if(speed <= 0) {
		return;
	}
	
	int i, j, z;
	char str[256];
	double w, h, l, n, x, y; 
	
	LCUI_Pos pos; 
	LCUI_Queue rq;
	LCUI_Rect bg_rect, rt, fg_rect;
	
	x = Get_Widget_Pos(widget).x;
	y = Get_Widget_Pos(widget).y;
	/* 求两点之间的距离 */
	w = des_pos.x-x;
	h = des_pos.y-y;
	l = sqrt(pow(w, 2) + pow(h, 2));
	
	n = l/speed;	/* 求移动所需时间 */
	n = n*100;	/* 求移动次数，乘以100，是因为每隔0.01移动一次位置 */
	w = w/n;	/* 计算每次移动的x和y轴坐标的移动距离 */
	h = h/n;
	j = (int)(n>(int)n?n+1:n);
	
	Rect_Init(&rt);
	RectQueue_Init(&rq); 
	
	for(i=0; i<j; i++) {
		bg_rect = Get_Widget_Rect(bg);
		x += w; y += h;
		pos = Pos(x, y);
		Move_Widget(widget, pos);
		fg_rect = Get_Widget_Rect(widget);
		/* 分割前景和背景矩形的重叠区域，得到分割后的矩形 */
		Cut_Overlay_Rect(bg_rect, fg_rect, &rq); 
		/* 根据分割后的矩形，设置子区域的尺寸和位置 */
		for(z=0; RectQueue_Get(&rt, 0, &rq); ++z) {
			Move_Widget(child_area[z], Pos_Sub(Pos(rt.x, rt.y), widget->pos)); 
			Resize_Widget(child_area[z], Size(rt.width, rt.height));
			Queue_Delete(&rq, 0);
		} 
		/* 如果两个矩形未重叠，就会重置这些区域的尺寸 */
		for(; z<5; z++) {
			Resize_Widget(child_area[z], Size(0, 0));
		}
		sprintf( str, "测试矩形裁剪功能(FPS:%d)", LCUI_GetFPS() );
		Set_Window_Title_Text( window, str );
		usleep(10000);/* 停顿0.01秒 */
	} 
}

static void *
test()
{
	int i; 
	/* 6个位置的坐标 */
	LCUI_Pos pos[6]={
		{0, 0},{220, 0},{220, 55},{0, 55},{0, 110},{220, 110}
	};
	/* 5个区域的颜色 */
	LCUI_RGB color[5]={
		{255,165,0}, {165,32,42}, {255,192,203}, 
		{127,127,127}, {139,105,20}
	};
	LCUI_Widget *bg, *label[5], *fg, *area[5];
	
	/* 创建前景和背景区域 */
	bg = Create_Widget(NULL);
	fg = Create_Widget(NULL);
	/* 创建5个区域 */
	for(i=0; i<5; i++) {
		area[i]	= Create_Widget(NULL);  
		label[i] = Create_Widget("label");
	} 
	/* 设置背景 */
	Set_Widget_Backcolor(bg, RGB(50, 50, 255));
	Set_Widget_Backcolor(fg, RGB(255, 50, 50));
	Set_Widget_BG_Mode(bg, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_BG_Mode(fg, BG_MODE_FILL_BACKCOLOR);
	/* 设置背景区域的布局 */
	Set_Widget_Align(bg, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* 调整背景区域的尺寸 */
	Resize_Widget(bg, Size(110, 110)); 
	Resize_Widget(fg, Size(100, 100));
	/* 将前景和背景区域加入至窗口客户区 */
	Window_Client_Area_Add(window, bg);  
	Window_Client_Area_Add(window, fg);  
	/* 循环设置5个区域 */
	for(i=0; i<5; i++) {
		Set_Widget_BG_Mode(area[i], BG_MODE_FILL_BACKCOLOR);
		Set_Widget_Backcolor(area[i], color[i]); 
		Label_Text(label[i], "%d", i+1);
		Set_Widget_Align(label[i], ALIGN_MIDDLE_CENTER, Pos(0,0));
		Widget_Container_Add(area[i], label[i]);
		Widget_Container_Add(fg, area[i]);
		Show_Widget(label[i]); 
		Show_Widget(area[i]); 
	}
	/* 设置前景区域的初始位置 */
	Set_Widget_Pos(fg, pos[0]);
	/* 显示部件 */
	Show_Widget(bg);
	Show_Widget(fg);
	Show_Widget(window);
	/* 开始移动前景区域 */
	while(1) { 
		for(i=0; i<6; ++i) {
			test_move_widget(fg, area, pos[i], 100, bg);
		}
		Resize_Widget(fg, Size(140, 100));
		for(i=4; i>=0; --i) {
			test_move_widget(fg, area, pos[i], 100, bg); 
		}
		Resize_Widget(fg, Size(100, 100));
	}
	LCUI_Thread_Exit(NULL);
}

int main(int argc, char*argv[]) 
{
	thread_t t;
	LCUI_Init( argc, argv ); 
	window = Create_Widget("window"); 
	Resize_Widget( window, Size(320, 240) );
	Set_Window_Title_Text( window, "测试矩形裁剪功能" );
	/* 创建一个线程，以让区域移动 */
	LCUI_Thread_Create(&t, NULL, test, NULL);
	return LCUI_Main();
}
