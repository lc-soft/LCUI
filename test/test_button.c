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
    
    LCUI_Widget *window, *button;
    /* 创建部件 */
    window  = Create_Widget("window");
    button  = Create_Widget("button");
    
    /* 设定窗口标题的文本 */
    Set_Window_Title_Text(window, "测试按钮"); 
    /* 改变窗口的尺寸，也可以用Resize_Widget() */
    window->resize(window, Size(320, 240));
    /* 将窗口客户区作为按钮的容器添加进去 */
    Window_Client_Area_Add(window, button);
    /* 居中显示按钮,也可以用Set_Widget_Align() */
    button->set_align(button, ALIGN_MIDDLE_CENTER, Pos(0, 0));
    /* 禁用部件的自动尺寸调整 */
    Widget_AutoSize( button, FALSE, 0 );
    /* 自定义按钮的尺寸 */
    button->resize(button, Size(180, 70));
    
    /* 设定按钮上显示的文本内容，和label部件的用法一样,支持样式标签 */
    Set_Button_Text(button, 
                "<size=30px><color=255,0,0>彩</color>"
                "<color=0,255，0>色</color>"
                "<color=0,0,255>的</color>"
                "<color=255,255,0>按</color>"
                "<color=255,255,255>钮</color></size>");
    
    /* 显示部件，也可以用Show_Widget()函数 */
    button->show(button);
    window->show(window); 
    return LCUI_Main(); /* 进入主循环 */
}

