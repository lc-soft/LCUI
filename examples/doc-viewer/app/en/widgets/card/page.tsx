import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import CardBasicDemo from "../../../examples/card-basic/index";
import CardSmallDemo from "../../../examples/card-small/index";
import CardImageDemo from "../../../examples/card-image/index";
import FieldTable from "../../../components/field-table";
import { cardFields } from "../../../widget-fields/card";
import { cardHeaderFields } from "../../../widget-fields/card-header";
import { cardTitleFields } from "../../../widget-fields/card-title";
import { cardDescriptionFields } from "../../../widget-fields/card-description";
import { cardActionFields } from "../../../widget-fields/card-action";
import { cardContentFields } from "../../../widget-fields/card-content";
import { cardFooterFields } from "../../../widget-fields/card-footer";

export default function CardDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Card</h1>
      <p className="doc-page-desc">{`A container for related information and actions, composed of a header, content, and footer.`}</p>
      <CardBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: forms, account details, settings, article summaries, and other grouped content`}</Text></li>
          <li><Text>{`[b]Use[/b]: cards that need a lightweight action such as edit or more in the top-right corner`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: large expandable/collapsible content (use [bgcolor=#eee] dialog [/bgcolor] or a dedicated page)`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: short status messages (use [bgcolor=#eee] alert [/bgcolor] or [bgcolor=#eee] toast [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/card/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Card[/color], [color=#8250df]CardContent[/color], [color=#8250df]CardDescription[/color], [color=#8250df]CardFooter[/color], [color=#8250df]CardHeader[/color], [color=#8250df]CardTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/card/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
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
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Card [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/card/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Card`}</Text>
                <Text className="code-line">{`├── CardHeader`}</Text>
                <Text className="code-line">{`│   ├── CardTitle`}</Text>
                <Text className="code-line">{`│   ├── CardDescription`}</Text>
                <Text className="code-line">{`│   └── CardAction`}</Text>
                <Text className="code-line">{`├── CardContent`}</Text>
                <Text className="code-line">{`└── CardFooter`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Small</h3>
        <p>{`Use [bgcolor=#eee] size="sm" [/bgcolor] for tighter spacing and a smaller title.`}</p>
        <CardSmallDemo />
        <h3>Image</h3>
        <p>{`Place cover content before [bgcolor=#eee] CardHeader [/bgcolor]. LCUI currently has no standalone image widget, so the example uses a solid placeholder [bgcolor=#eee] div [/bgcolor] with an absolutely positioned overlay to simulate a cover image.`}</p>
        <CardImageDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Card</h3>
        <p>{`The card container manages its size and child layout.`}</p>
        <FieldTable fields={cardFields} locale="en" />
        <h3>CardHeader</h3>
        <p>{`The card header holds the title, description, and optional action.`}</p>
        <FieldTable fields={cardHeaderFields} locale="en" />
        <h3>CardTitle</h3>
        <p>{`The card title.`}</p>
        <FieldTable fields={cardTitleFields} locale="en" />
        <h3>CardDescription</h3>
        <p>{`Supporting text below the title.`}</p>
        <FieldTable fields={cardDescriptionFields} locale="en" />
        <h3>CardAction</h3>
        <p>{`The top-right action area in the header.`}</p>
        <FieldTable fields={cardActionFields} locale="en" />
        <h3>CardContent</h3>
        <p>{`The main card body.`}</p>
        <FieldTable fields={cardContentFields} locale="en" />
        <h3>CardFooter</h3>
        <p>{`The bottom area for actions or secondary content.`}</p>
        <FieldTable fields={cardFooterFields} locale="en" />
      </Widget>
    </Widget>
  );
}
