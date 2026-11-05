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
      <p className="doc-page-desc">{`用层级链接展示当前资源所在的路径，并在空间不足时自动折叠中间节点。`}</p>
      <BreadcrumbDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：文件浏览器、文档站、设置页等需要展示当前位置层级的页面`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要从完整路径快速回退到任意上级节点的导航`}</Text></li>
          <li><Text>{`[b]不适用[/b]：步骤流程或进度展示（使用 [bgcolor=#eee] progress [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：同级页面之间的切换（使用 [bgcolor=#eee] sidebar [/bgcolor] 或 [bgcolor=#eee] dropdown-menu [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/breadcrumb/0">
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
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/breadcrumb/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Breadcrumb[/color] path=[color=#0a3069]"/workspace/docs/components"[/color] />`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`C API 通过追加节点构建，可以传入自定义标签：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/breadcrumb/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"workspace"[/color], [color=#0a3069]"工作区"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"docs"[/color], [color=#0a3069]"项目文档"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`ui_breadcrumb_append_item(breadcrumb, [color=#0a3069]"components"[/color], [color=#0a3069]"组件库"[/color]);`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#eee] Breadcrumb [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/breadcrumb/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Breadcrumb`}</Text>
                <Text className="code-line">{`└── BreadcrumbList`}</Text>
                <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                <Text className="code-line">{`    │   ├── BreadcrumbLink 或 BreadcrumbPage`}</Text>
                <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                <Text className="code-line">{`    ├── BreadcrumbItem（折叠时）`}</Text>
                <Text className="code-line">{`    │   └── BreadcrumbEllipsis`}</Text>
                <Text className="code-line">{`    │       └── DropdownMenu`}</Text>
                <Text className="code-line">{`    ├── BreadcrumbItem`}</Text>
                <Text className="code-line">{`    │   ├── BreadcrumbLink 或 BreadcrumbPage`}</Text>
                <Text className="code-line">{`    │   └── BreadcrumbSeparator`}</Text>
                <Text className="code-line">{`    └── ...`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`除最后一项外，每个 item 自带一个分隔符；折叠时被隐藏的 item 会连同分隔符一起隐藏。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>基础</h3>
        <p>{`显式声明三个节点；最后一项使用 [bgcolor=#eee] BreadcrumbPage [/bgcolor]，不响应点击。`}</p>
        <BreadcrumbBasicDemoDemo />
        <h3>自定义分隔符</h3>
        <p>{`通过根节点的 [bgcolor=#eee] separator [/bgcolor] 属性替换自动分隔符的图标。`}</p>
        <BreadcrumbSeparatorDemo />
        <h3>溢出折叠</h3>
        <p>{`容器宽度不足时，从 [bgcolor=#eee] collapse-from [/bgcolor] 指定的节点开始折叠，并尽可能多地保留尾部节点；只有仍然放不下时才继续向左折叠，最后一个节点始终显示。折叠后的节点收进省略号下拉菜单。`}</p>
        <BreadcrumbCollapseDemo />
        <h3>自定义节点</h3>
        <p>{`[bgcolor=#eee] append() [/bgcolor] 可以追加任意 widget。自定义内容需要参与折叠下拉和事件时，在追加前设置 [bgcolor=#eee] data-key [/bgcolor] / [bgcolor=#eee] data-label [/bgcolor]；其点击行为由内容自己处理。`}</p>
        <BreadcrumbCustomDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Breadcrumb</h3>
        <p>{`面包屑根节点，持有节点模型、折叠状态和 [bgcolor=#eee] change [/bgcolor] 事件。`}</p>
        <FieldTable fields={breadcrumbFields} locale="zh-CN" />
        <h3>BreadcrumbList</h3>
        <p>{`节点列表容器；根节点默认创建一个，显式声明时可以省略。`}</p>
        <FieldTable fields={breadcrumbListFields} locale="zh-CN" />
        <h3>BreadcrumbItem</h3>
        <p>{`单个节点容器，通过 [bgcolor=#eee] data-path [/bgcolor] 提供节点 key。`}</p>
        <FieldTable fields={breadcrumbItemFields} locale="zh-CN" />
        <h3>BreadcrumbLink</h3>
        <p>{`可点击的上级节点，点击后从根节点发出 [bgcolor=#eee] change [/bgcolor]。`}</p>
        <FieldTable fields={breadcrumbLinkFields} locale="zh-CN" />
        <h3>BreadcrumbPage</h3>
        <p>{`当前节点，不可点击，也不发出 [bgcolor=#eee] change [/bgcolor]。`}</p>
        <FieldTable fields={breadcrumbPageFields} locale="zh-CN" />
        <h3>BreadcrumbSeparator</h3>
        <p>{`节点之间的分隔符，默认渲染 [bgcolor=#eee] chevron-right [/bgcolor] 图标。`}</p>
        <FieldTable fields={breadcrumbSeparatorFields} locale="zh-CN" />
        <h3>BreadcrumbEllipsis</h3>
        <p>{`折叠指示器，显示 [bgcolor=#eee] MoreHorizontal [/bgcolor] 图标；由根节点在溢出时自动插入。`}</p>
        <FieldTable fields={breadcrumbEllipsisFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] change [/bgcolor] 的载荷是节点 key[/b]。事件从 [bgcolor=#eee] breadcrumb [/bgcolor] 根节点发出，[bgcolor=#eee] emit_arg [/bgcolor] 为 [bgcolor=#eee] const char *key [/bgcolor]；key 只在本次分发期间有效，需要长期保存时请自行复制。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]key 表示节点名，不是完整路径[/b]。[bgcolor=#eee] append_item() [/bgcolor] 的 [bgcolor=#eee] key [/bgcolor] 只用于标识节点，允许重复；面包屑不会根据 key 自动导航，也不会修改自身状态。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]自定义内容不自动发事件[/b]。通过 [bgcolor=#eee] append() [/bgcolor] 追加的 widget 由调用方处理交互；只有标准 [bgcolor=#eee] BreadcrumbLink [/bgcolor] 和省略号下拉项会发出 [bgcolor=#eee] change [/bgcolor]。最后一项 [bgcolor=#eee] BreadcrumbPage [/bgcolor] 与禁用 item 同样不发出。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]自定义标签优先使用 C API[/b]。[bgcolor=#eee] build_from_path() [/bgcolor] 的 label provider 只在本次构建调用中生效；XML / TSX 的 [bgcolor=#eee] path [/bgcolor] 属性没有 provider，标签回退为路径段。需要自定义标签时，使用显式 item 或 [bgcolor=#eee] ui_breadcrumb_build_from_path() [/bgcolor]。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]需要「节点切换菜单」时，在 item 内组合 [bgcolor=#eee] DropdownMenu [/bgcolor][/b]。顶部示例中的省略号就是一个 [bgcolor=#eee] BreadcrumbItem [/bgcolor] + [bgcolor=#eee] DropdownMenu [/bgcolor]，无需额外的专用 API。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]声明式写法默认是静态展示[/b]。XML / TSX 里直接写 [bgcolor=#eee] BreadcrumbItem [/bgcolor] 和 [bgcolor=#eee] BreadcrumbLink [/bgcolor] 即可，不需要 [bgcolor=#eee] data-* [/bgcolor]；只有需要 [bgcolor=#eee] change [/bgcolor] 事件或折叠下拉标签时，才在 item 上设置 [bgcolor=#eee] data-path [/bgcolor] / [bgcolor=#eee] data-key [/bgcolor] / [bgcolor=#eee] data-label [/bgcolor]，或改用 C API。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]显式模式不会自动插入分隔符[/b]。调用方需要把 [bgcolor=#eee] BreadcrumbSeparator [/bgcolor] 放在对应 item 内部；折叠时它会随 item 一起隐藏。[bgcolor=#eee] data-* [/bgcolor] 只用于行为查找，不参与 CSS 选择器，样式请使用 class 或 [bgcolor=#eee] breadcrumb-item:collapsed [/bgcolor] 状态。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
