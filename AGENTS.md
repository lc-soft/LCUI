## 代码格式

遵循 .clang-format 文件中定义的规则，修改文件后需格式化。

### 缩进

使用八个空格缩进。

### 代码结构

源文件中的代码结构应该为：

1. 预处理指令
2. 类型
3. 全局变量
4. 函数声明
5. 函数定义

其中“函数定义”应该按照功能类别分组、按依赖关系从基础到高级的顺序排列，例如：

```c
static int var1;
static int var1;

static void func1();
static void func2();

// object 1

void object1_func1()
{
  // ..
}

void object1_func2() {
    // ...
     object1_func1();
}


// object 2

void object2_func1()
{
  // ..
}

void object2_func2() {
    // ...
     object2_func1();
}
```

注意！优先通过调整函数定义代码块的顺序来解决声明问题，而不是前置声明函数。

### 未使用参数

未使用的函数参数（包括回调签名里必须存在的 `void *arg`、事件回调的 `ui_event_t *e` 等）
**不要写 `(void)xxx;`**。GCC、Clang、MSVC 都默认不警告未使用参数名，只有未使用局部变量
才会警告。`(void)xxx;` 是冗余噪音，掩盖真正该处理的警告。

❌ 禁止：
```c
static void on_event(ui_widget_t *w, ui_event_t *e, void *arg)
{
    (void)w;
    (void)e;
    (void)arg;
    /* 实际逻辑 */
}
```

✅ 允许：
```c
static void on_event(ui_widget_t *w, ui_event_t *e, void *arg)
{
    /* 直接用到的参数正常使用；用不到的参数名直接保留在签名里 */
}
```

> 例外：C++ 模式下某些编译器会警告未使用参数，需要按上下文决定。LCUI 是纯 C，
> 不适用此例外。

### 事件处理器参数

`ui_widget_on(widget, event, handler, listener_data)` 的第四个参数是监听器上下文，
事件分发时通过 `e->data` 传给处理函数：

```c
static void on_click(ui_widget_t *w, ui_event_t *e, void *emit_arg)
{
        app_state_t *state = e->data;
}

ui_widget_on(button, "click", on_click, state);
```

处理函数的第三个参数 `emit_arg` 来自 `ui_widget_emit_event(widget, event,
emit_arg)`，不是 `ui_widget_on()` 的第四个参数。常规监听器必须从 `e->data`
获取通过 `ui_widget_on()` 绑定的上下文；只有显式发射事件传入的上下文才使用
处理函数的第三个参数。

## lcui-toolkit 本地开发包与依赖安装

- 排查 `@lcui/cli`、`@lcui/react`、`@lcui/fluent-icons` 等 lcui-toolkit 相关包的问题时，先回顾用户是否提过本地开发版仓库路径（例如 `F:\code\lcui-toolkit\packages`）。
- 如果用户提供过本地开发版路径，先检查当前实际使用的包是否软连接/目录联接到对应本地包；同时检查项目内 `node_modules` 与全局 `lcui` CLI 的实际解析路径。用 `Get-Item -Force` 的 `LinkType`/`Target` 或等效方法核实，不要只依据包名、版本号或 `NODE_PATH` 推断。
- 如果相关包没有链接到用户提供的本地开发版，汇报包名、当前解析路径和链接状态后暂停工作，请用户手动修复链接。不要改用 npm 发布版继续排错，不要自行 `npm link`、创建 junction/symlink、复制包文件或改包源码绕过问题。
- 执行 `npm install` / `npm ci` 前，先检查并记录所有 lcui-toolkit 相关包的链接目标。如果它们已经链接到本地开发版，安装后必须恢复到相同目标并再次核实；如果用户曾提供本地开发版路径但链接缺失，按上一条汇报并暂停，不要执行安装。

## 测试用例

### 归属规则

按下面优先级判定一个测试归属：

1. case 实际 `#include` 的库头文件集合（含传递依赖）。
2. 调用的运行时入口（`lcui_init` / `ui_init` / `pd_*_init` 等）。
3. xmake target 上需要的 `add_deps`。

仅触达单 lib（外加 yutil/ctest/标准库）的测试放 `lib/<name>/tests/`；触达两个及以上同级 lib，或依赖 `src/widgets/` 注册的 widget 类型的测试放 `tests/integration/`。

### 文件与函数命名

- 文件名：`test_<topic>.c`
- 套件入口（lib 内）：`void test_<lib>_<topic>(void)`，如 `test_ui_xml_parser`、`test_pandagl_image_reader`
- 套件入口（顶层集成）：`void test_<topic>(void)`，无前缀，如 `test_settings`
- 套件入口必须在两个位置都注册：所属 lib 的 `tests/main.c` 与顶层 `tests/main.c` 中的 `suites[]` 表
- 内部分组用 `static void <动词>_<group>(void)`，由 `ctest_describe` 注册

### 描述文本风格

- `ctest_describe(name, fn)` 的 `name` 是名词性主题，全小写空格分词，无 `test` 前缀。例：`"widget opacity"`、`"flex layout"`、`"settings.fps_cap"`
- `ctest_equal_*(name, ...)` 的 `name` 用 `should ...` 行为陈述。例：`"should default fps_cap to 120"`、`"should match parent border color"`
- 当上下文清晰（例如 layout case 中描述某 selector 对应的 box）时，可保留 jQuery 选择器风格的描述，无需强行加 should

### 资源文件

- 跨 lib 共享的 fixture 放 `tests/fixtures/`
- 仅本 lib 用的 fixture 也建议复制到 `tests/fixtures/`（顶层 lcui-tests 与单 lib binary 共用同一 rundir）
- xmake target 的 `set_rundir` 指向 `tests/fixtures/`
- 测试代码加载资源时直接用文件名，不带目录前缀

### 三种文件职责

- `tests/integration/test_<topic>.c`：跨 lib 集成测试，自动断言。不调用 `lcui_main`，必要时由 `tests/previews/preview_<topic>.c` 提供可视诊断
- `tests/scenes/<topic>_scene.{c,h}`：可视化场景搭建模块，签名 `void <topic>_scene_build(...)`。只构造 widget 树和应用样式，不做断言、不调用 `ctest_*`、不调用 `lcui_main`/`lcui_quit`。给 cases 与未来的 examples demo 共用
- `lib/<name>/tests/test_<topic>.c`：纯 lib 测试，仅断言

### 编写示例

```c
#include <ctest-custom.h>

void test_my_case(void)
{
        ctest_equal_int("should add two numbers", 1 + 1, 2);
}
```

注册：

```c
/* lib/<name>/tests/main.c 或 tests/main.c */
extern void test_my_case(void);

static const ctest_suite_t suites[] = {
        { "my case", test_my_case },
        { NULL, NULL }
};

CTEST_MAIN(suites)
```

### 运行

- 全量：`xmake test`
- 按 pattern 过滤：`xmake test "*/widget*"`（匹配 target/test 名）
- 按 group 过滤：`xmake test -g tests`
- 单 binary 跑全部 suite：`xmake run <lib>-tests`，例如 `xmake run lcui-tests`
- 单 binary 内细粒度过滤：`xmake run lcui-tests --grep="<pattern>"`，子串匹配 suite 名
- 不要使用 `xmake test "lcui-tests/*" -- --grep "<pattern>"`：当前参数转发不会让
  `--grep` 在 `lcui-tests` 中生效，实际会运行整个 binary
- 单 binary 列出 suite：`xmake run lcui-tests --list`
- 内存检查：`xmake f --memcheck=y && xmake test`，调用 drmemory（Windows）或 valgrind（Linux）；恢复正常运行：`xmake f --memcheck=n`

### 不要触碰

- `lib/yutil/tests/`：使用旧 libtest 框架自管理，不并入 ctest 体系，不被 lcui-tests 收集

## 重构约定

### 合并重复分支

当存在两个分支仅输入不同但后续处理相同（如选择 obs->root 或 ui_root），用局部变量合并公共逻辑，避免重复代码。

```c
/* before */
if (ctx->logger) {
    write_log(ctx->logger, msg);
} else {
    logger_t *logger = get_default_logger();
    write_log(logger, msg);
}

/* after */
logger_t *logger = ctx->logger ? ctx->logger : get_default_logger();
write_log(logger, msg);
```

### 提取公共逻辑

若公共逻辑较长，可提取为独立函数，并保持调用路径一致，减少分叉实现。

### 优先用属性组合复用部件样式，而非新建带样式子类型

当某个角色（如关闭按钮、确认按钮）本质上是既有部件的一种用法时，不要为它新建子类型原型
（例如 `dialog-close` 继承自 `button`），而应直接用既有部件 + `data-*` 属性 + class 组合出该角色。
原因：

- LCUI 的 CSS 类型选择器只精确匹配 `w->type` 字符串，不做原型链匹配。子类型的 `w->type` 是
  子类型名，匹配不到 `button:ghost`、`button:icon` 这类基于 `button` 的变体规则，只能重写一份
  近似样式，造成重复与视觉漂移。
- 行为绑定走 `data-*` 属性（如 `data-dialog-close`）而非类型判断，与 select 等部件的声明式关联
  协议一致；工厂函数负责把属性设好（`variant`/`size`/`data-*`/`tabindex`），调用方零配置即可用。
- 公共工厂保留语义化名字（如 `ui_create_dialog_close()`），但返回的是组合好的标准 `button`
  部件；文档与字段表仍按角色（DialogClose）描述。

```c
/* before：子类型匹配不到 button:ghost/button:icon，需要为 dialog-close 重写整套视觉 */
ui_dialog_close_prototype = ui_create_widget_prototype("dialog-close", "button");
/* css 里被迫写 dialog-close:ghost { ... } —— 与 button:ghost 重复 */

/* after：标准部件 + 属性组合，复用 button 全部变体样式 */
ui_widget_t *close = ui_create_widget("button");
ui_widget_add_class(close, "dialog-close");            /* 仅作绝对定位钩子，不定义视觉 */
ui_widget_set_attr(close, "variant", "ghost");
ui_widget_set_attr(close, "size", "icon");
ui_widget_set_attr(close, "data-dialog-close", "true"); /* 行为靠属性绑定 */
close->tab_index = 0;
```

这相当于各大 React 组件库里 `asChild` / `render` 的思路：角色部件不自带视觉实现，而是以组合
方式借用基础部件（Button）的外观与交互，避免样式实现重复。

## LCUI CSS 引擎约束

### 选择器

- 仅支持：通配符 `*`、类型 `type`、类 `.cls`、ID `#id`、后代空格 `A B`
- 不支持：子代 `>`、相邻兄弟 `+`、通用兄弟 `~`
- 不支持：属性选择器 `[attr]` `[data-x]`
- 不支持：功能性伪类 `:not()` `:is()` `:where()` `:has()` `:nth-child()`（解析器无 `(` 语法）
- 不支持：伪元素 `::before` `::after`

### 优先级

- Rank：`GENERAL=0`，`TYPE=1`，`CLASS=10`，`PCLASS=10`，`ID=100`
- 同 rank 用 `batch_num`（声明顺序）决胜，后声明覆盖先声明
- 无 `!important` 机制
- class 与 pclass rank 相同（都是 10）

### 属性与值

- `position`：仅 `static` / `relative` / `absolute`，无 `fixed`
- `white-space`：仅 `normal` / `nowrap`
- `border-style`：仅 `none` / `solid`
- 无 `calc()` / `var()` / CSS 自定义属性
- 无 `calc()` / `var()` / `em` / `rem` / `vh` / `vw` 单位（可用 `dp` / `px` / `pt` / `%`）
- 无 `overflow` 属性

### 图标尺寸

- `icon` 部件（fluent-icons）按字体符号渲染，尺寸由 `font-size` 决定，
  不像 SVG 可以通过宽高工具类（`size-4` / `w-4` / `h-4`）控制；
  给图标写 `size-*` 类是无效的，属于常见错误。

### 带图标按钮

- 带图标且带标签的 `button`，标签必须作为独立的 `text` 子部件追加；不得使用
  `ui_button_set_text()` 或直接文本子节点。Button 自身的文本排版不会参与子部件布局，
  否则图标与标签无法按预期共同排版。
- C 使用 `ui_create_widget("text")` 和 `ui_text_set_content()` 创建标签后追加到 Button；
  XML 使用 `<text>标签</text>`；TSX 使用 `<Text>标签</Text>`。仅图标按钮不适用此规则。

## 内置样式构建（style-nova.css）

- `src/styles/style-nova.css` 里的 `@apply` 会被 `lcui build` 展开成生成文件
  `src/styles/style-nova.css.h`，库与集成测试链接的是这个 `.h`，不是 `.css` 源。
- 修改 `style-nova.css` 后必须重新生成：`npm run build-styles`
  （等价于 `lcui build src/styles/style-nova.css --force --skip-xmake`），
  否则改动不会进入 `lcui.lib`，测试与运行时仍按展开前的旧样式生效。
- `style-nova.css` 会先经 Sass 处理，**注释里不能出现 `*/`**（例如把 `data-*` 写成
  `data-*/foo` 会提前闭合注释），否则残留文本被当作选择器解析，报 `expected selector`。

## 新组件开发

- 新增内建部件时，必须同时提供 `include/LCUI/widgets/<name>.h` 公共 API、
  `src/widgets/<name>.c` 实现、`include/LCUI/widgets.h` 聚合导出，以及
  `src/lcui_widgets.c` 注册。
- 有复杂结构或交互的部件，C API 应提供工厂、build 和状态/属性 setter，方便 C 调用方
  按需构造默认结构；同时必须允许 TSX 直接声明子树，C 端优先绑定显式子节点，缺失时才
  补齐默认节点。可参考 `src/widgets/select.c`。
- 每个内建部件须有聚焦的 `tests/integration/test_<topic>.c`，覆盖公共 API、声明式
  结构、状态/事件和销毁。测试注册到 `tests/main.c`。
- 每个部件文档须有 `docs/widget-fields/<name>.ts`、中文 MDX，以及 C、XML、TSX 三种
  demo，并更新 `docs/sidebars.json`。先完成并审核中文文档，再一次性同步英文；中文未
  确认前不得创建或修改对应英文页面。
- 部件文档、示例（C / XML / TSX 三变体）、`widget-fields`、demo 类名约定等编写规范，
  统一以 `examples/doc-viewer/AGENTS.md` 为**唯一权威来源**；编写或修改部件文档 / 示例
  前必须先读该文件，不在本文件重复定义、避免与其冲突。

### 属性设置函数与 on_setattr

- 部件对外的属性设置函数（`ui_<widget>_set_*`）参数应优先使用能表达取值范围的类型：
  布尔值用 `bool`，数值用 `int` / `float`，固定取值集合用 `enum`；不要让
  `const char *` 承担枚举语义。
- 字符串只作为声明层输入：`ui_widget_set_attr()`、XML 属性、TSX props。部件原型的
  `setattr`（`on_setattr`）负责把字符串解析成上述类型并同步到内部状态；公共 setter
  使用类型化参数设置属性，由 `on_setattr` 完成转换。
- `on_setattr` 中不要回调会再次调用 `ui_widget_set_attr()` 的公共 setter，以免递归；
  直接更新内部字段，或调用不写属性的内部 sync 函数。
- 示例：

```c
typedef enum ui_tooltip_side {
        UI_TOOLTIP_SIDE_TOP = 0,
        UI_TOOLTIP_SIDE_RIGHT,
        UI_TOOLTIP_SIDE_BOTTOM,
        UI_TOOLTIP_SIDE_LEFT,
} ui_tooltip_side_t;

void ui_tooltip_content_set_side(ui_widget_t *w, ui_tooltip_side_t side)
{
        ui_widget_set_attr(w, "side", ui_tooltip_side_names[side]);
}

static void ui_tooltip_content_on_set_attr(ui_widget_t *w, const char *name,
                                           const char *value)
{
        if (strcmp(name, "side") == 0) {
                data->side = ui_tooltip_side_from_name(value);
        }
}

ui_tooltip_content_prototype->setattr = ui_tooltip_content_on_set_attr;
```

## 浮层组件

- `combobox`、`dropdown-menu`、`dialog`、`alert-dialog` 等浮层必须复用 Portal，不得
  私建覆盖 root 或复制跨树事件转发逻辑。
- Portal content 的结构所有权、定位、outside 关闭和 destroy 顺序必须通过 Portal API
  管理；浮层测试至少覆盖 logical click、outside 事件、锚定/overlay、content-first 和
  owner-first 销毁。
- 普通锚定浮层使用 Portal 的 anchor/side/align；模态 Dialog 将全屏 overlay/content
  wrapper 作为 Portal content，不以 trigger 坐标定位。

## 自绘动效

- Spinner、Shimmer 等自绘动效使用 Canvas 的逻辑绘制 API 和 `lcui_request_frame()`；停止、
  隐藏或销毁时必须取消 frame。调用层不得读取或处理实际 DPI、`pd_canvas_t` 或后端像素。
- 每帧不得分配离屏缓冲；尺寸或实际 DPI 变化时由 Canvas/渲染后端重建。通用绘制、遮罩和
  合成能力先在 Canvas 后端定义并测试，再由 widget 使用。
- 自绘动效须同时提供集成像素测试和 `tests/previews/` 可视诊断，检查裁切、透明度和
  高 DPI 行为。

## 指令

### gen-commit

用于生成符合 Angular 规范的提交信息，scope 应为 `lib/` 下的库名（例如：ui、css、ptk）
或 `src/` 下的模块名（例如：widgets、styles）；纯文档改动可不带 scope。
标题长度限制在 80 字符以内，应结合本次会话内容生成，无需读取实际改动文件内容。
