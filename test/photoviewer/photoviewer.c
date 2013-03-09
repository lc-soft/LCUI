#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_DISPLAY_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_FONT_H
#include LC_RES_H
#include LC_INPUT_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_ACTIVEBOX_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <math.h>

#define IMG_PATH_WND_ICON	"drawable/icon.png"
#define IMG_PATH_LOADING	"drawable/publish_loading_%02d.png"
#define IMG_PATH_FAILD_LOAD	"drawable/pic_fail.png"
#define IMG_PATH_DONE_LOAD	"drawable/pic_ok.png"
#define IMG_PATH_BTN_ZOOM_PIC0 "drawable/btn_zoom_up_normal.png"
#define IMG_PATH_BTN_ZOOM_PIC1 "drawable/btn_zoom_up_pressed.png"
#define IMG_PATH_BTN_ZOOM_PIC2 "drawable/btn_zoom_up_disabled.png"
#define IMG_PATH_BTN_ZOOM_PIC3 "drawable/btn_zoom_down_normal.png"
#define IMG_PATH_BTN_ZOOM_PIC4 "drawable/btn_zoom_down_pressed.png"
#define IMG_PATH_BTN_ZOOM_PIC5 "drawable/btn_zoom_down_disabled.png"
#define IMG_PATH_BTN_SWITCH_PIC0 "drawable/btn_next_up_normal.png"
#define IMG_PATH_BTN_SWITCH_PIC1 "drawable/btn_next_up_pressed.png"
#define IMG_PATH_BTN_SWITCH_PIC2 "drawable/btn_next_up_disabled.png"
#define IMG_PATH_BTN_SWITCH_PIC3 "drawable/btn_prev_down_normal.png"
#define IMG_PATH_BTN_SWITCH_PIC4 "drawable/btn_prev_down_pressed.png"
#define IMG_PATH_BTN_SWITCH_PIC5 "drawable/btn_prev_down_disabled.png"

#define PATH_FONTFILE_MSYH	"../../fonts/msyh.ttf"
#define TEXT_WND_TITLE		"照片查看器 v0.6"
#define TEXT_FAID_LOAD		"图片载入失败!"
#define TEXT_LOADING		"正在载入..."
#define TEXT_DONE_LOAD		"图片载入成功!"

#define TOTAL_SLEEP_TIME	2000

static char 
name[256], **filename = NULL;

static char
usage_text[] = {
	"<size=15px><color=30,145,220>命令行用法：</color></size>\n"
	"<color=30,220,60>%s</color>  文件路径\n\n"
	"<size=15px><color=30,145,220>按键控制方法：</color></size>\n"
	"<color=255,0,0>方向键：</color> 移动浏览区域\n"
	"<color=255,0,0>返回键：</color> 退出程序\n"
	"<color=255,0,0>+/-键：</color>  放大和缩小图像\n" 
	"<color=255,0,0>N/B键：</color> 上下切换图像\n" 
	"<color=255,0,0> I 键：</color>  显示图像信息\n"
	"你也可以使用鼠标点击界面上的按钮进行控制。\n"
	"本程序目前支持浏览bmp, png, jpeg格式的图片。\n"	
};

static LCUI_Widget
*window, *help_text, *tip_pic, *tip_box, *tip_text, *tip_icon,
*btn_zoom[2], *btn_switch[2], *container[2], *image_box, 
*image_info_box, *image_info_text;

static LCUI_Graph
imagebox_bg, wnd_icon, pic_loading[8], btn_zoom_pic[6], btn_switch_pic[6];

static int
total_imgfile_num = 0, current_file_num = 0, need_refresh = FALSE;

static LCUI_Thread
thread_viewer;

static float 
mini_scale = 1.0, scale = 1.0;

static int 
sleep_time[2] = {TOTAL_SLEEP_TIME,TOTAL_SLEEP_TIME}, timer_id[3] = {-1,-1,-1};

static uchar_t 
tipbox_alpha = 200, infobox_alpha = 200, btn_alpha = 255;

/*------------------------ 部件的隐藏处理 -------------------------------*/
static void 
hide_tip_box(void)
/* 隐藏提示框，但不是瞬间隐藏，带 淡出 效果 */
{
	if( timer_id[0] == -1 ) {
		/* 创建定时器，每隔50毫秒调用一次此函数 */
		timer_id[0] = set_timer( 50, hide_tip_box, TRUE );
	} else {
		/* 已经创建过定时器，如果alpha为200，说明之前已经暂停了定时器 */
		if( tipbox_alpha == 200 ) {
			/* 让定时器继续 */
			usleep(500000);
			continue_timer( timer_id[0] );
		}
		/* 当alpha接近0时，直接隐藏提示框，并暂停定时器 */
		if( tipbox_alpha <= 15 ) {
			Widget_Hide( tip_box );
			tipbox_alpha = 200;
			pause_timer( timer_id[0] );
		} else {
			/* 设置透明度 */
			Widget_SetAlpha( tip_box, tipbox_alpha );
			tipbox_alpha -= 15;
		}
	}
}

static void 
show_tip_box(void)
/* 显示提示框 */
{
	tipbox_alpha = 200;
	Widget_SetAlpha( tip_box, tipbox_alpha );
	Widget_Show( tip_box );
	if( timer_id[0] != -1 ) {
		pause_timer( timer_id[0] );
	}
}

static void 
hide_image_info_box(void)
{
	if( sleep_time[0] >= 50 ) {
		sleep_time[0] -= 50;
		return;
	}
	if( infobox_alpha == 0 );
	else if( infobox_alpha <= 15 ) {
		Widget_Hide( image_info_box );
		infobox_alpha = 0;
	} else {
		Widget_SetAlpha( image_info_box, infobox_alpha );
		infobox_alpha -= 15;
	}
}

static void 
show_image_info_box(void)
{
	sleep_time[0] = TOTAL_SLEEP_TIME;
	infobox_alpha = 200;
	Widget_SetAlpha( image_info_box, infobox_alpha );
	Widget_Show( image_info_box );
	/* 如果没有设置定时器，那么就设置定时器 */
	if( timer_id[1] == -1 ) {
		timer_id[1] = set_timer( 50, hide_image_info_box, TRUE );
	} 
}

static void 
hide_btn_area(void)
{
	if( sleep_time[1] >= 50 ) {
		sleep_time[1] -= 50;
		return;
	}
	if( btn_alpha == 0 );
	else if( btn_alpha <= 20 ) {
		Widget_Hide( container[0] );
		Widget_Hide( container[1] );
		btn_alpha = 0;
	} else {
		Widget_SetAlpha( container[0], btn_alpha );
		Widget_SetAlpha( container[1], btn_alpha );
		btn_alpha -= 20;
	}
}

static void 
show_btn_area( void )
{
	sleep_time[1] = TOTAL_SLEEP_TIME;
	btn_alpha = 255;
	Widget_SetAlpha( container[0], btn_alpha );
	Widget_SetAlpha( container[1], btn_alpha );
	Widget_Show( container[0] );
	Widget_Show( container[1] );
	if( timer_id[2] == -1 ) {
		timer_id[2] = set_timer( 50, hide_btn_area, TRUE );
	} 
}

static void
__show_btn_area( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	show_btn_area();
}
/*--------------------------------------------------------------------*/


/*---------------------------- 文件路径处理 ----------------------------*/
static void 
get_filename(char *filepath, char *out_name)
/* 根据文件路径，获取文件名 */
{
	int m,n = 0;
	char *p;
	for(m=0; m<strlen(filepath); ++m) {
		if(filepath[m]=='/')  
			n = m+1; 
	}
	p = filepath + n;
	strcpy(out_name, p);
}

int get_format(char *format, char *filename)
/* 获取文件格式 */
{
	int i, j, len;
	len = strlen(filename);
	for(i=len-1; i>=0; --i) {
		if(filename[i] == '.') {
			for(j=0, ++i; i<len; ++i, ++j) {
				format[j] = filename[i];
			} 
			format[j] = 0;
			return 0;
		}
	}
	strcpy(format, "");
	return -1;
}

static char **
scan_imgfile( char *dir, int *file_num )
/* 扫描图片文件，并获得文件列表 */
{
	int i, len, n; 
	char **filelist, format[256], path[1024];
	struct dirent **namelist;
	
	if(strlen(dir)>0 && dir[strlen(dir)-2] != '/') {
		strcat(dir, "/");
	}
	if(strlen(dir) == 0) {
		n = scandir(".", &namelist, 0, alphasort);
	} else {
		n = scandir(dir, &namelist, 0, alphasort);
	}
	if (n < 0) {
		return 0; 
	}
	
	filelist = (char **)malloc(sizeof(char *)*n);
	for(i=0, *file_num=0; i<n; i++) {
		/* 如果不是文件 */ 
		if( namelist[i]->d_type !=8 ) {
			continue;
		}
		/* 获取文件格式 */
		get_format(format, namelist[i]->d_name);
		if(strlen(format) > 2 && 
		(strcasecmp(format, "png") == 0
		|| strcasecmp(format, "bmp") == 0
		|| strcasecmp(format, "jpg") == 0
		|| strcasecmp(format, "jpeg") == 0)) {
			sprintf(path, "%s%s", dir, namelist[i]->d_name);
			len = strlen( path );
			filelist[*file_num] = (char *)malloc(sizeof(char)*(len+1)); 
			strcpy(filelist[*file_num], path);  
			++*file_num;
		}
	}
	if(*file_num > 0) {
		filelist = (char**)realloc(filelist, *file_num*sizeof(char*));
	} else {
		free(filelist);
		filelist = NULL;
	} 
	for(i=0;i<n;++i) {
		free(namelist[i]); 
	}
	free(namelist); 
	
	return filelist;
}

void get_filepath(char *filepath, char *out_path)
/* 用于获取程序所在的文件目录 */
{
	int num;
	strcpy(out_path, filepath);
	for(num = strlen(filepath) - 1; num >= 0; --num) {
		if(filepath[num] == '/')  {
			out_path[num+1] = 0;
			return;
		}
	}
	memset(out_path, 0, sizeof(char)*strlen(out_path)); 
}

static int
generate_imgfilelist( char *filepath, char *imgfile )
/* 生成文件列表 */
{
	DIR *dirptr = NULL; 
	char dirpath[1024];
	
	/* 判断这个文件路径是文件还是目录 */ 
	dirptr = opendir( filepath );
	if( !dirptr ) {
		/* 获取文件所在目录 */
		get_filepath( filepath, dirpath );
		/* 扫描该文件目录中的其它图片文件 */
		filename = scan_imgfile( dirpath, &total_imgfile_num );
		strcpy ( imgfile, filepath );
	} else {
		closedir(dirptr);
		/* 扫描图片文件 */
		filename = scan_imgfile( filepath, &total_imgfile_num );
		/* 判断文件数是否有效 */
		if( total_imgfile_num > 0 ) {
			/* 默认打开第一个图片 */
			strcpy ( imgfile, filename[0] );
		} else {
			return -1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------*/

int load_imagefile(void)
/* 载入图片文件 */
{ 
	int i; 
	LCUI_Size size;
	LCUI_Graph *image;
	char file[1024], info[512];
	
	Label_Text( tip_text, TEXT_LOADING );
	Widget_Hide( tip_icon );
	Widget_Show( tip_pic );
	show_tip_box();
	
	/* 播放动画 */
	ActiveBox_Play( tip_pic );
	
	if( current_file_num < total_imgfile_num ) {
		strcpy( file, filename[current_file_num] );
	} else {
		return -1;
	}
	get_filename(file, name);
	image = PictureBox_GetImage( image_box ); 
	i = PictureBox_SetImageFile( image_box, file ); 
	if(i != 0) {/* 如果图片文件读取失败 */
		Label_Text( tip_text, TEXT_FAID_LOAD );
		PictureBox_SetImageFile( tip_icon, IMG_PATH_FAILD_LOAD );
		Widget_Show( tip_icon );
		Widget_Hide( tip_pic );
		/* 暂停动画播放 */
		ActiveBox_Pause( tip_pic );
		size.w = size.h = 0;
	} else {
		Label_Text( tip_text, TEXT_DONE_LOAD );
		PictureBox_SetImageFile( tip_icon, IMG_PATH_DONE_LOAD );
		Widget_Show( tip_icon );
		Widget_Hide( tip_pic );
		ActiveBox_Pause( tip_pic );
		
		image = PictureBox_GetImage(image_box); 
		size = Graph_GetSize(image); 
		/* 
		 * 不能使用Widget_GetSize()函数获取image_box，因为
		 * 该函数获取的尺寸可能不准确 
		 * */ 
		if( 1 == Size_Cmp(size, _Widget_GetSize(image_box) )) {
		/* 如果图片尺寸大于image_box的尺寸，就改变image_box的图像处理模式 */
			PictureBox_SetSizeMode(image_box, SIZE_MODE_BLOCK_ZOOM);
			mini_scale = PictureBox_GetScale(image_box);
		} else {
			PictureBox_SetSizeMode(image_box, SIZE_MODE_CENTER);
			mini_scale = 0.25; 
		}
	}
	
	scale = PictureBox_GetScale( image_box ); 
	sprintf( info,  
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current_file_num+1, total_imgfile_num, name,
		size.w, size.h, 100*scale );
		
	Label_Text( image_info_text, info );
	show_image_info_box();
	return i;
}

static void viewer( void *unused )
/* 显示已载入的图片 */
{
	int result;
	LCUI_Graph *image;
	
	while( LCUI_Active() ) {
		/* 等待需要刷新 */ 
		while( !need_refresh ) {
			usleep(10000);
		}
		need_refresh = FALSE;
		/* 载入图片 */
		result = load_imagefile();
		if(result == 0) {/* 如果正常打开了图片 */ 
			scale = PictureBox_GetScale( image_box ); /* 获取缩放比例 */
			image = PictureBox_GetImage( image_box ); /* 获取图像指针 */
			if( Graph_IsPNG(image) && Graph_HaveAlpha(image) ) {
				/* 平铺背景图 */
				Widget_SetBackgroundImage( image_box, &imagebox_bg );
				Widget_SetBackgroundLayout( image_box, LAYOUT_TILE );
			} else {
				Widget_SetBackgroundImage( image_box, NULL );
				Widget_SetBackgroundLayout( image_box, LAYOUT_NONE );
			}
			hide_tip_box();
		}
		/* 如果接近最小缩放比例，那就禁用“缩小”按钮，否则，启用 */
		if(fabs(mini_scale - scale) < 0.01) {
			Widget_Disable( btn_zoom[0] ); 
		} else {
			Widget_Enable( btn_zoom[0] );
		}
		/* 启用放大按钮 */
		Widget_Enable( btn_zoom[1] );
	}
	LCUIThread_Exit(NULL);
}

static void 
open_imgfile(char *file)
/* 打开图片文件 */
{
	int i;
	need_refresh = TRUE;
	
	for(i=0; i<total_imgfile_num; ++i) {
		if(strcmp(file, filename[i]) == 0) { 
			current_file_num = i;
			break;
		}
	}
}

static void 
move_view_area(LCUI_Widget *widget, LCUI_WidgetEvent *event)
/* 移动图像浏览区域 */
{ 
	LCUI_Pos pos; 
	static LCUI_Pos old_pos, offset_pos;
	/* 如果是刚被点击 */ 
	if(event->drag.first_click) {
		/* 记录被点击时的位置，以后的移动，就以该位置为基础计算出新位置 */
		old_pos = PictureBox_GetViewAreaPos(widget);
		/* 浏览区域位置与图片框位置的偏移距离 */
		offset_pos = Pos_Sub(Widget_GetGlobalPos(widget), old_pos); 
	}
	else if(event->drag.end_click);
	else {/* 否则，那就是正被拖动 */ 
		/* 获取浏览区域的新位置 */
		pos = Pos_Sub(event->drag.new_pos, offset_pos);
		/* 得出新位置与老位置的“差” */
		pos = Pos_Sub(old_pos, pos);
		/* 由于浏览区域是向鼠标移动方向相反的方向移动，因此，新位置实际是老位置与“差”的和 */
		pos = Pos_Add(old_pos, pos); 
		PictureBox_MoveViewArea(widget, pos);
	}
}

void image_zoom_in( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
/* 放大图像 */
{
	LCUI_Size size;
	LCUI_Graph *image; 
	char info[512];
	
	scale += 0.25; /* 缩放比例自增25% */
	if(scale >= 2) {
		Widget_Disable(btn_zoom[1]); /* 禁用“放大”按钮 */
		scale = 2.0; /* 最大不能超过200% */
	}
	/* 为了能达到100%，加了个判断，如果缩放比例接近100%，那就直接等于100% */
	if(fabs(scale - 1.0) < 0.2 && scale != mini_scale) {
		scale = 1; 
	}
	/* 按比例缩放浏览区域 */
	PictureBox_ZoomViewArea( image_box, scale );
	scale = PictureBox_GetScale( image_box );/* 获取缩放比例 */
	image = PictureBox_GetImage( image_box ); /* 获取图像指针 */
	size = Graph_GetSize( image );
	sprintf( info, 
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current_file_num+1, total_imgfile_num, name,
		size.w, size.h, 100*scale );
	Label_Text( image_info_text, info );
	/* 启用“缩小”按钮 */
	Widget_Enable( btn_zoom[0] );
	/* 显示图片信息框 */
	show_image_info_box();
	if( widget ) {
		show_btn_area();
	}
}

static void 
image_zoom_out( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
/* 缩小图像 */
{
	LCUI_Size size;
	LCUI_Graph *image;
	char info[512];
	
	scale -= 0.25; /* 缩放比例自减25% */
	/* 最小不能低于mini_scale中表示的比例 */
	if(scale <= mini_scale) {
		scale = mini_scale;
		Widget_Disable(btn_zoom[0]);
	}
	if(fabs(scale - 1.0) < 0.2 && scale != mini_scale) {
		scale = 1; 
	}
	if(scale > 0.05 && scale < mini_scale + 0.05) {
		scale = mini_scale;
	}
	/* 按比例缩放浏览区域 */
	PictureBox_ZoomViewArea(image_box, scale);
	scale = PictureBox_GetScale(image_box);
	image = PictureBox_GetImage(image_box); /* 获取图像指针 */
	size = Graph_GetSize(image);
	sprintf( info, 
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current_file_num+1, total_imgfile_num, name,
		size.w, size.h, 100*scale );
		 
	Label_Text( image_info_text, info );
	if(fabs(mini_scale - scale) < 0.01) {
		Widget_Disable( btn_zoom[0] ); 
	} else {
		Widget_Enable( btn_zoom[0] );
	}
	Widget_Enable( btn_zoom[1] ); 
	show_image_info_box();
	if( widget ) {
		show_btn_area();
	}
}

static void
prev_image( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
/* 切换至上一张图 */
{ 
	if( widget ) {
		show_btn_area();
	}
	if(total_imgfile_num <= 0) {
		return;
	}
	
	if(current_file_num <= 0) {
		current_file_num = total_imgfile_num-1;
	} else {
		--current_file_num;
	}
	need_refresh = TRUE;	/* 标记图片框内的图片需要刷新 */
}

static void 
next_image( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
/* 切换至下一张图 */
{
	/* 如果部件指针有效，说明是点击按钮部件后调用此函数，那么需要延长等待时间 */
	if( widget ) {
		show_btn_area();
	}
	if(total_imgfile_num <= 0) {
		return;
	}
		
	if(current_file_num == total_imgfile_num-1) {
		current_file_num = 0;
	} else {
		++current_file_num;
	}
	need_refresh = TRUE; 
}


//如果是用按钮控制浏览区域，就使用下面的四个函数
void left_move_area(void)
{
	LCUI_Rect rect;
	rect = PictureBox_GetViewArea(image_box);
	rect.x -= rect.width/2.0;
	if(rect.x < 0) rect.x = 0;
	PictureBox_MoveViewArea(image_box, Pos(rect.x, rect.y));
}

void right_move_area(void)
{ 
	LCUI_Rect rect;
	rect = PictureBox_GetViewArea(image_box);
	rect.x += rect.width/2.0; 
	PictureBox_MoveViewArea(image_box, Pos(rect.x, rect.y));
}

void up_move_area(void)
{

	LCUI_Rect rect;
	rect = PictureBox_GetViewArea(image_box);
	rect.y -= rect.height/2.0;
	if(rect.y < 0) rect.y = 0;
	PictureBox_MoveViewArea(image_box, Pos(rect.x, rect.y));
}

void down_move_area(void)
{ 
	LCUI_Rect rect;
	rect = PictureBox_GetViewArea(image_box);
	rect.y += rect.height/2.0; 
	PictureBox_MoveViewArea(image_box, Pos(rect.x, rect.y));
}

static void 
key_proc( LCUI_Widget *widget, LCUI_WidgetEvent *event )
/* 响应按键输入，并进行相应操作 */
{
	switch( event->key.key_code ) {
	    case '+':
	    case '=':
	    case KEY_AA:
		image_zoom_in( NULL, NULL );
		break;
	    case '-':
	    case '_':
	    case KEY_BB:
		image_zoom_out( NULL, NULL );
		break;
	    case LCUIKEY_n:
	    case LCUIKEY_N:
		next_image( NULL, NULL );
		break;
	    case LCUIKEY_b:
	    case LCUIKEY_B:
		prev_image( NULL, NULL );
		break;
	    case LCUIKEY_i:
	    case LCUIKEY_I:
		show_image_info_box();
		break;
	    case LCUIKEY_LEFT: left_move_area();break;
	    case LCUIKEY_RIGHT: right_move_area(); break;
	    case LCUIKEY_UP: up_move_area(); break;
	    case LCUIKEY_DOWN: down_move_area(); break;   
	}
}

static void
enter_viewer_mode( void )
{
	Widget_Show( image_box );
	Widget_Show( container[0] );
	Widget_Show( container[1] );
	Widget_Show( tip_pic );
	Widget_Show( tip_text );
	Widget_Show( image_info_text );
	show_btn_area();
	/* 关联部件的拖动事件 */
	Widget_Event_Connect( image_box, EVENT_DRAG, move_view_area );
	/* 关联按钮的点击事件 */
	Widget_Event_Connect( btn_switch[0], EVENT_CLICKED, next_image );
	Widget_Event_Connect( btn_switch[1], EVENT_CLICKED, prev_image );
	Widget_Event_Connect( btn_zoom[0], EVENT_CLICKED, image_zoom_out );
	Widget_Event_Connect( btn_zoom[1], EVENT_CLICKED, image_zoom_in );
	Widget_Event_Connect( image_box, EVENT_CLICKED, __show_btn_area );
	Widget_Event_Connect( window, EVENT_KEYBOARD, key_proc );
	
	LCUIThread_Create( &thread_viewer, viewer, NULL );
}

static void 
widgets_structure( void )
/* 构造图形界面中的部件 */
{
	window = Widget_New( "window" );
	/* 创建两个容器 */
	container[0] = Widget_New(NULL);
	container[1] = Widget_New(NULL);
	/* 以下两个按钮用于放大缩小 */
	btn_zoom[0] = Widget_New("button"); /* “缩小”按钮 */
	btn_zoom[1] = Widget_New("button"); /* “放大”按钮 */
	/* 以下两个按钮用于切换图片 */
	btn_switch[0] = Widget_New("button"); /* “上一张”按钮 */
	btn_switch[1] = Widget_New("button"); /* “下一张”按钮 */
	
	tip_pic = Widget_New( "active_box" ); /* 动画框 */
	tip_text = Widget_New("label"); /* 提示文本 */
	tip_box = Widget_New(NULL); /* 作为提示文本的容器 */ 
	image_box = Widget_New("picture_box");
	
	tip_icon = Widget_New("picture_box"); 
	image_info_box = Widget_New(NULL);
	image_info_text = Widget_New("label");
}

static void
destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

static void 
widgets_configure( void )
/* 配置图形界面中的部件的属性 */
{
	int i;
	LCUI_TextStyle tip_style;
	LCUI_Size screen_size, wnd_size;
	LCUI_Size size[2], btn_switch_size[2], btn_zoom_size[2];

	/* 将部件加入相应的容器中 */
	Widget_Container_Add( container[0], btn_zoom[0] );
	Widget_Container_Add( container[0], btn_zoom[1] );
	Widget_Container_Add( container[1], btn_switch[0] );
	Widget_Container_Add( container[1], btn_switch[1] );
	Widget_Container_Add( tip_box, tip_text );
	Widget_Container_Add( tip_box, tip_icon );
	Widget_Container_Add( tip_box, tip_pic );
	Widget_Container_Add( image_info_box, image_info_text );
	Window_ClientArea_Add( window, container[0] ); 
	Window_ClientArea_Add( window, container[1] );
	Window_ClientArea_Add( window, image_box );
	Window_ClientArea_Add( window, tip_box );
	Window_ClientArea_Add( window, image_info_box );
	
	/* 如果屏幕分辨率小于640x480，那就使用320x240分辨率 */
	screen_size = LCUIScreen_GetSize();
	if( screen_size.w < 640 && screen_size.h < 480 ){
		wnd_size = Size(320, 240);
	} else {
		wnd_size = Size(640, 480);
	}
	Widget_Resize( window, wnd_size );
	Window_SetTitleText( window, TEXT_WND_TITLE );
	Window_SetTitleIcon( window, &wnd_icon );
	
	for(i=0; i<8; ++i) {
		ActiveBox_AddFrame( tip_pic, &pic_loading[i], Pos(0,0), 50 );
	}
	Widget_Resize( tip_pic, Size(32, 24) );
	Widget_Resize( tip_box, Size(150, 34) );
	ActiveBox_SetFramesSize( tip_pic, Size(26, 26) );
	/* 设定按钮图形样式 */
	Button_CustomStyle(	btn_zoom[0], &btn_zoom_pic[3], &btn_zoom_pic[3], 
				&btn_zoom_pic[4], NULL, &btn_zoom_pic[5] );
	Button_CustomStyle(	btn_zoom[1], &btn_zoom_pic[0], &btn_zoom_pic[0], 
				&btn_zoom_pic[1], NULL, &btn_zoom_pic[2] );
	
	Button_CustomStyle(	btn_switch[0], &btn_switch_pic[0], &btn_switch_pic[0], 
				&btn_switch_pic[1], NULL, &btn_switch_pic[2] );
	Button_CustomStyle(	btn_switch[1], &btn_switch_pic[3], &btn_switch_pic[3], 
				&btn_switch_pic[4], NULL, &btn_switch_pic[5] );

	btn_zoom_size[0] = Graph_GetSize(&btn_zoom_pic[0]);
	btn_zoom_size[1] = Graph_GetSize(&btn_zoom_pic[0]);
	btn_switch_size[0] = Graph_GetSize(&btn_switch_pic[0]);
	btn_switch_size[1] = Graph_GetSize(&btn_switch_pic[1]);
	size[0] = Size(	btn_zoom_size[0].w, 
			btn_zoom_size[0].h + btn_zoom_size[1].h );
	size[1] = Size( btn_switch_size[0].w+btn_switch_size[1].w,
			btn_switch_size[0].h );
	
	Widget_Resize( btn_zoom[0], btn_zoom_size[0] );
	Widget_Resize( btn_zoom[1], btn_zoom_size[1] );
	Widget_Resize( btn_switch[0], btn_switch_size[0] );
	Widget_Resize( btn_switch[1], btn_switch_size[1] );
	Widget_Resize( container[0], size[0] );
	Widget_Resize( container[1], size[1] );
	Widget_Resize(tip_icon, Size(32, 24));
	
	/* 设置边框及背景 */
	Widget_SetBorder( image_info_box, Border(1,BORDER_STYLE_SOLID, RGB(50,50,50)) );
	Widget_SetBorder( tip_box, Border(1,BORDER_STYLE_SOLID, RGB(200,200,200)) );
	Widget_SetBackgroundColor( image_info_box, RGB(180,215,255) );
	Widget_SetBackgroundColor( tip_box, RGB(20,20,20) );
	Widget_SetBackgroundTransparent( image_info_box, FALSE );
	Widget_SetBackgroundTransparent( tip_box, FALSE );
	
	TextStyle_Init( &tip_style );
	TextStyle_FontColor( &tip_style, RGB(230, 230, 230) );
	Label_TextStyle( tip_text, tip_style );
	
	Widget_SetAlpha( tip_box, 200 );
	/* 设定部件的布局 */
	Widget_SetAlign( btn_zoom[0], ALIGN_BOTTOM_CENTER, Pos(0, 0) );
	Widget_SetAlign( btn_zoom[1], ALIGN_TOP_CENTER, Pos(0, 0) );
	
	Widget_SetAlign( btn_switch[0], ALIGN_MIDDLE_RIGHT, Pos(0, 0) );
	Widget_SetAlign( btn_switch[1], ALIGN_MIDDLE_LEFT, Pos(0, 0) );
	
	Widget_SetAlign( container[0], ALIGN_MIDDLE_RIGHT, Pos(-2, 0) );
	Widget_SetAlign( container[1], ALIGN_BOTTOM_LEFT, Pos(2, -2) );
	
	Widget_SetAlign( tip_box, ALIGN_BOTTOM_RIGHT, Pos(-1, -1) );
	Widget_SetAlign( tip_text, ALIGN_MIDDLE_LEFT, Pos(35, 0) );
	Widget_SetAlign( tip_pic, ALIGN_MIDDLE_LEFT, Pos(2, 0) );
	Widget_SetAlign( tip_icon, ALIGN_MIDDLE_LEFT, Pos(2, 0) );
	
	Widget_SetAlign(image_info_text, ALIGN_MIDDLE_LEFT, Pos(5, 0));
	Widget_SetAlign(image_info_box, ALIGN_TOP_LEFT, Pos(3, 3));
	
	Widget_SetDock( image_box, DOCK_TYPE_FILL );
	
	Widget_SetAutoSize( image_info_box, TRUE, 0 );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
}

static void
free_graphic_resource( void )
/* 释放图形资源 */
{
	Graph_Free( &btn_zoom_pic[0] );
	Graph_Free( &btn_zoom_pic[1] );
	Graph_Free( &btn_zoom_pic[2] );
	Graph_Free( &btn_zoom_pic[3] );
	Graph_Free( &btn_zoom_pic[4] );
	Graph_Free( &btn_zoom_pic[5] );
	Graph_Free( &btn_switch_pic[0] );
	Graph_Free( &btn_switch_pic[1] );
	Graph_Free( &btn_switch_pic[2] );
	Graph_Free( &btn_switch_pic[3] );
	Graph_Free( &btn_switch_pic[4] );
	Graph_Free( &btn_switch_pic[5] );
	Graph_Free( &imagebox_bg );
}

static void
load_graphic_resource( void )
/* 根据程序所在目录，载入图形资源 */
{
	int i;
	char path[1024];
	
	Graph_Init( &wnd_icon );
	/* 载入图标 */
	Load_Image( IMG_PATH_WND_ICON, &wnd_icon );
	/* 为动画添加8帧图像，供ActiveBox部件播放 */
	for(i=0; i<8; ++i) {
		snprintf( path, sizeof(path), IMG_PATH_LOADING, i+1 );
		Load_Image( path, &pic_loading[i] );
	}
	
	/* 载入按钮在各个状态下显示的图形 */ 
	Load_Image( IMG_PATH_BTN_ZOOM_PIC0, &btn_zoom_pic[0] );
	Load_Image( IMG_PATH_BTN_ZOOM_PIC1, &btn_zoom_pic[1] );
	Load_Image( IMG_PATH_BTN_ZOOM_PIC2, &btn_zoom_pic[2] );
	Load_Image( IMG_PATH_BTN_ZOOM_PIC3, &btn_zoom_pic[3] );
	Load_Image( IMG_PATH_BTN_ZOOM_PIC4, &btn_zoom_pic[4] );
	Load_Image( IMG_PATH_BTN_ZOOM_PIC5, &btn_zoom_pic[5] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC0, &btn_switch_pic[0] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC1, &btn_switch_pic[1] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC2, &btn_switch_pic[2] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC3, &btn_switch_pic[3] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC4, &btn_switch_pic[4] );
	Load_Image( IMG_PATH_BTN_SWITCH_PIC5, &btn_switch_pic[5] );
	/* 载入马赛克图形 */
	Load_Graph_Mosaics( &imagebox_bg );
}

static void
Show_GUI( void )
/* 显示图形界面 */
{
	Widget_Show( window );
	Widget_Show( btn_zoom[0] );
	Widget_Show( btn_zoom[1] );
	Widget_Show( btn_switch[0] );
	Widget_Show( btn_switch[1] );
}

static void 
Create_GUI( void )
/* 创建图形界面 */
{
	load_graphic_resource();
	/* 在LCUI退出时调用free_graphic_resource函数，以释放图形资源 */
	LCUIApp_AtQuit( free_graphic_resource );
	widgets_structure();
	widgets_configure();
}

static void 
show_usage( char *app_path )
/* 显示用法 */
{
	char myname[256];
	char real_usage_text[1024];
	get_filename( app_path, myname );
	help_text = Widget_New( "label" );
	sprintf( real_usage_text, usage_text, myname );
	Label_Text(help_text, real_usage_text );
	Widget_SetAlign( help_text, ALIGN_MIDDLE_CENTER, Pos(10, 10) );
	Window_ClientArea_Add( window, help_text );
	Widget_Show( help_text );
}

int main( int argc, char *argv[] )
{
	int ret;
	char imgfilepath[1024];
	/* 添加环境变量，设置字体文件位置 */
	setenv( "LCUI_FONTFILE", PATH_FONTFILE_MSYH, FALSE );
	LCUI_Init();
	Create_GUI();
	if( argc == 2 ) {
		/* 生成图片文件列表 */
		ret = generate_imgfilelist( argv[1], imgfilepath );
		if( ret == 0 ) {
			open_imgfile( imgfilepath );
			 /* 进入浏览模式 */
			enter_viewer_mode();
		} else {
			show_usage( argv[0] );
		}
	} else {
		show_usage( argv[0] );
	}
	/* 显示界面 */
	Show_GUI(); 
	return LCUI_Main();
}
