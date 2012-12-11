/* ***************************************************************************
 * framebuffer.c -- FrameBuffer support for graphical output
 * 
 * Copyright (C) 2012 by
 * Liu Chao
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
 * ****************************************************************************/
 
/* ****************************************************************************
 * framebuffer.c -- 图形输出的帧缓冲（FrameBuffer）支持
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
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
 * ****************************************************************************/

#include "config.h"
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H 

#ifdef GRAPH_OUTPUT_USE_FRAMEBUFFER
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h> 
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>


void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color)
/* 功能：填充指定位置的像素点的颜色 */
{
	int k;
	uchar_t *dest;
	
	k = (pos.y * LCUI_Sys.screen.size.w + pos.x) << 2; 
	//这里需要根据不同位的显示器来进行相应处理
	dest = LCUI_Sys.screen.fb_mem;	/* 指向帧缓冲 */
	dest[k] = color.blue;
	dest[k + 1] = color.green;
	dest[k + 2] = color.red; 
}

int Get_Screen_Graph(LCUI_Graph *out)
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */
{
	uchar_t  *dest;
	int i, temp, h, w;
	if( !LCUI_Sys.init ) {/* 如果没有初始化过 */
		return -1; 
	}
	
	out->have_alpha = FALSE;/* 无alpha通道 */
	out->type = TYPE_BMP;
	temp = Graph_Create(out, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h);
	if(temp != 0) {
		return -2; 
	}
	/* 指针指向帧缓冲的内存 */
	dest = LCUI_Sys.screen.fb_mem;
	switch( Get_Screen_Bits() ) {
	    case 32:
		for (i=0,h=0; h < LCUI_Sys.screen.size.h; ++h) {
			for (w = 0; w < LCUI_Sys.screen.size.w; ++w) {
				out->rgba[2][i] = *(dest++);
				out->rgba[1][i] = *(dest++);
				out->rgba[0][i] = *(dest++);
				dest++;
				++i; 
			}
		}
		break;
	    case 24:
		for (i=0,h=0; h < LCUI_Sys.screen.size.h; ++h) {
			for (w = 0; w < LCUI_Sys.screen.size.w; ++w) {
				/* 读取帧缓冲的内容 */
				out->rgba[2][i] = *(dest++);
				out->rgba[1][i] = *(dest++);
				out->rgba[0][i] = *(dest++);
				++i; 
			}
		}
		break;
	    case 16: 
		break;
	    case 8: 
		break;
	}
	return 0;
}

static void print_screeninfo(
		struct fb_var_screeninfo fb_vinfo,
		struct fb_fix_screeninfo fb_fix
	)
/* 功能：打印屏幕相关的信息 */
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

int Screen_Init()
/* 功能：初始化屏幕 */
{
	__u16 rr[256],gg[256],bb[256];
	struct fb_var_screeninfo fb_vinfo;
	struct fb_fix_screeninfo fb_fix;
	struct fb_cmap oldcmap = {0,256,rr,gg,bb} ;
	
	char *fb_dev;
	/* 获取环境变量中指定的帧缓冲设备的位置 */
	fb_dev = getenv("LCUI_FB_DEVICE");
	if(fb_dev == NULL) {
		fb_dev = FB_DEV;
	}
	
	nobuff_printf("open video output device..."); 
	LCUI_Sys.screen.fb_dev_fd = open(fb_dev, O_RDWR);
	if (LCUI_Sys.screen.fb_dev_fd== -1) {
		printf("fail\n");
		perror("error");
		exit(-1);
	} else {
		printf("success\n");
	}
	LCUI_Sys.screen.fb_dev_name = fb_dev;
	/* 获取屏幕相关信息 */
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo);
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_FSCREENINFO, &fb_fix);
	/* 打印屏幕信息 */
	print_screeninfo(fb_vinfo, fb_fix);
	
	LCUI_Sys.screen.bits = fb_vinfo.bits_per_pixel;
	if (fb_vinfo.bits_per_pixel==8) {
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGETCMAP, &oldcmap); 
	}
	nobuff_printf("mapping framebuffer...");
	LCUI_Sys.screen.smem_len = fb_fix.smem_len;/* 保存内存空间大小 */
	/* 映射帧缓存至内存空间 */
	LCUI_Sys.screen.fb_mem = mmap(NULL,fb_fix.smem_len,
					PROT_READ|PROT_WRITE,MAP_SHARED,
					LCUI_Sys.screen.fb_dev_fd, 0);
							
	if((void *)-1 == LCUI_Sys.screen.fb_mem) { 
		printf("fail\n");
		perror(strerror(errno));
		exit(-1);
	} else {
		printf("success\n");
	}
	
	Graph_Init(&LCUI_Sys.screen.buff); /* 初始化图形数据 */
	
	LCUI_Sys.screen.buff.type = TYPE_BMP;/* bmp位图 */
	LCUI_Sys.screen.size.w = fb_vinfo.xres; /* 保存屏幕尺寸 */
	LCUI_Sys.screen.size.h = fb_vinfo.yres; 
	
	/* 保存当前屏幕内容，以便退出LCUI后还原 */
	Get_Screen_Graph(&LCUI_Sys.screen.buff); 
	return 0;
}

int Screen_Destroy()
{
	int err;
	LCUI_Sys.status = KILLED;
	/* 恢复屏幕初始内容 */ 
	Graph_Display (&LCUI_Sys.screen.buff, Pos(0, 0));	
	/* 解除帧缓冲在内存中的映射 */
	err = munmap (LCUI_Sys.screen.fb_mem, LCUI_Sys.screen.smem_len);
	if (err != 0) {
		perror ("munmap()");
		return err;
	}
	close (LCUI_Sys.screen.fb_dev_fd);  
	return 0;
}

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos)
/* 
 * 功能：显示图形 
 * 说明：此函数使用帧缓冲（FrameBuffer）进行图形输出
 * *注：主要代码参考自mgaveiw的mga_vfb.c文件中的write_to_fb函数.
 * */
{
	int bits;
	uchar_t *dest;
	struct fb_cmap kolor; 
	unsigned int x, y, n, k, count;
	unsigned int temp1, temp2, temp3, i; 
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Graph_Valid (src)) {
		return -1;
	}
	/* 指向帧缓冲 */
	dest = LCUI_Sys.screen.fb_mem;		
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( Get_Screen_Size(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* 如果需要裁剪图形 */
		if(!Rect_Valid(cut_rect)) {
			return -2;
		}
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Graph_Cut (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Graph_Lock (pic, 0);
	/* 获取显示器的位数 */
	bits = Get_Screen_Bits(); 
	switch(bits) {
	    case 32:/* 32位，其中RGB各占8位，剩下的8位用于alpha，共4个字节 */ 
		k = pos.y * LCUI_Sys.screen.size.w + pos.x;
		for (n=0,y = 0; y < pic->height; ++y) {
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 4 * (k + x);/* 计算需填充的像素点的坐标 */
				count = count << 2; 
				/* 由于帧缓冲(FrameBuffer)的颜色排列是BGR，图片数组是RGB，需要改变一下写入顺序 */
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n]; 
			}
			k += LCUI_Sys.screen.size.w;
		}
		break;
	    case 24:/* 24位，RGB各占8位，也就是共3个字节 */ 
		k = pos.y * LCUI_Sys.screen.size.w + pos.x;
		for (n=0, y = 0; y < pic->height; ++y) {
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 3 * (k + x); 
				count = (count << 1) + count;
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n];
			}
			k += LCUI_Sys.screen.size.w;
		}
		break;
	    case 16:/* 16位，rgb分别占5位，6位，5位，也就是RGB565 */
		/*
		 * RGB565彩色模式, 一个像素占两个字节, 其中:
		 * 低字节的前5位用来表示B(BLUE)
		 * 低字节的后三位+高字节的前三位用来表示G(Green)
		 * 高字节的后5位用来表示R(RED)
		 * */  
		k = pos.y * LCUI_Sys.screen.size.w + pos.x;
		for (n=0, y = 0; y < pic->height; ++y) {
			for (x = 0; x < pic->width; ++x, ++n) {
				count = (k + x) << 1;//count = 2 * (k + x);
				temp1 = pic->rgba[0][n];
				temp2 = pic->rgba[2][n];
				temp3 = pic->rgba[1][n];
				dest[count] = ((temp3 & 0x1c)<<3)+((temp2 & 0xf8)>>3);
				dest[count+1] = ((temp1 & 0xf8))+((temp3 & 0xe0)>>5);
			}
			k += LCUI_Sys.screen.size.w;
		}
		break;
	    case 8: /* 8位，占1个字节 */
		kolor.start = 0;
		kolor.len = 255; 
		kolor.red = calloc(256, sizeof(__u16));
		kolor.green = calloc(256, sizeof(__u16));
		kolor.blue = calloc(256, sizeof(__u16));
		kolor.transp = 0; 
		
		for (i=0;i<256;i++) {
			kolor.red[i]=0;
			kolor.green[i]=0;
			kolor.blue[i]=0;
		}
		
		k = pos.y * LCUI_Sys.screen.size.w + pos.x;
		for (n=0, y = 0; y < pic->height; ++y) {
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;
				
				temp1 = pic->rgba[0][n]*0.92;
				temp2 = pic->rgba[1][n]*0.92;
				temp3 = pic->rgba[2][n]*0.92; 
				
				i = ((temp1 & 0xc0))
					+((temp2 & 0xf0)>>2)
					+((temp3 & 0xc0)>>6);
						
				kolor.red[i] = temp1*256;
				kolor.green[i] = temp2*256;
				kolor.blue[i] = temp3*256;
				dest[count] = (((temp1 & 0xc0))
						+((temp2 & 0xf0)>>2)
						+((temp3 & 0xc0)>>6)); 
			}
			k += LCUI_Sys.screen.size.w;
		}
		
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOPUTCMAP, &kolor); 
		free(kolor.red);
		free(kolor.green);
		free(kolor.blue);
		break;
	    default: break;
	}
	Graph_Unlock (pic);
	Graph_Free (&temp);
	return 0;
}

#endif
