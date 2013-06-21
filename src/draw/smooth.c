#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

LCUI_API int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
/* 对图像进行高斯模糊处理 */
{
	LCUI_Graph temp;
	int kcenter, i;
	int x, y, ksize;
	int temp_pos, temp_start_pos;
	int a_src_pos, b_src_pos, src_start_pos;
	double amul, bmul, gmul, rmul, *kvalue_ptr;
	double *kernel, scale, cons, sum = 0;
	const double PI = 3.141592653;
	
	sigma = sigma > 0 ? sigma : -sigma;
	//ksize为奇数 
	ksize = ceil(sigma * 3) * 2 + 1; 
	if(ksize == 1) {
		Graph_Copy( des, src );
		return 0;
	}

	//计算一维高斯核 
	scale = -0.5/(sigma*sigma);
	cons = 1/sqrt(-scale / PI); 
	kcenter = ksize/2; 
	kernel = (double*) malloc( ksize*sizeof(double) );
	for(i = 0; i < ksize; ++i) {
		int x = i - kcenter;
		*(kernel+i) = cons * exp(x * x * scale);//一维高斯函数
		sum += *(kernel+i);
	} 
	//归一化,确保高斯权值在[0,1]之间
	for(i = 0; i < ksize; i++) {
		*(kernel+i) /= sum;
	}
	
	des->color_type = src->color_type;
	if( Graph_Create( des, src->w, src->h ) != 0 ) {
		return -1;
	}
	
	Graph_Init( &temp );
	temp.color_type = src->color_type;
	if( Graph_Create( &temp, src->w, src->h ) != 0 ) {
		return -2;
	}
	
	Graph_Lock( &temp );
	
	//x方向一维高斯模糊
	temp_start_pos = 0;
	src_start_pos = 0 - kcenter;
	for(y = 0; y < src->h; y++) {
		temp_pos = temp_start_pos;
		b_src_pos = src_start_pos;
		for(x = 0; x < src->w; x++) { 
			sum = amul = bmul = gmul = rmul = 0;
			a_src_pos = b_src_pos;
			kvalue_ptr = kernel;
			for(i = -kcenter; i <= kcenter; i++) {
				if((x+i) >= 0 && (x+i) < src->w) {
					rmul += src->rgba[0][a_src_pos]*(*kvalue_ptr);
					gmul += src->rgba[1][a_src_pos]*(*kvalue_ptr);
					bmul += src->rgba[2][a_src_pos]*(*kvalue_ptr);
					if(src->color_type == COLOR_TYPE_RGBA) {
						amul += src->rgba[3][a_src_pos]*(*kvalue_ptr);
					}
					sum += (*kvalue_ptr);
				}
				++a_src_pos;
				++kvalue_ptr;
			}
			temp.rgba[0][temp_pos] = rmul/sum;
			temp.rgba[1][temp_pos] = gmul/sum;
			temp.rgba[2][temp_pos] = bmul/sum;
			if(temp.color_type == COLOR_TYPE_RGBA) {
				temp.rgba[3][temp_pos] = amul/sum;
			}
			++temp_pos;
			++b_src_pos;
		}
		temp_start_pos += temp.w;
		src_start_pos += src->w;
	}
	Graph_Unlock( &temp );
	
	Graph_Lock( &temp );
	//y方向一维高斯模糊
	src_start_pos = 0 - kcenter*temp.w;
	src_start_pos = 0 - kcenter*temp.w;
	for(x = 0; x < temp.w; x++) {
		temp_start_pos = x;
		b_src_pos = src_start_pos;
		for(y = 0; y < temp.h; y++) { 
			sum = amul = bmul = gmul = rmul = 0;
			a_src_pos = b_src_pos;
			kvalue_ptr = kernel;
			for(i = -kcenter; i <= kcenter; i++) {
				if((y+i) >= 0 && (y+i) < temp.h) { 
					rmul += temp.rgba[0][a_src_pos]*(*kvalue_ptr);
					gmul += temp.rgba[1][a_src_pos]*(*kvalue_ptr);
					bmul += temp.rgba[2][a_src_pos]*(*kvalue_ptr);
					if(temp.color_type == COLOR_TYPE_RGBA) {
						amul += temp.rgba[3][a_src_pos]*(*kvalue_ptr);
					}
					sum += (*kvalue_ptr);
				}
				a_src_pos += temp.w;
				++kvalue_ptr;
			}
			des->rgba[0][temp_start_pos] = rmul/sum;
			des->rgba[1][temp_start_pos] = gmul/sum;
			des->rgba[2][temp_start_pos] = bmul/sum;
			if(des->color_type == COLOR_TYPE_RGBA) {
				des->rgba[3][temp_start_pos] = amul/sum; 
			}
			temp_start_pos += des->w;
			b_src_pos += temp.w;
		}
		++src_start_pos;
	}
	
	Graph_Unlock( &temp );
	Graph_Free( &temp );
	free(kernel);
	return 0;
}

LCUI_API int Graph_Smooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
/* 对图像进行模糊处理 */
{
	return GaussianSmooth( src, des, sigma );
}
