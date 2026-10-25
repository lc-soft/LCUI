import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AttachmentBasicDemo from "../../../examples/attachment-basic/index";
import AttachmentImageDemo from "../../../examples/attachment-image/index";
import AttachmentStatesDemo from "../../../examples/attachment-states/index";
import AttachmentSizesDemo from "../../../examples/attachment-sizes/index";
import AttachmentTriggerDemo from "../../../examples/attachment-trigger/index";
import FieldTable from "../../../components/field-table";
import { attachmentFields } from "../../../widget-fields/attachment";
import { attachmentMediaFields } from "../../../widget-fields/attachment-media";
import { attachmentContentFields } from "../../../widget-fields/attachment-content";
import { attachmentTitleFields } from "../../../widget-fields/attachment-title";
import { attachmentDescriptionFields } from "../../../widget-fields/attachment-description";
import { attachmentActionsFields } from "../../../widget-fields/attachment-actions";
import { attachmentActionFields } from "../../../widget-fields/attachment-action";
import { attachmentTriggerFields } from "../../../widget-fields/attachment-trigger";
import { attachmentGroupFields } from "../../../widget-fields/attachment-group";

export default function AttachmentDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Attachment</h1>
      <p className="doc-page-desc">{`展示文件或图片附件、名称、元数据、上传状态与操作的组合部件。`}</p>
      <AttachmentBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：聊天输入框、消息线程和上传列表中的文件附件`}</Text></li>
          <li><Text>{`[b]适用[/b]：带缩略图、文件名、大小和移除操作的图片附件`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要表达等待上传、处理中、失败和完成状态的附件`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要完整文件管理器能力的场景（使用独立的文件列表页面）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/attachment/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "204px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentAction[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentActions[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/attachment/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Attachment[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]sales-dashboard.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]PDF · 2.4 MB[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove sales-dashboard.pdf"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`</[color=#8250df]Attachment[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#eee] Attachment [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/attachment/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Attachment`}</Text>
                <Text className="code-line">{`├── AttachmentMedia`}</Text>
                <Text className="code-line">{`├── AttachmentContent`}</Text>
                <Text className="code-line">{`│   ├── AttachmentTitle`}</Text>
                <Text className="code-line">{`│   └── AttachmentDescription`}</Text>
                <Text className="code-line">{`├── AttachmentActions`}</Text>
                <Text className="code-line">{`│   └── AttachmentAction`}</Text>
                <Text className="code-line">{`└── AttachmentTrigger`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`使用 [bgcolor=#eee] AttachmentGroup [/bgcolor] 横向排列多个附件：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/attachment/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`AttachmentGroup`}</Text>
                <Text className="code-line">{`├── Attachment`}</Text>
                <Text className="code-line">{`└── Attachment`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>图片</h3>
        <p>{`将 [bgcolor=#eee] AttachmentMedia [/bgcolor] 的 [bgcolor=#eee] variant [/bgcolor] 设为 [bgcolor=#eee] image [/bgcolor]，通过 [bgcolor=#eee] src [/bgcolor] 属性或背景图显示图片。垂直排列时媒体位于内容上方，适合图片缩略图。`}</p>
        <AttachmentImageDemo />
        <h3>状态</h3>
        <p>{`[bgcolor=#eee] state [/bgcolor] 用于表达上传生命周期。[bgcolor=#eee] uploading [/bgcolor] 和 [bgcolor=#eee] processing [/bgcolor] 会在标题位置显示 Shimmer 扫光，媒体槽位可以放置 Spinner。`}</p>
        <AttachmentStatesDemo />
        <h3>尺寸</h3>
        <p>{`使用 [bgcolor=#eee] size [/bgcolor] 在 [bgcolor=#eee] default [/bgcolor]、[bgcolor=#eee] sm [/bgcolor] 和 [bgcolor=#eee] xs [/bgcolor] 之间切换附件尺寸。`}</p>
        <AttachmentSizesDemo />
        <h3>分组</h3>
        <p>{`基础示例使用 [bgcolor=#eee] AttachmentGroup [/bgcolor] 将三个图片附件横向排列。每个附件保持自身尺寸，操作按钮不会被覆盖。`}</p>
        <h3>触发器</h3>
        <p>{`[bgcolor=#eee] AttachmentTrigger [/bgcolor] 是一个绝对定位的透明触发层，覆盖整张附件卡片，同时让 [bgcolor=#eee] AttachmentActions [/bgcolor] 保持独立可点击。带触发器的附件在 hover 时显示 [bgcolor=#eee] bg-muted/50 [/bgcolor] 背景，在 active/focus 时用 box-shadow 模拟 outline。点击事件通过 [bgcolor=#eee] data-dialog-trigger [/bgcolor] 等声明式关联连接到预览、链接或对话框。`}</p>
        <AttachmentTriggerDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Attachment</h3>
        <p>{`附件根容器，管理状态、尺寸和排列方向。`}</p>
        <FieldTable fields={attachmentFields} locale="zh-CN" />
        <h3>AttachmentMedia</h3>
        <p>{`行首或顶部的图标、图片预览槽位。`}</p>
        <FieldTable fields={attachmentMediaFields} locale="zh-CN" />
        <h3>AttachmentContent</h3>
        <p>{`标题和描述的弹性内容槽位。`}</p>
        <FieldTable fields={attachmentContentFields} locale="zh-CN" />
        <h3>AttachmentTitle</h3>
        <p>{`附件名称。上传中和处理中会显示 Shimmer 扫光。`}</p>
        <FieldTable fields={attachmentTitleFields} locale="zh-CN" />
        <h3>AttachmentDescription</h3>
        <p>{`文件类型、大小或上传状态等辅助信息。`}</p>
        <FieldTable fields={attachmentDescriptionFields} locale="zh-CN" />
        <h3>AttachmentActions</h3>
        <p>{`操作按钮的布局槽位，垂直排列时固定在右上角。`}</p>
        <FieldTable fields={attachmentActionsFields} locale="zh-CN" />
        <h3>AttachmentAction</h3>
        <p>{`图标操作按钮，基于 [bgcolor=#eee] button [/bgcolor] 的 [bgcolor=#eee] ghost [/bgcolor] 变体和 [bgcolor=#eee] icon-xs [/bgcolor] 尺寸。`}</p>
        <FieldTable fields={attachmentActionFields} locale="zh-CN" />
        <h3>AttachmentTrigger</h3>
        <p>{`覆盖整张卡片的透明触发层，位于操作按钮下方。`}</p>
        <FieldTable fields={attachmentTriggerFields} locale="zh-CN" />
        <h3>AttachmentGroup</h3>
        <p>{`横向排列多个附件的容器。`}</p>
        <FieldTable fields={attachmentGroupFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
