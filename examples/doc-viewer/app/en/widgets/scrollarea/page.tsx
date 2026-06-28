import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ScrollareaBasicDemo from "../../../examples/scrollarea-basic/index";

export default function ScrollareaDocPage() {
  return (
    <Widget className="doc-page">
      <h1>ScrollArea</h1>
      <p className="doc-page-desc">{`A container that provides scrollable content when children overflow.`}</p>
      <ScrollareaBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b] — content height or width may exceed the container (long lists, documents, logs).`}</Text></li>
          <li><Text>{`[b]Suitable[/b] — scrollbars needed for visual feedback.`}</Text></li>
          <li><Text>{`[b]Not suitable[/b] — content always fits within the container (use a plain [bgcolor=#eee] widget [/bgcolor]).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/scrollarea/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ScrollArea[/color], [color=#8250df]ScrollAreaContent[/color], [color=#8250df]Scrollbar[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/scrollarea/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]ScrollArea[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ScrollAreaContent[/color]>[/color]{/* child content */}[color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Scrollbar[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]ScrollArea[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] ScrollArea [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/scrollarea/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ScrollArea`}</Text>
            <Text className="code-line">{`├── ScrollAreaContent`}</Text>
            <Text className="code-line">{`└── Scrollbar`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API</h2>
        <h3>`ui_create_scrollarea`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *[color=#8250df]ui_create_scrollarea[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Creates and returns a new scroll area widget.`}</p>
        <h3>`ui_create_scrollarea_content`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *[color=#8250df]ui_create_scrollarea_content[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Creates and returns a new scroll content container to be appended inside a scroll area.`}</p>
        <h3>`ui_scrollarea_set_scroll_top`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_scrollarea_set_scroll_top[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]float[/color] value)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the vertical scroll position in pixels.`}</p>
        <h3>`ui_scrollarea_set_scroll_left`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_scrollarea_set_scroll_left[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]float[/color] value)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the horizontal scroll position in pixels.`}</p>
        <h3>`ui_scrollarea_get_scroll_top`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]float[/color] [color=#8250df]ui_scrollarea_get_scroll_top[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Returns the current vertical scroll position.`}</p>
        <h3>`ui_scrollarea_get_scroll_left`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/8">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]float[/color] [color=#8250df]ui_scrollarea_get_scroll_left[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Returns the current horizontal scroll position.`}</p>
        <h3>`ui_scrollarea_get_scroll_width`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/9">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]float[/color] [color=#8250df]ui_scrollarea_get_scroll_width[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Returns the total scrollable width of the content.`}</p>
        <h3>`ui_scrollarea_get_scroll_height`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/10">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]float[/color] [color=#8250df]ui_scrollarea_get_scroll_height[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Returns the total scrollable height of the content.`}</p>
        <h3>`ui_scrollarea_set_wheel_scroll_direction`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/11">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_scrollarea_set_wheel_scroll_direction[/color][color=#0550ae]([/color]`}</Text>
            <Text className="code-line">{`[color=#0550ae]    [color=#953800]ui_widget_t[/color] *w, [color=#953800]ui_scrollarea_direction_t[/color] direction)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets which direction the mouse wheel scrolls.`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "232px" }}>
              <Text>{`Value`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow mdx-table-head-cell" style={{ minWidth: "48px" }}>
              <Text>{`Description`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "232px" }}>
              <Text>{`[bgcolor=#eee] UI_SCROLLAREA_AUTO [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Vertical if content is taller; horizontal if wider`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "232px" }}>
              <Text>{`[bgcolor=#eee] UI_SCROLLAREA_VERTICAL [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Always scroll vertically`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "232px" }}>
              <Text>{`[bgcolor=#eee] UI_SCROLLAREA_HORIZONTAL [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Always scroll horizontally`}</Text>
            </Widget>
          </Widget>
        </Widget>
        <h3>`ui_scrollarea_update`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/scrollarea/12">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_scrollarea_update[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Forces the scroll area to recalculate its layout and scrollbar positions.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>XML tag</h2>
        <Widget className="code-block" data-language="xml" data-source="en/widgets/scrollarea/13">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]scrollarea[/color] [color=#0550ae]style[/color]=[color=#0a3069]"width: 300px; height: 200px;"[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#6e7781]<!-- content items -->[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]</[color=#0550ae]scrollarea[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
