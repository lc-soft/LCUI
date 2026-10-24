import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CardBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/card-basic/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Card[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardFooter[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full max-w-sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Login to your account[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Enter your email below to login to your account`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color]>[/color]Sign Up[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"clip-safe flex flex-col gap-6"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-email"[/color]>[/color]Email[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"card-email"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]type[/color]=[color=#0a3069]"email"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]placeholder[/color]=[color=#0a3069]"m@example.com"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-password"[/color]>[/color]Password[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]className[/color]=[color=#0a3069]"ml-auto text-sm text-foreground"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              >[/color]`}</Text>
          <Text className="code-line">{`                Forgot your password?`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-password"[/color] [color=#0550ae]type[/color]=[color=#0a3069]"password"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardFooter[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-col gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Login`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Login with Google`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/card-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/card-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_input[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *placeholder,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                 [color=#953800]bool[/color] password)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_id(input, id);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(input, placeholder);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (password) {`}</Text>
          <Text className="code-line">{`                ui_textinput_set_password_char(input, [color=#0a3069]L&#x27;*&#x27;[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] input;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_card_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header_action = ui_create_card_action();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sign_up = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *form = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *email_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *email_label = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *email_input =`}</Text>
          <Text className="code-line">{`            create_input([color=#0a3069]"card-email"[/color], [color=#0a3069]"m@example.com"[/color], [color=#0550ae]false[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *password_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *password_row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *password_label = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *forgot = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *password_input = create_input([color=#0a3069]"card-password"[/color], [color=#0a3069]""[/color], [color=#0550ae]true[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer = ui_create_card_footer();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *login = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *google = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(card, [color=#0a3069]"card-basic-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Login to your account"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(description,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Enter your email below to login to your account"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(sign_up, [color=#0a3069]"Sign Up"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(sign_up, UI_BUTTON_VARIANT_LINK);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header_action, sign_up);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, header_action);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"clip-safe"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(form, [color=#0a3069]"card-form"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(form, [color=#0a3069]"clip-safe"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(email_field, [color=#0a3069]"card-field"[/color]);`}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(email_field, [color=#0a3069]"vertical"[/color]);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(email_label, [color=#0a3069]"card-email"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(email_label, [color=#0a3069]"Email"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(email_field, email_label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(email_field, email_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(form, email_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(password_field, [color=#0a3069]"card-field"[/color]);`}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(password_field, [color=#0a3069]"vertical"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(password_row, [color=#0a3069]"card-label-row"[/color]);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(password_label, [color=#0a3069]"card-password"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(password_label, [color=#0a3069]"Password"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(forgot, [color=#0a3069]"Forgot your password?"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(forgot, UI_BUTTON_VARIANT_LINK);`}</Text>
          <Text className="code-line">{`        ui_button_set_size(forgot, UI_BUTTON_SIZE_SM);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(forgot, [color=#0a3069]"card-forgot"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(password_row, password_label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(password_row, forgot);`}</Text>
          <Text className="code-line">{`        ui_widget_append(password_field, password_row);`}</Text>
          <Text className="code-line">{`        ui_widget_append(password_field, password_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(form, password_field);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, form);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(footer, [color=#0a3069]"card-footer-stack"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(login, [color=#0a3069]"Login"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(login, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(google, [color=#0a3069]"Login with Google"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(google, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, login);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, google);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, footer);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, card);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/card-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]card[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-basic-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-title[/color]>[/color]Login to your account[color=#116329]</[color=#0550ae]card-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-description[/color]>[/color]Enter your email below to login to your account[color=#116329]</[color=#0550ae]card-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color]>[/color]Sign Up[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-form clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-field"[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-email"[/color]>[/color]Email[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-email"[/color] [color=#0550ae]type[/color]=[color=#0a3069]"email"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"m@example.com"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-field"[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-label-row"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-password"[/color]>[/color]Password[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-forgot"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]Forgot your password?[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-password"[/color] [color=#0550ae]type[/color]=[color=#0a3069]"password"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-footer[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-footer-stack"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Login[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Login with Google[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/card-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
