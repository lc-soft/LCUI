
typedef enum _font_style
{ 
	FONT_STYLE_NORMAL = 0, 
	FONT_STYLE_ITALIC = 1, 
	FONT_STYLE_OBIQUE = 2 
}
enum_font_style;

typedef enum _font_weight
{
	FONT_WEIGHT_NORMAL	= 0,
	FONT_WEIGHT_BOLD	= 1 
} 
enum_font_weight;

typedef enum _font_decoration
{
	FONT_DECORATION_NONE		= 0,	/* 无装饰 */
	FONT_DECORATION_BLINK		= 1,	/* 闪烁 */
	FONT_DECORATION_UNDERLINE	= 2,	/* 下划线 */
	FONT_DECORATION_LINE_THROUGH	= 3,	/* 贯穿线 */
	FONT_DECORATION_OVERLINE	= 4	/* 上划线 */
}
enum_font_decoration; 


typedef struct _LCUI_TextStyle
{
	BOOL _family:1;
	BOOL _style:1;
	BOOL _weight:1;
	BOOL _decoration:1;
	BOOL _back_color:1;
	BOOL _fore_color:1;
	BOOL _pixel_size:1;
	
	char family[256];
	enum_font_style	style		:3;
	enum_font_weight	weight		:3;
	enum_font_decoration	decoration	:4;
	
	LCUI_RGB fore_color;
	LCUI_RGB back_color;
	
	int pixel_size;	
}
LCUI_TextStyle;
