import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import AlertBasicDemo from "../../../../examples/alert-basic/index";
import AlertVariantsDemo from "../../../../examples/alert-variants/index";
import AlertDestructiveDemo from "../../../../examples/alert-destructive/index";
import AlertActionDemo from "../../../../examples/alert-action/index";
import FieldTable from "../../../../components/field-table";
import { alertFields } from "../../../../widget-fields/alert";
import { alertContentFields } from "../../../../widget-fields/alert-content";
import { alertTitleFields } from "../../../../widget-fields/alert-title";
import { alertDescriptionFields } from "../../../../widget-fields/alert-description";
import { alertActionFields } from "../../../../widget-fields/alert-action";

export default function AlertDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Alert</h1>
      <p className="doc-page-desc">{`在当前内容区域显示非阻塞的状态、结果或风险提示。`}</p>
      <AlertBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：展示保存结果、权限提醒或可恢复错误`}</Text></li>
          <li><Text>{`[b]适用[/b]：在局部内容附近提供后续操作`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要全局队列或自动消失的通知`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/alert/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Alert[/color], [color=#8250df]AlertContent[/color], [color=#8250df]AlertDescription[/color], [color=#8250df]AlertTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Alert[/color], [color=#d2a8ff]AlertContent[/color], [color=#d2a8ff]AlertDescription[/color], [color=#d2a8ff]AlertTitle[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/alert/1">
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
                    <Text className="code-line">{`<[color=#8250df]Alert[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]Payment successful[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Your payment has been processed.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#8250df]Alert[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Alert[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]AlertContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]AlertTitle[/color]>[/color]Payment successful[color=#7ee787]</[color=#79c0ff]AlertTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]AlertDescription[/color]>[/color]Your payment has been processed.[color=#7ee787]</[color=#79c0ff]AlertDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]AlertContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Alert[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] Alert [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/alert/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Alert`}</Text>
                  <Text className="code-line">{`├── Icon`}</Text>
                  <Text className="code-line">{`├── AlertContent`}</Text>
                  <Text className="code-line">{`│   ├── AlertTitle`}</Text>
                  <Text className="code-line">{`│   └── AlertDescription`}</Text>
                  <Text className="code-line">{`└── AlertAction`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Alert`}</Text>
                  <Text className="code-line">{`├── Icon`}</Text>
                  <Text className="code-line">{`├── AlertContent`}</Text>
                  <Text className="code-line">{`│   ├── AlertTitle`}</Text>
                  <Text className="code-line">{`│   └── AlertDescription`}</Text>
                  <Text className="code-line">{`└── AlertAction`}</Text>
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
        <p>{`使用 [bgcolor=#7f7f7f29] variant [/bgcolor] 属性切换提示的视觉语义。`}</p>
        <AlertVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>破坏性提示</h2>
        <p>{`使用 [bgcolor=#7f7f7f29] variant="destructive" [/bgcolor] 表示无法恢复或需要立即处理的错误。`}</p>
        <AlertDestructiveDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>操作</h2>
        <p>{`使用 [bgcolor=#7f7f7f29] AlertAction [/bgcolor] 放置与提示相关的后续操作。`}</p>
        <AlertActionDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Alert</h3>
        <p>{`提示容器，控制整体视觉变体。`}</p>
        <FieldTable fields={alertFields} locale="zh-CN" />
        <h3>AlertContent</h3>
        <p>{`标题和说明的纵向内容容器。`}</p>
        <FieldTable fields={alertContentFields} locale="zh-CN" />
        <h3>AlertTitle</h3>
        <p>{`提示标题。`}</p>
        <FieldTable fields={alertTitleFields} locale="zh-CN" />
        <h3>AlertDescription</h3>
        <p>{`提示补充说明。`}</p>
        <FieldTable fields={alertDescriptionFields} locale="zh-CN" />
        <h3>AlertAction</h3>
        <p>{`操作按钮的布局槽位。`}</p>
        <FieldTable fields={alertActionFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
