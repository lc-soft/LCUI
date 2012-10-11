#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H

#include <math.h>

int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
{
	LCUI_Graph temp;
	
	uint_t x, y, i;
	uint_t ksize, kcenter;
	uint_t temp_pos, temp_start_pos;
	uint_t a_src_pos, b_src_pos, src_start_pos;
	double bmul, gmul, rmul, sum = 0;
	double *kernel, *kvalue_ptr, scale, cons;
	const double PI = 3.141592653;
	
	sigma = sigma > 0 ? sigma : -sigma;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	//ksize为奇数
	ksize = ceil(sigma * 3) * 2 + 1;
	kcenter = ksize/2;

	if(ksize == 1) {
		Graph_Copy( des, src ); 
		return 0;
	}

	//计算一维高斯核
	
	scale = -0.5/(sigma*sigma);
	cons = 1/sqrt(-scale / PI);
	kernel = (double*) malloc( ksize*sizeof(double) );
	for(i = 0; i < ksize; i++) {
		x = i - kcenter;
		*(kernel+i) = cons * exp(x * x * scale);//一维高斯函数
		sum += *(kernel+i); 
	} 
	//归一化,确保高斯权值在[0,1]之间
	for(i = 0; i < ksize; i++) {
		*(kernel+i) /= sum; 
	} 
	
	des->have_alpha = src->have_alpha;
	if( Graph_Create( des, src->width, src->height ) != 0 ) {
		return -1;
	}
	
	Graph_Init( &temp );
	temp.have_alpha = src->have_alpha;
	if( Graph_Create( &temp, src->width, src->height ) != 0 ) {
		return -2;
	}

	//x方向一维高斯模糊
	temp_start_pos = 0;
	src_start_pos = 0 - kcenter;
	for(y = 0; y < src->height; y++) {
		temp_pos = temp_start_pos;
		b_src_pos = src_start_pos;
		for(x = 0; x < src->width; x++) { 
			sum = bmul = gmul = rmul = 0;
			a_src_pos = b_src_pos;
			kvalue_ptr = kernel;
			for(i = -kcenter; i <= kcenter; i++) {
				if((x+i) >= 0 && (x+i) < src->width) {
					rmul += src->rgba[0][a_src_pos]*(*kvalue_ptr);
					gmul += src->rgba[1][a_src_pos]*(*kvalue_ptr);
					bmul += src->rgba[2][a_src_pos]*(*kvalue_ptr);
					sum += (*kvalue_ptr);
				}
				++a_src_pos;
				++kvalue_ptr;
			}
			temp.rgba[0][temp_pos] = rmul/sum;
			temp.rgba[1][temp_pos] = gmul/sum;
			temp.rgba[2][temp_pos] = bmul/sum;
			++temp_pos;
			++b_src_pos;
		}
		temp_start_pos += temp.width;
		src_start_pos += src->width;
	}
	
	//y方向一维高斯模糊
	src_start_pos = 0 - kcenter*temp.width;
	for(x = 0; x < temp.width; x++) {
		temp_start_pos = x;
		b_src_pos = src_start_pos;
		for(y = 0; y < temp.height; y++) { 
			sum = bmul = gmul = rmul = 0;
			a_src_pos = b_src_pos;
			for(i = -kcenter; i <= kcenter; i++) {
				if((y+i) >= 0 && (y+i) < temp.height) { 
					rmul += temp.rgba[0][a_src_pos]*(*(kernel+kcenter+i));
					gmul += temp.rgba[1][a_src_pos]*(*(kernel+kcenter+i));
					bmul += temp.rgba[2][a_src_pos]*(*(kernel+kcenter+i));
					sum += (*(kernel+kcenter+i));
				}
				a_src_pos += temp.width;
			}
			des->rgba[0][temp_start_pos] = rmul/sum;
			des->rgba[1][temp_start_pos] = gmul/sum;
			des->rgba[2][temp_start_pos] = bmul/sum;
			temp_start_pos += des->width;
			b_src_pos += temp.width;
		}
		++src_start_pos;
	}
	
	free(kernel);
	return 0;
}
