/*
 * 文件名：ts_calibrate.c
 * 说明：这是一个简单的触瓶校准程序，借用了tslib给的示例源代码
 * */

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_DISPLAY_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_FONT_H
#include LC_RES_H
#include LC_INPUT_H 

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/fb.h>

#include <tslib.h>


typedef struct {
	int x[5], xfb[5];
	int y[5], yfb[5];
	int a[7];
} calibration;

static int sort_by_x(const void* a, const void *b)
{
	return (((struct ts_sample *)a)->x - ((struct ts_sample *)b)->x);
}

static int sort_by_y(const void* a, const void *b)
{
	return (((struct ts_sample *)a)->y - ((struct ts_sample *)b)->y);
}

void getxy(struct tsdev *ts, int *x, int *y)
{
#define MAX_SAMPLES 128
	struct ts_sample samp[MAX_SAMPLES];
	int index, middle;

	do {
		if (ts_read_raw(ts, &samp[0], 1) < 0) {
			perror("ts_read");
			exit(1);
		}
		
	} while (samp[0].pressure == 0);

	/* Now collect up to MAX_SAMPLES touches into the samp array. */
	index = 0;
	do {
		if (index < MAX_SAMPLES-1)
			index++;
		if (ts_read_raw(ts, &samp[index], 1) < 0) {
			perror("ts_read"); 
			exit(1);
		}
	} while (samp[index].pressure > 0);
	printf("Took %d samples...\n",index);

	/*
	 * At this point, we have samples in indices zero to (index-1)
	 * which means that we have (index) number of samples.  We want
	 * to calculate the median of the samples so that wild outliers
	 * don't skew the result.  First off, let's assume that arrays
	 * are one-based instead of zero-based.  If this were the case
	 * and index was odd, we would need sample number ((index+1)/2)
	 * of a sorted array; if index was even, we would need the
	 * average of sample number (index/2) and sample number
	 * ((index/2)+1).  To turn this into something useful for the
	 * real world, we just need to subtract one off of the sample
	 * numbers.  So for when index is odd, we need sample number
	 * (((index+1)/2)-1).  Due to integer division truncation, we
	 * can simplify this to just (index/2).  When index is even, we
	 * need the average of sample number ((index/2)-1) and sample
	 * number (index/2).  Calculate (index/2) now and we'll handle
	 * the even odd stuff after we sort.
	 */
	middle = index/2;
	if (x) {
		qsort(samp, index, sizeof(struct ts_sample), sort_by_x);
		if (index & 1)
			*x = samp[middle].x;
		else
			*x = (samp[middle-1].x + samp[middle].x) / 2;
	}
	if (y) {
		qsort(samp, index, sizeof(struct ts_sample), sort_by_y);
		if (index & 1)
			*y = samp[middle].y;
		else
			*y = (samp[middle-1].y + samp[middle].y) / 2;
	}
}


int perform_calibration(calibration *cal) {
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

	// Get sums for matrix
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) 
	{
		n += 1.0;
		x += (float)cal->x[j];
		y += (float)cal->y[j];
		x2 += (float)(cal->x[j]*cal->x[j]);
		y2 += (float)(cal->y[j]*cal->y[j]);
		xy += (float)(cal->x[j]*cal->y[j]);
	}

	// Get determinant of matrix -- check if determinant is too small
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
		printf("ts_calibrate: determinant is too small -- %f\n",det);
		return 0;
	}

	// Get elements of inverse matrix
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

	// Get sums for x calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->xfb[j];
		zx += (float)(cal->xfb[j]*cal->x[j]);
		zy += (float)(cal->xfb[j]*cal->y[j]);
	}

	// Now multiply out to get the calibration for framebuffer x coord
	cal->a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[2] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n",(a*z + b*zx + c*zy),
	(b*z + e*zx + f*zy),
	(c*z + f*zx + i*zy));

	// Get sums for y calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->yfb[j];
		zx += (float)(cal->yfb[j]*cal->x[j]);
		zy += (float)(cal->yfb[j]*cal->y[j]);
	}

	// Now multiply out to get the calibration for framebuffer y coord
	cal->a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[5] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n",(a*z + b*zx + c*zy),
	(b*z + e*zx + f*zy),
	(c*z + f*zx + i*zy));

	// If we got here, we're OK, so assign scaling to a[6] and return
	cal->a[6] = (int)scaling;
	return 1;
	/*	
// This code was here originally to just insert default values
	for(j=0;j<7;j++) {
		c->a[j]=0;
	}
	c->a[1] = c->a[5] = c->a[6] = 1;
	return 1;
*/

}


void put_cross(int x, int y, LCUI_Widget *cross)
{
	Move_Widget(cross, Pos(x - (cross->size.w)/2, y - (cross->size.h)/2));
}

#define NR_STEPS 10

void get_sample (struct tsdev *ts, calibration *cal,
int index, int x, int y, LCUI_Widget *cross)
{
	static int last_x = -1, last_y;

	if (last_x != -1) 
	{
		int dx = ((x - last_x) << 16) / NR_STEPS;
		int dy = ((y - last_y) << 16) / NR_STEPS;
		int i;
		last_x <<= 16;
		last_y <<= 16;
		for (i = 0; i < NR_STEPS; i++) 
		{
			put_cross (last_x >> 16, last_y >> 16, cross);
			usleep (1000);
			put_cross (last_x >> 16, last_y >> 16, cross);
			last_x += dx;
			last_y += dy;
		}
	}

	put_cross(x, y, cross);
	getxy (ts, &cal->x [index], &cal->y [index]);
	put_cross(x, y, cross);

	last_x = cal->xfb [index] = x;
	last_y = cal->yfb [index] = y;

	printf("[index]:X = %4d Y = %4d\n", cal->x [index], cal->y [index]);
}

int clearbuf(struct tsdev *ts)
{
	int fd = ts_fd(ts);
	fd_set fdset;
	struct timeval tv;
	int nfds;
	struct ts_sample sample;

	while (1) {
		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		nfds = select(fd + 1, &fdset, NULL, NULL, &tv);
		if (nfds == 0) break;

		if (ts_read_raw(ts, &sample, 1) < 0) {
			perror("ts_read");
			exit(1);
		}
	}
	return 0;
}


/* 校正后生成的校正文件 */
#define TS_POINTERCAL "/mnt/Data/LC-SOFT/pointercal"


void *calibrate_func(void *widget)
{
	struct tsdev *ts = LCUI_Sys.ts.td;
	if(ts != NULL)
	{
		calibration cal;
		int cal_fd;
		int width, height;
		char cal_buffer[256];
		char *calfile = NULL;
		unsigned int i, len;
		LCUI_Widget *cross = (LCUI_Widget *)widget;
		width = Get_Screen_Width();
		height = Get_Screen_Height();
		// Clear the buffer
		clearbuf(ts);

		get_sample (ts, &cal, 0, 50,        50,        cross);
		clearbuf(ts);
		get_sample (ts, &cal, 1, width - 50, 50,        cross);
		clearbuf(ts);
		get_sample (ts, &cal, 2, width - 50, height - 50, cross);
		clearbuf(ts);
		get_sample (ts, &cal, 3, 50,        height - 50, cross);
		clearbuf(ts);
		get_sample (ts, &cal, 4, width / 2,   height / 2,  cross);

		if (perform_calibration (&cal)) 
		{
			printf ("Calibration constants: ");
			for (i = 0; i < 7; i++) printf("%d ", cal.a [i]);
			printf("\n");
			if ((calfile = getenv("TSLIB_CALIBFILE")) != NULL) {
				cal_fd = open (calfile, O_CREAT | O_RDWR,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			} else {
				cal_fd = open (TS_POINTERCAL, O_CREAT | O_RDWR,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			}
			len = sprintf(cal_buffer,"%d %d %d %d %d %d %d %d %d",
			cal.a[1], cal.a[2], cal.a[0],
			cal.a[4], cal.a[5], cal.a[3], cal.a[6],
			width, height);
			i = write (cal_fd, cal_buffer, len);
			close (cal_fd); 
		} else {
			printf("Calibration failed.\n"); 
		}
	}
	else
		printf("error: your device can not support touch screen!\n");
	Main_Loop_Quit();
	LCUI_Thread_Exit(NULL);
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
	pthread_t		thread;
	LCUI_Widget	*window, *label, *click_pic; /* 使用指向widget部件的指针 */
	LCUI_Graph		bg, pic_cross;/* 存储按钮各种状态要显示的图形 */
	int				width, height;  
	char			my_path[1024], file_path[1024]; /* 存储字符串 */
	/**/
	/* 自定义默认字体文件位置 */
	//Set_Default_Font("/mnt/Data/LC-SOFT/fonts/微软雅黑.ttf");
	Set_Default_Font("../fonts/msyh.ttf");
	/* 初始化LCUI */
	LCUI_Init(argc, argv); 
	/* 初始化图片数据结构体 */
	Graph_Init(&pic_cross); 
	Graph_Init(&bg); 
	
	/* 获取文件的路径，之后打开并载入图片 */
	Get_Path(argv[0], my_path);
	sprintf(file_path, "%scross.png", my_path);
	Load_Image(file_path, &pic_cross);
	sprintf(file_path, "%sbg.png", my_path);
	Load_Image(file_path, &bg);
	
	width  = Get_Screen_Width(); /* 获取屏幕的宽度 */
	height = Get_Screen_Height(); /* 获取屏幕的高度 */ 
	
	/* 创建一个窗口部件 */
	window = Create_Widget("window"); 
	/* 窗口部件边框风格为无(NONE) */
	Set_Widget_Border_Style(window, BORDER_STYLE_NONE);
	/* 部件背景图为bg，填充模式为拉伸 */
	Set_Widget_Background_Image(window, &bg, FILL_MODE_STRETCH);
	/* 改变部件尺寸 */
	Resize_Widget(window, Size(width, height));
	
	label       = Create_Widget("label"); /* 该部件用于显示文字 */ 
	click_pic   = Create_Widget("picture_box"); /* 该部件用于显示图像 */
	
	/* 设定label部件显示的文本内容 */
	Set_Label_Text(label, "点击圆圈中心，笔点校正");
	/* 调整部件的大小 */
	Resize_Widget(click_pic, Size(pic_cross.width, pic_cross.height));
	/* 设定部件中显示的图形 */
	Set_PictureBox_Image_From_Graph(click_pic, &pic_cross);
	/* 设定部件对齐方式以及偏移距离 */
	Set_Widget_Align(label, ALIGN_MIDDLE_CENTER, Pos(0, label->size.h + 1)); 
	
	/* 将返回键与Main_Loop_Quit函数关联，当返回键被按下后，程序退出主循环 */
	LCUI_Key_Event_Connect(KEY_ESC, Main_Loop_Quit, NULL);
	
	/* 将这两个部件放入窗口客户区内 */
	Window_Client_Area_Add(window, label);
	Window_Client_Area_Add(window, click_pic);
	
	/* 显示部件以及窗口 */
	Show_Widget(label); 
	Show_Widget(click_pic);
	Show_Widget(window);
	/* 创建线程 */
	LCUI_Thread_Create(&thread, NULL, calibrate_func, (void*)click_pic);
	return LCUI_Main(); /* 进入LCUI的主循环 */
}
