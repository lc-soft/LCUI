/*
 * lib/ui-xml/include/ui_xml.h: -- The GUI build module, parse UI config code and build UI.
 *
 * Copyright (c) 2018-2022-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_XML_INCLUDE_XML_H
#define LIB_UI_XML_INCLUDE_XML_H

#include "ui_xml/common.h"
#include <ui.h>

LIBUI_XML_BEGIN_DECLS

/**
 * 从字符串中载入界面配置代码，解析并生成相应的图形界面(元素)
 * @param[in] str 包含界面配置代码的字符串
 * @return 正常解析会返回一个部件，出现错误则返回 NULL
 */
LIBUI_XML_PUBLIC ui_widget_t* ui_load_xml_string(const char *str, int size);

/**
 * 从文件中载入界面配置代码，解析并生成相应的图形界面(元素)
 * @param[in] filepath 文件路径
 * @return 正常解析会返回一个部件，出现错误则返回 NULL
 */
LIBUI_XML_PUBLIC ui_widget_t* ui_load_xml_file(const char *filepath);

LIBUI_XML_END_DECLS

#endif
