// 用LCUI实现hello,world程序

#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>

int main(int argc, char*argv[]) 
{
	/* 目前传递给LCUI_Init()函数的参数没用，以后会完善，比如指定字体路径，是否显示鼠标游标等 */
    LCUI_Init(argc, argv);
    LCUI_Widget *window, *label;
    LCUI_Graph pic;
    /* 初始化结构体 */
    Graph_Init(&pic);
    /* 载入库中自带的图形数据，这个图形是18x18尺寸的LCUI的图标 */
    Load_Graph_Icon_LCUI_18x18(&pic);
    /* 创建部件 */
    window  = Create_Widget("window");
    label   = Create_Widget("label");
    
    /* 设定窗口标题的文本 */
    Set_Window_Title_Text(window, "测试窗口 abcde");
    /* 设定窗口标题的图标 */
    Set_Window_Title_Icon(window, &pic);
    /* 改变窗口的尺寸 */
    window->resize(window, Size(320, 240));
    /* 将窗口客户区作为label部件的容器添加进去 */
    Window_Client_Area_Add(window, label);
    /* 居中显示 */
    label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0));
    /* 
     * 设定label部件显示的文本
     * <color=R,G,B>文字</color> 表示的是：“文字”将使用自定义颜色 
     * Set_Label_Text函数使用方法和sprintf函数基本一样，支持控制符
     **/
    Set_Label_Text(label, 
                "Hello,World!\n"
                "<color=255,0,0>世</color>"
                "<color=100,155,0>界</color>"
                "<color=0,0,0>,</color>"
                "<color=0,255,0>你</color>"
                "<color=0,100,155>好</color>"
                "<color=0,0,255>！</color>\n");
    /* 
     * 改变label部件的字体大小，单位为pixel(像素)，不是pt（点数）
     * 第三个参数是自定义使用的字体文件，我设定为NULL，表示不使用。
     **/
    Set_Label_Font(label, 24, NULL);
    /* 显示部件 */
    label->show(label);
    window->show(window);
    return LCUI_Main(); /* 进入主循环 */
}

