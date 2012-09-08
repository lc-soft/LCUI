// 测试LCUI的窗口
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H 
#include <unistd.h>

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
    LCUI_Init(argc, argv);
    LCUI_Widget *w1, *w2, *w3;
    /* 创建部件 */
    w1  = Create_Widget("window");
    w2  = Create_Widget("window");
    w3  = Create_Widget("window");
    /* 改变窗口的尺寸 */
    Resize_Widget(w1, Size(320, 240));
    Resize_Widget(w2, Size(200, 140));
    Resize_Widget(w3, Size(120, 100));
    Set_Window_Title_Text(w1, "窗口1");
    Set_Window_Title_Text(w2, "窗口2");
    Set_Window_Title_Text(w3, "窗口3");
    Window_Client_Area_Add(w2, w3);
    Window_Client_Area_Add(w1, w2);
    Show_Widget(w1); 
    Show_Widget(w2); 
    Show_Widget(w3); 
    LCUI_Main(); /* 进入主循环 */ 
    return 0;
}

