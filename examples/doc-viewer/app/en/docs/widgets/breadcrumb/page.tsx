import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import BreadcrumbDemoDemo from "../../../../examples/breadcrumb-demo/index";
import BreadcrumbBasicDemoDemo from "../../../../examples/breadcrumb-basic-demo/index";
import BreadcrumbSeparatorDemo from "../../../../examples/breadcrumb-separator/index";
import BreadcrumbCollapseDemo from "../../../../examples/breadcrumb-collapse/index";
import BreadcrumbCustomDemo from "../../../../examples/breadcrumb-custom/index";
import FieldTable from "../../../../components/field-table";
import { breadcrumbFields } from "../../../../widget-fields/breadcrumb";
import { breadcrumbListFields } from "../../../../widget-fields/breadcrumb-list";
import { breadcrumbItemFields } from "../../../../widget-fields/breadcrumb-item";
import { breadcrumbLinkFields } from "../../../../widget-fields/breadcrumb-link";
import { breadcrumbPageFields } from "../../../../widget-fields/breadcrumb-page";
import { breadcrumbSeparatorFields } from "../../../../widget-fields/breadcrumb-separator";
import { breadcrumbEllipsisFields } from "../../../../widget-fields/breadcrumb-ellipsis";

export default function BreadcrumbDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Breadcrumb</h1>
      <p className="doc-page-desc">{`Show the current resource path as hierarchical links and collapse middle nodes when space runs out.`}</p>
      <BreadcrumbDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: file browsers, documentation sites and settings pages that show where the current page sits in a hierarchy`}</Text></li>
          <li><Text>{`[b]Use[/b]: navigating from the full path back to any ancestor`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: step flows or progress display (use [bgcolor=#7f7f7f29] progress [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: switching between sibling pages (use [bgcolor=#7f7f7f29] sidebar [/bgcolor] or [bgcolor=#7f7f7f29] dropdown-menu [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/breadcrumb/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "204px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#8250df]Breadcrumb[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbEllipsis[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbItem[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbLink[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbList[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbPage[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#8250df]BreadcrumbSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Breadcrumb[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbEllipsis[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbItem[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbLink[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbList[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbPage[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]BreadcrumbSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/breadcrumb/1">
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
                    <Text className="code-line">{`<[color=#8250df]Breadcrumb[/color] path=[color=#0a3069]"/workspace/docs/components"[/color] />`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Breadcrumb[/color] path=[color=#a5d6ff]"/workspace/docs/components"[/color] />`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`The C API builds the breadcrumb by appending items and accepts custom labels:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/breadcrumb/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"workspace"[/color], [color=#0a3069]"Workspace"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"docs"[/color], [color=#0a3069]"Docs"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"components"[/color], [color=#0a3069]"Components"[/color]);`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#a5d6ff]"workspace"[/color], [color=#a5d6ff]"Workspace"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#a5d6ff]"docs"[/color], [color=#a5d6ff]"Docs"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#a5d6ff]"components"[/color], [color=#a5d6ff]"Components"[/color]);`}</Text>
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
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#7f7f7f29] Breadcrumb [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/breadcrumb/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Breadcrumb`}</Text>
                  <Text className="code-line">{`└── BreadcrumbList`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                  <Text className="code-line">{`    │   ├── BreadcrumbLink or BreadcrumbPage`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem (when collapsed)`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbEllipsis`}</Text>
                  <Text className="code-line">{`    │       └── DropdownMenu`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                  <Text className="code-line">{`    │   ├── BreadcrumbLink or BreadcrumbPage`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                  <Text className="code-line">{`    └── ...`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Breadcrumb`}</Text>
                  <Text className="code-line">{`└── BreadcrumbList`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                  <Text className="code-line">{`    │   ├── BreadcrumbLink or BreadcrumbPage`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem (when collapsed)`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbEllipsis`}</Text>
                  <Text className="code-line">{`    │       └── DropdownMenu`}</Text>
                  <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                  <Text className="code-line">{`    │   ├── BreadcrumbLink or BreadcrumbPage`}</Text>
                  <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                  <Text className="code-line">{`    └── ...`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Every item except the last one owns a separator; an item hidden during collapse hides its separator as well.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Basic</h3>
        <p>{`Declares three items explicitly; the last one uses [bgcolor=#7f7f7f29] BreadcrumbPage [/bgcolor] and ignores clicks.`}</p>
        <BreadcrumbBasicDemoDemo />
        <h3>Custom separator</h3>
        <p>{`Use the root's [bgcolor=#7f7f7f29] separator [/bgcolor] prop to replace the automatic separator icon.`}</p>
        <BreadcrumbSeparatorDemo />
        <h3>Overflow collapse</h3>
        <p>{`When the container is too narrow, items collapse starting from the node set by [bgcolor=#7f7f7f29] collapse-from [/bgcolor] while keeping as many trailing items as possible. It keeps collapsing leftwards only when they still do not fit, and the last item always stays visible. Collapsed items move into the ellipsis dropdown.`}</p>
        <BreadcrumbCollapseDemo />
        <h3>Custom items</h3>
        <p>{`[bgcolor=#7f7f7f29] append() [/bgcolor] accepts any widget. When custom content should participate in the collapse dropdown and events, set [bgcolor=#7f7f7f29] data-key [/bgcolor] / [bgcolor=#7f7f7f29] data-label [/bgcolor] before appending; the content handles its own clicks.`}</p>
        <BreadcrumbCustomDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Breadcrumb</h3>
        <p>{`Breadcrumb root. Owns the item model, the collapse state and the [bgcolor=#7f7f7f29] change [/bgcolor] event.`}</p>
        <FieldTable fields={breadcrumbFields} locale="en" />
        <h3>BreadcrumbList</h3>
        <p>{`Item list container. The root creates one by default, so it can be omitted when items are declared explicitly.`}</p>
        <FieldTable fields={breadcrumbListFields} locale="en" />
        <h3>BreadcrumbItem</h3>
        <p>{`Single item container. [bgcolor=#7f7f7f29] data-path [/bgcolor] supplies the item key.`}</p>
        <FieldTable fields={breadcrumbItemFields} locale="en" />
        <h3>BreadcrumbLink</h3>
        <p>{`Clickable ancestor item. Emits [bgcolor=#7f7f7f29] change [/bgcolor] from the root when clicked.`}</p>
        <FieldTable fields={breadcrumbLinkFields} locale="en" />
        <h3>BreadcrumbPage</h3>
        <p>{`The current item. Not clickable and emits no [bgcolor=#7f7f7f29] change [/bgcolor].`}</p>
        <FieldTable fields={breadcrumbPageFields} locale="en" />
        <h3>BreadcrumbSeparator</h3>
        <p>{`Separator between items. Renders the [bgcolor=#7f7f7f29] chevron-right [/bgcolor] icon by default.`}</p>
        <FieldTable fields={breadcrumbSeparatorFields} locale="en" />
        <h3>BreadcrumbEllipsis</h3>
        <p>{`Collapse indicator showing the [bgcolor=#7f7f7f29] MoreHorizontal [/bgcolor] icon. The root inserts it automatically on overflow.`}</p>
        <FieldTable fields={breadcrumbEllipsisFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]The [bgcolor=#7f7f7f29] change [/bgcolor] payload is the item key.[/b] The event is emitted by the [bgcolor=#7f7f7f29] breadcrumb [/bgcolor] root with [bgcolor=#7f7f7f29] emit_arg [/bgcolor] set to [bgcolor=#7f7f7f29] const char *key [/bgcolor]. The key is only valid for the duration of the dispatch; copy it if you need to keep it.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]A key identifies the item name, not the full path.[/b] The [bgcolor=#7f7f7f29] key [/bgcolor] passed to [bgcolor=#7f7f7f29] append_item() [/bgcolor] only identifies the node and may repeat. The breadcrumb never navigates or changes state based on it.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]Custom content does not emit events automatically.[/b] Widgets appended with [bgcolor=#7f7f7f29] append() [/bgcolor] handle their own interaction; only standard [bgcolor=#7f7f7f29] BreadcrumbLink [/bgcolor] items and ellipsis dropdown entries emit [bgcolor=#7f7f7f29] change [/bgcolor]. The final [bgcolor=#7f7f7f29] BreadcrumbPage [/bgcolor] and disabled items emit nothing.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]Prefer the C API for custom labels.[/b] The label provider of [bgcolor=#7f7f7f29] build_from_path() [/bgcolor] only applies to that build call; the XML / TSX [bgcolor=#7f7f7f29] path [/bgcolor] prop has no provider, so labels fall back to path segments. Use explicit items or [bgcolor=#7f7f7f29] ui_breadcrumb_build_from_path() [/bgcolor] when you need custom labels.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]For an item switcher menu, compose a [bgcolor=#7f7f7f29] DropdownMenu [/bgcolor] inside the item.[/b] The ellipsis in the demo above is a [bgcolor=#7f7f7f29] BreadcrumbItem [/bgcolor] + [bgcolor=#7f7f7f29] DropdownMenu [/bgcolor]; no dedicated API is required.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]Declarative usage is static by default.[/b] Writing [bgcolor=#7f7f7f29] BreadcrumbItem [/bgcolor] and [bgcolor=#7f7f7f29] BreadcrumbLink [/bgcolor] in XML / TSX needs no [bgcolor=#7f7f7f29] data-* [/bgcolor]; add [bgcolor=#7f7f7f29] data-path [/bgcolor] / [bgcolor=#7f7f7f29] data-key [/bgcolor] / [bgcolor=#7f7f7f29] data-label [/bgcolor] or use the C API only when you need the [bgcolor=#7f7f7f29] change [/bgcolor] event or collapse labels.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Explicit mode does not insert separators automatically.[/b] Put [bgcolor=#7f7f7f29] BreadcrumbSeparator [/bgcolor] inside the matching item; it hides together with the item on collapse. [bgcolor=#7f7f7f29] data-* [/bgcolor] is only used for behavior lookup, not CSS selectors — style with classes or the [bgcolor=#7f7f7f29] breadcrumb-item:collapsed [/bgcolor] status.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
