import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import BadgeBasicDemo from "../../../../examples/badge-basic/index";
import BadgeVariantsDemo from "../../../../examples/badge-variants/index";
import BadgeIconDemo from "../../../../examples/badge-icon/index";
import BadgeColorsDemo from "../../../../examples/badge-colors/index";
import FieldTable from "../../../../components/field-table";
import { badgeFields } from "../../../../widget-fields/badge";

export default function BadgeDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Badge</h1>
      <p className="doc-page-desc">{`用于显示短标签、状态和计数的轻量文本部件。`}</p>
      <BadgeBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：在列表、卡片和表单中标记状态或分类`}</Text></li>
          <li><Text>{`[b]适用[/b]：展示简短的计数或提示文本`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要点击行为的命令控件（用 [bgcolor=#eee] button [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/badge/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Badge[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/badge/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Badge[/color] variant=[color=#0a3069]"secondary"[/color]>[color=#8250df]Secondary[/color]</[color=#8250df]Badge[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>变体</h2>
        <p>{`使用 [bgcolor=#eee] variant [/bgcolor] 属性切换徽标的预置外观。`}</p>
        <BadgeVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>带图标</h2>
        <p>{`组合 Icon 与 Text 子部件，可以用图标和文本共同表达状态或类别。把 Icon 放在 Text 之前即为前置图标，放在之后即为尾随图标。Badge 会自动横向排列子部件、保持 20px 高度，并将图标字号约束为与文本一致，因此无需额外设置间距或尺寸类。`}</p>
        <BadgeIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>自定义颜色</h2>
        <p>{`使用 [bgcolor=#eee] color [/bgcolor] 属性或 [bgcolor=#eee] ui_badge_set_color() [/bgcolor] 设置命名配色。`}</p>
        <BadgeColorsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={badgeFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
