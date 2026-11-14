import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import IconBrowser from "../../../../components/icon-browser";

export default function IconLibraryDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Icon Library</h1>
      <p className="doc-page-desc">{`[bgcolor=#7f7f7f29] @lcui/fluent-icons [/bgcolor] is an icon library adapted for LCUI. All icons come from Microsoft's fluentui-system-icons project.`}</p>
      <Widget className="doc-section">
        <h2>Installation</h2>
        <Widget className="code-block" data-language="sh" data-source="en/devtools/icon-library/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`npm install @lcui/fluent-icons`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`npm install @lcui/fluent-icons`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Choosing icons</h2>
        <p>{`Search and select icons in the browser below. For example, the zoom-in icon is typically named "Zoom In" in English.`}</p>
        <IconBrowser locale="en" />
        <p>{`Fluent UI System Icons provides direction-related metadata, but its icon data has no category field, so the detail panel does not show category tags.`}</p>
        <p>{`fluentui-system-icons icons are available in sizes like 16, 20, and 24, named in the format "icon name + size + style". [bgcolor=#7f7f7f29] @lcui/fluent-icons [/bgcolor] uses the naming format "icon name + style", and when the style is Regular it can be omitted.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/devtools/icon-library/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ZoomIn[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#116329]<[color=#0550ae]ZoomIn[/color] />[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]ZoomIn[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/fluent-icons"[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#7ee787]<[color=#79c0ff]ZoomIn[/color] />[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`The default glyph is regular 24; when an icon has no regular 24 glyph, the closest available regular size is used (ties choose the smaller size). [bgcolor=#7f7f7f29] size [/bgcolor] selects a design glyph size and, when explicitly set, also sets its display size. [bgcolor=#7f7f7f29] fontSize [/bgcolor] changes only the rendered size without changing glyph selection. If neither is set, the icon inherits the surrounding text size, so [bgcolor=#7f7f7f29] className [/bgcolor] or [bgcolor=#7f7f7f29] style [/bgcolor] can also control the display size:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/devtools/icon-library/2">
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
                    <Text className="code-line">{`<[color=#8250df]ZoomIn[/color] size={[color=#0550ae]32[/color]} />`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]ZoomIn[/color] size={[color=#79c0ff]32[/color]} />`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`In C, create a Fluent icon with the convenience constructor. In XML, omitting [bgcolor=#7f7f7f29] library [/bgcolor] defaults to Fluent; specify it only when using another icon library:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/devtools/icon-library/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#0a3069]"dismiss"[/color]);`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#a5d6ff]"dismiss"[/color]);`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="xml" data-source="en/devtools/icon-library/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"dismiss"[/color] />[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
