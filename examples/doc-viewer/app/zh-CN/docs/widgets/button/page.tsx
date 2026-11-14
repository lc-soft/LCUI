import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ButtonBasicDemo from "../../../../examples/button-basic/index";
import ButtonVariantsDemo from "../../../../examples/button-variants/index";
import ButtonIconDemo from "../../../../examples/button-icon/index";
import ButtonWithIconDemo from "../../../../examples/button-with-icon/index";
import ButtonSizeDemo from "../../../../examples/button-size/index";
import ButtonDisabledDemo from "../../../../examples/button-disabled/index";
import FieldTable from "../../../../components/field-table";
import { buttonFields } from "../../../../widget-fields/button";

export default function ButtonDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Button</h1>
      <p className="doc-page-desc">{`可点击的按钮部件，通常用于触发某个操作。`}</p>
      <ButtonBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：用户点击执行动作（提交表单、打开页面、切换状态）`}</Text></li>
          <li><Text>{`[b]适用[/b]：工具栏、对话框、导航栏中的交互入口`}</Text></li>
          <li><Text>{`[b]不适用[/b]：不需要交互的纯文本展示（用 [bgcolor=#7f7f7f29] text [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/button/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Button[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/button/1">
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
                    <Text className="code-line">{`<[color=#8250df]Button[/color]>[color=#8250df]Button[/color]</[color=#8250df]Button[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Button[/color]>[color=#d2a8ff]Button[/color]</[color=#d2a8ff]Button[/color]>`}</Text>
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
        <h2>变体</h2>
        <p>{`Button 默认使用描边（outline）外观，无需显式指定；主操作使用 [bgcolor=#7f7f7f29] primary [/bgcolor]，次级操作使用 [bgcolor=#7f7f7f29] secondary [/bgcolor]，低强调操作可使用 [bgcolor=#7f7f7f29] ghost [/bgcolor]，危险操作可使用 [bgcolor=#7f7f7f29] destructive [/bgcolor]，[bgcolor=#7f7f7f29] link [/bgcolor] 则会在 hover 时显示下划线。`}</p>
        <ButtonVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>图标</h2>
        <p>{`[bgcolor=#7f7f7f29] icon [/bgcolor] 是尺寸而不是视觉变体。通过 [bgcolor=#7f7f7f29] size="icon" [/bgcolor] 将按钮设置为紧凑的方形，可与任意变体组合。`}</p>
        <ButtonIconDemo />
        <h3>带图标</h3>
        <p>{`图标与文字通过按钮内部的弹性布局自动横排对齐，图标在文字前即为图标前置，在文字后即为图标后置，两者的间距都由按钮的 gap 控制。`}</p>
        <ButtonWithIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>尺寸</h2>
        <p>{`使用 [bgcolor=#7f7f7f29] size [/bgcolor] 属性设置按钮的大小。`}</p>
        <ButtonSizeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>禁用状态</h2>
        <p>{`为按钮设置 [bgcolor=#7f7f7f29] disabled [/bgcolor] 后，其整体透明度降低，且不再响应鼠标与键盘交互；各变体的禁用表现一致。`}</p>
        <ButtonDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={buttonFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
