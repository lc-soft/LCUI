// 用LCUI实现的简易照片查看器

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_ACTIVEBOX_H
#include LC_MEM_H
#include LC_LABEL_H 
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_INPUT_H 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <math.h>

/* 与这些函数共享数据，只有使用全局变量，传参数很麻烦 */
static LCUI_Widget	*window, *image_box, *tip_text, *tip_box,
			*tip_icon, *tip_pic, *image_info_text, *image_info_box,
			*btn_zoom[2], *btn_switch[2], *container[2];
						
static float	mini_scale = 1.0, scale = 1.0;   /* 记录缩放比率 */
static char	name[256], **filename = NULL;
static int	total_files = 0, current = 0, need_refresh = IS_FALSE;
static LCUI_Size image_box_size, window_size;
static pthread_t thread_hide, thread_viewer;

void image_zoom_in()
/* 功能：放大图像 */
{
	LCUI_Size size;
	LCUI_Graph *image; 
	scale += 0.25; /* 缩放比例自增25% */
	if(scale >= 2) {
		Disable_Widget(btn_zoom[1]);/* 禁用“放大”按钮 */
		scale = 2.0; /* 最大不能超过200% */
	}
	/* 为了能达到100%，加了个判断，如果缩放比例接近100%，那就直接等于100% */
	if(fabs(scale - 1.0) < 0.2 && scale != mini_scale) {
		scale = 1; 
	}
	/* 按比例缩放浏览区域 */
	Zoom_PictureBox_View_Area(image_box, scale);
	scale = Get_PictureBox_Zoom_Scale(image_box);/* 获取缩放比例 */
	image = Get_PictureBox_Graph(image_box); /* 获取图像指针 */
	size = Get_Graph_Size(image);
	Set_Label_Text(
		image_info_text, 
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale);
	/* 启用“缩小”按钮 */
	Enable_Widget(btn_zoom[0]); 
}

void image_zoom_out()
/* 功能：缩小图像 */
{
	LCUI_Size size;
	LCUI_Graph *image;
	
	scale -= 0.25; /* 缩放比例自减25% */
	/* 最小不能低于mini_scale中表示的比例 */
	if(scale <= mini_scale) {
		scale = mini_scale;
		Disable_Widget(btn_zoom[0]);
	}
	if(fabs(scale - 1.0) < 0.2 && scale != mini_scale) {
		scale = 1; 
	}
	if(scale > 0.05 && scale < mini_scale + 0.05) {
		scale = mini_scale;
	}
	/* 按比例缩放浏览区域 */
	Zoom_PictureBox_View_Area(image_box, scale);
	scale = Get_PictureBox_Zoom_Scale(image_box);
	image = Get_PictureBox_Graph(image_box); /* 获取图像指针 */
	size = Get_Graph_Size(image);
	Set_Label_Text(
		image_info_text, 
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale);
		 
	if(fabs(mini_scale - scale) < 0.01) Disable_Widget(btn_zoom[0]); 
	else Enable_Widget(btn_zoom[0]);
	Enable_Widget(btn_zoom[1]); 
}


void move_view_area(LCUI_Widget *widget, LCUI_DragEvent *event)
/* 功能：移动浏览区域 */
{ 
	LCUI_Pos pos; 
	static LCUI_Pos old_pos, offset_pos;
	if(event->first_click) {/* 如果是刚被点击 */ 
		/* 记录被点击时的位置，以后的移动，就以该位置为基础计算出新位置 */
		old_pos = Get_PictureBox_View_Area_Pos(widget);
		/* 浏览区域位置与图片框位置的偏移距离 */
		offset_pos = Pos_Sub(Get_Widget_Global_Pos(widget), old_pos); 
	} else if(event->end_click);
	else {/* 否则，那就是正被拖动 */ 
		/* 获取浏览区域的新位置 */
		pos = Pos_Sub(event->new_pos, offset_pos);
		/* 得出新位置与老位置的“差” */
		pos = Pos_Sub(old_pos, pos);
		/* 由于浏览区域是向鼠标移动方向相反的方向移动，因此，新位置实际是老位置与“差”的和 */
		pos = Pos_Add(old_pos, pos); 
		Move_PictureBox_View_Area(widget, pos);
	}
}

//如果是用按钮控制浏览区域，就使用下面的四个函数
void left_move_area()
{
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.x -= rect.width/2.0;
	if(rect.x < 0) rect.x = 0;
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void right_move_area()
{ 
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.x += rect.width/2.0; 
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void up_move_area()
{

	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.y -= rect.height/2.0;
	if(rect.y < 0) rect.y = 0;
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

void down_move_area()
{ 
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(image_box);
	rect.y += rect.height/2.0; 
	Move_PictureBox_View_Area(image_box, Pos(rect.x, rect.y));
}

//创建的线程用于隐藏按钮以及图像信息
static int wait_hide_btn = 4, wait_hide_info = 4;
void *hide()
{
	while(LCUI_Active()) {
		if(wait_hide_btn == 0) {
			Hide_Widget ( container[0] );
			Hide_Widget ( container[1] );
		}
		if(wait_hide_info == 0)
			Hide_Widget ( image_info_box );
			
		if(wait_hide_btn > -1)
			wait_hide_btn--;
		if(wait_hide_info > -1)
			wait_hide_info--;
		sleep(1);
	}
	LCUI_Thread_Exit(NULL);
}

void show_image_info()
{
	Show_Widget ( image_info_box );
	wait_hide_info = 4;
}

void show_button()
{
	Show_Widget ( container[0] );
	Show_Widget ( container[1] );
	wait_hide_btn = 4;
}

void get_filename(char *filepath, char *out_name)
/* 功能：用于获取文件名 */
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

void get_filepath(char *filepath, char *out_path)
/* 功能：用于获取程序所在的文件目录 */
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
 
int get_format(char *format, char *filename)
/* 功能：获取文件格式 */
{
	int i, j, len;
	len = strlen(filename);
	for(i=len-1; i>=0; --i) {
		if(filename[i] == '.') {
			for(j=0, ++i; i<len; ++i, ++j) 
				format[j] = filename[i]; 
			format[j] = 0;
			return 0;
		}
	}
	strcpy(format, "");
	return -1;
} 

 
char **scan_imgfile(char *dir, int *file_num)
/* 功能：扫描图片文件，并获得文件列表 */
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
	if (n < 0) return 0; 
	
	filelist = (char **)malloc(sizeof(char *)*n);
	for(i=0, *file_num=0; i<n; i++) {
		if(namelist[i]->d_type==8) {/* 如果是文件 */ 
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

int load_imagefile()
/* 功能：载入图片文件 */
{ 
	int i; 
	LCUI_Size size;
	LCUI_Graph *image;
	char file[1024];
	
	Set_Label_Text(tip_text, "正载入图片...");  
	tip_box->set_alpha(tip_box, 200);
	Hide_Widget(tip_icon);
	Show_Widget(tip_pic);
	Show_Widget(tip_text); 
	Show_Widget(tip_box);
	/* 播放动画 */
	ActiveBox_Play(tip_pic);  
	if(current < total_files) {
		strcpy(file, filename[current]);
	} else {
		return -1;
	}
	get_filename(file, name);
	
	image = Get_PictureBox_Graph(image_box); 
	i = Set_PictureBox_Image_From_File(image_box, file); 
	if(i != 0) {/* 如果图片文件读取失败 */
	printf("3\n");
		Set_Label_Text(tip_text, "图片载入失败!");
		Set_PictureBox_Image_From_File(tip_icon, "drawable/pic_fail.png");
		Show_Widget(tip_icon);
		Hide_Widget(tip_pic);
		/* 暂停动画播放 */
		ActiveBox_Pause(tip_pic);
		size.w = 0;
		size.h = 0;
	} else { 
		image = Get_PictureBox_Graph(image_box); 
		size = Get_Graph_Size(image); 
		/* 
		 * 不能使用Get_Widget_Size()函数获取image_box，因为
		 * 该函数获取的尺寸可能不准确 
		 * */ 
		if( 1 == Size_Cmp(size, IMAGE_BOX_SIZE)) {
		/* 如果图片尺寸大于image_box的尺寸，就改变image_box的图像处理模式 */ 
			Set_PictureBox_Size_Mode(image_box, SIZE_MODE_BLOCK_ZOOM);
			mini_scale = Get_PictureBox_Zoom_Scale(image_box);
		} else {
			Set_PictureBox_Size_Mode(image_box, SIZE_MODE_CENTER);
			mini_scale = 0.25; 
		}
	}
	
	scale = Get_PictureBox_Zoom_Scale(image_box); 
	Set_Label_Text(
		image_info_text, 
		"文件： (%d/%d) %s\n"
		"尺寸： %d x %d 像素\n"
		"缩放： %.2f%%",
		current+1, total_files, name,
		size.w, size.h, 100*scale); 
	/* 设置透明度 */ 
	image_info_box->set_alpha(image_info_box, 200);
	Show_Widget(image_info_text);
	Show_Widget(image_info_box); 
	return i;
}

static void *viewer()
/* 功能：将载入的图片显示出来 */
{
	LCUI_Graph *image, bg;
	int result; 
	
	Graph_Init(&bg);
	while(1) {
		/* 等待需要刷新 */ 
		while( !need_refresh ) {
			usleep(10000);
		}
		need_refresh = IS_FALSE; 
		show_button();
		show_image_info();
		/* 载入图片 */
		result = load_imagefile();
		if(result == 0) {/* 如果正常打开了图片 */ 
			scale = Get_PictureBox_Zoom_Scale(image_box); /* 获取缩放比例 */
			image = Get_PictureBox_Graph(image_box); /* 获取图像指针 */
			if(Graph_Is_PNG(image) && Graph_Have_Alpha(image)) {
				/* 载入马赛克图形 */
				Load_Graph_Mosaics(&bg);
				/* 平铺背景图 */
				Set_Widget_Background_Image(image_box, &bg, LAYOUT_TILE);
				Free_Graph(&bg);
			} else {
				Set_Widget_Background_Image(image_box, NULL, LAYOUT_NONE);
			}
		} 
		/* 如果接近最小缩放比例，那就禁用“缩小”按钮，否则，启用 */
		if(fabs(mini_scale - scale) < 0.01) {
			Disable_Widget(btn_zoom[0]); 
		} else {
			Enable_Widget(btn_zoom[0]);
		}
		/* 启用放大按钮 */
		Enable_Widget(btn_zoom[1]);
		Hide_Widget(tip_box); 
	}
	LCUI_Thread_Exit(NULL);
}

static void open_image_file(char *file)
/* 功能：打开图片文件 */
{
	int i;
	need_refresh = IS_TRUE;
	
	for(i=0; i<total_files; ++i) {
		if(strcmp(file, filename[i]) == 0) { 
			current = i;
			break;
		}
	}
	
	LCUI_Thread_Create(&thread_viewer, NULL, viewer, NULL); 
}

void prev_image(LCUI_Widget *widget, void *arg)
/* 功能：切换至上一张图 */
{ 
	if(total_files <= 0) {
		return;
	}
		
	if(current <= 0) {
		current = total_files-1;
	} else {
		--current;
	}
	need_refresh = IS_TRUE; 
}

void next_image(LCUI_Widget *widget, void *arg)
/* 功能：切换至下一张图 */
{
	if(total_files <= 0) return;
		
	if(current == total_files-1) {
		current = 0;
	} else {
		++current;
	}
		
	need_refresh = IS_TRUE; 
}

int main(int argc, char*argv[]) 
{ 
	int i;
	LCUI_Size screen_size;
	char path[1024];
	LCUI_Graph pic_loading[8], app_icon, btn_zoom_pic[6], btn_switch_pic[6];
	/* 设定默认字体文件位置 */
	Set_Default_Font("../../fonts/msyh.ttf");
	LCUI_Init(argc, argv); 
	Graph_Init(&app_icon); 
	/* 如果屏幕分辨率小于640x480，那就使用320x240分辨率 */
	screen_size = Get_Screen_Size();
	if(screen_size.w < 640 && screen_size.h <480){
		image_box_size = Size(318, 215);
		window_size = Size(320, 240);
	} else {
		image_box_size = Size(638, 455);
		window_size = Size(640, 480);
	}
	
	for(i=0; i<6; ++i) {
		Graph_Init(&btn_zoom_pic[i]); 
		Graph_Init(&btn_switch_pic[i]); 
	}
	/* 创建窗口部件 */
	window		= Create_Widget("window");
	/* 创建两个容器 */
	container[0]	= Create_Widget(NULL);
	container[1]	= Create_Widget(NULL);
	/* 以下两个按钮用于放大缩小 */
	btn_zoom[0]	= Create_Widget("button"); /* “缩小”按钮 */
	btn_zoom[1]	= Create_Widget("button"); /* “放大”按钮 */
	/* 以下两个按钮用于切换图片 */
	btn_switch[0]	= Create_Widget("button"); /* “上一张”按钮 */
	btn_switch[1]	= Create_Widget("button"); /* “下一张”按钮 */
	
	image_info_box = Create_Widget(NULL);
	image_info_text = Create_Widget("label");
	
	tip_text = Create_Widget("label"); /* 提示文本 */
	tip_box  = Create_Widget(NULL); /* 作为提示文本的容器 */
	tip_pic = Create_Widget("active_box"); /* ActiveBox部件用于显示载入动画 */
	tip_icon = Create_Widget("picture_box"); 
	image_box = Create_Widget("picture_box");/* 创建一个图片盒子，用于显示图片 */
	
	/* 载入程序图标 */ 
	Load_Image("drawable/icon.png", &app_icon);
	/* 载入这几个按钮在各个状态下显示的图形 */ 
	Load_Image("drawable/btn_zoom_up_normal.png", &btn_zoom_pic[0]); 
	Load_Image("drawable/btn_zoom_up_pressed.png", &btn_zoom_pic[1]); 
	Load_Image("drawable/btn_zoom_up_disabled.png", &btn_zoom_pic[2]); 
	Load_Image("drawable/btn_zoom_down_normal.png", &btn_zoom_pic[3]); 
	Load_Image("drawable/btn_zoom_down_pressed.png", &btn_zoom_pic[4]); 
	Load_Image("drawable/btn_zoom_down_disabled.png", &btn_zoom_pic[5]); 
	
	Load_Image("drawable/btn_next_up_normal.png", &btn_switch_pic[0]); 
	Load_Image("drawable/btn_next_up_pressed.png", &btn_switch_pic[1]); 
	Load_Image("drawable/btn_next_up_disabled.png", &btn_switch_pic[2]);  
	Load_Image("drawable/btn_prev_down_normal.png", &btn_switch_pic[3]); 
	Load_Image("drawable/btn_prev_down_pressed.png", &btn_switch_pic[4]); 
	Load_Image("drawable/btn_prev_down_disabled.png", &btn_switch_pic[5]);
	/* 设定按钮图形样式 */
	Custom_Button_Style(	btn_zoom[0], &btn_zoom_pic[3], &btn_zoom_pic[3], 
				&btn_zoom_pic[4], NULL, &btn_zoom_pic[5] );
	Custom_Button_Style(	btn_zoom[1], &btn_zoom_pic[0], &btn_zoom_pic[0], 
				&btn_zoom_pic[1], NULL, &btn_zoom_pic[2] );
							
	Custom_Button_Style(	btn_switch[0], &btn_switch_pic[0], &btn_switch_pic[0], 
				&btn_switch_pic[1], NULL, &btn_switch_pic[2] );
	Custom_Button_Style(	btn_switch[1], &btn_switch_pic[3], &btn_switch_pic[3], 
				&btn_switch_pic[4], NULL, &btn_switch_pic[5] );
							
	Set_Widget_Border (image_info_box, RGB(50,50,50), Border(1,1,1,1));/* 设置边框 */
	Set_Widget_Border (tip_box, RGB(200,200,200), Border(1,1,1,1));
	Set_Widget_Backcolor (image_info_box, RGB(180,215,255));
	Set_Widget_Backcolor (tip_box, RGB(20,20,20)); 
	Set_Widget_BG_Mode (image_info_box, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_BG_Mode (tip_box, BG_MODE_FILL_BACKCOLOR);
	
	Set_Label_Font_Default_Color(tip_text, RGB(230, 230, 230));
	
	Enable_Widget_Auto_Size(image_info_box);
	
	LCUI_Size size[2], btn_switch_size[2], btn_zoom_size[2];
	btn_zoom_size[0] = Get_Graph_Size(&btn_zoom_pic[0]);
	btn_zoom_size[1] = Get_Graph_Size(&btn_zoom_pic[0]);
	btn_switch_size[0] = Get_Graph_Size(&btn_switch_pic[0]);
	btn_switch_size[1] = Get_Graph_Size(&btn_switch_pic[1]);
	size[0] = Size(	btn_zoom_size[0].w, 
			btn_zoom_size[0].h + btn_zoom_size[1].h);
	size[1] = Size( btn_switch_size[0].w+btn_switch_size[1].w,
			btn_switch_size[0].h);
	/* 更改各个部件的尺寸 */
	Resize_Widget(window, WINDOW_SIZE);
	Resize_Widget(btn_zoom[0], btn_zoom_size[0]);
	Resize_Widget(btn_zoom[1], btn_zoom_size[1]);
	Resize_Widget(btn_switch[0], btn_switch_size[0]);
	Resize_Widget(btn_switch[1], btn_switch_size[1]);
	Resize_Widget(container[0], size[0]);
	Resize_Widget(container[1], size[1]);
	Resize_Widget(image_box, IMAGE_BOX_SIZE);
	Resize_Widget(tip_box, Size(150, 34));
	Resize_Widget(tip_pic, Size(32, 24));
	Resize_Widget(tip_icon, Size(32, 24));
	ActiveBox_Set_Frames_Size(tip_pic, Size(26, 26));
	/* 设定标题栏中显示的程序图标以及文本 */
	Set_Window_Title_Icon(window, &app_icon); 
	
	char version[20];
	Get_LCUI_Version(version);
	if(strcmp(version, "0.12.5") == 0) {
		Set_Window_Title_Text(window, "照片查看器 v0.4"); 
	} else {
		Set_Window_Title_Text(window, 
		 "照片查看器 v0.3 <color=240,0,0>(存在兼容性问题)</color>"); 
	 }
	
	/* 设定部件的布局 */
	Set_Widget_Align(btn_zoom[0], ALIGN_BOTTOM_CENTER, Pos(0, 0));
	Set_Widget_Align(btn_zoom[1], ALIGN_TOP_CENTER, Pos(0, 0));
	Set_Widget_Align(btn_switch[0], ALIGN_MIDDLE_RIGHT, Pos(0, 0));
	Set_Widget_Align(btn_switch[1], ALIGN_MIDDLE_LEFT, Pos(0, 0));
	Set_Widget_Align(container[0], ALIGN_MIDDLE_RIGHT, Pos(-2, 0));
	Set_Widget_Align(container[1], ALIGN_BOTTOM_LEFT, Pos(2, -2));
	Set_Widget_Align(image_box, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Set_Widget_Align(tip_box, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Set_Widget_Align(tip_text, ALIGN_MIDDLE_LEFT, Pos(35, 0));
	Set_Widget_Align(tip_icon, ALIGN_MIDDLE_LEFT, Pos(2, 0));
	Set_Widget_Align(tip_pic, ALIGN_MIDDLE_LEFT, Pos(2, 0));
	Set_Widget_Align(image_info_text, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Set_Widget_Align(image_info_box, ALIGN_TOP_LEFT, Pos(3, 3));
	
	/* 将部件加入相应的容器中 */
	Widget_Container_Add(container[0], btn_zoom[0]);
	Widget_Container_Add(container[0], btn_zoom[1]);
	Widget_Container_Add(container[1], btn_switch[0]);
	Widget_Container_Add(container[1], btn_switch[1]);
	Widget_Container_Add(tip_box, tip_text);
	Widget_Container_Add(tip_box, tip_icon);
	Widget_Container_Add(tip_box, tip_pic);
	Widget_Container_Add(image_info_box, image_info_text);
	Window_Client_Area_Add(window, container[0]); 
	Window_Client_Area_Add(window, container[1]); 
	Window_Client_Area_Add(window, image_box);
	Window_Client_Area_Add(window, tip_box);
	Window_Client_Area_Add(window, image_info_box);
	/* 关联部件的鼠标点击事件 */
	Widget_Clicked_Event_Connect(btn_switch[0], next_image, NULL);
	Widget_Clicked_Event_Connect(btn_switch[1], prev_image, NULL);
	Widget_Clicked_Event_Connect(btn_zoom[0], image_zoom_out, NULL);
	Widget_Clicked_Event_Connect(btn_zoom[1], image_zoom_in, NULL);
	Widget_Drag_Event_Connect(image_box, move_view_area);
	/* 显示部件 */
	Show_Widget(image_box);
	Show_Widget(container[0]);
	Show_Widget(container[1]);
	Show_Widget(window); 
	/* 为动画添加8帧图像，供ActiveBox部件播放 */
	for(i=0; i<8; ++i) {
		sprintf(path, "drawable/publish_loading_%02d.png", i+1);
		Load_Image(path, &pic_loading[i]); 
		ActiveBox_Add_Frame(tip_pic, &pic_loading[i], Pos(0,0), 10);
	}
	
	LCUI_Key_Event_Connect( KEY_ESC,   Main_Loop_Quit,  NULL);
	if(argc == 2) {/* 如果总共有2个参数 */ 
		DIR *dirptr = NULL; 
		
		/* 判断这个文件路径是文件还是目录 */ 
		if((dirptr = opendir(argv[1])) == NULL) {
			get_filepath(argv[1], path);
			filename = scan_imgfile(path, &total_files);
			open_image_file (argv[1]);
		} else {
			closedir(dirptr);
			filename = scan_imgfile(argv[1], &total_files);
			/* 判断文件数是否有效 */
			if( total_files > 0 ) {
				open_image_file (filename[0]);
			} else {
				goto skip_openfile;
			}
		} 
		
		Show_Widget(btn_zoom[0]);
		Show_Widget(btn_zoom[1]);
		Show_Widget(btn_switch[0]);
		Show_Widget(btn_switch[1]);
		/* 关联按键事件，当按下指定键值的按键后，会调用已关联的回调函数 */
		LCUI_Key_Event_Connect( '=',   image_zoom_in,	NULL);
		LCUI_Key_Event_Connect( '-',   image_zoom_out,   NULL);
		LCUI_Key_Event_Connect( KEY_AA,   image_zoom_in,	NULL);
		LCUI_Key_Event_Connect( KEY_BB,   image_zoom_out,   NULL);
		LCUI_Key_Event_Connect( KEY_LEFT,   left_move_area,  NULL);
		LCUI_Key_Event_Connect( KEY_RIGHT,  right_move_area, NULL);
		LCUI_Key_Event_Connect( KEY_UP,	 up_move_area,	NULL);
		LCUI_Key_Event_Connect( KEY_DOWN,   down_move_area,  NULL);
		LCUI_Key_Event_Connect( 'n',   next_image,  NULL);
		LCUI_Key_Event_Connect( 'N',   next_image,  NULL);
		LCUI_Key_Event_Connect( 'b',   prev_image,  NULL);
		LCUI_Key_Event_Connect( 'B',   prev_image,  NULL);
		LCUI_Key_Event_Connect( 'i',   show_image_info,  NULL);
		LCUI_Key_Event_Connect( 'I',   show_image_info,  NULL);
		Widget_Clicked_Event_Connect( image_box, show_button, NULL);
		LCUI_Thread_Create(&thread_hide, NULL, hide, NULL);
	} else {
skip_openfile:;
		char myname[256];
		LCUI_Widget *help_text;
		get_filename(argv[0], myname);
		help_text = Create_Widget("label");
		Set_Label_Text(help_text,
		"<color=30,145,220>用法：</color>\n"
		"<color=30,220,60>%s</color>  文件\n\n"
		"<color=30,145,220>控制：</color>\n"
		"<color=255,0,0>方向键：</color> 移动浏览区域\n"
		"<color=255,0,0>返回键：</color> 退出程序\n"
		"<color=255,0,0>+/-键：</color>  放大和缩小图像\n" 
		"<color=255,0,0>N/B键：</color> 上下切换图像\n" 
		"<color=255,0,0> I 键：</color>  显示图像信息\n"
		"你也可以使用鼠标点击界面上的按钮进行控制。\n"
		"本程序目前支持浏览bmp, png, jpeg格式的图片。\n",
		myname
		);
		Set_Widget_Align(help_text, ALIGN_TOP_LEFT, Pos(10, 10));
		Window_Client_Area_Add(window, help_text);
		Show_Widget(help_text);
		Hide_Widget(container[0]);
		Hide_Widget(container[1]); 
	}
	return LCUI_Main(); /* 进入主循环 */  
}

