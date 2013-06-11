// 猜拳游戏

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_GRAPH_H
#include LC_RES_H
#include LC_INPUT_H

#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define WND_SIZE	Size(320,240)
#define TEXT_WND_TITLE	"猜拳"
#define TEXT_TIP	"本猜拳游戏基于 LCUI"
#define TEXT_SCROE	"玩家成绩: 赢%d次, 输%d次, 平局%d次"
#define TEXT_VS		"<size=55px><color=255,0,0>Vs.</color></size>"
#define TEXT_CLOTH	"布"
#define TEXT_KNIFE	"剪刀"
#define TEXT_STONE	"石头"
#define NEXT_ONE	"下一局"

static LCUI_Widget
*window, *l_vs, *btn_next, *btn_s, *btn_j, *btn_b, *btn_area,
*label, *me_pic_box, *cpu_pic_box;

static LCUI_Graph
icon, btn_normal, btn_focus, btn_down,
btn_over, stone, knife, cloth, think;

/* 已被水平翻转的图片数据 */
static LCUI_Graph
left_stone, left_knife, left_cloth;

/* 成绩 */
static int
win = 0, lose = 0, standoff = 0;

/* 更新文本 */
static void
update_text( void )
{
	char str[256];
	sprintf( str, TEXT_SCROE, win, lose, standoff );
	Label_Text( label, str );
}

static void
run_game(int select)
/* 功能：运行游戏 */
{
	int cpu_select;
	cpu_select = rand() % 3;  /* 产生3以内的随机数 */
	/* 根据随机数的取值，让PictureBox部件显示不同的图像 */
	if(cpu_select == 0) { /* 石头 */
		PictureBox_SetImage(cpu_pic_box, &stone);
	}
	else if(cpu_select == 1) { /* 剪刀 */
		PictureBox_SetImage(cpu_pic_box, &knife);
	} else {/* 布 */
		PictureBox_SetImage(cpu_pic_box, &cloth);
	}
	if(cpu_select == select) {/* 如果出一样的,平局次数+1 */
		standoff += 1;
	}
	else if((cpu_select == 0 && select == 1)
		|| (cpu_select == 1 && select == 2)
		|| (cpu_select == 2 && select == 0)) {
		/* 如果是程序赢了, 输的次数+1 */
		lose += 1;
	} else { /* 否则只有玩家赢了,赢的次数+1 */
		win += 1;
	}
	update_text();/* 更新文本内容 */
}

static void
clear_game()
/* 功能：清理游戏，恢复PictureBox部件显示的图像 */
{
	PictureBox_SetImage( me_pic_box, &think );
	PictureBox_SetImage( cpu_pic_box, &think );
	Widget_Disable(btn_next);
}

/* 玩家选择石头 */
static void
select_stone( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	/* 如果没有存储已水平翻转的图像，就进行水平翻转 */
	if(!Graph_IsValid(&left_stone)) {
		Graph_HorizFlip(&stone, &left_stone);
	}
	/* 设定显示的图像为石头 */
	PictureBox_SetImage(me_pic_box, &left_stone);
	Widget_Disable(btn_j);
	Widget_Disable(btn_s);
	Widget_Disable(btn_b);
	Widget_Enable(btn_next);
	run_game(0);/* 进入游戏 */
}

/* 玩家选择剪刀 */
static void
select_knife( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	if(!Graph_IsValid(&left_knife)) {
		Graph_HorizFlip(&knife, &left_knife);
	}
	PictureBox_SetImage(me_pic_box, &left_knife);
	Widget_Disable(btn_j);
	Widget_Disable(btn_s);
	Widget_Disable(btn_b);
	Widget_Enable(btn_next);
	run_game(1);
}

/* 玩家选择布 */
static void
select_cloth( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	if(!Graph_IsValid(&left_cloth)) {
		Graph_HorizFlip( &cloth, &left_cloth );
	}
	PictureBox_SetImage( me_pic_box, &left_cloth );
	Widget_Disable( btn_j );
	Widget_Disable( btn_s );
	Widget_Disable( btn_b );
	Widget_Enable( btn_next );
	run_game(2);
}

static void
next()
/* 功能：进行下一局 */
{
	Widget_Enable( btn_j );
	Widget_Enable( btn_s );
	Widget_Enable( btn_b );
	Widget_Disable( btn_next );
	clear_game();
}

static void
load_graphic_resource( void )
{
	Graph_Init(&btn_normal);
	Graph_Init(&btn_focus);
	Graph_Init(&btn_down);
	Graph_Init(&btn_over);
	Graph_Init(&stone);
	Graph_Init(&left_stone);
	Graph_Init(&cloth);
	Graph_Init(&left_cloth);
	Graph_Init(&knife);
	Graph_Init(&left_knife);
	Graph_Init(&think);
	Graph_Init(&icon);

	Graph_LoadImage("drawable/jsb_b.png", &cloth);
	Graph_LoadImage("drawable/jsb_s.png", &stone);
	Graph_LoadImage("drawable/jsb_j.png", &knife);
	Graph_LoadImage("drawable/think.png", &think);
	Graph_LoadImage("drawable/btn_focus.png", &btn_focus);
	Graph_LoadImage("drawable/btn_normal.png", &btn_normal);
	Graph_LoadImage("drawable/btn_over.png", &btn_over);
	Graph_LoadImage("drawable/btn_down.png", &btn_down);
	Graph_LoadImage("drawable/icon.png", &icon);
}

static void
widgets_structure( void )
{
	/* 创建一个窗口 */
	window = Widget_New("window");

	/* 创建label部件，用于显示文字 */
	label = Widget_New("label");
	l_vs = Widget_New("label");
	/* 创建几个按钮 */
	btn_b = Button_New( TEXT_CLOTH );
	btn_s = Button_New( TEXT_STONE );
	btn_j = Button_New( TEXT_KNIFE );
	btn_next = Button_New( NEXT_ONE );
	btn_area = Widget_New( NULL );
	cpu_pic_box = Widget_New("picture_box");
	me_pic_box = Widget_New("picture_box");

}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	LCUI_MainLoop_Quit(NULL);
}

static void
widgets_configure( void )
{
	/* 设定标题栏显示的文本 */
	Window_SetTitleText( window, TEXT_WND_TITLE );
	Window_SetTitleIcon( window, &icon );
	Widget_Resize( window, WND_SIZE );
	/* 将部件加入至相应的容器 */
	Widget_Container_Add( btn_area, btn_s );
	Widget_Container_Add( btn_area, btn_j );
	Widget_Container_Add( btn_area, btn_b );
	Widget_Container_Add( btn_area, btn_next );
	Window_ClientArea_Add( window, label );
	Window_ClientArea_Add( window, l_vs );
	Window_ClientArea_Add( window, me_pic_box );
	Window_ClientArea_Add( window, cpu_pic_box );
	Window_ClientArea_Add( window, btn_area );

	/* 设定label部件显示的文本内容 */
	Label_Text( label,  TEXT_TIP );

	/* 设定label部件中的字体大小为55像素，颜色为红色 */
	Label_Text( l_vs, TEXT_VS );
	/* 禁用按钮的自动尺寸调整 */
	Widget_SetAutoSize( btn_b, FALSE, 0 );
	Widget_SetAutoSize( btn_s, FALSE, 0 );
	Widget_SetAutoSize( btn_j, FALSE, 0 );
	Widget_SetAutoSize( btn_next, FALSE, 0 );
	/* 调整这些部件的大小 */
	Widget_SetSize( btn_b, "78px", "30px" );
	Widget_SetSize( btn_s, "77px", "30px" );
	Widget_SetSize( btn_j, "77px", "30px" );
	Widget_SetSize( btn_next, "78px", "30px" );
	Widget_SetSize( btn_area, NULL, "30px" );
	Widget_SetDock( btn_area, DOCK_TYPE_BOTTOM );
	Widget_Resize( me_pic_box, Size(110, 140) );
	Widget_Resize( cpu_pic_box, Size(110, 140) );
	/* 自定义这四个按钮的风格 */
	Button_CustomStyle(	btn_b, &btn_normal, &btn_over,
				&btn_down, &btn_focus, NULL);
	Button_CustomStyle(	btn_s, &btn_normal, &btn_over,
				&btn_down, &btn_focus, NULL);
	Button_CustomStyle(	btn_j, &btn_normal, &btn_over,
				&btn_down, &btn_focus, NULL);
	Button_CustomStyle(	btn_next, &btn_normal, &btn_over,
				&btn_down, &btn_focus, NULL);
	/* 设定部件的定位类型 */
	Widget_SetPosType( btn_s, POS_TYPE_STATIC );
	Widget_SetPosType( btn_j, POS_TYPE_STATIC );
	Widget_SetPosType( btn_b, POS_TYPE_STATIC );
	Widget_SetPosType( btn_next, POS_TYPE_STATIC );
	/* 设定布局 */
	Widget_SetAlign(me_pic_box, ALIGN_MIDDLE_LEFT, Pos(5, -5));
	Widget_SetAlign(cpu_pic_box, ALIGN_MIDDLE_RIGHT, Pos(-5, -5));
	Widget_SetAlign(label, ALIGN_TOP_CENTER, Pos(0, 3));
	Widget_SetAlign(l_vs, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	/* 设定部件的边框及颜色 */
	Widget_SetBorder(me_pic_box, Border(1, BORDER_STYLE_SOLID, RGB(0,0,0)));
	Widget_SetBorder(cpu_pic_box, Border(1, BORDER_STYLE_SOLID, RGB(0,0,0)));
	/* 关联这些按钮的单击事件 */
	Widget_Event_Connect(btn_s, EVENT_CLICKED, select_stone );
	Widget_Event_Connect(btn_j, EVENT_CLICKED, select_knife );
	Widget_Event_Connect(btn_b, EVENT_CLICKED, select_cloth );
	Widget_Event_Connect(btn_next, EVENT_CLICKED, next );
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, destroy);
}

static void
Show_GUI( void )
{
	Widget_Show( btn_s );
	Widget_Show( btn_j );
	Widget_Show( btn_b );
	Widget_Show( label );
	Widget_Show( btn_area );
	Widget_Show( l_vs );
	Widget_Show( me_pic_box );
	Widget_Show( cpu_pic_box );
	Widget_Show( btn_next );
	Widget_Show( window );
}

static void
Create_GUI( void )
{
	load_graphic_resource();
	widgets_structure();
	widgets_configure();
}

int main(int argc, char*argv[])
{
	LCUI_Init(0,0,0); /* 初始化LCUI */
	Create_GUI(); /* 创建图形界面 */
	clear_game(); /* 清理游戏 */
	Show_GUI(); /* 显示图形界面 */
	return LCUI_Main();
}
