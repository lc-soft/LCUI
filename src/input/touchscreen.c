#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_CURSOR_H
#include LC_WIDGET_H
#include LC_DISPLAY_H
#include LC_INPUT_H

static void * Handle_TouchScreen_Input ()
/* 功能：处理触屏输入 */
{
	char *tsdevice;
	struct ts_sample samp;
	int button, x, y, ret;
	LCUI_MouseEvent event;
	
	char str[100];
	while (LCUI_Active()) {
		if (LCUI_Sys.ts.status != INSIDE) {
			tsdevice = getenv("TSLIB_TSDEVICE");
			if( tsdevice != NULL ) {
				LCUI_Sys.ts.td = ts_open(tsdevice, 0);
			}
			else tsdevice = TS_DEV;
			LCUI_Sys.ts.td = ts_open (tsdevice, 0);
			if (!LCUI_Sys.ts.td) { 
				sprintf (str, "ts_open: %s", tsdevice);
				perror (str);
				LCUI_Sys.ts.status = REMOVE;
				break;
			}

			if (ts_config (LCUI_Sys.ts.td))
			{
				perror ("ts_config");
				LCUI_Sys.ts.status = REMOVE;
				break;
			}
			LCUI_Sys.ts.status = INSIDE;
		}

		/* 开始获取触屏点击处的坐标 */ 
		ret = ts_read (LCUI_Sys.ts.td, &samp, 1); 
		if (ret < 0) {
			perror ("ts_read");
			continue;
		}

		if (ret != 1) continue;

		x = samp.x;
		y = samp.y;
		
		if (x > Get_Screen_Width ())
			x = Get_Screen_Width ();
		if (y > Get_Screen_Height ())
			y = Get_Screen_Height ();
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		/* 设定游标位置 */ 
		Set_Cursor_Pos (Pos(x, y));
		
		event.global_pos.x = x;
		event.global_pos.y = y;
		/* 获取当前鼠标指针覆盖到的部件的指针 */
		event.widget = Get_Cursor_Overlay_Widget();
		/* 如果有覆盖到的部件，就需要计算鼠标指针与部件的相对坐标 */
		if(event.widget != NULL) {
			event.pos.x = x - Get_Widget_Global_Pos(event.widget).x;
			event.pos.y = y - Get_Widget_Global_Pos(event.widget).y;
		} else {/* 否则，和全局坐标一样 */
			event.pos.x = x;
			event.pos.y = y;
		}
		if (samp.pressure > 0)  button = 1; 
		else  button = 0; 
			/* 处理鼠标事件 */
		Handle_Mouse_Event(button, &event); 
		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
	}
	if(LCUI_Sys.ts.status == INSIDE) {
		ts_close(LCUI_Sys.ts.td); 
	}
	LCUI_Sys.ts.status = REMOVE;
	pthread_exit (NULL);
}

int Enable_TouchScreen_Input()
/* 功能：启用鼠标输入处理 */
{
	/* 创建一个线程，用于刷显示鼠标指针 */
	if(LCUI_Sys.ts.status == REMOVE) {
		return  pthread_create ( &LCUI_Sys.ts.thread, NULL, 
					Handle_TouchScreen_Input, NULL ); 
	}
	return 0;
}

int Disable_TouchScreen_Input()
/* 功能：撤销鼠标输入处理 */
{
	if(LCUI_Sys.ts.status == INSIDE) {
		return pthread_cancel ( LCUI_Sys.ts.thread );/* 撤销LCUI子线程 */ 
	}
	return 0;
}

