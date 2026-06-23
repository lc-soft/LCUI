import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CompositionDocPage() {
  return (
    <Widget className="doc-page">
      <h1>组合</h1>
      <p className="doc-page-desc">{`构建复杂界面意味着把多个部件组合成层级结构。LCUI 提供三种常见方式来完成这件事：在 C 中构建部件层级、用 XML 组合界面、用 TSX 组合组件。`}</p>
      <Widget className="doc-section">
        <h2>在 C 中构建部件层级</h2>
        <p>{`使用 [bgcolor=#eee] ui_widget_append() [/bgcolor] 嵌套部件：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/composition/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *root = ui_root();`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *container = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *title = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *btn = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_text_set_content(title, [color=#0a3069]"My App"[/color]);`}</Text>
            <Text className="code-line">{`ui_button_set_text(btn, [color=#0a3069]"Click me"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_widget_append(container, title);`}</Text>
            <Text className="code-line">{`ui_widget_append(container, btn);`}</Text>
            <Text className="code-line">{`ui_widget_append(root, container);`}</Text>
          </Widget>
        </Widget>
        <p>{`容器部件（[bgcolor=#eee] container [/bgcolor]）是普通的未命名部件（[bgcolor=#eee] ui_create_widget(NULL) [/bgcolor] 创建），它的作用就是把多个子部件组合成一组。`}</p>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`LCUI 没有原生的 [bgcolor=#eee] h1 [/bgcolor] / [bgcolor=#eee] p [/bgcolor] 等语义标签。如果你在用 [bgcolor=#eee] @lcui/cli [/bgcolor] 的编译流程，可以用注册别名的方式添加文本原型的语义组件（见下方 语义标签）。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>使用 XML 组合界面</h2>
        <p>{`用 XML 直接描述整个部件树：`}</p>
        <Widget className="code-block" data-language="xml" data-source="zh-CN/handbook/composition/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">app/ui.xml</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display: flex; flex-direction: column; padding: 16px;"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]style[/color]=[color=#0a3069]"font-size: 24px; font-weight: bold;"[/color]>[/color]My App[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]id[/color]=[color=#0a3069]"action-btn"[/color]>[/color]Click me[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`在 C 代码里加载并绑定事件：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/composition/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_action_click[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]ui_event_t[/color] *e, [color=#953800]void[/color] *arg)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        ui_text_set_content(([color=#953800]ui_widget_t[/color] *)arg, [color=#0a3069]"按钮被点击！"[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *root, *btn, *title;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        lcui_init();`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        root = ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
            <Text className="code-line">{`        ui_widget_append(ui_root(), root);`}</Text>
            <Text className="code-line">{`        ui_widget_unwrap(root);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        btn = ui_get_widget([color=#0a3069]"action-btn"[/color]);`}</Text>
            <Text className="code-line">{`        title = ui_get_widget([color=#0a3069]"title"[/color]);`}</Text>
            <Text className="code-line">{`        [color=#cf222e]if[/color] (btn) {`}</Text>
            <Text className="code-line">{`                ui_widget_on(btn, [color=#0a3069]"click"[/color], on_action_click, title);`}</Text>
            <Text className="code-line">{`        }`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>使用 TSX 组合组件</h2>
        <p>{`配合 [bgcolor=#eee] @lcui/cli [/bgcolor]，你可以用 TSX 编写可复用组件：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/composition/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">components/Card.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]function[/color] [color=#8250df]Card[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]border:[/color] "[color=#0550ae]1px[/color] [color=#0550ae]solid[/color] #[color=#0550ae]ccc[/color]", [color=#0550ae]padding:[/color] [color=#0550ae]16[/color], [color=#0550ae]borderRadius:[/color] [color=#0550ae]4[/color] }}>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]fontWeight:[/color] "[color=#0550ae]bold[/color]", [color=#0550ae]fontSize:[/color] [color=#0550ae]18[/color] }}>[/color]标题[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Button[/color]>[/color]开始使用[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`在应用中使用：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/composition/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/App.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Card[/color] } [color=#cf222e]from[/color] [color=#0a3069]"./components/Card"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] [color=#116329]<[color=#0550ae]Card[/color] />[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>事件处理</h2>
        <p>{`部件之间通常通过事件通信。使用 [bgcolor=#eee] ui_widget_on() [/bgcolor] 绑定处理函数：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/composition/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_button_click[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]ui_event_t[/color] *e, [color=#953800]void[/color] *arg)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = arg;`}</Text>
            <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"按钮已被点击！"[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *btn = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_button_set_text(btn, [color=#0a3069]"Click me"[/color]);`}</Text>
            <Text className="code-line">{`ui_widget_on(btn, [color=#0a3069]"click"[/color], on_button_click, label);`}</Text>
          </Widget>
        </Widget>
        <p>{`事件处理函数签名固定为 [bgcolor=#eee] (ui_widget_t *w, ui_event_t *e, void *arg) [/bgcolor]：`}</p>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] w [/bgcolor][/b] — 触发事件的部件。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] e [/bgcolor][/b] — 事件对象，包含事件类型和附带数据。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] arg [/bgcolor][/b] — 绑定事件时传入的自定义上下文（通常指向另一个部件或状态结构）。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>语义标签</h2>
        <p>{`LCUI 默认的部件原型只有 [bgcolor=#eee] text [/bgcolor]、[bgcolor=#eee] button [/bgcolor]、[bgcolor=#eee] textinput [/bgcolor]、[bgcolor=#eee] scrollarea [/bgcolor] 等。[bgcolor=#eee] h1 [/bgcolor] / [bgcolor=#eee] h2 [/bgcolor] / [bgcolor=#eee] p [/bgcolor] 这些语义标签需要自行注册为 [bgcolor=#eee] text [/bgcolor] 的别名：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/composition/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *aliases\\[] = { [color=#0a3069]"h1"[/color], [color=#0a3069]"h2"[/color], [color=#0a3069]"h3"[/color], [color=#0a3069]"h4"[/color], [color=#0a3069]"h5"[/color], [color=#0a3069]"h6"[/color], [color=#0a3069]"p"[/color], [color=#0550ae]NULL[/color] };`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]register_semantic_tags[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#cf222e]for[/color] ([color=#953800]const[/color] [color=#953800]char[/color] **n = aliases; *n; ++n) {`}</Text>
            <Text className="code-line">{`                ui_create_widget_prototype(*n, [color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`        }`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        lcui_init();`}</Text>
            <Text className="code-line">{`        register_semantic_tags();`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* ... 之后再实例化 h1 / p 等部件 */[/color]`}</Text>
            <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`注册完成后就可以在 XML 或 TSX 里像写 HTML 那样使用语义标签：`}</p>
        <Widget className="code-block" data-language="xml" data-source="zh-CN/handbook/composition/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]h1[/color]>[/color]页面标题[color=#116329]</[color=#0550ae]h1[/color]>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]p[/color]>[/color]正文段落[color=#116329]</[color=#0550ae]p[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]混合文本与元素不被支持。[/b] LCUI 的部件树里，一个部件要么是纯文本（[bgcolor=#eee] <text> [/bgcolor]），要么全是子部件。不能在同一个部件内混用"一段文本"和"一个嵌套的 [bgcolor=#eee] <button> [/bgcolor]"。需要内联强调/代码等样式时，用 LCUI 的文本标签语法（如 [bgcolor=#eee] [b]bold[/b] [/bgcolor]、[bgcolor=#eee] [color=#336]text[/color] [/bgcolor]）。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] ui_load_xml_file() [/bgcolor] 返回的是包裹节点。[/b] 返回的根部件包含 XML 中 [bgcolor=#eee] <ui> [/bgcolor] 节点下声明的所有子部件，你需要用 [bgcolor=#eee] ui_widget_unwrap() [/bgcolor] 把内部子部件展开到目标容器中。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
