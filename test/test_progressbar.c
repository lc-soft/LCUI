
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
//测试进度条部件的示例程序
#include LC_WORK_H
#include LC_MISC_H
#include LC_THREAD_H
#include LC_PROGBAR_H
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
void *change_progress(void *arg)
/* 功能：动态改变label部件的文本内容 */
{
    int i, max;
    max = 100;
    LCUI_Widget *widget = (LCUI_Widget *)arg; /* 转换类型 */
    Set_ProgressBar_Max_Value(widget, max); /* 最大值 */
    srand(time(NULL));
    for(i=0; i<max; i+=rand()%5)
    {
        Set_ProgressBar_Value(widget, i);/* 当前值 */
        usleep(100000);/* 暂停0.1秒 */
    }
    Set_ProgressBar_Value(widget, max);
    LCUI_Thread_Exit(NULL);
}


int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
    pthread_t t[2];
    LCUI_Init(argc, argv);
    
    LCUI_Widget *window, *pb_a, *pb_b;
    /* 创建部件 */
    window = Create_Widget("window");
    pb_a = Create_Widget("progress_bar");
    pb_b = Create_Widget("progress_bar");
    /* 设定窗口标题的文本 */
    Set_Window_Title_Text(window, "测试进度条部件"); 
    /* 改变窗口的尺寸 */
    Resize_Widget(window, Size(320, 240));
    /* 将窗口客户区作为部件的容器添加进去 */
    Window_Client_Area_Add(window, pb_a);
    Window_Client_Area_Add(window, pb_b);
    /* 居中显示 */
    Set_Widget_Align(pb_a, ALIGN_MIDDLE_CENTER, Pos(0, -25)); 
    Set_Widget_Align(pb_b, ALIGN_MIDDLE_CENTER, Pos(0, 25));
    /* 设定部件的风格 */
    Set_Widget_Style(pb_a, "dynamic");
    Set_Widget_Style(pb_b, "classic");
    /* 改变部件尺寸 */
    Resize_Widget(pb_a, Size(300, 25));
    Resize_Widget(pb_b, Size(300, 25));
    /* 创建线程，此函数和pthread_create函数用法一样 */
    LCUI_Thread_Create(&t[0], NULL, change_progress, (void*)pb_a);
    LCUI_Thread_Create(&t[1], NULL, change_progress, (void*)pb_b); 
    /* 显示部件 */
    Show_Widget(pb_a);
    Show_Widget(pb_b);
    Show_Widget(window); 
    return LCUI_Main(); /* 进入主循环 */  
}

