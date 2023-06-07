# 事件循环

本文对 SDL 和多个操作系统提供的事件/消息循环做了简单的调研，仅供参考，部分内容还有待验证。

## Windows 消息循环

系统仅在线程执行需要事件队列的相关操作时创建事件队列。如果线程创建了一个或更多个窗口，则必须提供消息循环。此消息循环从线程的消息队列中检索消息，并将其分派到适当的窗口过程。

参考资料：

- [visual c++ - Is it possible to create a message loop without creating a window in C++ - Stack Overflow](https://stackoverflow.com/questions/51942953/is-it-possible-to-create-a-message-loop-without-creating-a-window-in-c)
- [Using Messages and Message Queues - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues#creating-a-message-loop)

## X11 事件循环

XSendEvent() 的第二个参数必须指定一个窗口，因此，如果想实现窗口无关的事件通信的话，则需要额外创建一个专用窗口。

参考资料：

- [Xlib Programming Manual: XSendEvent](https://tronche.com/gui/x/xlib/event-handling/XSendEvent.html)
