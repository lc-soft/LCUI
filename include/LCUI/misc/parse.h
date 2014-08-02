#ifndef __LCUI_MISC_PARSE_H__
#define __LCUI_MISC_PARSE_H__

/** 样式变量类型 */
typedef enum StyleVarType {
	SVT_NONE,
	SVT_SCALE,		/**< 比例 */
	SVT_PX,			/**< 像素 */
	SVT_PT,			/**< 点 */
	SVT_COLOR		/**< 色彩 */
} SVT;

/** 用于描述样式的变量类型 */
typedef struct StyleVar {
	SVT type;			/**< 哪一种类型 */
	union {
		int px;			/**< pixel, 像素 */
		int pt;			/**< point，点数（一般用于字体大小单位） */
		double scale;		/**< 比例 */
		LCUI_Color color;	/**< 色彩值 */
	};
} StyleVar;

#endif
