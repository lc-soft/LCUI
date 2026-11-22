# textinput 编辑内核设计（stb_textedit）

## 目标

为 `textinput` 引入 `stb_textedit.h` v1.16 作为编辑状态内核，补齐多行编辑能力：
光标、选区、撤销/重做、剪贴板快捷键、鼠标拖选。排版与绘制继续由 Pandagl 的
`pd_text_t` 负责，避免两套字体/布局实现漂移。

## 范围

v1 包含：

- 单行 / 多行（硬换行 `\n`）
- 软换行（依赖 `pd_text` 的 autowrap；索引映射需跳过软换行行尾）
- 光标移动：Left/Right/Up/Down/Home/End/PageUp/PageDown
- 选区：Shift + 方向键、Shift + Home/End、鼠标点击/拖拽
- 撤销/重做：Ctrl+Z / Ctrl+Y
- 剪贴板：Ctrl+A / Ctrl+C / Ctrl+X / Ctrl+V（有选区时复制/剪切选区）
- 输入法提交文本（`textinput` 事件）作为一次编辑
- 密码模式、只读模式、placeholder

v1 不包含：

- IME preedit（组合态）内联显示；只接收最终提交文本
- 样式标签在编辑场景下的索引同步（编辑场景保持禁用）
- 语法高亮、多光标、行号、bidi、grapheme cluster 级移动（仅保证不拆
  Windows 代理对）

## 分层

```
PTK 输入事件
  ↓
textinput widget（焦点、只读、密码、剪贴板、IME 事件）
  ↓
textedit（canonical 文本 + stb 状态 + 索引映射 + 撤销栈）
  ↓
pd_text（排版、换行、行高、字符 advance、光标像素）
  ↓
Canvas 绘制（文本 + 选区 overlay + textcaret）
```

## 数据结构

```c
typedef struct ui_text_edit {
        wchar_t *text;              /* canonical，统一使用 \n */
        int length;
        int capacity;
        STB_TexteditState state;    /* cursor / selection / undo */
        pd_text_t *layer;           /* 投影层，用于排版/渲染 */
        float layer_scale;          /* 逻辑像素 -> 实际像素 */
        bool style_tag_enabled;     /* v1 编辑时禁用 */
} ui_text_edit_t;
```

- canonical 是唯一事实来源；`pd_text` 是它的投影。
- `\r\n` / `\r` 在进入 canonical 前统一归一化为 `\n`。
- 公共读取 API（`ui_textinput_get_text_w` / `get_text_length`）改读 canonical。

## stb 配置

```c
#define STB_TEXTEDIT_CHARTYPE wchar_t
#define STB_TEXTEDIT_NEWLINE L'\n'
#define STB_TEXTEDIT_STRING ui_text_edit_t
#define STB_TEXTEDIT_STRINGLEN(e) ((e)->length)
#define STB_TEXTEDIT_GETCHAR(e, i) ((e)->text[i])
#define STB_TEXTEDIT_INSERTCHARS(e, i, c, n) ui_text_edit_insert_raw(e, i, c, n)
#define STB_TEXTEDIT_DELETECHARS(e, i, n) ui_text_edit_delete_raw(e, i, n)
#define STB_TEXTEDIT_LAYOUTROW(r, e, i) ui_text_edit_layout_row(r, e, i)
#define STB_TEXTEDIT_GETWIDTH(e, start, i) ui_text_edit_get_width(e, start, i)
#define STB_TEXTEDIT_GETPREVCHARINDEX(e, i) ui_text_edit_prev_index(e, i)
#define STB_TEXTEDIT_GETNEXTCHARINDEX(e, i) ui_text_edit_next_index(e, i)
#define STB_TEXTEDIT_IS_SPACE(ch) iswspace(ch)
```

- 不定义 `STB_TEXTEDIT_KEYTOTEXT`；字符输入统一走 `stb_textedit_text()`，避免
  stb 把按键与字符混在一起。
- Undo 使用默认 99 条 / 999 字符；后续如需要可在 `textedit.c` 内调整宏。

## 索引映射

- canonical 索引 ↔ pd_text 视觉行列通过一次线性扫描完成：
  - 每个视觉行消耗 `line->length` 个字符；
  - 硬换行（`eol != PD_TEXT_EOL_NONE`）额外消耗 1 个 `\n`；
  - 软换行（`eol == PD_TEXT_EOL_NONE`）不消耗字符。
- `LAYOUTROW(r, e, index)`：从 `index` 开始找到视觉行，`num_chars = 行剩余
  字符数 + (硬换行 ? 1 : 0)`，`baseline_y_delta = line->height`。
- `GETWIDTH` 直接读 `line->string[col]->bitmap->metrics.hori_advance`，不调用
  `pd_text_get_char_pixel()`，避免 O(N²)。

## 排版同步

- 每次 `INSERTCHARS` / `DELETECHARS` 后：
  1. 更新 canonical；
  2. 用 `pd_text_clear` + `pd_text_append` 重建投影层（小文本可接受）；
  3. 立即 `pd_text_update()`，保证 stb 后续查询看到新行结构。
- caret 像素由 `pd_text_set_caret()` + `pd_text_get_caret_pixel()` 提供；
  textinput 负责除以 `layer_scale` 转逻辑像素。

## 选区绘制

- 在 `ui_textinput_on_paint()` 中、`pd_text_render_to()` 之前绘制。
- 按视觉行拆分 `[select_start, select_end)`，用 `pd_text_get_char_pixel()` 取
  每行起止 x，`line->height` 取高度。
- 选区颜色使用主题 `primary/20` 或 `foreground/15`（实现时二选一并加测试）。
- 密码模式可选：v1 允许选中，但绘制仍显示屏蔽字符；不额外暴露明文。

## 失败模式（Review Focus）

1. 软换行边界：光标落在软换行行尾/下一行行首时必须只对应一个 canonical 索引。
2. 硬换行删除：Backspace 跨行合并、Delete 行尾合并必须与 canonical 一致。
3. Windows 代理对（emoji）：左右移动和退格不能拆代理对。
4. 排版延迟：插入/删除后未立即 `pd_text_update()` 会导致光标/选区错位。
5. CRLF：canonical 必须归一化，否则 `pd_text->length` 与 canonical 索引错位。
6. 密码/只读：只读仍允许选择与复制；密码模式不能把明文写进剪贴板以外的地方。
7. 样式标签：编辑场景若开启会破坏索引，v1 必须显式拒绝或忽略。

## 许可

`stb_textedit.h` 为 MIT 或公有领域双许可，与 LCUI 的 MIT 兼容。vendor 时保留
文件头与许可证原文，并在 `src/widgets/third-party/README.md` 记录来源与版本。
