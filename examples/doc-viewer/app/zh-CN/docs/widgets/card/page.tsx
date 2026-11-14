import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import CardBasicDemo from "../../../../examples/card-basic/index";
import CardSmallDemo from "../../../../examples/card-small/index";
import CardImageDemo from "../../../../examples/card-image/index";
import FieldTable from "../../../../components/field-table";
import { cardFields } from "../../../../widget-fields/card";
import { cardHeaderFields } from "../../../../widget-fields/card-header";
import { cardTitleFields } from "../../../../widget-fields/card-title";
import { cardDescriptionFields } from "../../../../widget-fields/card-description";
import { cardActionFields } from "../../../../widget-fields/card-action";
import { cardContentFields } from "../../../../widget-fields/card-content";
import { cardFooterFields } from "../../../../widget-fields/card-footer";

export default function CardDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Card</h1>
      <p className="doc-page-desc">{`用于承载一组相关信息与操作的容器，由头部、内容区和底部组合而成。`}</p>
      <CardBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：表单、账户信息、设置项、文章摘要等需要分组展示的内容`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要在卡片右上角放置编辑、更多等轻量操作的场景`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要展开/收起的大量内容（使用 [bgcolor=#7f7f7f29] dialog [/bgcolor] 或独立页面）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：纯提示信息（使用 [bgcolor=#7f7f7f29] alert [/bgcolor] 或 [bgcolor=#7f7f7f29] toast [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/card/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Card[/color], [color=#8250df]CardContent[/color], [color=#8250df]CardDescription[/color], [color=#8250df]CardFooter[/color], [color=#8250df]CardHeader[/color], [color=#8250df]CardTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Card[/color], [color=#d2a8ff]CardContent[/color], [color=#d2a8ff]CardDescription[/color], [color=#d2a8ff]CardFooter[/color], [color=#d2a8ff]CardHeader[/color], [color=#d2a8ff]CardTitle[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/card/1">
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
                    <Text className="code-line">{`<[color=#8250df]Card[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Card Title[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]CardDescription[/color]>[/color]Card Description[color=#116329]</[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CardContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]p[/color]>[/color]Card Content[color=#116329]</[color=#0550ae]p[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]p[/color]>[/color]Card Footer[color=#116329]</[color=#0550ae]p[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`</[color=#8250df]Card[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Card[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]CardHeader[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]CardTitle[/color]>[/color]Card Title[color=#7ee787]</[color=#79c0ff]CardTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]CardDescription[/color]>[/color]Card Description[color=#7ee787]</[color=#79c0ff]CardDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]CardHeader[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]CardContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]p[/color]>[/color]Card Content[color=#7ee787]</[color=#79c0ff]p[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]CardContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]CardFooter[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]p[/color]>[/color]Card Footer[color=#7ee787]</[color=#79c0ff]p[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]CardFooter[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Card[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] Card [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/card/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Card`}</Text>
                  <Text className="code-line">{`├── CardHeader`}</Text>
                  <Text className="code-line">{`│   ├── CardTitle`}</Text>
                  <Text className="code-line">{`│   ├── CardDescription`}</Text>
                  <Text className="code-line">{`│   └── CardAction`}</Text>
                  <Text className="code-line">{`├── CardContent`}</Text>
                  <Text className="code-line">{`└── CardFooter`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Card`}</Text>
                  <Text className="code-line">{`├── CardHeader`}</Text>
                  <Text className="code-line">{`│   ├── CardTitle`}</Text>
                  <Text className="code-line">{`│   ├── CardDescription`}</Text>
                  <Text className="code-line">{`│   └── CardAction`}</Text>
                  <Text className="code-line">{`├── CardContent`}</Text>
                  <Text className="code-line">{`└── CardFooter`}</Text>
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
        <h3>小尺寸</h3>
        <p>{`使用 [bgcolor=#7f7f7f29] size="sm" [/bgcolor] 让卡片使用更紧凑的间距和小一号标题。`}</p>
        <CardSmallDemo />
        <h3>图片</h3>
        <p>{`在 [bgcolor=#7f7f7f29] CardHeader [/bgcolor] 前放置封面内容。当前 LCUI 没有独立的 image 部件，示例使用纯色占位 [bgcolor=#7f7f7f29] div [/bgcolor] 和绝对定位遮罩层模拟封面图。`}</p>
        <CardImageDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Card</h3>
        <p>{`卡片根容器，管理尺寸和子部件布局。`}</p>
        <FieldTable fields={cardFields} locale="zh-CN" />
        <h3>CardHeader</h3>
        <p>{`卡片头部，包含标题、描述和可选操作。`}</p>
        <FieldTable fields={cardHeaderFields} locale="zh-CN" />
        <h3>CardTitle</h3>
        <p>{`卡片标题。`}</p>
        <FieldTable fields={cardTitleFields} locale="zh-CN" />
        <h3>CardDescription</h3>
        <p>{`标题下方的辅助描述。`}</p>
        <FieldTable fields={cardDescriptionFields} locale="zh-CN" />
        <h3>CardAction</h3>
        <p>{`位于头部右上角的操作区域。`}</p>
        <FieldTable fields={cardActionFields} locale="zh-CN" />
        <h3>CardContent</h3>
        <p>{`卡片主体内容区域。`}</p>
        <FieldTable fields={cardContentFields} locale="zh-CN" />
        <h3>CardFooter</h3>
        <p>{`卡片底部区域，用于操作或次要信息。`}</p>
        <FieldTable fields={cardFooterFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
