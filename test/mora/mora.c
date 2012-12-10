// 猜拳游戏

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_DRAW_H
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

static void 
update_text()
/* 功能：更新文本 */
{
	Label_Text(label, TEXT_SCROE, win, lose, standoff); 
}

static void 
run_game(int select)
/* 功能：运行游戏 */
{
	int cpu_select;
	cpu_select = rand() % 3;  /* 产生3以内的随机数 */ 
	/* 根据随机数的取值，让PictureBox部件显示不同的图像 */
	if(cpu_select == 0) { /* 石头 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &stone); 
	}
	else if(cpu_select == 1) { /* 剪刀 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &knife); 
	} else {/* 布 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &cloth); 
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
	Set_PictureBox_Image_From_Graph( me_pic_box, &think ); 
	Set_PictureBox_Image_From_Graph( cpu_pic_box, &think );
	Disable_Widget(btn_next);
}

void select_stone(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择石头 */
{
	/* 如果没有存储已水平翻转的图像，就进行水平翻转 */
	if(!Graph_Valid(&left_stone)) {
		Graph_Flip_Horizontal(&stone, &left_stone);
	}
	/* 设定显示的图像为石头 */
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_stone);
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(0);/* 进入游戏 */
}

static void 
select_knife(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择剪刀 */
{
	if(!Graph_Valid(&left_knife)) {
		Graph_Flip_Horizontal(&knife, &left_knife);
	}
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_knife); 
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(1);
}

static void 
select_cloth(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择布 */
{
	if(!Graph_Valid(&left_cloth)) {
		Graph_Flip_Horizontal( &cloth, &left_cloth );
	}
	Set_PictureBox_Image_From_Graph( me_pic_box, &left_cloth ); 
	Disable_Widget( btn_j );
	Disable_Widget( btn_s );
	Disable_Widget( btn_b );
	Enable_Widget( btn_next );
	run_game(2);
}

static void 
next()
/* 功能：进行下一局 */
{
	Enable_Widget( btn_j );
	Enable_Widget( btn_s );
	Enable_Widget( btn_b );
	Disable_Widget( btn_next );
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
	
	Load_Image("drawable/jsb_b.png", &cloth); 
	Load_Image("drawable/jsb_s.png", &stone);  
	Load_Image("drawable/jsb_j.png", &knife); 
	Load_Image("drawable/think.png", &think); 
	Load_Image("drawable/btn_focus.png", &btn_focus); 
	Load_Image("drawable/btn_normal.png", &btn_normal); 
	Load_Image("drawable/btn_over.png", &btn_over); 
	Load_Image("drawable/btn_down.png", &btn_down); 
	Load_Image("drawable/icon.png", &icon);
}

static void
widgets_structure( void )
{
	/* 创建一个窗口 */
	window = Create_Widget("window"); 
	
	/* 创建label部件，用于显示文字 */ 
	label = Create_Widget("label");
	l_vs = Create_Widget("label");
	/* 创建几个按钮 */
	btn_b = Create_Button_With_Text( TEXT_CLOTH ); 
	btn_s = Create_Button_With_Text( TEXT_STONE );
	btn_j = Create_Button_With_Text( TEXT_KNIFE );
	btn_next = Create_Button_With_Text( NEXT_ONE );
	btn_area = Create_Widget( NULL );
	cpu_pic_box = Create_Widget("picture_box");
	me_pic_box = Create_Widget("picture_box");
	
}

static void
widgets_configure( void )
{
	/* 设定标题栏显示的文本 */
	Set_Window_Title_Text( window, TEXT_WND_TITLE );
	Set_Window_Title_Icon( window, &icon );
	Resize_Widget( window, WND_SIZE );
	/* 将部件加入至相应的容器 */
	Widget_Container_Add( btn_area, btn_s );
	Widget_Container_Add( btn_area, btn_j );
	Widget_Container_Add( btn_area, btn_b );
	Widget_Container_Add( btn_area, btn_next );
	Window_Client_Area_Add( window, label );
	Window_Client_Area_Add( window, l_vs );
	Window_Client_Area_Add( window, me_pic_box );
	Window_Client_Area_Add( window, cpu_pic_box );
	Window_Client_Area_Add( window, btn_area );

	/* 设定label部件显示的文本内容 */
	Label_Text( label,  TEXT_TIP );
	
	/* 设定label部件中的字体大小为55像素，颜色为红色 */
	Label_Text( l_vs, TEXT_VS );
	/* 禁用按钮的自动尺寸调整 */
	Widget_AutoSize( btn_b, FALSE, 0 );
	Widget_AutoSize( btn_s, FALSE, 0 );
	Widget_AutoSize( btn_j, FALSE, 0 );
	Widget_AutoSize( btn_next, FALSE, 0 );
	/* 调整这些部件的大小 */
	Set_Widget_Size( btn_b, "78px", "30px" );
	Set_Widget_Size( btn_s, "77px", "30px" );
	Set_Widget_Size( btn_j, "77px", "30px" );
	Set_Widget_Size( btn_next, "78px", "30px" );
	Set_Widget_Size( btn_area, NULL, "30px" );
	Set_Widget_Dock( btn_area, DOCK_TYPE_BOTTOM );
	Resize_Widget(me_pic_box, Size(110, 140));
	Resize_Widget(cpu_pic_box, Size(110, 140));
	/* 自定义这四个按钮的风格 */
	Custom_Button_Style(btn_b,    &btn_normal, 
		&btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_s,    &btn_normal, 
		&btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_j,    &btn_normal, 
		&btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_next, &btn_normal, 
		&btn_over, &btn_down, &btn_focus, NULL); 
	/* 设定部件的定位类型 */
	Set_Widget_PosType( btn_s, POS_TYPE_STATIC );
	Set_Widget_PosType( btn_j, POS_TYPE_STATIC );
	Set_Widget_PosType( btn_b, POS_TYPE_STATIC );
	Set_Widget_PosType( btn_next, POS_TYPE_STATIC );
	/* 设定布局 */
	Set_Widget_Align(me_pic_box, ALIGN_MIDDLE_LEFT, Pos(5, -5));
	Set_Widget_Align(cpu_pic_box, ALIGN_MIDDLE_RIGHT, Pos(-5, -5));
	Set_Widget_Align(label, ALIGN_TOP_CENTER, Pos(0, 3));
	Set_Widget_Align(l_vs, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	/* 设定部件的边框及颜色 */
	Set_Widget_Border(me_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	Set_Widget_Border(cpu_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	/* 关联这些按钮的单击事件 */ 
	Widget_Clicked_Event_Connect(btn_s, select_stone, NULL);
	Widget_Clicked_Event_Connect(btn_j, select_knife, NULL);
	Widget_Clicked_Event_Connect(btn_b, select_cloth, NULL);
	Widget_Clicked_Event_Connect(btn_next, next, NULL);
}

static void
Show_GUI( void )
{
	Show_Widget( btn_s );
	Show_Widget( btn_j );
	Show_Widget( btn_b );
	Show_Widget( label );
	Show_Widget( btn_area );
	Show_Widget( l_vs );
	Show_Widget( me_pic_box );
	Show_Widget( cpu_pic_box );
	Show_Widget( btn_next );
	Show_Widget( window );
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
	setenv( "LCUI_FONTFILE", "../../fonts/msyh.ttf", FALSE );
	LCUI_Init(argc, argv); /* 初始化LCUI */
	Create_GUI(); /* 创建图形界面 */
	clear_game(); /* 清理游戏 */
	Show_GUI(); /* 显示图形界面 */
	return LCUI_Main();
}
