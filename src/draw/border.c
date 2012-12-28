#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <math.h>

void Border_Init( LCUI_Border *border )
/* 初始化边框数据 */
{
	border->top_width = 0;
	border->bottom_width = 0;
	border->left_width = 0;
	border->right_width = 0;
	border->top_style = BORDER_STYLE_NONE;
	border->bottom_style = BORDER_STYLE_NONE;
	border->left_style = BORDER_STYLE_NONE;
	border->right_style = BORDER_STYLE_NONE;
	border->top_color = RGB(0,0,0);
	border->bottom_color = RGB(0,0,0);
	border->left_color = RGB(0,0,0);
	border->right_color = RGB(0,0,0);
	border->top_left_radius = 0;
	border->top_right_radius = 0;
	border->bottom_left_radius = 0;
	border->bottom_right_radius = 0;
}

LCUI_Border Border( int width_px, BORDER_STYLE style, LCUI_RGB color )
/* 简单的设置边框样式，并获取该样式数据 */
{
	LCUI_Border border;
	border.top_width = width_px;
	border.bottom_width = width_px;
	border.left_width = width_px;
	border.right_width = width_px;
	border.top_style = style;
	border.bottom_style = style;
	border.left_style = style;
	border.right_style = style;
	border.top_color = color;
	border.bottom_color = color;
	border.left_color = color;
	border.right_color = color;
	border.top_left_radius = 0;
	border.top_right_radius = 0;
	border.bottom_left_radius = 0;
	border.bottom_right_radius = 0;
	return border; 
}

void Border_Radius( LCUI_Border *border, int radius )
/* 设置边框的圆角半径 */
{
	border->top_left_radius = radius;
	border->top_right_radius = radius;
	border->bottom_left_radius = radius;
	border->bottom_right_radius = radius;
}

static int 
Graph_Draw_RoundBorder( 
	LCUI_Graph *des, LCUI_Pos center, 
	int radius, int line_width, 
	LCUI_RGB line_color, BOOL hide_outarea )
/* 
 * 功能：为图层绘制圆角边框 
 * 参数说明：
 * 	des		目标图层
 * 	center		圆心在目标图层中的坐标
 * 	radius		圆的半径
 * 	line_width	线条宽度
 * 	line_color	线条颜色
 * 	hide_outarea	指示是否需要隐藏圆外的区域
 * */
{
	int pos, k, j, y, x;
	LCUI_Rect real_rect;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	/* 递归调用，先绘制最里面的同心圆，并且，对圆的外部区域进行隐藏处理 */
	Graph_Draw_RoundBorder( des, center, radius-1, 
		line_width-1, line_color, FALSE );
	/* 里面一层圆绘制完了后，再绘制这一层圆 */
	real_rect = Get_Graph_Valid_Rect( des );
	des = Get_Quote_Graph( des );
	if( !Graph_Valid( des ) ) {
		return -1;
	}
	
	/* 先记录起点的线性坐标 */
	k = (real_rect.y + center.y) * des->width;
	k = j = k + center.x + real_rect.x;
	/* 根据y轴计算各点的x轴坐标并填充点 */
	for(y=0; y<radius; ++y) {
		/* 四舍五入，计算出x轴整数坐标 */
		x = sqrt( pow(radius, 2) - y*y )+0.5;
		if( center.x-x >= 0 
		 && center.x-x < real_rect.x+real_rect.width) {
			if( center.y-y >= 0
			 && center.y-y < real_rect.y+real_rect.height ) {
				/* 左上半圆 */
				pos = j - x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
			if( center.y+y < real_rect.y+real_rect.height ) {
				/* 左下半圆 */
				pos = k - x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
		}
		if( center.x+x < real_rect.x+real_rect.width ) {
			/* 右上半圆 */
			if( center.y-y >= 0
			 && center.y-y < real_rect.y+real_rect.height ) {
				pos = j + x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
			if( center.y+y < real_rect.y+real_rect.height ) {
				/* 右下半圆 */
				pos = k + x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
		}
		/* 
		 * 由于圆是y轴上下对称，可以用变量k记录下半圆的下一行点的y轴坐标，
		 * 而变量j则相反
		 * */
		k += des->width;
		j -= des->width;
	}
	/* 
	 * 那个 pos = k - y * des->width - x; 之前是这样的：
	 * pos = (center.y-y)*des->width+center.x-x;
	 * 式子可以转换成这样：
	 * pos = (center.y*des->width+center.x)-y*des->width-x;
	 * 鉴于center.y*des->width+center.x的运算结果是不变的，所以，
	 * 每次就不必重复将它们代入进去进行运算，在运算前，一次性计算出它的
	 * 值即可。
	 * */
	/* 先计算固定的二维坐标对应的线性坐标 */
	k = (real_rect.y+center.y) * des->width;
	k = k + center.x + real_rect.x;
	/* 根据x轴计算各点的y轴坐标并填充点 */
	for(x=0; x<radius; ++x) {
		y = sqrt( pow(radius, 2) - x*x )+0.5;
		if( center.y-y >= 0
		 && center.y-y < real_rect.y+real_rect.height ) {
			if( center.x-x >= 0 
			 && center.x-x < real_rect.x+real_rect.width) {
				pos = k - y * des->width - x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
			if( center.x+x < real_rect.x+real_rect.width ) {
				pos = k - y * des->width + x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
		}
		if( center.y+y < real_rect.y+real_rect.height ) {
			if( center.x-x >= 0 
			 && center.x-x < real_rect.x+real_rect.width) {
				pos = k + y * des->width - x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
			if( center.x+x < real_rect.x+real_rect.width ) {
				pos = k + y * des->width + x;
				des->rgba[0][pos] = line_color.red;
				des->rgba[1][pos] = line_color.green;
				des->rgba[2][pos] = line_color.blue;
			}
		}
	}
	return 0;
}

static int max( int a, int b )
{
	return a>b?a:b;
}

int Graph_Draw_Border( LCUI_Graph *des, LCUI_Border border )
/* 简单的为图形边缘绘制边框 */
{
	if( !Graph_Valid(des) ) {
		return -1;
	}
	
	LCUI_Graph des_area;
	LCUI_Rect rect;
	int  radius, x, y,count, k, w[2], h[2], start_x, start_y;
	
	w[0] = des->width - border.top_right_radius;
	h[0] = des->height - border.bottom_left_radius;
	w[1] = des->width - border.bottom_right_radius;
	h[1] = des->height - border.bottom_right_radius;
	
	Graph_Lock(des, 1);
	
	radius = border.top_left_radius;
	/* 绘制左上角的圆角，先引用左上角区域，再将圆绘制到这个区域里 */
	rect = Rect( 0, 0, radius, radius );
	count = max( border.left_width, border.top_width );
	Quote_Graph( &des_area, des, rect );
	/* 根据边框粗细程度，绘制相应数量的同心圆 */
	Graph_Draw_RoundBorder( &des_area, Pos(radius,radius), 
		radius, count, RGB(0,0,0), FALSE );
	
	/* 绘制右上角的圆角 */
	radius = border.top_right_radius;
	rect = Rect( des->width-radius-1, 0, radius, radius );
	count = max( border.right_width, border.top_width );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder( &des_area, 
		Pos(0,radius), radius, count, RGB(0,0,0), FALSE );
	
	/* 绘制左下角的圆角 */
	radius = border.bottom_left_radius;
	rect = Rect( 0, des->height-radius-1, radius, radius );
	count = max( border.left_width, border.bottom_width );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder( &des_area, 
		Pos(radius,0), radius, count, RGB(0,0,0), FALSE );
	
	/* 绘制右下角的圆角 */
	radius = border.bottom_right_radius;
	rect = Rect( des->width-radius-1, 
		des->height-radius-1, radius, radius );
	count = max( border.right_width, border.bottom_width );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder( &des_area, 
		Pos(0,0), radius, count, RGB(0,0,0), FALSE );
	
	/* 绘制上边框 */
	k = des->width;
	for(y=0;y<border.top_width;++y) {
		k = y * des->width;
		for(x = border.top_left_radius; x < w[0]; ++x) {
			count = k + x;
			des->rgba[0][count] = border.top_color.red;
			des->rgba[1][count] = border.top_color.green;
			des->rgba[2][count] = border.top_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	start_y = des->height - border.bottom_width;
	/* 绘制下边的线 */
	for(y=0;y<border.bottom_width;++y) {
		k = (start_y+y) * des->width;
		for(x = border.bottom_left_radius; x < w[1]; ++x) {
			count = k + x;
			des->rgba[0][count] = border.bottom_color.red;
			des->rgba[1][count] = border.bottom_color.green;
			des->rgba[2][count] = border.bottom_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* 绘制左边的线 */
	for(y=border.top_left_radius; y<h[0]; ++y) {
		k = y * des->width;
		for(x=0;x<border.left_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.left_color.red;
			des->rgba[1][count] = border.left_color.green;
			des->rgba[2][count] = border.left_color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* 绘制右边的线 */
	start_x = des->width - border.right_width;
	for(y=border.top_right_radius; y<h[1]; ++y) {
		k = y * des->width + start_x;
		for(x=0;x<border.right_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.right_color.red;
			des->rgba[1][count] = border.right_color.green;
			des->rgba[2][count] = border.right_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* 边框线绘制完成 */
	Graph_Unlock(des);
	return 0;
}
