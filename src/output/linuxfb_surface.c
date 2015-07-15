/* ***************************************************************************
 * linuxfb_surface.c -- surface support for linux framebuffer.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************/

/* ****************************************************************************
 * linuxfb_surface.c -- 基于linux平台帧缓冲（framebuffer）的 surface 功能支持。
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ***************************************************************************/

#include <LCUI_Build.h>
#ifdef LCUI_VIDEO_DRIVER_FRAMEBUFFER
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

#define FB_DEV 		"/dev/fb0"
#define FB_BACKUP_FILE 	"/tmp/framebuffer.bak"

static struct LinuxFBContent {
	int dev_fd;
	uchar_t *mem;
	int mem_len;
	int bits_per_pixel;
	LCUI_Size screen_size;
	LCUI_SurfaceMethods methods;
} linuxfb;

/** 备份帧缓冲内容 */
static int LinuxFB_Backup(void)
{
	char cmd[256], *fb_dev;
	fb_dev = getenv("LCUI_FB_DEVICE");
	if(fb_dev == NULL) {
		fb_dev = FB_DEV;
	}
	sprintf( cmd, "cat %s > %s", fb_dev, FB_BACKUP_FILE );
	return system( cmd );
}

/** 恢复帧缓冲内容 */
static int LinuxFB_Restore(void)
{
	char cmd[256], *fb_dev;
	fb_dev = getenv("LCUI_FB_DEVICE");
	if(fb_dev == NULL) {
		fb_dev = FB_DEV;
	}
	sprintf( cmd, "cat %s > %s", FB_BACKUP_FILE, fb_dev );
	return system( cmd );
}

static int LinuxFB_GetWidth(void)
{
	return linuxfb.screen_size.w;
}

static int LinuxFB_GetHeight(void)
{
	return linuxfb.screen_size.h;
}

/** 打印屏幕相关的信息 */
static void print_screeninfo(	struct fb_var_screeninfo fb_vinfo,
				struct fb_fix_screeninfo fb_fix )
{
	char visual[256], type[256];

	switch(fb_fix.type) {
	    case FB_TYPE_PACKED_PIXELS:
		strcpy(type, "packed pixels");break;
	    case FB_TYPE_PLANES:
		strcpy(type, "non interleaved planes");break;
	    case FB_TYPE_INTERLEAVED_PLANES:
		strcpy(type, "interleaved planes");break;
	    case FB_TYPE_TEXT:
		strcpy(type, "text/attributes");break;
	    case FB_TYPE_VGA_PLANES:
		strcpy(type, "EGA/VGA planes");break;
	    default:
		strcpy(type, "unkown");break;
	}

	switch(fb_fix.visual) {
	    case FB_VISUAL_MONO01:
		strcpy(visual, "Monochr. 1=Black 0=White");break;
	    case FB_VISUAL_MONO10:
		strcpy(visual, "Monochr. 1=White 0=Black");break;
	    case FB_VISUAL_TRUECOLOR:
		strcpy(visual, "true color");break;
	    case FB_VISUAL_PSEUDOCOLOR:
		strcpy(visual, "pseudo color (like atari)");break;
	    case FB_VISUAL_DIRECTCOLOR:
		strcpy(visual, "direct color");break;
	    case FB_VISUAL_STATIC_PSEUDOCOLOR:
		strcpy(visual, "pseudo color readonly");break;
	    default:
		strcpy(type, "unkown");break;
	}
	printf(
		"============== screen info =============\n"
		"FB mem start  : 0x%08lX\n"
		"FB mem length : %d\n"
		"FB type       : %s\n"
		"FB visual     : %s\n"
		"accel         : %d\n"
		"geometry      : %d %d %d %d %d\n"
		"timings       : %d %d %d %d %d %d\n"
		"rgba          : %d/%d, %d/%d, %d/%d, %d/%d\n"
		"========================================\n",
		fb_fix.smem_start, fb_fix.smem_len,
		type, visual,
		fb_fix.accel,
		fb_vinfo.xres, fb_vinfo.yres,
		fb_vinfo.xres_virtual, fb_vinfo.yres_virtual,
		fb_vinfo.bits_per_pixel,
		fb_vinfo.upper_margin, fb_vinfo.lower_margin,
		fb_vinfo.left_margin, fb_vinfo.right_margin,
		fb_vinfo.hsync_len, fb_vinfo.vsync_len,
		fb_vinfo.red.length, fb_vinfo.red.offset,
		fb_vinfo.green.length, fb_vinfo.green.offset,
		fb_vinfo.blue.length, fb_vinfo.blue.offset,
		fb_vinfo. transp.length, fb_vinfo. transp.offset
	);
}

static LCUI_Surface LinuxFB_New(void)
{
	return (void*)&linuxfb.dev_fd;
}

static void LinuxFB_Delete( LCUI_Surface surface )
{

}

static void LinuxFB_Resize( LCUI_Surface surface, int w, int h )
{
	// ...
}

static LCUI_BOOL LinuxFB_IsReady( LCUI_Surface surface )
{
	return TRUE;
}

static void LinuxFB_Update( LCUI_Surface surface )
{

}

static void LinuxFB_SetRenderMode( LCUI_Surface surface, int mode )
{
	// ...
}

LCUI_PaintContext LinuxFB_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	LCUI_PaintContext paint;
	paint = (LCUI_PaintContext)malloc(sizeof(LCUI_PaintContextRec_));
	Graph_Init( &paint->canvas );
	Graph_Create( &paint->canvas, rect->w, rect->h );
	return paint;
}

/**
 * 将图形绘制到帧缓存中，适用于像素数据格式为BGRA的32位色模式
 * 一个像素点的数据共占32位，其中RGB各占8位，剩下的8位用于alpha，共4个字节
 * @warning 目前是默认将所有 Graph 都视为使用32位BGRA色彩模式，其它模式暂不做处理
 */
static void LinuxFB_PutGraph32( LCUI_Rect rect, LCUI_Graph *canvas )
{
	int y, line_bytes;
	uchar_t *pixel_line;

	line_bytes = linuxfb.screen_size.w*4;
	pixel_line = linuxfb.mem + rect.y*linuxfb.screen_size.w + rect.x;
	for( y=0; y<canvas->h; ++y ) {
		/* 都是BRGA格式，直接填充 */
		memcpy( pixel_line, canvas->bytes, line_bytes );
		pixel_line += line_bytes;
	}
}

/**
 * 将图形绘制到帧缓存中，适用于像素数据格式为BGR的24位色模式
 * 一个像素点的数据共占24位，RGB各占8位，共3个字节
 */
static void LinuxFB_PutGraph24( LCUI_Rect rect, LCUI_Graph *canvas )
{
	int x, y, dest_line_bytes;
	uchar_t *dest_line, *dest, *src_pixel_line;
	LCUI_Color *src_pixel;

	dest_line = linuxfb.mem + rect.y*linuxfb.screen_size.w + rect.x;
	dest_line_bytes = linuxfb.screen_size.w*3;
	src_pixel_line = canvas->bytes;
	for( y=0; y<canvas->h; ++y ) {
		dest = dest_line;
		src_pixel = (LCUI_Color*)src_pixel_line;
		for( x=0; x<canvas->w; ++x ) {
			*dest++ = src_pixel->blue;
			*dest++ = src_pixel->green;
			*dest++ = src_pixel->red;
			++src_pixel;
		}
		dest_line += dest_line_bytes;
		src_pixel_line += canvas->w;
	}
}

/**
 * 将图形绘制到帧缓存中，适用于像素数据格式为RGB565的16位色模式
 * 一个像素点的数据共占16位，rgb分别占5位，6位，5位，共2个字节
 */
static void LinuxFB_PutGraph16( LCUI_Rect rect, LCUI_Graph *canvas )
{
	int x, y, dest_line_bytes;
	uchar_t *dest_line, *dest, *src_pixel_line;
	LCUI_Color *src_pixel;

	dest_line = linuxfb.mem + rect.y*linuxfb.screen_size.w + rect.x;
	dest_line_bytes = linuxfb.screen_size.w*2;
	src_pixel_line = canvas->bytes;
	for( y=0; y<canvas->h; ++y ) {
		dest = dest_line;
		src_pixel = (LCUI_Color*)src_pixel_line;
		for( x=0; x<canvas->w; ++x ) {
			/*
			 * 低字节的前5位用来表示B(BLUE)
			 * 低字节的后三位+高字节的前三位用来表示G(Green)
			 * 高字节的后5位用来表示R(RED)
			 * */
			*dest = ((src_pixel->green & 0x1c)<<3);
			*dest += ((src_pixel->blue & 0xf8)>>3);
			++dest;
			*dest = ((src_pixel->red & 0xf8));
			*dest += ((src_pixel->green & 0xe0)>>5);
			++dest;
			++src_pixel;
		}
		dest_line += dest_line_bytes;
		src_pixel_line += canvas->w;
	}
}

/** 将图形绘制到帧缓存中，适用于像素数据格式为8位色模式 */
static void LinuxFB_PutGraph8( LCUI_Rect rect, LCUI_Graph *canvas )
{
	int x, y, dest_line_bytes;
	uchar_t *dest_line, *dest, *src_pixel_line;
	struct fb_cmap kolor;
	LCUI_Color *src_pixel;
	unsigned int r, g, b, i;

	kolor.start = 0;
	kolor.len = 255;
	kolor.red = calloc(256, sizeof(__u16));
	kolor.green = calloc(256, sizeof(__u16));
	kolor.blue = calloc(256, sizeof(__u16));
	kolor.transp = 0;
	dest_line = linuxfb.mem + rect.y*linuxfb.screen_size.w + rect.x;
	dest_line_bytes = linuxfb.screen_size.w;
	src_pixel_line = canvas->bytes;

	for( i=0; i<256; ++i ) {
		kolor.red[i] = 0;
		kolor.green[i] = 0;
		kolor.blue[i] = 0;
	}
	for( y=0; y<canvas->h; ++y ) {
		src_pixel = (LCUI_Color*)src_pixel_line;
		dest = dest_line;
		for( x=0; x<canvas->w; ++x ) {
			r = src_pixel->red * 0.92;
			g = src_pixel->green * 0.92;
			b = src_pixel->blue * 0.92;
			i = (r & 0xc0) + ((g & 0xf0)>>2) + ((b & 0xc0)>>6);
			kolor.red[i] = r*256;
			kolor.green[i] = g*256;
			kolor.blue[i] = b*256;
			*dest++ = i;
			++src_pixel;
		}
		dest_line += dest_line_bytes;
		src_pixel_line += canvas->w;
	}

	ioctl( linuxfb.dev_fd, FBIOPUTCMAP, &kolor );
	free( kolor.red );
	free( kolor.green );
	free( kolor.blue );
}

void LinuxFB_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	LCUI_Rect cut_rect;
	LCUI_Graph canvas;

	LCUIRect_GetCutArea( linuxfb.screen_size, paint_ctx->rect, &cut_rect );
	if( cut_rect.w <= 0 || cut_rect.h <= 0 ) {
		free( paint_ctx );
		return;
	}
	Graph_Init( &canvas );
	/* 如果需要裁剪图形 */
	if( cut_rect.x != 0 || cut_rect.y != 0
	 || cut_rect.w != paint_ctx->rect.w
	 || cut_rect.h != paint_ctx->rect.h ) {
		paint_ctx->rect.x += cut_rect.x;
		paint_ctx->rect.y += cut_rect.y;
		paint_ctx->rect.w = cut_rect.w;
		paint_ctx->rect.h = cut_rect.h;
		Graph_Quote( &canvas, &paint_ctx->canvas, &cut_rect );
	} else {
		Graph_Quote( &canvas, &paint_ctx->canvas, NULL );
	}

	switch( linuxfb.bits_per_pixel ) {
	    case 32:
		LinuxFB_PutGraph32( paint_ctx->rect, &canvas );
		break;
	    case 24:
		LinuxFB_PutGraph24( paint_ctx->rect, &canvas );
		break;
	    case 16:
		LinuxFB_PutGraph16( paint_ctx->rect, &canvas );
		break;
	    case 8:
		LinuxFB_PutGraph8( paint_ctx->rect, &canvas );
		break;
	    default: break;
	}
	Graph_Free( &canvas );
	free( paint_ctx );
}

static void LinuxFB_Present( LCUI_Surface surface )
{

}

/** 初始化适用于 Linux 帧缓冲（FrameBuffer） 的 surface 支持 */
LCUI_SurfaceMethods *LCUIDisplay_InitLinuxFB( LCUI_DisplayInfo *info )
{
	char *fb_dev;
	__u16 rr[256],gg[256],bb[256];
	struct fb_var_screeninfo fb_vinfo;
	struct fb_fix_screeninfo fb_fix;
	struct fb_cmap oldcmap = {0,256,rr,gg,bb};

	/* 获取环境变量中指定的帧缓冲设备的位置 */
	fb_dev = getenv("LCUI_FB_DEVICE");
	if( fb_dev == NULL ) {
		fb_dev = FB_DEV;
	}
	nobuff_printf("[linuxfb] open video output device...");
	linuxfb.dev_fd = open(fb_dev, O_RDWR);
	if( linuxfb.dev_fd == -1 ) {
		printf("failed\n");
		perror("[linuxfb] error");
		exit(-1);
	} else {
		printf("ok\n");
	}
	/* 获取屏幕相关信息 */
	ioctl( linuxfb.dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo );
	ioctl( linuxfb.dev_fd, FBIOGET_FSCREENINFO, &fb_fix );
	/* 打印屏幕信息 */
	print_screeninfo( fb_vinfo, fb_fix );
	if( fb_vinfo.bits_per_pixel == 8 ) {
		ioctl( linuxfb.dev_fd, FBIOGETCMAP, &oldcmap );
	}
	nobuff_printf("[linuxfb] mapping framebuffer...");
	linuxfb.mem_len = fb_fix.smem_len;
	/* 映射帧缓存至内存空间 */
	linuxfb.mem = mmap( NULL, fb_fix.smem_len,
			PROT_READ|PROT_WRITE, MAP_SHARED,
			linuxfb.dev_fd, 0 );

	if( (void *)-1 == linuxfb.mem ) {
		printf("fail\n");
		perror(strerror(errno));
		exit(-1);
	} else {
		printf("success\n");
	}
	LinuxFB_Backup();
	linuxfb.screen_size.w = fb_vinfo.xres;
	linuxfb.screen_size.h = fb_vinfo.yres;
	strncpy( info->name, "linux framebuffer", 32 );
	info->getWidth = LinuxFB_GetWidth;
	info->getHeight = LinuxFB_GetHeight;
	linuxfb.methods.new = LinuxFB_New;
	linuxfb.methods.delete = LinuxFB_Delete;
	linuxfb.methods.isReady = LinuxFB_IsReady;
	linuxfb.methods.show = NULL;
	linuxfb.methods.hide = NULL;
	linuxfb.methods.move = NULL;
	linuxfb.methods.resize = LinuxFB_Resize;
	linuxfb.methods.update = LinuxFB_Update;
	linuxfb.methods.present = LinuxFB_Present;
	linuxfb.methods.setCaptionW = NULL;
	linuxfb.methods.setRenderMode = LinuxFB_SetRenderMode;
	linuxfb.methods.setOpacity = NULL;
	linuxfb.methods.beginPaint = LinuxFB_BeginPaint;
	linuxfb.methods.endPaint = LinuxFB_EndPaint;
	linuxfb.methods.onInvalidRect = NULL;
	linuxfb.methods.onEvent = NULL;
	return &linuxfb.methods;
}

int LCUIDisplay_ExitLinuxFB(void)
{
	LinuxFB_Restore();
	return 0;
}

#endif
