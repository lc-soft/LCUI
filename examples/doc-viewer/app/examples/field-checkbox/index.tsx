import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldCheckboxDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-checkbox/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Checkbox[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLegend[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSeparator[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Show these items on the desktop[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Select the items you want to show on the desktop.[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color] [color=#0550ae]className[/color]=[color=#0a3069]"gap-3"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color] [color=#0550ae]defaultChecked[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Hard disks`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              External disks`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              CDs, DVDs, and iPods`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Connected servers`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSeparator[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color] [color=#0550ae]defaultChecked[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Sync Desktop [color=#0550ae]&amp;[/color] Documents folders`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Your Desktop [color=#0550ae]&amp;[/color] Documents folders are being synced with iCloud`}</Text>
          <Text className="code-line">{`            Drive. You can access them from other devices.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-checkbox/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_checkbox_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        example_load(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-checkbox/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox/main.c: -- Field widget demo (C variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]make_check_row[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent, [color=#953800]const[/color] [color=#953800]char[/color] *id,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                           [color=#953800]const[/color] [color=#953800]char[/color] *text, [color=#953800]int[/color] checked)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cb = ui_create_checkbox();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(row, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(cb, id);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (checked) {`}</Text>
          <Text className="code-line">{`                ui_widget_set_attr(cb, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, cb);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(lbl, id);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl, text);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, lbl);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_checkbox_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *[color=#953800]set[/color], *legend, *desc, *group;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sep;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sync_field, *sync_cb, *sync_content;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sync_lbl, *sync_desc;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]set[/color] = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        legend = ui_create_field_legend();`}</Text>
          <Text className="code-line">{`        desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        group = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        sep = ui_create_field_separator();`}</Text>
          <Text className="code-line">{`        sync_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        sync_cb = ui_create_checkbox();`}</Text>
          <Text className="code-line">{`        sync_content = ui_create_field_content();`}</Text>
          <Text className="code-line">{`        sync_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        sync_desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"max-width"[/color], [color=#0a3069]"400px"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(legend, [color=#0a3069]"Show these items on the desktop"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], legend);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            desc, [color=#0a3069]"Select the items you want to show on the desktop."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], desc);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(group, [color=#0a3069]"gap"[/color], [color=#0a3069]"12px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], group);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color], [color=#0a3069]"Hard disks"[/color],`}</Text>
          <Text className="code-line">{`                       [color=#0550ae]1[/color]);`}</Text>
          <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color],`}</Text>
          <Text className="code-line">{`                       [color=#0a3069]"External disks"[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color],`}</Text>
          <Text className="code-line">{`                       [color=#0a3069]"CDs, DVDs, and iPods"[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color],`}</Text>
          <Text className="code-line">{`                       [color=#0a3069]"Connected servers"[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, [color=#953800]set[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, sep);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(sync_field, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(sync_cb, [color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(sync_cb, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(sync_field, sync_cb);`}</Text>
          <Text className="code-line">{`        ui_widget_append(sync_field, sync_content);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(sync_lbl, [color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(sync_lbl, [color=#0a3069]"Sync Desktop & Documents folders"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(sync_content, sync_lbl);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            sync_desc, [color=#0a3069]"Your Desktop & Documents folders are being synced with "[/color]`}</Text>
          <Text className="code-line">{`                       [color=#0a3069]"iCloud Drive. You can access them from other devices."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(sync_content, sync_desc);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, sync_field);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-checkbox/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:column; gap:8px; max-width:400px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Show these items on the desktop[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Select the items you want to show on the desktop.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color] [color=#0550ae]style[/color]=[color=#0a3069]"gap:12px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color]>[/color]Hard disks[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color]>[/color]External disks[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color]>[/color]CDs, DVDs, and iPods[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color]>[/color]Connected servers[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]>[/color]Sync Desktop [color=#0550ae]&amp;[/color] Documents folders[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Your Desktop [color=#0550ae]&amp;[/color] Documents folders are being synced with iCloud Drive. You can access them from other devices.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-checkbox/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
