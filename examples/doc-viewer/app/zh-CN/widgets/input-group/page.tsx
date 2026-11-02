import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import InputGroupDemoDemo from "../../../examples/input-group-demo/index";
import InputGroupInlineStartDemo from "../../../examples/input-group-inline-start/index";
import InputGroupInlineEndDemo from "../../../examples/input-group-inline-end/index";
import InputGroupBlockStartDemo from "../../../examples/input-group-block-start/index";
import InputGroupBlockEndDemo from "../../../examples/input-group-block-end/index";
import InputGroupIconDemo from "../../../examples/input-group-icon/index";
import InputGroupTextDemo from "../../../examples/input-group-text/index";
import InputGroupButtonDemo from "../../../examples/input-group-button/index";
import InputGroupKbdDemo from "../../../examples/input-group-kbd/index";
import InputGroupDropdownDemo from "../../../examples/input-group-dropdown/index";
import InputGroupSpinnerDemo from "../../../examples/input-group-spinner/index";
import InputGroupTextareaDemo from "../../../examples/input-group-textarea/index";
import InputGroupCustomDemo from "../../../examples/input-group-custom/index";
import FieldTable from "../../../components/field-table";
import { inputGroupFields } from "../../../widget-fields/input-group";
import { inputGroupAddonFields } from "../../../widget-fields/input-group-addon";
import { inputGroupTextFields } from "../../../widget-fields/input-group-text";
import { inputGroupKbdFields } from "../../../widget-fields/input-group-kbd";
import { inputGroupInputFields } from "../../../widget-fields/input-group-input";
import { inputGroupTextareaFields } from "../../../widget-fields/input-group-textarea";
import { inputGroupButtonFields } from "../../../widget-fields/input-group-button";

export default function InputGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>InputGroup</h1>
      <p className="doc-page-desc">{`把输入框、图标、说明文字、按钮和下拉菜单组合成统一边框的输入区域。`}</p>
      <InputGroupDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：搜索框、金额输入、密码框、评论框等需要前置/后置图标或操作按钮的输入区域`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要把输入框和说明、计数、单位、提交按钮放在同一视觉容器内`}</Text></li>
          <li><Text>{`[b]不适用[/b]：只需要一个普通输入框（直接使用 [bgcolor=#eee] TextInput [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要多字段表单布局（使用 [bgcolor=#eee] Field [/bgcolor] / [bgcolor=#eee] FieldGroup [/bgcolor] 组合）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/input-group/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupAddon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupButton[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupInput[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupKbd[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupText[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]InputGroupTextarea[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/input-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]InputGroup[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]InputGroupInput[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Search..."[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]InputGroupAddon[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"search"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]InputGroupAddon[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`</[color=#8250df]InputGroup[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] InputGroup [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/input-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`InputGroup`}</Text>
                <Text className="code-line">{`├── InputGroupInput or InputGroupTextarea`}</Text>
                <Text className="code-line">{`├── InputGroupAddon`}</Text>
                <Text className="code-line">{`├── InputGroupButton`}</Text>
                <Text className="code-line">{`├── InputGroupText`}</Text>
                <Text className="code-line">{`└── InputGroupKbd`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>行内起始</h3>
        <p>{`使用 [bgcolor=#eee] align="inline-start" [/bgcolor] 把图标放到输入框左侧。`}</p>
        <InputGroupInlineStartDemo />
        <h3>行内末尾</h3>
        <p>{`使用 [bgcolor=#eee] align="inline-end" [/bgcolor] 把图标放到输入框右侧。`}</p>
        <InputGroupInlineEndDemo />
        <h3>块级起始</h3>
        <p>{`[bgcolor=#eee] block-start [/bgcolor] 会把 addon 放到输入框上方，并让根容器切换为纵向布局。`}</p>
        <InputGroupBlockStartDemo />
        <h3>块级末尾</h3>
        <p>{`[bgcolor=#eee] block-end [/bgcolor] 会把 addon 放到输入框下方，适合单位、计数和提交按钮。`}</p>
        <InputGroupBlockEndDemo />
        <h3>图标</h3>
        <p>{`addon 可以只放图标，也可以同时放起始和末尾图标。`}</p>
        <InputGroupIconDemo />
        <h3>文本</h3>
        <p>{`使用 [bgcolor=#eee] InputGroupText [/bgcolor] 放置货币符号、域名后缀等说明文本。`}</p>
        <InputGroupTextDemo />
        <h3>按钮</h3>
        <p>{`在 addon 中放复制、收藏或提交按钮；图标按钮使用 [bgcolor=#eee] icon-xs [/bgcolor]。`}</p>
        <InputGroupButtonDemo />
        <h3>Kbd</h3>
        <p>{`使用 [bgcolor=#eee] InputGroupKbd [/bgcolor] 展示键盘快捷键。`}</p>
        <InputGroupKbdDemo />
        <h3>下拉菜单</h3>
        <p>{`把 [bgcolor=#eee] DropdownMenu [/bgcolor] 放进 addon，为输入框提供附加操作。`}</p>
        <InputGroupDropdownDemo />
        <h3>Spinner</h3>
        <p>{`加载状态可以放在输入框左侧或右侧，并可与说明文字组合。`}</p>
        <InputGroupSpinnerDemo />
        <h3>Textarea</h3>
        <p>{`多行输入可以和 block-start / block-end addon 组合，用于文件名、计数和运行按钮。`}</p>
        <InputGroupTextareaDemo />
        <h3>自定义控件</h3>
        <p>{`第三方或自定义可聚焦控件可以通过 [bgcolor=#eee] ui_input_group_mark_control() [/bgcolor] 接入输入组的聚焦与状态逻辑。`}</p>
        <InputGroupCustomDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>InputGroup</h3>
        <p>{`输入组根容器，统一边框、圆角、focus 外框和高度。`}</p>
        <FieldTable fields={inputGroupFields} locale="zh-CN" />
        <h3>InputGroupAddon</h3>
        <p>{`承载图标、文本、按钮等附加内容，通过 [bgcolor=#eee] align [/bgcolor] 控制位置。`}</p>
        <FieldTable fields={inputGroupAddonFields} locale="zh-CN" />
        <h3>InputGroupText</h3>
        <p>{`输入组内的 muted 说明文字容器。`}</p>
        <FieldTable fields={inputGroupTextFields} locale="zh-CN" />
        <h3>InputGroupKbd</h3>
        <p>{`输入组内的键盘提示样式容器。`}</p>
        <FieldTable fields={inputGroupKbdFields} locale="zh-CN" />
        <h3>InputGroupInput</h3>
        <p>{`去掉自身边框和背景的 [bgcolor=#eee] TextInput [/bgcolor]，自动撑满输入组。`}</p>
        <FieldTable fields={inputGroupInputFields} locale="zh-CN" />
        <h3>InputGroupTextarea</h3>
        <p>{`基于多行 [bgcolor=#eee] TextInput [/bgcolor] 的输入组多行输入。`}</p>
        <FieldTable fields={inputGroupTextareaFields} locale="zh-CN" />
        <h3>InputGroupButton</h3>
        <p>{`输入组内的小尺寸按钮，默认 [bgcolor=#eee] ghost [/bgcolor] + [bgcolor=#eee] xs [/bgcolor]。`}</p>
        <FieldTable fields={inputGroupButtonFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]focus 与 disabled 由 C 维护[/b]。LCUI 没有 [bgcolor=#eee] :focus-within [/bgcolor] 和 [bgcolor=#eee] :has() [/bgcolor]，根容器通过冒泡的 [bgcolor=#eee] focus [/bgcolor]/[bgcolor=#eee] blur [/bgcolor] 维护 [bgcolor=#eee] focused [/bgcolor] status，并通过子控件状态维护 [bgcolor=#eee] has-disabled [/bgcolor] status。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]addon 会自动排序[/b]。无论先写输入框还是先写 addon，根容器都会按 [bgcolor=#eee] align [/bgcolor] 把 inline-start / block-start 放到输入框前，inline-end / block-end 放到输入框后。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]block 与 inline addon 不建议混用[/b]。同一 group 混用时，block 布局优先，inline addon 会按纵向布局处理。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]一个 group 只放一个输入控件[/b]。addon 点击聚焦只针对第一个匹配控件；disabled 扫描覆盖整个子树。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]多行输入仍有限制[/b]。[bgcolor=#eee] InputGroupTextarea [/bgcolor] 基于多行 [bgcolor=#eee] TextInput [/bgcolor]，XML 变体只展示结构，多行编辑能力由 C/TSX 工厂启用。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
