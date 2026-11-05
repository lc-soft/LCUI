import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import SidebarBasicDemo from "../../../../examples/sidebar-basic/index";
import SidebarGroupCollapsibleDemo from "../../../../examples/sidebar-group-collapsible/index";
import SidebarCollapsedDemo from "../../../../examples/sidebar-collapsed/index";
import FieldTable from "../../../../components/field-table";
import { sidebarProviderFields } from "../../../../widget-fields/sidebar-provider";
import { sidebarFields } from "../../../../widget-fields/sidebar";
import { sidebarTriggerFields } from "../../../../widget-fields/sidebar-trigger";
import { sidebarRailFields } from "../../../../widget-fields/sidebar-rail";
import { sidebarHeaderFields } from "../../../../widget-fields/sidebar-header";
import { sidebarFooterFields } from "../../../../widget-fields/sidebar-footer";
import { sidebarContentFields } from "../../../../widget-fields/sidebar-content";
import { sidebarGroupFields } from "../../../../widget-fields/sidebar-group";
import { sidebarGroupLabelFields } from "../../../../widget-fields/sidebar-group-label";
import { sidebarGroupActionFields } from "../../../../widget-fields/sidebar-group-action";
import { sidebarGroupContentFields } from "../../../../widget-fields/sidebar-group-content";
import { sidebarMenuFields } from "../../../../widget-fields/sidebar-menu";
import { sidebarMenuItemFields } from "../../../../widget-fields/sidebar-menu-item";
import { sidebarMenuActionFields } from "../../../../widget-fields/sidebar-menu-action";
import { sidebarMenuSubFields } from "../../../../widget-fields/sidebar-menu-sub";
import { sidebarMenuSubItemFields } from "../../../../widget-fields/sidebar-menu-sub-item";
import { sidebarMenuBadgeFields } from "../../../../widget-fields/sidebar-menu-badge";

export default function SidebarDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Sidebar</h1>
      <p className="doc-page-desc">{`用于构建可折叠应用导航侧栏的组合式部件。`}</p>
      <SidebarBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：后台、文档站、桌面应用的主导航侧栏`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要在展开、图标收起和无收起模式间切换的布局`}</Text></li>
          <li><Text>{`[b]不适用[/b]：只需要一个普通按钮组或短列表（使用 [bgcolor=#eee] button [/bgcolor]、[bgcolor=#eee] menu [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/sidebar/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]SidebarProvider[/color], [color=#8250df]SidebarTrigger[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/sidebar/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]SidebarProvider[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SidebarContent[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]Sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]main[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SidebarTrigger[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]main[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`</[color=#8250df]SidebarProvider[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#eee] Sidebar [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/sidebar/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`SidebarProvider`}</Text>
                <Text className="code-line">{`├── Sidebar`}</Text>
                <Text className="code-line">{`│   ├── SidebarHeader`}</Text>
                <Text className="code-line">{`│   ├── SidebarContent`}</Text>
                <Text className="code-line">{`│   │   └── SidebarGroup`}</Text>
                <Text className="code-line">{`│   │       ├── SidebarGroupLabel`}</Text>
                <Text className="code-line">{`│   │       ├── SidebarGroupAction`}</Text>
                <Text className="code-line">{`│   │       └── SidebarGroupContent`}</Text>
                <Text className="code-line">{`│   │           └── SidebarMenu`}</Text>
                <Text className="code-line">{`│   │               └── SidebarMenuItem`}</Text>
                <Text className="code-line">{`│   │                   ├── SidebarMenuButton（class）`}</Text>
                <Text className="code-line">{`│   │                   ├── SidebarMenuAction`}</Text>
                <Text className="code-line">{`│   │                   ├── SidebarMenuBadge`}</Text>
                <Text className="code-line">{`│   │                   └── SidebarMenuSub`}</Text>
                <Text className="code-line">{`│   ├── SidebarFooter`}</Text>
                <Text className="code-line">{`│   └── SidebarRail`}</Text>
                <Text className="code-line">{`└── SidebarTrigger`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>可折叠分组</h3>
        <p>{`将 [bgcolor=#eee] SidebarGroup [/bgcolor] 放进 [bgcolor=#eee] Collapsible [/bgcolor]，并给 [bgcolor=#eee] SidebarGroupLabel [/bgcolor] 加上 [bgcolor=#eee] data-collapsible-trigger [/bgcolor]，即可点击分组标题折叠内容。`}</p>
        <SidebarGroupCollapsibleDemo />
        <h3>图标收起</h3>
        <p>{`[bgcolor=#eee] collapsible="icon" [/bgcolor] 收起时保留图标并隐藏菜单文字；Trigger 可以重新展开。`}</p>
        <SidebarCollapsedDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>SidebarProvider</h3>
        <p>{`侧栏状态容器，保存展开/收起状态，并向上查找最近的 Provider。`}</p>
        <FieldTable fields={sidebarProviderFields} locale="zh-CN" />
        <h3>Sidebar</h3>
        <p>{`主侧栏面板，负责宽度、边框和折叠模式。`}</p>
        <FieldTable fields={sidebarFields} locale="zh-CN" />
        <h3>SidebarTrigger</h3>
        <p>{`切换最近 Provider 的按钮，默认使用 [bgcolor=#eee] panel-left [/bgcolor] 图标。`}</p>
        <FieldTable fields={sidebarTriggerFields} locale="zh-CN" />
        <h3>SidebarRail</h3>
        <p>{`位于侧栏边缘的窄触发区域，点击同样切换侧栏。`}</p>
        <FieldTable fields={sidebarRailFields} locale="zh-CN" />
        <h3>SidebarHeader</h3>
        <p>{`侧栏头部容器，用于品牌、工作区切换或标题。`}</p>
        <FieldTable fields={sidebarHeaderFields} locale="zh-CN" />
        <h3>SidebarFooter</h3>
        <p>{`侧栏底部容器，通常放置设置或账户入口。`}</p>
        <FieldTable fields={sidebarFooterFields} locale="zh-CN" />
        <h3>SidebarContent</h3>
        <p>{`侧栏主体区域；它只负责 flex 布局，需要滚动时在内部组合 [bgcolor=#eee] ScrollArea [/bgcolor]。`}</p>
        <FieldTable fields={sidebarContentFields} locale="zh-CN" />
        <h3>SidebarGroup</h3>
        <p>{`侧栏分组容器，可包含 Label、Action 和 Content。`}</p>
        <FieldTable fields={sidebarGroupFields} locale="zh-CN" />
        <h3>SidebarGroupLabel</h3>
        <p>{`分组标题；添加 [bgcolor=#eee] data-collapsible-trigger [/bgcolor] 后可切换外层 Collapsible。`}</p>
        <FieldTable fields={sidebarGroupLabelFields} locale="zh-CN" />
        <h3>SidebarGroupAction</h3>
        <p>{`分组标题右侧的操作按钮。`}</p>
        <FieldTable fields={sidebarGroupActionFields} locale="zh-CN" />
        <h3>SidebarGroupContent</h3>
        <p>{`分组内容容器，通常承载 SidebarMenu。`}</p>
        <FieldTable fields={sidebarGroupContentFields} locale="zh-CN" />
        <h3>SidebarMenu</h3>
        <p>{`侧栏菜单列表。`}</p>
        <FieldTable fields={sidebarMenuFields} locale="zh-CN" />
        <h3>SidebarMenuItem</h3>
        <p>{`菜单项容器，可包含菜单按钮、操作、徽标和子菜单。`}</p>
        <FieldTable fields={sidebarMenuItemFields} locale="zh-CN" />
        <h3>SidebarMenuAction</h3>
        <p>{`菜单项右侧的轻量操作按钮。`}</p>
        <FieldTable fields={sidebarMenuActionFields} locale="zh-CN" />
        <h3>SidebarMenuSub</h3>
        <p>{`菜单项内的子菜单容器。`}</p>
        <FieldTable fields={sidebarMenuSubFields} locale="zh-CN" />
        <h3>SidebarMenuSubItem</h3>
        <p>{`子菜单项容器。`}</p>
        <FieldTable fields={sidebarMenuSubItemFields} locale="zh-CN" />
        <h3>SidebarMenuBadge</h3>
        <p>{`菜单项右侧的徽标。`}</p>
        <FieldTable fields={sidebarMenuBadgeFields} locale="zh-CN" />
        <h3>SidebarMenuButton</h3>
        <p>{`[bgcolor=#eee] SidebarMenuButton [/bgcolor] 不注册独立部件，只使用 class 契约：`}</p>
        <ul>
          <li><Text>{`给 [bgcolor=#eee] button [/bgcolor]、[bgcolor=#eee] a [/bgcolor] 或 [bgcolor=#eee] RouterLink [/bgcolor] 添加 [bgcolor=#eee] sidebar-menu-button [/bgcolor]`}</Text></li>
          <li><Text>{`文字放在带 [bgcolor=#eee] sidebar-menu-label [/bgcolor] class 的 [bgcolor=#eee] text [/bgcolor] 子节点中`}</Text></li>
          <li><Text>{`当前项额外添加 [bgcolor=#eee] active [/bgcolor] class`}</Text></li>
          <li><Text>{`未来 [bgcolor=#eee] @lcui/react [/bgcolor] 的 [bgcolor=#eee] SidebarMenuButton [/bgcolor] 默认创建带该 class 的 button；指定 [bgcolor=#eee] render [/bgcolor] 时把 class 追加到渲染的子元素`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] SidebarContent [/bgcolor] 不会自动创建 ScrollArea。需要滚动时，请在 [bgcolor=#eee] SidebarContent [/bgcolor] 内组合 [bgcolor=#eee] ScrollArea [/bgcolor] 和 [bgcolor=#eee] ScrollAreaContent [/bgcolor]。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`当前版本暂不支持 floating/inset variant、键盘快捷键和 cookie 持久化。[bgcolor=#eee] SidebarInset [/bgcolor] 也未实现。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
