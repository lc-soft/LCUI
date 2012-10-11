#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H

#include <math.h>

int __Graph_Smooth( LCUI_Graph *graph, LCUI_Graph *out, LCUI_Rect area )
{ 
	int Smooth_Box[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 };
	int Smooth_Gauss[10] = { 1, 2, 1, 2, 4, 2, 1, 2, 1, 4 };
	int Sharpen_Laplacian[10] = { -1, -1, -1, -1, 9, -1, -1, -1, -1, 1 };

	int *tEffect = NULL; 
	tEffect = Smooth_Gauss;
	/***
	switch (type) {
		case SMOOTH_BOX:
		tEffect = Smooth_Box;
		break;
		case SMOOTH_GAUSS:
		tEffect = Smooth_Gauss;
		break;
		case SHARPEN_LL:
		tEffect = Sharpen_Laplacian;
		break;
		default:
		return -1;
	}
	***/
	int col;
	uint_t r, g, b, y, des_row, src_row, des_n, index, n, tmp_m, i, total;
	LCUI_Graph *src;
	LCUI_Rect src_rect;
	
	src = Get_Quote_Graph( graph );
	if( !Graph_Valid( src ) ) {
		return -1;
	}
	src_rect = Get_Graph_Valid_Rect( graph ); 
	area = Get_Valid_Area( Size(src_rect.width, src_rect.height), area );
	
	if( Graph_Create( out, area.width, area.height ) != 0 ) {
		return -1;
	}
	
	Graph_Lock( src, 1 ); 
	
	y = area.height-1;
	src_row = (area.y + 1 + src_rect.y) * src->width + area.x + src_rect.x + 1; 
	des_row = 0;
	while ( --y ) {
		tmp_m = src_row;
		des_n = des_row;
		total = tmp_m + area.width - 1;
		for (++tmp_m; tmp_m < total; ++tmp_m,++des_n) { 
			r = src->rgba[0][tmp_m]; 
			g = src->rgba[1][tmp_m];
			b = src->rgba[2][tmp_m]; 
			for (index=0,col = -1; col <= 1; ++col) {
				n = src_row + col*src->width -1;
				for(i=0; i<3; ++i,++n,++index) {
					r += src->rgba[0][n] * tEffect[index];
					g += src->rgba[1][n] * tEffect[index];
					b += src->rgba[2][n] * tEffect[index];
				}
			}
			r >>= tEffect[index]; 
			g >>= tEffect[index]; 
			b >>= tEffect[index]; 
			out->rgba[0][des_n] = r;
			out->rgba[1][des_n] = g;
			out->rgba[2][des_n] = b;
		}
		des_row += area.width;
		src_row += src->width;
	}
	Graph_Unlock( src ); 
	return 0;
}

int Graph_Smooth(LCUI_Graph *graph, LCUI_Graph *out, double sigma, int radius)   
{
	LCUI_Graph *src;
	LCUI_Rect src_rect;
	
	src = Get_Quote_Graph( graph );
	if( !Graph_Valid( src ) ) {
		return -1;
	}
	src_rect = Get_Graph_Valid_Rect( graph ); 
	out->have_alpha = src->have_alpha;
	if( Graph_Create( out, graph->width, graph->height ) != 0 ) {
		return -1;
	}
	
	double *gauss_matrix, gauss_sum, _2sigma2;   
	int x, y, xx, yy, xxx, yyy;   
	double *pdbl, a, r, g, b, d; 
	uint_t src_i, src_n, des_i, tmp_m, tmp_n, total;
	
	gauss_sum = 0.0;
	_2sigma2 = 2 * sigma * sigma;
	gauss_matrix = pdbl = (double *)malloc((radius * 2 + 1) * (radius * 2 + 1) * sizeof(double));   
	if (!gauss_matrix) {
		Graph_Free( out );   
		return 0;   
	}
	for (y = -radius; y <= radius; y++) {   
		for (x = -radius; x <= radius; x++) {   
			a = exp(-(double)(x * x + y * y) / _2sigma2);   
			*pdbl++ = a;
			gauss_sum += a;
		}   
	}
	pdbl = gauss_matrix;
	for (y = -radius; y <= radius; y++) {   
		for (x = -radius; x <= radius; x++) {   
			*pdbl++ /= gauss_sum;
		}
	}
	
	uint_t des_pos, src_pos, des_start_pos, src_start_pos, src_start_pos_1, src_start_pos_2;
	
	des_start_pos = 0;
	src_start_pos = (src_rect.y-radius) * src->width + src_rect.x - radius;
	for (y = 0; y < src_rect.height; y++) {
		des_pos = des_start_pos;
		src_start_pos_1 = src_start_pos;
		for (x=0; x < src_rect.width; ++x,++des_pos) {
			a = r = g = b = 0.0;
			pdbl = gauss_matrix; 
			src_start_pos_2 = src_start_pos_1+x;
			for (yy = (-radius); yy <= radius; ++yy) { 
				yyy = y + yy;
				src_pos = src_start_pos_2;
				if (yyy >= 0 && yyy < src_rect.height) {
					for (xx = (-radius); xx <= radius; ++xx) {   
						xxx = x + xx; 
						if (xxx >= 0 && xxx < src_rect.width) {
							r += *pdbl * src->rgba[0][src_pos];   
							g += *pdbl * src->rgba[1][src_pos];  
							b += *pdbl * src->rgba[2][src_pos];
							if(Graph_Have_Alpha(src)) {
								a += *pdbl * src->rgba[3][src_pos];
							}
						}
						++pdbl;
						++src_pos;
					}
				} else {
					pdbl += (radius * 2 + 1);   
				}
				src_start_pos_2 += src->width;
			}
			out->rgba[0][des_pos] = r;
			out->rgba[1][des_pos] = g;
			out->rgba[2][des_pos] = b;
			if(Graph_Have_Alpha(src)) {
				out->rgba[3][des_pos] = a;
			}
		}
		des_start_pos += src_rect.width;
		src_start_pos += src->width;
	}
	/************
	for (y = 0; y < graph->height; y++) { 
		for (x = 0; x < graph->width; x++) {   
			a = r = g = b = 0.0;
			pdbl = gauss_matrix;   
			for (yy = -radius; yy <= radius; yy++) {   
				yyy = y + yy;
				if (yyy >= 0 && yyy < graph->height) {   
					for (xx = -radius; xx <= radius; xx++) {   
						xxx = x + xx;
						if (xxx >= 0 && xxx < graph->width) {   
							d = *pdbl;   
							r += d * src->rgba[0][xxx + yyy * graph->width];   
							g += d * src->rgba[1][xxx + yyy * graph->width];  
							b += d * src->rgba[2][xxx + yyy * graph->width];  
							//a += d * bbb[3];   
						}
						pdbl++;   
					}   
				} else {
					pdbl += (radius * 2 + 1);   
				}   
			}
			out->rgba[0][x + y * graph->width] = r;
			out->rgba[1][x + y * graph->width] = g;
			out->rgba[2][x + y * graph->width] = b;
		}   
	}
	* ***********/
	return 1;   
}

int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma )
{
	sigma = sigma > 0 ? sigma : -sigma;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	//ksize为奇数
	int ksize = ceil(sigma * 3) * 2 + 1;

	//cout << "ksize=" <<ksize<<endl;
	//	dst.create(src.size(), src.type());
	if(ksize == 1) {
		//src.copyTo(dst);	
		return 0;
	}

	//计算一维高斯核
	double *kernel = (double*) malloc( ksize*sizeof(double) );

	double scale = -0.5/(sigma*sigma);
	const double PI = 3.141592653;
	double cons = 1/sqrt(-scale / PI);

	double sum = 0;
	int kcenter = ksize/2;
	int i = 0, j = 0;
	for(i = 0; i < ksize; i++) {
		int x = i - kcenter;
		*(kernel+i) = cons * exp(x * x * scale);//一维高斯函数
		sum += *(kernel+i);

//		cout << " " << *(kernel+i);
	}
//	cout << endl;
	//归一化,确保高斯权值在[0,1]之间
	for(i = 0; i < ksize; i++) {
		*(kernel+i) /= sum;
//		cout << " " << *(kernel+i);
	}
//	cout << endl;
	
	//dst.create(src.size(), src.type());
	des->have_alpha = src->have_alpha;
	if( Graph_Create( des, src->width, src->height ) != 0 ) {
		return -1;
	}
	
	//Mat temp;
	//temp.create(src.size(), src.type());
	LCUI_Graph temp;
	
	Graph_Init( &temp );
	temp.have_alpha = src->have_alpha;
	if( Graph_Create( &temp, src->width, src->height ) != 0 ) {
		return -2;
	}

	int x, y;
	double bmul, gmul, rmul;
	//x方向一维高斯模糊
	for(y = 0; y < src->height; y++) {
		for(x = 0; x < src->width; x++) { 
			sum = 0;
			bmul = 0, gmul = 0, rmul = 0;
			for(i = -kcenter; i <= kcenter; i++) {
				if((x+i) >= 0 && (x+i) < src->width) {
					rmul += src->rgba[0][y*src->width+(x+i)]*(*(kernel+kcenter+i));
					gmul += src->rgba[1][y*src->width+(x+i)]*(*(kernel+kcenter+i));
					bmul += src->rgba[2][y*src->width+(x+i)]*(*(kernel+kcenter+i));
					sum += (*(kernel+kcenter+i));
				}
			}
			temp.rgba[0][y*temp.width+x] = rmul/sum;
			temp.rgba[1][y*temp.width+x] = gmul/sum;
			temp.rgba[2][y*temp.width+x] = bmul/sum;
		}
	}

	
	//y方向一维高斯模糊
	for(x = 0; x < temp.width; x++) {
		for(y = 0; y < temp.height; y++) { 
			sum = 0;
			bmul = 0, gmul = 0, rmul = 0;
			for(i = -kcenter; i <= kcenter; i++) {
				if((y+i) >= 0 && (y+i) < temp.height) { 
					rmul += temp.rgba[0][(y+i)*temp.width+x]*(*(kernel+kcenter+i));
					gmul += temp.rgba[1][(y+i)*temp.width+x]*(*(kernel+kcenter+i));
					bmul += temp.rgba[2][(y+i)*temp.width+x]*(*(kernel+kcenter+i));
					sum += (*(kernel+kcenter+i));
				}
			}
			des->rgba[0][y*temp.width+x] = rmul/sum;
			des->rgba[1][y*temp.width+x] = gmul/sum;
			des->rgba[2][y*temp.width+x] = bmul/sum;
		}
	}
	
	free(kernel);
	return 0;
}
#ifdef use_this_code
void GaussianSmooth(const Mat &src, Mat &dst, double sigma)
{
	if(src.channels() != 1 && src.channels() != 3)
		return;

	//
	sigma = sigma > 0 ? sigma : -sigma;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	//ksize为奇数
	int ksize = ceil(sigma * 3) * 2 + 1;

	//cout << "ksize=" <<ksize<<endl;
	//	dst.create(src.size(), src.type());
	if(ksize == 1)
	{
		src.copyTo(dst);	
		return;
	}

	//计算一维高斯核
	double *kernel = new double[ksize];

	double scale = -0.5/(sigma*sigma);
	const double PI = 3.141592653;
	double cons = 1/sqrt(-scale / PI);

	double sum = 0;
	int kcenter = ksize/2;
	int i = 0, j = 0;
	for(i = 0; i < ksize; i++)
	{
		int x = i - kcenter;
		*(kernel+i) = cons * exp(x * x * scale);//一维高斯函数
		sum += *(kernel+i);

//		cout << " " << *(kernel+i);
	}
//	cout << endl;
	//归一化,确保高斯权值在[0,1]之间
	for(i = 0; i < ksize; i++)
	{
		*(kernel+i) /= sum;
//		cout << " " << *(kernel+i);
	}
//	cout << endl;

	dst.create(src.size(), src.type());
	Mat temp;
	temp.create(src.size(), src.type());

	uchar* srcData = src.data;
	uchar* dstData = dst.data;
	uchar* tempData = temp.data;

	//x方向一维高斯模糊
	for(int y = 0; y < src.rows; y++)
	{
		for(int x = 0; x < src.cols; x++)
		{
			double mul = 0;
			sum = 0;
			double bmul = 0, gmul = 0, rmul = 0;
			for(i = -kcenter; i <= kcenter; i++)
			{
				if((x+i) >= 0 && (x+i) < src.cols)
				{
					if(src.channels() == 1)
					{
						mul += *(srcData+y*src.step+(x+i))*(*(kernel+kcenter+i));
					}
					else 
					{
						bmul += *(srcData+y*src.step+(x+i)*src.channels() + 0)*(*(kernel+kcenter+i));
						gmul += *(srcData+y*src.step+(x+i)*src.channels() + 1)*(*(kernel+kcenter+i));
						rmul += *(srcData+y*src.step+(x+i)*src.channels() + 2)*(*(kernel+kcenter+i));
					}
					sum += (*(kernel+kcenter+i));
				}
			}
			if(src.channels() == 1)
			{
				*(tempData+y*temp.step+x) = mul/sum;
			}
			else
			{
				*(tempData+y*temp.step+x*temp.channels()+0) = bmul/sum;
				*(tempData+y*temp.step+x*temp.channels()+1) = gmul/sum;
				*(tempData+y*temp.step+x*temp.channels()+2) = rmul/sum;
			}
		}
	}

	
	//y方向一维高斯模糊
	for(int x = 0; x < temp.cols; x++)
	{
		for(int y = 0; y < temp.rows; y++)
		{
			double mul = 0;
			sum = 0;
			double bmul = 0, gmul = 0, rmul = 0;
			for(i = -kcenter; i <= kcenter; i++)
			{
				if((y+i) >= 0 && (y+i) < temp.rows)
				{
					if(temp.channels() == 1)
					{
						mul += *(tempData+(y+i)*temp.step+x)*(*(kernel+kcenter+i));
					}
					else
					{
						bmul += *(tempData+(y+i)*temp.step+x*temp.channels() + 0)*(*(kernel+kcenter+i));
						gmul += *(tempData+(y+i)*temp.step+x*temp.channels() + 1)*(*(kernel+kcenter+i));
						rmul += *(tempData+(y+i)*temp.step+x*temp.channels() + 2)*(*(kernel+kcenter+i));
					}
					sum += (*(kernel+kcenter+i));
				}
			}
			if(temp.channels() == 1)
			{
				*(dstData+y*dst.step+x) = mul/sum;
			}
			else
			{
				*(dstData+y*dst.step+x*dst.channels()+0) = bmul/sum;
				*(dstData+y*dst.step+x*dst.channels()+1) = gmul/sum;
				*(dstData+y*dst.step+x*dst.channels()+2) = rmul/sum;
			}
		
		}
	}
	
	delete[] kernel;
}
#endif
