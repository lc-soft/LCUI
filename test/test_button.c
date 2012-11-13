// 测试按钮部件的程序例子
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_MISC_H
#include LC_BUTTON_H //用到了按钮部件，需要包含这个头文件
#include <unistd.h>

int main(int argc, char*argv[]) 
{
    LCUI_Init(argc, argv);
    LCUI_Widget *window, *button[2];
    /* 创建部件 */
    window  = Create_Widget("window");
    button[0]  = Create_Widget("button");
    button[1]  = Create_Widget("button");
    
    /* 设定窗口标题的文本 */
    Set_Window_Title_Text(window, "测试窗口"); 
    /* 改变窗口的尺寸，也可以用Resize_Widget() */
    window->resize(window, Size(320, 240));
    /* 将窗口客户区作为lael部件的容器添加进去 */
    Window_Client_Area_Add(window, button[0]);
    Window_Client_Area_Add(window, button[1]);
    /* 居中显示按钮,也可以用Set_Widget_Align() */
    button[0]->set_align(button[0], ALIGN_MIDDLE_CENTER, Pos(45, 0));
    button[1]->set_align(button[1], ALIGN_MIDDLE_CENTER, Pos(-50, 0));
    /* 禁用部件的自动尺寸调整 */
    Disable_Widget_Auto_Size(button[0]);
    Disable_Widget_Auto_Size(button[1]);
    /* 自定义按钮的尺寸 */
    button[0]->resize(button[0], Size(80, 30));
    button[1]->resize(button[1], Size(100, 40));
    
    Set_Widget_ClickableAlpha( button[1], 255, 0 );
    /* 设定按钮上显示的文本内容，和label部件的用法一样 */
    Set_Button_Text(button[0], 
                "<color=255,0,0>彩</color>"
                "<color=0,255，0>色</color>"
                "<color=0,0,255>的</color>"
                "<color=255,255,0>按</color>"
                "<color=255,255,255>钮</color>");
    Set_Button_Text(button[1], "你点击不到的按钮");
    
    /* 显示部件，也可以用Show_Widget()函数 */
    button[0]->show(button[0]);
    button[1]->show(button[1]);
    window->show(window); 
    return LCUI_Main(); /* 进入主循环 */
}

