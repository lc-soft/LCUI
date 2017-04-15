#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

#include <math.h>

static int getmax(int a, int b)
{
	return a > b ? a:b;
}

static double radian(int angle) 
{
	return angle*3.1415926/180;
}

/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自互联网相关代码
 * 算法有待优化完善。
 */
LCUI_API int Graph_Rotate( LCUI_Graph *src, int rotate_angle, LCUI_Graph *des )
{
	int width, height;
	int new_width,new_height;
	int src_x, src_y, des_x, des_y;
	double fRotateAngle; 
	double fSina, fCosa; 
	double fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	double fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	double f1,f2;
	uchar_t *pSrcRowByte, *pDesRowByte, *pSrcByte, *pDesByte;

	if(!Graph_IsValid(src)) {
		return -1;
	}
	// 获取图像的"宽度"（4的倍数）   
	width = src->width;
	// 获取图像的高度   
	height = src->height;
	   
	// 将旋转角度从度转换到弧度   
	fRotateAngle = (double) radian(rotate_angle); 
	// 计算旋转角度的正弦   
	fSina = (double) sin((double)fRotateAngle); 
	// 计算旋转角度的余弦   
	fCosa = (double) cos((double)fRotateAngle);   
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）   
	fSrcX1 = (double) (- (width  - 1) / 2);   
	fSrcY1 = (double) (  (height - 1) / 2);   
	fSrcX2 = (double) (  (width  - 1) / 2);   
	fSrcY2 = (double) (  (height - 1) / 2);   
	fSrcX3 = (double) (- (width  - 1) / 2);   
	fSrcY3 = (double) (- (height - 1) / 2);   
	fSrcX4 = (double) (  (width  - 1) / 2);   
	fSrcY4 = (double) (- (height - 1) / 2);   
	   
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
	new_width  = (long) ( getmax( (int)fabs(fDstX4 - fDstX1), (int)fabs(fDstX3 - fDstX2) ) + 0.5);   
	// 计算旋转后的图像高度   
	new_height = (long) (getmax( (int)fabs(fDstY4 - fDstY1), (int)fabs(fDstY3 - fDstY2) )  + 0.5);   
	   
	// 两个常数，这样不用以后每次都计算了   
	f1 = (double) (-0.5*(new_width-1)*fCosa-0.5*(new_height-1)*fSina+0.5*(width-1)); 
	f2 = (double) (0.5*(new_width-1)*fSina-0.5*(new_height-1)*fCosa+0.5*(height-1));   
	
	des->color_type = src->color_type;
	// 分配内存，储存新的图形
	if(Graph_Create(des, new_width, new_height) != 0) {
		return -1;
	}
	
	pDesRowByte = des->bytes;
	pSrcRowByte = src->bytes;
	// 针对图像每行进行操作
	for( des_y=0; des_y<new_height; ++des_y ) {
		pDesByte = pDesRowByte;
		pSrcByte = pSrcRowByte;
		// 针对图像每列进行操作   
		for( des_x=0; des_x<new_width; ++des_x ) {
			// 计算该象素在源图中的坐标   
			src_y = (long) (-((double) des_x) * fSina + ((double) des_y) * fCosa + f2 + 0.5);   
			src_x = (long) ( ((double) des_x) * fCosa + ((double) des_y) * fSina + f1 + 0.5);   
			   
			// 判断是否在源图范围内   
			if( (src_x >= 0) && (src_x < width) 
			&& (src_y >= 0) && (src_y < height)) {
				if( src->color_type == COLOR_TYPE_ARGB ) {
					pSrcByte = src->bytes + (width*src_y + src_x)*4;
					*pDesByte++ = *pSrcByte++;
					*pDesByte++ = *pSrcByte++;
					*pDesByte++ = *pSrcByte++;
					*pDesByte++ = *pSrcByte++;
				} else {
					pSrcByte = src->bytes + (width*src_y + src_x)*3;
					*pDesByte++ = *pSrcByte++;
					*pDesByte++ = *pSrcByte++;
					*pDesByte++ = *pSrcByte++;
				}
			} else {
				// 对于源图中没有的象素，直接赋值为255
				*pDesByte++ = 255;
				*pDesByte++ = 255;
				*pDesByte++ = 255;
				if( src->color_type == COLOR_TYPE_ARGB ) {
					*pDesByte++ = 0;
				}
			}
		}
		pDesRowByte += des->bytes_per_row;
		pSrcRowByte += src->bytes_per_row;
	}
	return 0;
}
