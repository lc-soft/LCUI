#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

static long max(long a, long b)
{
	return a > b ? a:b;
}

static double radian(int angle) 
{
	return angle*3.1415926/180;
}

int Graph_Rotate(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des)
/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自互联网相关代码
 * 算法有待优化完善。
 */
{
	if(!Graph_Valid(src)) {
		return -1;
	}
	// 源图像的宽度和高度
	int	width, height;
	// 旋转后图像的宽度和高度   
	int	new_width,new_height; 
	// 旋转角度（弧度）   
	float   fRotateAngle; 
	// 旋转角度的正弦和余弦   
	float   fSina, fCosa; 
	// 源图四个角的坐标（以图像中心为坐标系原点）   
	float   fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）   
	float   fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 两个中间常量   
	float   f1,f2; 
	// 获取图像的"宽度"（4的倍数）   
	width = src->width; 
	// 获取图像的高度   
	height = src->height;   
	   
	// 将旋转角度从度转换到弧度   
	fRotateAngle = (float) radian(rotate_angle); 
	// 计算旋转角度的正弦   
	fSina = (float) sin((double)fRotateAngle); 
	// 计算旋转角度的余弦   
	fCosa = (float) cos((double)fRotateAngle);   
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）   
	fSrcX1 = (float) (- (width  - 1) / 2);   
	fSrcY1 = (float) (  (height - 1) / 2);   
	fSrcX2 = (float) (  (width  - 1) / 2);   
	fSrcY2 = (float) (  (height - 1) / 2);   
	fSrcX3 = (float) (- (width  - 1) / 2);   
	fSrcY3 = (float) (- (height - 1) / 2);   
	fSrcX4 = (float) (  (width  - 1) / 2);   
	fSrcY4 = (float) (- (height - 1) / 2);   
	   
	// 计算新图四个角的坐标（以图像中心为坐标系原点）   
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;   
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;   
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;   
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;   
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;   
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;   
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;   
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;   
	   
	// 计算旋转后的图像实际宽度   
	new_width  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);   
	// 计算旋转后的图像高度   
	new_height = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);   
	   
	// 两个常数，这样不用以后每次都计算了   
	f1 = (float) (-0.5*(new_width-1)*fCosa-0.5*(new_height-1)*fSina+0.5*(width-1)); 
	f2 = (float) (0.5*(new_width-1)*fSina-0.5*(new_height-1)*fCosa+0.5*(height-1));   
	   
	if(Graph_Valid(des)) {
		Graph_Free(des);/* 先将这个内存释放 */
	}
	des->have_alpha = src->have_alpha;
	// 分配内存，储存新的图形
	if(Graph_Create(des, new_width, new_height) != 0) {
		return -1;
	}

	long m, n, z;
	long src_x, src_y, des_x, des_y;
	// 针对图像每行进行操作
	Graph_Lock(src, 0);
	Graph_Lock(des, 1);
	for(des_y = 0; des_y < new_height; ++des_y) {
		m = new_width * des_y;
		// 针对图像每列进行操作   
		for(des_x = 0; des_x < new_width; ++des_x) {
			n = m + des_x;
			// 计算该象素在源图中的坐标   
			src_y = (long) (-((float) des_x) * fSina + ((float) des_y) * fCosa + f2 + 0.5);   
			src_x = (long) ( ((float) des_x) * fCosa + ((float) des_y) * fSina + f1 + 0.5);   
			   
			// 判断是否在源图范围内   
			if( (src_x >= 0) && (src_x < width) 
			&& (src_y >= 0) && (src_y < height)) {
				// 指向源DIB第i0行，第j0个象素的指针
				z = width * src_y + src_x;
				des->rgba[0][n] = src->rgba[0][z];
				des->rgba[1][n] = src->rgba[1][z];
				des->rgba[2][n] = src->rgba[2][z];
				if(Graph_Have_Alpha(des)) {
					des->rgba[3][n] = src->rgba[3][z];
				}
			} else {
				// 对于源图中没有的象素，直接赋值为255   
				des->rgba[0][n] = 255;
				des->rgba[1][n] = 255;
				des->rgba[2][n] = 255;
				if(Graph_Have_Alpha(des)) {
					des->rgba[3][n] = 0;
				}
			}
		}
	}
	Graph_Unlock(src);
	Graph_Unlock(des);
	return 0;
}
