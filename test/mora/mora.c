#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_MEM_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_INPUT_H 

#include <unistd.h>
#include <time.h>   
#include <stdlib.h>

static LCUI_Widget *window, *l_vs, *btn_next, *btn_s, *btn_j, *btn_b; 
static LCUI_Graph  stone, knife, cloth, think; 
static LCUI_Graph  left_stone, left_knife, left_cloth; /* 已被水平翻转的图片数据 */
static LCUI_Widget *label, *me_pic_box, *cpu_pic_box;
int win = 0, lose = 0, standoff = 0; /* 成绩 */

void update_text()
/* 功能：更新文本 */
{
	Set_Label_Text(label, "玩家成绩: 赢%d次, 输%d次, 平局%d次", 
						win, lose, standoff); /* 设定label部件显示的文本 */
}


void run_game(int select)
/* 功能：运行游戏 */
{
	int cpu_select;
	cpu_select = rand() % 3;  /* 产生3以内的随机数 */ 
	/* 根据随机数的取值，让PictureBox部件显示不同的图像 */
	if(cpu_select == 0) /* 石头 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &stone); 
	else if(cpu_select == 1) /* 剪刀 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &knife); 
	else /* 布 */
		Set_PictureBox_Image_From_Graph(cpu_pic_box, &cloth); 
	
	if(cpu_select == select) /* 如果出一样的,平局次数+1 */
		standoff += 1; 
	else if((cpu_select == 0 && select == 1) 
			|| (cpu_select == 1 && select == 2) 
			|| (cpu_select == 2 && select == 0)
			)/* 如果是程序赢了, 输的次数+1 */
		lose += 1; 
	else  /* 否则只有玩家赢了,赢的次数+1 */
		win += 1; 
	update_text();/* 更新文本内容 */
}

void clear_game()
/* 功能：清理游戏，恢复PictureBox部件显示的图像 */
{
	Set_PictureBox_Image_From_Graph(me_pic_box, &think); 
	Set_PictureBox_Image_From_Graph(cpu_pic_box, &think);
	Disable_Widget(btn_next);
}

void select_stone(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择石头 */
{
	/* 如果没有存储已水平翻转的图像，就进行水平翻转 */
	if(!Valid_Graph(&left_stone)) 
		Graph_Flip_Horizontal(&stone, &left_stone);
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_stone); /* 设定显示的图像为石头 */
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(0);/* 进入游戏 */
}

void select_knife(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择剪刀 */
{
	if(!Valid_Graph(&left_knife)) 
		Graph_Flip_Horizontal(&knife, &left_knife);
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_knife); 
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(1);
}

void select_cloth(LCUI_Widget *widget, void *junk)
/* 功能：玩家选择布 */
{
	if(!Valid_Graph(&left_cloth)) 
		Graph_Flip_Horizontal(&cloth, &left_cloth);
		
	Set_PictureBox_Image_From_Graph(me_pic_box, &left_cloth); 
	Disable_Widget(btn_j);
	Disable_Widget(btn_s);
	Disable_Widget(btn_b);
	Enable_Widget(btn_next);
	run_game(2);
}

void next()
/* 功能：进行下一局 */
{
	Enable_Widget(btn_j);
	Enable_Widget(btn_s);
	Enable_Widget(btn_b);
	Disable_Widget(btn_next); 
	clear_game();
}

void Get_Path(char *filepath, char *out_path)
/* 功能：用于获取程序所在的文件目录 */
{
	int num; 
	strcpy(out_path, filepath);
	for(num = strlen(filepath) - 1; num >= 0; --num) 
	{
		if(filepath[num] == '/') 
		{
			out_path[num+1] = 0;
			return;
		}
	}
	out_path[0] = 0;
}


int main(int argc,char*argv[])
/* 主函数，程序的入口 */
{
	LCUI_Graph   icon, btn_normal, btn_focus, btn_down, btn_over;/* 存储按钮各种状态要显示的图形 */
	int			 width, height;
	/* 自定义默认字体文件位置 */
	//Set_Default_Font("/mnt/Data/LC-SOFT/fonts/微软雅黑.ttf");
	Set_Default_Font("../../fonts/msyh.ttf");
	/* 初始化LCUI */
	LCUI_Init(argc, argv); 
	/* 初始化图片数据结构体 */
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
	
	/* 获取文件的路径，之后打开并载入图片 */ 
	Load_Image("drawable/jsb_b.png", &cloth); 
	Load_Image("drawable/jsb_s.png", &stone);  
	Load_Image("drawable/jsb_j.png", &knife); 
	Load_Image("drawable/think.png", &think); 
	Load_Image("drawable/btn_focus.png", &btn_focus); 
	Load_Image("drawable/btn_normal.png", &btn_normal); 
	Load_Image("drawable/btn_over.png", &btn_over); 
	Load_Image("drawable/btn_down.png", &btn_down); 
	Load_Image("drawable/icon.png", &icon);
	
	/* 创建一个LCUI程序窗口 */
	width  = 320; /* 记录窗口的宽度 */
	height = 240; /* 记录窗口的高度 */ 
	
	/* 创建一个窗口 */
	window = Create_Widget("window"); 
	/* 设定标题栏显示的文本 */
	Set_Window_Title_Text(window, "猜拳");
	Set_Window_Title_Icon(window, &icon);
	Resize_Widget(window, Size(width, height));
	
	/* 在窗口内创建相应的部件，并获取部件指针 */ 
	label		= Create_Widget("label"); /* label部件，用于显示文字 */
	l_vs		= Create_Widget("label");
	/* button部件，提供可被点击的按钮，创建后，使用自定义的文本 */
	btn_b		= Create_Button_With_Text("布"); 
	btn_s		= Create_Button_With_Text("石头");
	btn_j		= Create_Button_With_Text("剪刀");
	btn_next	= Create_Button_With_Text("下一局");
	/* picture_box部件，用于显示图像 */
	cpu_pic_box = Create_Widget("picture_box"); /* 这个是用于显示程序出的拳 */
	me_pic_box  = Create_Widget("picture_box"); /* 这个是用于显示玩家出的拳 */
	/* 设定label部件显示的文本内容 */
	Set_Label_Text(label, "本猜拳游戏由liuchao35758600制作");
	
	/* 设定label部件中的字体大小为55像素，颜色为红色 */
	Set_Label_Font(l_vs, 55, NULL); 
	Set_Label_Text(l_vs, "<color=255,0,0>Vs.</color>");
	/* 禁用按钮的自动尺寸调整 */
	Disable_Widget_Auto_Size(btn_b);
	Disable_Widget_Auto_Size(btn_s);
	Disable_Widget_Auto_Size(btn_j);
	Disable_Widget_Auto_Size(btn_next);
	/* 调整这些部件的大小 */
	Resize_Widget(btn_b, Size(80, 30));
	Resize_Widget(btn_s, Size(80, 30));
	Resize_Widget(btn_j, Size(80, 30));
	Resize_Widget(btn_next, Size(80, 30));
	Resize_Widget(me_pic_box, Size(110, 140));
	Resize_Widget(cpu_pic_box, Size(110, 140));
	/* 自定义这四个按钮的风格 */
	Custom_Button_Style(btn_b,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_s,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_j,    &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	Custom_Button_Style(btn_next, &btn_normal, &btn_over, &btn_down, &btn_focus, NULL); 
	/* 设定部件对齐方式以及偏移距离 */
	Set_Widget_Align(btn_s, ALIGN_BOTTOM_LEFT, Pos(-1, 0));  /* 左下 */
	Set_Widget_Align(btn_j, ALIGN_BOTTOM_CENTER, Pos(-40, 0)); /* 中下 */
	Set_Widget_Align(btn_b, ALIGN_BOTTOM_CENTER, Pos(40, 0));  /* 中下 */
	Set_Widget_Align(btn_next, ALIGN_BOTTOM_RIGHT, Pos(1, 0)); /* 右下 */
	Set_Widget_Align(me_pic_box, ALIGN_MIDDLE_LEFT, Pos(5, -5)); /* 中左 */
	Set_Widget_Align(cpu_pic_box, ALIGN_MIDDLE_RIGHT, Pos(-5, -5)); /* 中右 */
	Set_Widget_Align(label, ALIGN_TOP_CENTER, Pos(0, 3));   /* 中上 */
	Set_Widget_Align(l_vs, ALIGN_MIDDLE_CENTER, Pos(0, 0)); /* 居中 */
	/* 设定部件的边框以及颜色 */
	Set_Widget_Border(me_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	Set_Widget_Border(cpu_pic_box, RGB(0,0,0), Border(1, 1, 1, 1));
	/* 清理游戏 */
	clear_game();
	/* 将返回键与 Main_Loop_Quit函数关联，当返回键被按下后，程序退出主循环 */
	LCUI_Key_Event_Connect(KEY_ESC, Main_Loop_Quit, NULL);
	/* 关联这些按钮的单击事件 */ 
	Widget_Clicked_Event_Connect(btn_s, select_stone, NULL); 
	
	Widget_Clicked_Event_Connect(btn_j, select_knife, NULL); 
	
	Widget_Clicked_Event_Connect(btn_b, select_cloth, NULL); 
	
	Widget_Clicked_Event_Connect(btn_next, next, NULL);
	/* 将它们加入至窗口客户区，也就是以窗口客户区为容器 */
	Window_Client_Area_Add(window,btn_s);
	Window_Client_Area_Add(window,btn_j);
	Window_Client_Area_Add(window,btn_b);
	Window_Client_Area_Add(window,label);
	Window_Client_Area_Add(window,l_vs);
	Window_Client_Area_Add(window,me_pic_box);
	Window_Client_Area_Add(window,cpu_pic_box);
	Window_Client_Area_Add(window,btn_next);
	/* 显示部件以及窗口 */ 
	Show_Widget(btn_s);
	Show_Widget(btn_j);
	Show_Widget(btn_b);
	Show_Widget(label);
	Show_Widget(l_vs);
	Show_Widget(me_pic_box);
	Show_Widget(cpu_pic_box);
	Show_Widget(btn_next);
	Show_Widget(window);
	return LCUI_Main(); /* 进入LCUI的主循环 */
}
 
