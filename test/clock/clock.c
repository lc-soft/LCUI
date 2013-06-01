// 时钟的例子程序

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H 
#include LC_DRAW_H
#include LC_FONT_H
#include LC_PICBOX_H
#include <time.h>

/* 图片文件 */
#define IMG_DIAL		"dial.png"
#define IMG_HOUR_PONTER		"hand_hour.png"
#define IMG_MINUTE_POINTER	"hand_minute.png"
#define IMG_SECOND_POINTER	"hand_second.png"

/* 时钟及各个指针部件 */
static LCUI_Widget *wdg_dial, *wdg_sec_ptr, *wdg_hour_ptr, *wdg_min_ptr; 
/* 源时钟指针图像 */
static LCUI_Graph clock_bg, hour_ptr, min_ptr, sec_ptr;
/* 旋转后的时钟指针图像 */
static LCUI_Graph tmp_hour_ptr, tmp_min_ptr, tmp_sec_ptr;

static void UpdateClockPointer(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	int h_angle, s_angle, m_angle;

	time ( &rawtime );
	/* 获取系统当前时间 */
	timeinfo = localtime ( &rawtime ); 
	/* 计算时针、分针、秒针的角度 */
	h_angle = 360*timeinfo->tm_hour / 12;
	m_angle = 360*timeinfo->tm_min / 60;
	s_angle = 360*timeinfo->tm_sec / 60;
	h_angle += m_angle / 60;
	/* 根据这个角度来旋转图形 */
	Graph_Rotate(&hour_ptr, h_angle, &tmp_hour_ptr);
	Graph_Rotate(&min_ptr, m_angle, &tmp_min_ptr);
	Graph_Rotate(&sec_ptr, s_angle, &tmp_sec_ptr);
	/* 更改PictureBox部件显示的图形 */
	PictureBox_SetImage(wdg_hour_ptr, &tmp_hour_ptr);
	PictureBox_SetImage(wdg_min_ptr, &tmp_min_ptr);
	PictureBox_SetImage(wdg_sec_ptr, &tmp_sec_ptr);
}

static void CreateClockWidget( LCUI_Widget *mian_window )
{
	/* 初始化图形数据结构 */
	Graph_Init(&clock_bg);
	Graph_Init(&sec_ptr);
	Graph_Init(&min_ptr);
	Graph_Init(&hour_ptr);
	Graph_Init(&tmp_hour_ptr);
	Graph_Init(&tmp_min_ptr);
	Graph_Init(&tmp_sec_ptr);
	/* 创建几个部件 */
	wdg_dial = Widget_New("picture_box");
	wdg_hour_ptr = Widget_New("picture_box");
	wdg_min_ptr = Widget_New("picture_box");
	wdg_sec_ptr = Widget_New("picture_box");
	/* PictureBox部件居中显示图片 */
	PictureBox_SetSizeMode(wdg_dial, SIZE_MODE_CENTER);
	PictureBox_SetSizeMode(wdg_hour_ptr, SIZE_MODE_CENTER);
	PictureBox_SetSizeMode(wdg_min_ptr, SIZE_MODE_CENTER);
	PictureBox_SetSizeMode(wdg_sec_ptr, SIZE_MODE_CENTER);
	/* 载入图片，保存图形数据 */
	Graph_LoadImage(IMG_DIAL, &clock_bg);
	Graph_LoadImage(IMG_HOUR_PONTER, &hour_ptr);
	Graph_LoadImage(IMG_MINUTE_POINTER, &min_ptr);
	Graph_LoadImage(IMG_SECOND_POINTER, &sec_ptr);
	/* 设定PictureBox部件显示的图形 */
	PictureBox_SetImage(wdg_dial, &clock_bg);
	PictureBox_SetImage(wdg_hour_ptr, &hour_ptr);
	PictureBox_SetImage(wdg_min_ptr, &min_ptr);
	PictureBox_SetImage(wdg_sec_ptr, &sec_ptr);
	/* 将这些部件添加至相应容器中 */
	Widget_Container_Add(wdg_dial, wdg_sec_ptr);
	Widget_Container_Add(wdg_dial, wdg_min_ptr);
	Widget_Container_Add(wdg_dial, wdg_hour_ptr);
	/* 将部件添加至窗口客户区中 */
	Window_ClientArea_Add(mian_window, wdg_dial);
	/* 改变部件尺寸，使用固定的尺寸 */
	Widget_Resize(wdg_dial, Graph_GetSize(&clock_bg));
	Widget_Resize(wdg_hour_ptr, Size(hour_ptr.height, hour_ptr.height));
	Widget_Resize(wdg_min_ptr, Size(min_ptr.height, min_ptr.height));
	Widget_Resize(wdg_sec_ptr, Size(sec_ptr.height, sec_ptr.height));
	/* 改变部件的布局方式，都是居中显示 */
	Widget_SetAlign(wdg_dial, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Widget_SetAlign(wdg_hour_ptr, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Widget_SetAlign(wdg_min_ptr, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Widget_SetAlign(wdg_sec_ptr, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	/* 设定各个指针的堆叠顺序 */
	Widget_SetZIndex( wdg_hour_ptr, 0 );
	Widget_SetZIndex( wdg_min_ptr, 1 );
	Widget_SetZIndex( wdg_sec_ptr, 2 );
	/* 先更新一次时钟上的指针 */
	UpdateClockPointer();
	/* 显示时钟 */
	Widget_Show(wdg_hour_ptr);
	Widget_Show(wdg_min_ptr);
	Widget_Show(wdg_sec_ptr);
	Widget_Show(wdg_dial);
}

static void Quit( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	LCUI_MainLoop_Quit( NULL );
}

int main( int argc, char **argv )
{
	LCUI_Widget *window;
	/* 初始化LCUI */
	// 缺省模式，默认是全屏
	LCUI_Init(0,0,0);
	// 全屏模式，分辨率为800x600
	//LCUI_Init(800,600,LCUI_INIT_MODE_FULLSCREEN);
	// 窗口模式，窗口尺寸为800x600
	//LCUI_Init(800,600,LCUI_INIT_MODE_WINDOW);
	/* 创建一个窗口 */
	window = Widget_New("window");
	/* 调整窗口尺寸 */
	Widget_Resize(window, Size(320, 320));
	/* 设置窗口标题文本 */
	Window_SetTitleText(window, "时钟");
	/* 创建时钟部件 */
	CreateClockWidget( window );
	/* 设置定时器，每隔1秒更新一次时指针 */
	LCUITimer_Set( 1000, UpdateClockPointer, TRUE );
	/* 关联窗口关闭按钮的CLOCKED事件 */
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, Quit);
	/* 显示窗口 */
	Widget_Show( window );
	return LCUI_Main();
}
