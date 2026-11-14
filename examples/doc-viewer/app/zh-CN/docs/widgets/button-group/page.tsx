import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ButtonGroupDemoDemo from "../../../../examples/button-group-demo/index";
import ButtonGroupOrientationDemo from "../../../../examples/button-group-orientation/index";
import ButtonGroupSizeDemo from "../../../../examples/button-group-size/index";
import ButtonGroupNestedDemo from "../../../../examples/button-group-nested/index";
import ButtonGroupSeparatorDemo from "../../../../examples/button-group-separator/index";
import ButtonGroupSplitDemo from "../../../../examples/button-group-split/index";
import ButtonGroupInputDemo from "../../../../examples/button-group-input/index";
import ButtonGroupInputGroupDemo from "../../../../examples/button-group-input-group/index";
import ButtonGroupDropdownDemo from "../../../../examples/button-group-dropdown/index";
import ButtonGroupSelectDemo from "../../../../examples/button-group-select/index";
import ButtonGroupPopoverDemo from "../../../../examples/button-group-popover/index";
import FieldTable from "../../../../components/field-table";
import { buttonGroupFields } from "../../../../widget-fields/button-group";
import { buttonGroupTextFields } from "../../../../widget-fields/button-group-text";
import { buttonGroupSeparatorFields } from "../../../../widget-fields/button-group-separator";

export default function ButtonGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>ButtonGroup</h1>
      <p className="doc-page-desc">{`把一组相关按钮、输入框或弹出控件组合成视觉连续的按钮组。`}</p>
      <ButtonGroupDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：把相邻的同类动作按钮合并成一组，如 Archive / Report / Snooze`}</Text></li>
          <li><Text>{`[b]适用[/b]：split button、输入框 + 操作按钮、Select + 输入框 + 发送按钮`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要把多个 ButtonGroup 嵌套以形成分组间距`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要在多个选项之间切换状态（使用 ToggleGroup 或 RadioGroup）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：按钮之间需要独立间距、不应共享边框的场景（直接使用 Button）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/button-group/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroupSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroupSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/button-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "104px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]ButtonGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Button[/color]>[/color]Button 1[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Button[/color]>[/color]Button 2[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`</[color=#8250df]ButtonGroup[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]ButtonGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Button 1[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Button 2[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]ButtonGroup[/color]>`}</Text>
                  </Widget>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] ButtonGroup [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/button-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`ButtonGroup`}</Text>
                  <Text className="code-line">{`├── Button or Input or InputGroup or Select`}</Text>
                  <Text className="code-line">{`├── ButtonGroupText`}</Text>
                  <Text className="code-line">{`├── ButtonGroupSeparator`}</Text>
                  <Text className="code-line">{`└── ButtonGroup`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`ButtonGroup`}</Text>
                  <Text className="code-line">{`├── Button or Input or InputGroup or Select`}</Text>
                  <Text className="code-line">{`├── ButtonGroupText`}</Text>
                  <Text className="code-line">{`├── ButtonGroupSeparator`}</Text>
                  <Text className="code-line">{`└── ButtonGroup`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>方向</h3>
        <p>{`设置 [bgcolor=#7f7f7f29] orientation="vertical" [/bgcolor] 后按钮纵向排列，首尾圆角与相邻边框自动切换。`}</p>
        <ButtonGroupOrientationDemo />
        <h3>尺寸</h3>
        <p>{`按钮组不设置统一尺寸，由内部按钮各自的 [bgcolor=#7f7f7f29] size [/bgcolor] 决定。`}</p>
        <ButtonGroupSizeDemo />
        <h3>嵌套</h3>
        <p>{`嵌套 ButtonGroup 会自动获得 8px 间距，适合图标组 + InputGroup 等组合。`}</p>
        <ButtonGroupNestedDemo />
        <h3>分隔线</h3>
        <p>{`[bgcolor=#7f7f7f29] ButtonGroupSeparator [/bgcolor] 用于在同组按钮之间增加视觉分隔。`}</p>
        <ButtonGroupSeparatorDemo />
        <h3>Split</h3>
        <p>{`主按钮 + 分隔线 + 图标按钮构成 split button。`}</p>
        <ButtonGroupSplitDemo />
        <h3>输入框</h3>
        <p>{`输入框可以直接作为 ButtonGroup 的子项，并自动 [bgcolor=#7f7f7f29] flex: 1 [/bgcolor] 撑满。`}</p>
        <ButtonGroupInputDemo />
        <h3>输入框组</h3>
        <p>{`ButtonGroup 可以嵌套 InputGroup，再与图标按钮或其它输入控件组合。`}</p>
        <ButtonGroupInputGroupDemo />
        <h3>下拉菜单</h3>
        <p>{`把 DropdownMenu 放进 ButtonGroup，形成主操作 + 附加操作菜单。`}</p>
        <ButtonGroupDropdownDemo />
        <h3>选择框</h3>
        <p>{`Select 与输入框可以放在同一个嵌套 ButtonGroup 中，再与发送按钮组合。`}</p>
        <ButtonGroupSelectDemo />
        <h3>气泡卡片</h3>
        <p>{`Popover 可以作为 ButtonGroup 的附加操作入口。`}</p>
        <ButtonGroupPopoverDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>ButtonGroup</h3>
        <p>{`按钮组根容器，管理排列方向和直接子项的边框/圆角。`}</p>
        <FieldTable fields={buttonGroupFields} locale="zh-CN" />
        <h3>ButtonGroupText</h3>
        <p>{`带 muted 背景和边框的说明文字容器。`}</p>
        <FieldTable fields={buttonGroupTextFields} locale="zh-CN" />
        <h3>ButtonGroupSeparator</h3>
        <p>{`按钮之间的 1px 分隔线。`}</p>
        <FieldTable fields={buttonGroupSeparatorFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]ButtonGroup 不是 ToggleGroup[/b]。它只负责布局和视觉合并，不维护选中状态；需要状态切换时使用 RadioGroup 或后续 ToggleGroup。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]首尾圆角基于直接子项[/b]。根容器给直接子项加内部标记 class，并利用 LCUI 的 [bgcolor=#7f7f7f29] first-child [/bgcolor] / [bgcolor=#7f7f7f29] last-child [/bgcolor] status 计算首尾圆角与相邻边框。包在普通 wrapper 里的按钮不会参与。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]分隔线放在中间[/b]。放在组首尾会占掉首/末按钮的圆角位置。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]垂直组中的横向分隔线需要显式设置[/b]。[bgcolor=#7f7f7f29] ButtonGroupSeparator [/bgcolor] 默认是竖线，垂直 group 中应设置 [bgcolor=#7f7f7f29] orientation="horizontal" [/bgcolor]。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
