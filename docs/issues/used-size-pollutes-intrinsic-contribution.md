# Used size 污染 intrinsic contribution

- 状态：已知问题 / 暂缓
- 严重程度：Medium（中等）。该问题会造成布局无法随内容收缩和出现多余留白，但不会导致崩溃或内存安全问题，且以固定父尺寸为主的普通空间分配通常不受影响。
- 发现日期：2026-07-27

## 现象

在 auto 或 fit-content 父级内放置百分比 flex item，并由固定宽度子元素提供 intrinsic contribution。初始子元素宽度为 `100px` 时，布局结果为：

- group content width：`333.344px`
- item border width：`100.003px`
- child border width：`100px`

将 child 宽度从 `100px` 改为 `50px` 并执行更新后，child 会变为 `50px`，但 group 仍为 `333.344px`，item 仍为 `100.003px`，二者均不收缩。

## 根因

`ui_widget_update_size()` 将 `content_box` 的 used size 写入 `max_content_width` 和 `max_content_height`。后续 fit-content 和 percentage intrinsic 计算会读取这些 `max_content_*` 值，因此上一轮布局得到的 used size 被误当成新的 intrinsic contribution。内容缩小后，历史尺寸继续参与计算，阻止父级和百分比 item 收缩。

## 影响范围

已确认或直接相关的场景包括：

- auto 或 fit-content 父级；
- 百分比 flex item；
- 子内容缩小或移除；
- 局部 style refresh 后重新布局。

以固定父尺寸为主的普通空间分配通常不受影响。Block、flex 以及其他读取 `max_content_*` 的路径也可能受影响，仍需进一步调查。

## 风险

- 容器和 item 无法随内容收缩；
- 页面产生多余留白；
- 最终布局依赖历史 used size，可能因更新顺序不同而不同；
- 该问题不属于崩溃或内存安全风险。

## 当前修复为何不包含此问题

当前 percentage wrap 修复处理百分比 flex item 的 intrinsic 宽度求解、换行和 gap 分配。used size 污染 intrinsic contribution 属于尺寸状态建模和更新生命周期问题，修复需要重新划分 `ui_widget_update_size()` 及其调用方维护的尺寸语义，并审计 block、flex 等共享读取路径。将其并入当前修复会显著扩大范围和回归风险，因此暂缓处理。当前修复不应被视为已经解决此问题。

## 未来修复方向

1. 区分 pure intrinsic contribution 与布局产生的 used size，避免共用 `max_content_width` / `max_content_height` 状态。
2. 审计所有 `max_content_*` 的写入点和读取点，明确每条路径需要 intrinsic size 还是 used size。
3. 保持 load 阶段为纯读取，禁止在 load 阶段触发 child reflow。
4. 使用 TDD 重新添加 child 从 `100px` 缩小到 `50px` 后 group 和 item 同步收缩的回归测试。

## 复现步骤

伪代码：

```c
group = create_fit_content_flex_group();
item = append_percentage_flex_item(group, 30_percent);
child = append_fixed_width_child(item, 100_px);
ui_update();

assert(group->content_box.width == 333.344_px);
assert(item->border_box.width == 100.003_px);

set_width(child, 50_px);
ui_update();

// 当前已知问题：child 已缩至 50px，但以下两个尺寸保持不变。
assert(group->content_box.width == 333.344_px);
assert(item->border_box.width == 100.003_px);
```

可使用 `tests/integration/test_flex_layout_percentage_wrap.c` 中的 `intrinsic percentage flex item` 场景作为初始边界覆盖；重新加入动态收缩断言前，应先完成尺寸语义修复。

## 验收标准

- child 从 `100px` 缩小到 `50px` 后，group 和百分比 item 在同一次正常更新流程中收缩到新的 intrinsic 解；
- item 的 content box 仍能容纳 child；
- 结果不依赖此前布局得到的 used size，也不依赖额外的全树 style refresh；
- 修复不在 load 阶段触发 child reflow；
- percentage wrap、固定父尺寸分配以及 block/flex 相关布局测试全部通过；
- 回归测试采用 TDD 恢复并覆盖内容缩小和移除场景。

## 相关文件

- `tests/integration/test_flex_layout_percentage_wrap.c`
- `tests/fixtures/test_flex_layout_percentage_wrap.xml`
- `tests/fixtures/test_flex_layout_percentage_wrap.css`
- `lib/ui/src/ui_updater.c`
- `lib/ui/src/ui_widget_layout.c`
- `lib/ui/src/ui_flexbox_layout.c`
- `lib/ui/src/ui_block_layout.c`
- `lib/ui/src/ui_resizer.c`
