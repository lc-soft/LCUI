#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H 

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h> 
#include <sys/ioctl.h> 

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos)
/* 
 * 功能：显示图形 
 * 说明：此函数使用帧缓冲（FrameBuffer）进行图形输出
 * *注：主要代码参考自mgaveiw的mga_vfb.c文件中的write_to_fb函数.
 * */
{
	int bits;
	unsigned char *dest;
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
		for (n=0,y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 4 * (k + x);/* 计算需填充的像素点的坐标 */
				count = count << 2; 
				/* 由于帧缓冲(FrameBuffer)的颜色排列是BGR，图片数组是RGB，需要改变一下写入顺序 */
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n]; 
			}
		}
		break;
	    case 24:/* 24位，RGB各占8位，也就是共3个字节 */ 
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 3 * (k + x); 
				count = (count << 1) + count;
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n];
			}
		}
		break;
	    case 16:/* 16位，rgb分别占5位，6位，5位，也就是RGB565 */
		/*
		 * GB565彩色模式, 一个像素占两个字节, 其中:
		 * 低字节的前5位用来表示B(BLUE)
		 * 低字节的后三位+高字节的前三位用来表示G(Green)
		 * 高字节的后5位用来表示R(RED)
		 * */  
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = (k + x) << 1;//count = 2 * (k + x);
				temp1 = pic->rgba[0][n];
				temp2 = pic->rgba[2][n];
				temp3 = pic->rgba[1][n];
				dest[count] = ((temp3 & 0x1c)<<3)+((temp2 & 0xf8)>>3);
				dest[count+1] = ((temp1 & 0xf8))+((temp3 & 0xe0)>>5);
			}
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
		
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
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
	return -1;
}
