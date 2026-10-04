import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function BadgeIconDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="c" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="ui.xml" style={{ display: "none" }}>ui.xml</Text>
            <Text className="demo-tab" data-language="xml" data-value="main.c" style={{ display: "none" }}>main.c</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="tsx">TSX</Text>
            <Text className="demo-tab" data-value="c">C</Text>
            <Text className="demo-tab" data-value="xml">XML</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/badge-icon/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Badge[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Bookmark[/color], [color=#8250df]CheckmarkStarburst[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-wrap gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"secondary"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CheckmarkStarburst[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color]>[/color]Verified[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color]>[/color]Bookmark[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Bookmark[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/badge-icon/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]badge_icon_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/badge-icon/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]badge_icon_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *wrapper = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *verified = ui_create_badge();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *verified_icon =`}</Text>
          <Text className="code-line">{`            ui_create_fluent_icon([color=#0a3069]"checkmark-starburst"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *verified_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *bookmark = ui_create_badge();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *bookmark_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *bookmark_icon = ui_create_fluent_icon([color=#0a3069]"bookmark"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(wrapper, [color=#0a3069]"badge-icon-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_badge_set_variant(verified, UI_BADGE_VARIANT_SECONDARY);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(verified_text, [color=#0a3069]"Verified"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(verified, verified_icon);`}</Text>
          <Text className="code-line">{`        ui_widget_append(verified, verified_text);`}</Text>
          <Text className="code-line">{`        ui_badge_set_variant(bookmark, UI_BADGE_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(bookmark_text, [color=#0a3069]"Bookmark"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(bookmark, bookmark_text);`}</Text>
          <Text className="code-line">{`        ui_widget_append(bookmark, bookmark_icon);`}</Text>
          <Text className="code-line">{`        ui_widget_append(wrapper, verified);`}</Text>
          <Text className="code-line">{`        ui_widget_append(wrapper, bookmark);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, wrapper);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/badge-icon/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"badge-icon-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"secondary"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"checkmark-starburst"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color]>[/color]Verified[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color]>[/color]Bookmark[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"bookmark"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/badge-icon/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]badge_icon_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
