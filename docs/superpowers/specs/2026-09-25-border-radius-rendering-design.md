# 圆角边框渲染修复设计

## 问题

Badge 使用大于自身高度一半的圆角时，底部边框会产生尖端与 1px 残影。原因是 UI 层计算角半径上限时将宽度误用两次，半径只被限制为 `width / 2`。

## 浏览器兼容目标

CSS Border Radius 规范在相邻圆角总和超过盒子宽度或高度时按比例缩小圆角。对于四角相同的 Badge，结果等价于将半径限制为 `min(width, height) / 2`，形成胶囊形而不形成椭圆尖端。

## 方案

- 在 `ui_widget_compute_border()` 使用 `min(border_box.width, border_box.height) / 2` 限制每个角半径。
- 不在 Badge 内部进行尺寸特判；所有 Widget 共享同一正确约束。
- 保持 PandaGL 的圆角绘制算法不变，除非像素测试证明在已规范化半径下仍有残影。

## 验证

- 新增渲染或像素测试，覆盖宽大于高且 `border-radius: 26px` 的 1px 实线边框。
- 断言四个角没有越出 border box，且上下边界不含多余边框像素。
- 保留现有 CSS 解析测试；新增测试归属 `lib/ui/tests` 或跨库时放 `tests/integration`。