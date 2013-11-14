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
#include <time.h>

static LCUI_Widget	*pic_bg, *time_box, *pic_btn_line,
			*pic_btn, *pic_l1, *pic_l2,
			*pic_c, *pic_r1, *pic_r2;

static LCUI_Widget *wday_label, *date_label;
static LCUI_Graph img_digital[9], img_dot, img_bg, img_btn, img_btn_bg;

static LCUI_BOOL need_move_pic_btn = FALSE;

static void PicBtn_UpdatePos( void *arg )
{
	static int n = 2;
	LCUI_Pos pos;
	if( !need_move_pic_btn ) {
		n = 4;
		return;
	}
	pos = _Widget_GetPos( pic_btn );
	if( pos.x == 0 ) {
		return;
	}
	pos.x -= n;
	if( pos.x < 0 ) {
		pos.x = 0;
	}
	pos.y = 0;
	n+=2;
	Widget_Move( pic_btn, pos );
}

/* 移动滑块 */
void move_pic_btn(LCUI_Widget *widget, LCUI_WidgetEvent *event)
{
	LCUI_Size size;
	LCUI_Rect des, rect;
	LCUI_Pos pos, parent;

	need_move_pic_btn = FALSE;
	parent = Widget_GetGlobalPos(widget->parent);
	pos = Pos_Sub(event->drag.new_pos, parent);
	Widget_Move(widget, pos);
	if(event->drag.end_click) {/* 如果拖动已经结束 */
		rect = Widget_GetRect(widget);
		size = Widget_GetSize(widget->parent);
		des = Rect(size.w-10, 0, 30, 30);/* 目标区域 */
		 /* 如果部件区域与目标区域重叠，则退出程序 */
		if(LCUIRect_Overlay(rect, des)) {
			LCUI_MainLoop_Quit(NULL);
		} else {/* 否则，让部件回到起始位置，这个使用的是匀速移动 */
			need_move_pic_btn = TRUE;
		}
	}
}

static void LoadIMG(void)
{
	int i;
	char filepath[1024];
	for(i=0; i<10; ++i) {
		sprintf( filepath, "drawable/time_%d.png", i );
		Graph_Init( &img_digital[i] );
		Graph_LoadImage( filepath, &img_digital[i] );
	}
	Graph_Init( &img_dot );
	Graph_Init( &img_btn );
	Graph_Init( &img_btn_bg );
	Graph_Init( &img_bg );
	Graph_LoadImage( "drawable/time_dot.png", &img_dot );
	Graph_LoadImage( "drawable/btn.png", &img_btn );
	Graph_LoadImage( "drawable/btn_bg.png", &img_btn_bg );
	Graph_LoadImage( "drawable/bg.png", &img_bg );
}

static void FreeIMG(void)
{
	int i;
	for(i=0; i<10; ++i) {
		Graph_Free( &img_digital[i] );
	}
	Graph_Free( &img_dot );
	Graph_Free( &img_btn );
	Graph_Free( &img_btn_bg );
	Graph_Free( &img_bg );
}

/* 动态改变label部件的文本内容 */
static void UpdateTimeView(void *arg)
{
	time_t rawtime;
	struct tm *timeinfo;
	wchar_t date_text[64];
	wchar_t day[][20] = {
		L"星期天", L"星期一", L"星期二",
		L"星期三", L"星期四", L"星期五", L"星期六"};

	time ( &rawtime );
	/* 获取系统当前时间 */
	timeinfo = localtime ( &rawtime );
	swprintf( date_text, 64,
		L"<color=40,165,45>%4d年%02d月%02d日</color>",
		timeinfo->tm_year+1900, timeinfo->tm_mon+1,
		timeinfo->tm_mday );

	Label_TextW( date_label, date_text );

	swprintf( date_text, 64,
		L"<color=40,165,45>%s</color>",
		day[(int) (timeinfo->tm_wday)] );

	Label_TextW( wday_label, date_text );

	Widget_SetBackgroundImage( pic_l1, &img_digital[timeinfo->tm_hour/10] );
	Widget_SetBackgroundImage( pic_l2, &img_digital[timeinfo->tm_hour%10] );
	Widget_SetBackgroundImage( pic_r1, &img_digital[timeinfo->tm_min/10] );
	Widget_SetBackgroundImage( pic_r2, &img_digital[timeinfo->tm_min%10] );
}

static void CreateGUI( LCUI_Widget *win )
{
	/* 创建部件 */
	time_box	= Widget_New(NULL);
	pic_bg		= Widget_New(NULL);
	pic_l1		= Widget_New(NULL);
	pic_l2		= Widget_New(NULL);
	pic_c		= Widget_New(NULL);
	pic_btn		= Widget_New(NULL);
	pic_btn_line	= Widget_New(NULL);
	pic_btn		= Widget_New(NULL);
	pic_r1		= Widget_New(NULL);
	pic_r2		= Widget_New(NULL);
	date_label	= Widget_New("label");
	wday_label	= Widget_New("label");
	/* 设置这些部件的初始背景图 */
	Widget_SetBackgroundImage( pic_bg, &img_bg );
	Widget_SetBackgroundImage( pic_btn_line, &img_btn_bg );
	Widget_SetBackgroundLayout( pic_btn_line, LAYOUT_CENTER );
	Widget_SetBackgroundImage( pic_btn, &img_btn );
	Widget_SetBackgroundImage( pic_l1, &img_digital[0] );
	Widget_SetBackgroundImage( pic_l2, &img_digital[0] );
	Widget_SetBackgroundImage( pic_r1, &img_digital[0] );
	Widget_SetBackgroundImage( pic_r2, &img_digital[0] );
	Widget_SetBackgroundImage( pic_c,  &img_dot );
	/* 调整尺寸 */
	Widget_Resize( time_box,	Size(162, 38) );
	Widget_Resize( pic_bg,		Size(300, 90) );
	Widget_Resize( pic_btn_line,	Size(245, 66) );
	Widget_Resize( pic_btn,		Size(46, 66) );
	Widget_Resize( pic_l1,		Size(33, 36) );
	Widget_Resize( pic_l2,		Size(33, 36) );
	Widget_Resize( pic_c,		Size(30, 38) );
	Widget_Resize( pic_r1,		Size(33, 36) );
	Widget_Resize( pic_r2,		Size(33, 36) );
	/* 调整布局 */
	Widget_SetAlign( pic_l1, ALIGN_MIDDLE_CENTER, Pos(-66,0) );
	Widget_SetAlign( pic_l2, ALIGN_MIDDLE_CENTER, Pos(-33,0) );
	Widget_SetAlign( pic_c, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_SetAlign( pic_r1, ALIGN_MIDDLE_CENTER, Pos(33,0) );
	Widget_SetAlign( pic_r2, ALIGN_MIDDLE_CENTER, Pos(66,0) );
	Widget_SetAlign( time_box, ALIGN_MIDDLE_CENTER, Pos(0,-50) );
	Widget_SetAlign( pic_bg, ALIGN_MIDDLE_CENTER, Pos(0,5) );
	Widget_SetAlign(date_label, ALIGN_MIDDLE_CENTER, Pos(0,35-50) );
	Widget_SetAlign( wday_label, ALIGN_TOP_CENTER, Pos(0,5) );
	Widget_SetAlign( pic_btn_line, ALIGN_BOTTOM_CENTER, Pos(0,-10) );
	/* 放入容器 */
	Widget_Container_Add( time_box, pic_l1 );
	Widget_Container_Add( time_box, pic_l2 );
	Widget_Container_Add( time_box, pic_c );
	Widget_Container_Add( time_box, pic_r1 );
	Widget_Container_Add( time_box, pic_r2 );
	Widget_Container_Add( pic_btn_line, pic_btn );

	Window_ClientArea_Add( win, pic_bg );
	Window_ClientArea_Add( win, time_box );
	Window_ClientArea_Add( win, date_label );
	Window_ClientArea_Add( win, wday_label );
	Window_ClientArea_Add (win, pic_btn_line );
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

	UpdateTimeView(NULL);
}

int main( int argc, char **argv )
{
	LCUI_Widget *window;
	/* 载入图像资源 */
	LoadIMG();
	/* 初始化LCUI */
	LCUI_Init(350,280,0);
	/* 创建窗口 */
	window = Widget_New("window");
	/* 设置窗口背景色 */
	Widget_SetBackgroundColor( window, RGB(255,255,255) );
	/* 调整窗口尺寸 */
	Widget_Resize( window, Size(320, 240) );
	/* 设置窗口为线条边框风格 */
	Widget_SetStyleID( window, WINDOW_STYLE_NONE );
	/* 将界面创建至窗口内 */
	CreateGUI( window );
	/* 设置定时器，用于定时更新时间显示 */
	LCUITimer_Set( 1000, UpdateTimeView, NULL, TRUE );
	/* 设置定时器，用于更新滑块的位置 */
	LCUITimer_Set( 20, PicBtn_UpdatePos, NULL, TRUE );
	/* 显示窗口 */
	Widget_Show( window );
	/* 在LCUI退出时释放图像资源 */
	LCUIApp_AtQuit( FreeIMG );
	return LCUI_Main();
}

