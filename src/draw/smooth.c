#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

static const double PI = 3.141592653;

/** X轴方向线性模糊 */
static void HorizSmoothARGB( LCUI_ARGB *pSrcPixels, LCUI_ARGB *pDesPixels, 
			     size_t len, double *kvalue, int kcenter )
{
	int j;
	double i;
	LCUI_ARGB *px_src, *px_des;
	double sum, amul, bmul, gmul, rmul, *pkvalue;

	for( i=0; i<len; ++i ) { 
		sum = amul = bmul = gmul = rmul = 0;
		px_src = pSrcPixels;
		px_des = pDesPixels;
		pkvalue = kvalue;
		for( j=-kcenter; j<=kcenter; ++j ) {
			if((i+j) >= 0 && (i+j) < len) {
				bmul += px_src->b*(*pkvalue);
				gmul += px_src->g*(*pkvalue);
				rmul += px_src->r*(*pkvalue);
				amul += px_src->a*(*pkvalue);
				sum += (*pkvalue);
			}
			++px_src;
			++pkvalue;
		}
		px_des->b = bmul/sum;
		px_des->g = gmul/sum;
		px_des->r = rmul/sum;
		px_des->a = amul/sum;
		++px_des;
		++px_src;
	}
}

/** Y轴方向线性模糊 */
static void VertiSmoothARGB( LCUI_ARGB *pSrcPixels, LCUI_ARGB *pDesPixels, 
		size_t rows, size_t cols, double *pkernel, int kcenter )
{
	int j;
	double i;
	LCUI_ARGB *px_src, *px_des;
	double sum, amul, bmul, gmul, rmul, *pkvalue;
	
	for( i=0; i<rows; ++i ) { 
		sum = amul = bmul = gmul = rmul = 0;
		px_src = pSrcPixels;
		px_des = pDesPixels;
		pkvalue = pkernel;
		for( j=-kcenter; j<=kcenter; ++j ) {
			if((i+j) >= 0 && (i+j) < rows) {
				bmul += px_src->b*(*pkvalue);
				gmul += px_src->g*(*pkvalue);
				rmul += px_src->r*(*pkvalue);
				amul += px_src->a*(*pkvalue);
				sum += (*pkvalue);
			}
			px_src += cols;
			++pkvalue;
		}
		px_des->b = bmul/sum;
		px_des->g = gmul/sum;
		px_des->r = rmul/sum;
		px_des->a = amul/sum;
		px_des += cols;
		px_src += cols;
	}
}

/** 对图像进行高斯模糊处理 */
LCUI_API int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
{
	LCUI_Graph temp;
	int kcenter, i;
	int x, y, ksize;
	double *kernel, scale, cons, sum = 0;
	LCUI_ARGB *p_src_px, *p_des_px;
	
	sigma = sigma > 0 ? sigma : -sigma;
	// ksize为奇数 
	ksize = ceil(sigma * 3) * 2 + 1; 
	if(ksize == 1) {
		Graph_Copy( des, src );
		return 0;
	}

	// 计算一维高斯核 
	scale = -0.5/(sigma*sigma);
	cons = 1/sqrt(-scale / PI); 
	kcenter = ksize/2; 
	kernel = (double*)malloc( ksize*sizeof(double) );
	for(i = 0; i < ksize; ++i) {
		int x = i - kcenter;
		*(kernel+i) = cons * exp(x * x * scale); // 一维高斯函数
		sum += *(kernel+i);
	} 
	// 归一化,确保高斯权值在[0,1]之间
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
	
	// x方向一维高斯模糊
	p_des_px = temp.argb;
	p_src_px = src->argb - kcenter;
	for( y=0; y<src->h; ++y ) {
		HorizSmoothARGB( p_src_px, p_des_px, src->w, kernel, kcenter );
		p_des_px += temp.w;
		p_src_px += src->w;
	}

	// y方向一维高斯模糊
	p_des_px = temp.argb;
	p_src_px = src->argb - kcenter*temp.w;
	for( x=0; x<temp.w; ++x ) {
		VertiSmoothARGB( p_src_px, p_des_px, src->h, src->w, kernel, kcenter );
		++p_des_px;
		++p_src_px;
	}
	
	Graph_Free( &temp );
	free(kernel);
	return 0;
}

/** 对图像进行模糊处理 */
LCUI_API int Graph_Smooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
{
	return GaussianSmooth( src, des, sigma );
}
