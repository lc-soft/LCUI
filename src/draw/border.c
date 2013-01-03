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


extern inline void 
fill_pixel( uchar_t **buff, int pos, LCUI_RGB color ) 
__attribute__((always_inline));

extern inline void 
fill_pixel( uchar_t **buff, int pos, LCUI_RGB color ) 
{
	buff[0][pos] = color.red;
	buff[1][pos] = color.green;
	buff[2][pos] = color.blue;
}

static int
Graph_Draw_RoundBorder_LeftTop( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_RGB line_color,	BOOL hide_outarea )
/* 绘制左上角的圆角，从左边框的上端到上边框的左端绘制一条圆角线 */
{
	LCUI_Rect real_rect;
	int pos, center_pos, y, x, i, n;
	int max_x, max_y, min_x, min_y;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	real_rect = Get_Graph_Valid_Rect( des );
	des = Get_Quote_Graph( des );
	if( !Graph_Valid( des ) ) {
		return -1;
	}
	/* 预先计算xy轴坐标的有效范围 */
	max_x = radius*2;
	if( center.x+radius > real_rect.x + real_rect.width ) {
		max_x -= (center.x+radius-real_rect.x-real_rect.width);
	}
	if( max_x < 0 ) {
		max_x = 0;
	}
	min_x = center.x>radius? 0:radius-center.x;
	if( min_x < 0 ) {
		min_x = 0;
	}
	max_y = radius*2;
	if( center.y+radius > real_rect.y + real_rect.height ) {
		max_y -= (center.y+radius-real_rect.y-real_rect.height);
	}
	min_y = center.y>radius? 0:radius-center.y;
	if( min_y < 0 ) {
		min_y = 0;
	}
	
	/* 预先计算圆心的线性坐标 */
	center_pos = (real_rect.y + center.y) * des->width;
	center_pos = center_pos + center.x + real_rect.x;

	/* 若需要隐藏圆外的区域 */
	if( hide_outarea && des->have_alpha ) {
		for( y=0; y<center.y-radius; ++y ) {
			pos = (real_rect.y+y)*des->width+real_rect.x;
			n = max_x-min_x;
			n += center.x - radius;
			memset( &des->rgba[3][pos], 0, n );
		}
	}
	/* 根据y轴计算各点的x轴坐标并填充点 */
	for( y=0; y<=radius; ++y, center_pos -= des->width ) {
		if( radius-y >= max_y || radius-y < min_y ) {
			continue;
		}
		/* 四舍五入，计算出x轴整数坐标 */
		x = sqrt( pow(radius, 2) - y*y )+0.5;
		
		if( line_width > 0 && radius-x >= min_x 
		 && radius-x <= max_x ) {
			pos = center_pos - x;
			fill_pixel( des->rgba, pos, line_color );
		}
		
		if( hide_outarea && des->have_alpha ) {
			/* 计算起点坐标 */
			pos = center_pos - center.x;
			if( radius-x > max_x ) {
				n = max_x - min_x;
			} else {
				n = radius-x-min_x;
			}
			/* 加上圆与背景图的左边距 */
			n += (center.x-radius);
			for(i=0; i<n; ++i) {
				des->rgba[3][pos++]=0;
			}
		}
		/* 计算需要向右填充的像素点的个数n */
		n = radius-x+line_width;
		n = n>radius ? x:line_width;
		/* 如果该点x轴坐标小于最小x轴坐标 */
		if( radius-x < min_x ) {
			/* 重新确定起点坐标pos和填充的像素点的个数n */
			pos = center_pos - radius+min_x-1;
			n -= (min_x-radius+x);
		} else {
			pos = center_pos - x;
		}
		/* 从下一个像素点开始 */
		++pos;
		/* 如果填充的像素点超出了最大x轴范围 */
		if( radius-x + n > max_x ) {
			/* 重新确定需要填充的像素点的个数n */
			n = max_x - radius + x;
		}
		/* 开始填充当前点右边的n-1个像素点 */
		for(i=0; i<n-1; ++i,++pos) {
			fill_pixel( des->rgba, pos, line_color );
		}
	}
	return 0;
}

static int
Graph_Draw_RoundBorder_RightTop( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_RGB line_color,	BOOL hide_outarea )
/* 绘制右上角的圆角，从右边框的上端到上边框的右端绘制一条圆角线 */
{
	LCUI_Rect real_rect;
	int pos, center_pos, y, x, i, n;
	int max_x, max_y, min_x, min_y;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	real_rect = Get_Graph_Valid_Rect( des );
	des = Get_Quote_Graph( des );
	if( !Graph_Valid( des ) ) {
		return -1;
	}
	
	max_x = radius*2;
	if( center.x+radius > real_rect.x + real_rect.width ) {
		max_x -= (center.x+radius-real_rect.x-real_rect.width);
	}
	if( max_x < 0 ) {
		max_x = 0;
	}
	min_x = center.x>radius? 0:radius-center.x;
	if( min_x < 0 ) {
		min_x = 0;
	}
	max_y = radius*2;
	if( center.y+radius > real_rect.y + real_rect.height ) {
		max_y -= (center.y+radius-real_rect.y-real_rect.height);
	}
	min_y = center.y>radius? 0:radius-center.y;
	if( min_y < 0 ) {
		min_y = 0;
	}
	
	center_pos = (real_rect.y + center.y) * des->width;
	center_pos = center_pos + center.x + real_rect.x;
	
	for( y=0; y<=radius; ++y, center_pos -= des->width ) {
		if( radius-y >= max_y || radius-y < min_y ) {
			continue;
		}
		x = sqrt( pow(radius, 2) - y*y )+0.5;
		
		if( line_width > 0 && radius+x >= min_x 
		 && radius+x <= max_x ) {
			pos = center_pos + x;
			fill_pixel( des->rgba, pos, line_color );
		}
		
		if( hide_outarea && des->have_alpha ) {
			n = center_pos + max_x - radius;
			if( radius+x < min_x ) {
				pos = center_pos + min_x - radius;
			} else {
				pos = center_pos + x;
			}
			for( ++pos; pos<=n; ++pos ) {
				des->rgba[3][pos] = 0;
			}
		}
		
		/* 计算需要向左填充的像素点的个数n */
		n = radius+x-line_width;
		n = n<min_x ? x+radius-min_x:line_width;
		
		if( radius+x > max_x ) {
			pos = center_pos - radius+max_x-1;
			n -= (radius+x-max_x);
		} else {
			pos = center_pos + x;
		}
		for(i=0; i<n; ++i,--pos) {
			fill_pixel( des->rgba, pos, line_color );
		}
	}
	return 0;
}

static int
Graph_Draw_RoundBorder_LeftBottom(
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_RGB line_color,	BOOL hide_outarea )
/* 绘制左下角的圆角，从左边框的下端到下边框的左端绘制一条圆角线 */
{
	LCUI_Rect real_rect;
	int pos, center_pos, y, x, i, n;
	int max_x, max_y, min_x, min_y;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	real_rect = Get_Graph_Valid_Rect( des );
	des = Get_Quote_Graph( des );
	if( !Graph_Valid( des ) ) {
		return -1;
	}
	
	max_x = radius*2;
	if( center.x+radius > real_rect.x + real_rect.width ) {
		max_x -= (center.x+radius-real_rect.x-real_rect.width);
	}
	if( max_x < 0 ) {
		max_x = 0;
	}
	min_x = center.x>radius? 0:radius-center.x;
	if( min_x < 0 ) {
		min_x = 0;
	}
	max_y = radius*2;
	if( center.y+radius > real_rect.y + real_rect.height ) {
		max_y -= (center.y+radius-real_rect.y-real_rect.height);
	}
	min_y = center.y>radius? 0:radius-center.y;
	if( min_y < 0 ) {
		min_y = 0;
	}
	
	center_pos = (real_rect.y + center.y) * des->width;
	center_pos = center_pos + center.x + real_rect.x;

	if( hide_outarea && des->have_alpha ) {
		
	}
	for( y=0; y<=radius; ++y, center_pos += des->width ) {
		if( radius+y >= max_y || radius+y < min_y ) {
			continue;
		}
		x = sqrt( pow(radius, 2) - y*y )+0.5;
		
		if( line_width > 0 && radius-x >= min_x 
		 && radius-x <= max_x ) {
			pos = center_pos - x;
			fill_pixel( des->rgba, pos, line_color );
		}
		if( hide_outarea && des->have_alpha ) {
			pos = center_pos - center.x;
			if( radius-x > max_x ) {
				n = max_x - min_x;
			} else {
				n = radius-x-min_x;
			}
			n += (center.x-radius);
			for(i=0; i<n; ++i) {
				des->rgba[3][pos++]=0;
			}
		}
		n = radius-x+line_width;
		n = n>radius ? x:line_width;
		if( radius-x < min_x ) {
			pos = center_pos - radius+min_x-1;
			n -= (min_x-radius+x);
		} else {
			pos = center_pos - x;
		}
		++pos;
		if( radius-x + n > max_x ) {
			n = max_x - radius + x;
		}
		for(i=0; i<n-1; ++i,++pos) {
			fill_pixel( des->rgba, pos, line_color );
		}
	}
	return 0;
}

static int
Graph_Draw_RoundBorder_RightBottom( 
	LCUI_Graph *des,	LCUI_Pos center,
	int radius,		int line_width,
	LCUI_RGB line_color,	BOOL hide_outarea )
/* 绘制右下角的圆角，从右边框的下端到下边框的右端绘制一条圆角线 */
{
	LCUI_Rect real_rect;
	int pos, center_pos, y, x, i, n;
	int max_x, max_y, min_x, min_y;
	
	if( line_width <= 0 && !hide_outarea ) {
		return 1;
	}
	
	real_rect = Get_Graph_Valid_Rect( des );
	des = Get_Quote_Graph( des );
	if( !Graph_Valid( des ) ) {
		return -1;
	}
	
	max_x = radius*2;
	if( center.x+radius > real_rect.x + real_rect.width ) {
		max_x -= (center.x+radius-real_rect.x-real_rect.width);
	}
	if( max_x < 0 ) {
		max_x = 0;
	}
	min_x = center.x>radius? 0:radius-center.x;
	if( min_x < 0 ) {
		min_x = 0;
	}
	max_y = radius*2;
	if( center.y+radius > real_rect.y + real_rect.height ) {
		max_y -= (center.y+radius-real_rect.y-real_rect.height);
	}
	min_y = center.y>radius? 0:radius-center.y;
	if( min_y < 0 ) {
		min_y = 0;
	}
	
	center_pos = (real_rect.y + center.y) * des->width;
	center_pos = center_pos + center.x + real_rect.x;
	
	for( y=0; y<=radius; ++y, center_pos += des->width ) {
		if( radius+y >= max_y || radius+y < min_y ) {
			continue;
		}
		x = sqrt( pow(radius, 2) - y*y )+0.5;
		
		if( line_width > 0 && radius+x >= min_x 
		 && radius+x <= max_x ) {
			pos = center_pos + x;
			fill_pixel( des->rgba, pos, line_color );
		}
		
		if( hide_outarea && des->have_alpha ) {
			n = center_pos + max_x - radius;
			if( radius+x < min_x ) {
				pos = center_pos + min_x - radius;
			} else {
				pos = center_pos + x;
			}
			for( ++pos; pos<=n; ++pos ) {
				des->rgba[3][pos] = 0;
			}
		}
		
		n = radius+x-line_width;
		n = n<min_x ? x+radius-min_x:line_width;
		
		if( radius+x > max_x ) {
			pos = center_pos - radius+max_x-1;
			n -= (radius+x-max_x);
		} else {
			pos = center_pos + x;
		}
		for(i=0; i<n; ++i,--pos) {
			fill_pixel( des->rgba, pos, line_color );
		}
	}
	return 0;
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
	
	/* 绘制左上角的圆角，先引用左上角区域，再将圆绘制到这个区域里 */
	radius = border.top_left_radius;
	rect = Rect( 0, 0, radius, radius );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder_LeftTop( 
		&des_area		, Pos( radius, radius ), 
		radius			, border.left_width, 
		border.left_color	, TRUE 
	);
	
	/* 右上角 */
	radius = border.top_right_radius;
	rect = Rect( des->width-radius-1, 0, radius, radius );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder_RightTop( 
		&des_area		, Pos( 0, radius ), 
		radius			, border.right_width, 
		border.right_color	, TRUE 
	);
	
	/* 左下角 */
	radius = border.bottom_left_radius;
	rect = Rect( 0, des->height-radius, radius, radius );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder_LeftBottom( 
		&des_area		, Pos( radius, 0 ), 
		radius			, border.left_width, 
		border.left_color	, TRUE 
	);
	
	/* 右下角 */
	radius = border.bottom_left_radius;
	rect = Rect(	des->width-radius-1, 
			des->height-radius, radius, radius );
	Quote_Graph( &des_area, des, rect );
	Graph_Draw_RoundBorder_RightBottom( 
		&des_area		, Pos( 0, 0 ), 
		radius			, border.right_width, 
		border.right_color	, TRUE 
	);
	
	/* 绘制上边框 */
	k = des->width;
	for(y=0;y<border.top_width;++y) {
		k = y * des->width;
		for(x = border.top_left_radius; x < w[0]; ++x) {
			count = k + x;
			des->rgba[0][count] = border.top_color.red;
			des->rgba[1][count] = border.top_color.green;
			des->rgba[2][count] = border.top_color.blue;
			if( des->have_alpha ) {
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
			if( des->have_alpha ) {
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
			if( des->have_alpha ) {
				des->rgba[3][count] = 255;
			}
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
			if( des->have_alpha ) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* 边框线绘制完成 */
	Graph_Unlock(des);
	return 0;
}
