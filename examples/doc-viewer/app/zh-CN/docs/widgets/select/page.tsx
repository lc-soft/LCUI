import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import SelectBasicDemo from "../../../../examples/select-basic/index";
import SelectGroupsDemo from "../../../../examples/select-groups/index";
import SelectAlignItemDemo from "../../../../examples/select-align-item/index";
import SelectScrollableDemo from "../../../../examples/select-scrollable/index";
import SelectDisabledDemo from "../../../../examples/select-disabled/index";
import FieldTable from "../../../../components/field-table";
import { selectFields } from "../../../../widget-fields/select";
import { selectTriggerFields } from "../../../../widget-fields/select-trigger";
import { selectContentFields } from "../../../../widget-fields/select-content";
import { selectGroupFields } from "../../../../widget-fields/select-group";
import { selectLabelFields } from "../../../../widget-fields/select-label";
import { selectSeparatorFields } from "../../../../widget-fields/select-separator";
import { selectItemFields } from "../../../../widget-fields/select-item";

export default function SelectDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Select</h1>
      <p className="doc-page-desc">{`Select 提供一个可展开的下拉选项列表，用于从多个候选值中选择一项。`}</p>
      <SelectBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：从多个选项中选择唯一值`}</Text></li>
          <li><Text>{`[b]适用[/b]：选项较多、需要节省页面空间的表单`}</Text></li>
          <li><Text>{`[b]适用[/b]：选项可以按类别分组展示`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要同时选择多个值的场景（用 [bgcolor=#7f7f7f29] checkbox [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/select/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Select[/color], [color=#8250df]SelectTrigger[/color], [color=#8250df]SelectValue[/color], [color=#8250df]SelectContent[/color], [color=#8250df]SelectGroup[/color], [color=#8250df]SelectLabel[/color], [color=#8250df]SelectItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Select[/color], [color=#d2a8ff]SelectTrigger[/color], [color=#d2a8ff]SelectValue[/color], [color=#d2a8ff]SelectContent[/color], [color=#d2a8ff]SelectGroup[/color], [color=#d2a8ff]SelectLabel[/color], [color=#d2a8ff]SelectItem[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/select/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Select[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"选择主题"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectLabel[/color]>[/color]主题[color=#116329]</[color=#0550ae]SelectLabel[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]浅色[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]深色[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]跟随系统[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`    [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`</[color=#8250df]Select[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Select[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]SelectTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]SelectValue[/color] [color=#79c0ff]placeholder[/color]=[color=#a5d6ff]"选择主题"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]SelectTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]SelectContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectLabel[/color]>[/color]主题[color=#7ee787]</[color=#79c0ff]SelectLabel[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"light"[/color]>[/color]浅色[color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"dark"[/color]>[/color]深色[color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"system"[/color]>[/color]跟随系统[color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]SelectContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Select[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] Select [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/select/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Select`}</Text>
                  <Text className="code-line">{`├── SelectTrigger`}</Text>
                  <Text className="code-line">{`│   └── SelectValue`}</Text>
                  <Text className="code-line">{`└── SelectContent`}</Text>
                  <Text className="code-line">{`    ├── SelectGroup`}</Text>
                  <Text className="code-line">{`    │   ├── SelectLabel`}</Text>
                  <Text className="code-line">{`    │   ├── SelectItem`}</Text>
                  <Text className="code-line">{`    │   └── SelectItem`}</Text>
                  <Text className="code-line">{`    ├── SelectSeparator`}</Text>
                  <Text className="code-line">{`    └── SelectGroup`}</Text>
                  <Text className="code-line">{`        ├── SelectLabel`}</Text>
                  <Text className="code-line">{`        ├── SelectItem`}</Text>
                  <Text className="code-line">{`        └── SelectItem`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Select`}</Text>
                  <Text className="code-line">{`├── SelectTrigger`}</Text>
                  <Text className="code-line">{`│   └── SelectValue`}</Text>
                  <Text className="code-line">{`└── SelectContent`}</Text>
                  <Text className="code-line">{`    ├── SelectGroup`}</Text>
                  <Text className="code-line">{`    │   ├── SelectLabel`}</Text>
                  <Text className="code-line">{`    │   ├── SelectItem`}</Text>
                  <Text className="code-line">{`    │   └── SelectItem`}</Text>
                  <Text className="code-line">{`    ├── SelectSeparator`}</Text>
                  <Text className="code-line">{`    └── SelectGroup`}</Text>
                  <Text className="code-line">{`        ├── SelectLabel`}</Text>
                  <Text className="code-line">{`        ├── SelectItem`}</Text>
                  <Text className="code-line">{`        └── SelectItem`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>分组</h2>
        <p>{`用 [bgcolor=#7f7f7f29] SelectGroup [/bgcolor] 包裹一组相关的选项，[bgcolor=#7f7f7f29] SelectLabel [/bgcolor] 作为分组标题；当选项面板包含多组内容时，用 [bgcolor=#7f7f7f29] SelectSeparator [/bgcolor] 在分组之间画一条分隔线。`}</p>
        <SelectGroupsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>对齐触发器</h2>
        <p>{`默认情况下，打开 [bgcolor=#7f7f7f29] Select [/bgcolor] 时选项面板会[b]覆盖触发器[/b]，让[b]选中项[/b]与触发器处于同一水平位置；没有选中项时对齐第一个选项。若触发器靠近窗口底部、空间不足以对齐，则改为翻到触发器上方并显示滚动条。将 [bgcolor=#7f7f7f29] align-item-with-trigger [/bgcolor] 设为 [bgcolor=#7f7f7f29] false [/bgcolor] 可关闭对齐，改为贴着触发器边缘展开。`}</p>
        <SelectAlignItemDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>可滚动</h2>
        <p>{`当选项内容超出可用高度时，选项面板会被限制为可用高度并出现滚动条；触发器靠近窗口底部时，面板会翻到触发器上方并以该侧的可用高度显示。`}</p>
        <SelectScrollableDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>禁用</h2>
        <p>{`为 [bgcolor=#7f7f7f29] Select [/bgcolor] 设置 [bgcolor=#7f7f7f29] disabled [/bgcolor] 后，触发器整体降低透明度且不再打开选项面板；也可以单独为某个 [bgcolor=#7f7f7f29] SelectItem [/bgcolor] 设置 [bgcolor=#7f7f7f29] disabled [/bgcolor]，被禁用的选项不可选中。`}</p>
        <SelectDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Select</h3>
        <p>{`Select 状态容器，管理当前选中值、placeholder 和下拉面板。`}</p>
        <FieldTable fields={selectFields} locale="zh-CN" />
        <h3>SelectTrigger</h3>
        <p>{`触发下拉面板打开或关闭的按钮。`}</p>
        <FieldTable fields={selectTriggerFields} locale="zh-CN" />
        <h3>SelectContent</h3>
        <p>{`通过 Portal 挂载到 root 层级的选项面板。`}</p>
        <FieldTable fields={selectContentFields} locale="zh-CN" />
        <h3>SelectGroup</h3>
        <p>{`将一组相关选项组织在一起的容器，可配合 [bgcolor=#7f7f7f29] SelectLabel [/bgcolor] 显示分组标题。`}</p>
        <FieldTable fields={selectGroupFields} locale="zh-CN" />
        <h3>SelectLabel</h3>
        <p>{`选项分组的小号标题文本。`}</p>
        <FieldTable fields={selectLabelFields} locale="zh-CN" />
        <h3>SelectSeparator</h3>
        <p>{`分隔不同选项分组的一条细分隔线。`}</p>
        <FieldTable fields={selectSeparatorFields} locale="zh-CN" />
        <h3>SelectItem</h3>
        <p>{`代表一个可选值的选项。`}</p>
        <FieldTable fields={selectItemFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
