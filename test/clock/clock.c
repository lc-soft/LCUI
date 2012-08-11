// 时钟的例子程序

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPHICS_H
#include LC_THREAD_H
#include LC_MISC_H
#include LC_PICBOX_H
#include <unistd.h>

void *update_clock(void *arg)
{
    time_t rawtime;
    struct tm * timeinfo;
    LCUI_Graph h_temp, m_temp, clock_bg, hour_pointer, minute_pointer;
    LCUI_Widget *widget = (LCUI_Widget *)arg; /* 转换类型 */
    LCUI_Widget *bg, *hour, *minute; 
    int h_angle, m_angle;
    /* 初始化图形数据结构 */
    Graph_Init(&clock_bg);
    Graph_Init(&minute_pointer);
    Graph_Init(&hour_pointer);
    Graph_Init(&h_temp);
    Graph_Init(&m_temp);
    /* 创建几个部件 */
    bg = Create_Widget("picture_box");
    hour = Create_Widget("picture_box");
    minute = Create_Widget("picture_box");
    /* PictureBox部件居中显示图片 */
    Set_PictureBox_Size_Mode(bg, SIZE_MODE_CENTER);
    Set_PictureBox_Size_Mode(hour, SIZE_MODE_CENTER);
    Set_PictureBox_Size_Mode(minute, SIZE_MODE_CENTER);
    /* 载入图片，保存图形数据 */
    Load_Image("new_daytime_background.png", &clock_bg);
    Load_Image("new_daytime_hour.png", &hour_pointer);
    Load_Image("new_daytime_minute.png", &minute_pointer);
    /* 设定PictureBox部件显示的图形 */
    Set_PictureBox_Image_From_Graph(bg, &clock_bg);
    Set_PictureBox_Image_From_Graph(hour, &hour_pointer);
    Set_PictureBox_Image_From_Graph(minute, &minute_pointer);
    /* 将这些部件添加至相应容器中 */
    Widget_Container_Add(bg, hour);
    Widget_Container_Add(bg, minute);
    /* 将部件添加至窗口客户区中 */
    Window_Client_Area_Add(widget, bg);
    /* 改变部件尺寸，使用固定的尺寸 */
    Resize_Widget(bg, Size(280, 280));
    Resize_Widget(hour, Size(120, 120));
    Resize_Widget(minute, Size(120, 120));
    /* 改变部件的布局方式，都是居中显示 */
    Set_Widget_Align(bg, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(hour, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    Set_Widget_Align(minute, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    /* 显示 */
    Show_Widget(hour);
    Show_Widget(minute);
    Show_Widget(bg);
    
    while(1)
    {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */
        /* 计算时针分针的角度 */
        h_angle = 360*timeinfo->tm_hour / 12.0;
        m_angle = 360*timeinfo->tm_min / 60.0;
        h_angle += m_angle / 60;
        /* 根据这个角度来旋转图形 */
        Rotate_Graph(&hour_pointer, h_angle, &h_temp);
        Rotate_Graph(&minute_pointer, m_angle, &m_temp);
        /* 更改PictureBox部件显示的图形 */
        Set_PictureBox_Image_From_Graph(hour, &h_temp);
        Set_PictureBox_Image_From_Graph(minute, &m_temp);
        sleep(1);/* 暂停1秒 */
    }
    
    LCUI_Thread_Exit(NULL);
}

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
    LCUI_Widget *window;
    pthread_t t;
	Set_Default_Font("../../fonts/msyh.ttf");
    LCUI_Init(argc, argv);
    /* 创建部件 */
    window  = Create_Widget("window");
    /* 改变窗口的尺寸 */
    Resize_Widget(window, Size(320, 320));
    /* 设定窗口标题内容 */
    Set_Window_Title_Text(window, "时钟");
    /* 创建线程 */
    LCUI_Thread_Create(&t, NULL, update_clock, (void*)window); 
    Show_Widget(window); 
    return LCUI_Main(); /* 进入主循环 */  
}


