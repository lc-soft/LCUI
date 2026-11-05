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
      <p className="doc-page-desc">{`A composable widget for building collapsible application navigation sidebars.`}</p>
      <SidebarBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Primary navigation sidebars for dashboards, documentation sites, and desktop apps`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Layouts that switch between expanded, icon-collapsed, and non-collapsible modes`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Cases that only need a regular button group or a short list (use [bgcolor=#eee] button [/bgcolor] or [bgcolor=#eee] menu [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/sidebar/0">
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
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/sidebar/1">
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
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Sidebar [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/sidebar/2">
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
                <Text className="code-line">{`│   │                   ├── SidebarMenuButton (class)`}</Text>
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
        <h2>Examples</h2>
        <h3>Collapsible groups</h3>
        <p>{`Place a [bgcolor=#eee] SidebarGroup [/bgcolor] inside a [bgcolor=#eee] Collapsible [/bgcolor] and add [bgcolor=#eee] data-collapsible-trigger [/bgcolor] to its [bgcolor=#eee] SidebarGroupLabel [/bgcolor] to toggle the group content by clicking the group heading.`}</p>
        <SidebarGroupCollapsibleDemo />
        <h3>Icon collapse</h3>
        <p>{`With [bgcolor=#eee] collapsible="icon" [/bgcolor], collapsing keeps the icons and hides the menu labels; the trigger can expand the sidebar again.`}</p>
        <SidebarCollapsedDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>SidebarProvider</h3>
        <p>{`The sidebar state container, which stores the expanded/collapsed state and looks up the nearest provider.`}</p>
        <FieldTable fields={sidebarProviderFields} locale="en" />
        <h3>Sidebar</h3>
        <p>{`The main sidebar panel, which handles width, border, and collapse mode.`}</p>
        <FieldTable fields={sidebarFields} locale="en" />
        <h3>SidebarTrigger</h3>
        <p>{`A button that toggles the nearest provider; it uses the [bgcolor=#eee] panel-left [/bgcolor] icon by default.`}</p>
        <FieldTable fields={sidebarTriggerFields} locale="en" />
        <h3>SidebarRail</h3>
        <p>{`A narrow trigger area along the sidebar edge that also toggles the sidebar when clicked.`}</p>
        <FieldTable fields={sidebarRailFields} locale="en" />
        <h3>SidebarHeader</h3>
        <p>{`The sidebar header container, used for the brand, workspace switcher, or title.`}</p>
        <FieldTable fields={sidebarHeaderFields} locale="en" />
        <h3>SidebarFooter</h3>
        <p>{`The sidebar footer container, usually for settings or account entries.`}</p>
        <FieldTable fields={sidebarFooterFields} locale="en" />
        <h3>SidebarContent</h3>
        <p>{`The main sidebar area; it only handles flex layout, so compose [bgcolor=#eee] ScrollArea [/bgcolor] inside it when scrolling is needed.`}</p>
        <FieldTable fields={sidebarContentFields} locale="en" />
        <h3>SidebarGroup</h3>
        <p>{`A sidebar group container that can hold a label, action, and content.`}</p>
        <FieldTable fields={sidebarGroupFields} locale="en" />
        <h3>SidebarGroupLabel</h3>
        <p>{`The group heading; after you add [bgcolor=#eee] data-collapsible-trigger [/bgcolor], it toggles the outer Collapsible.`}</p>
        <FieldTable fields={sidebarGroupLabelFields} locale="en" />
        <h3>SidebarGroupAction</h3>
        <p>{`The action button on the right side of the group heading.`}</p>
        <FieldTable fields={sidebarGroupActionFields} locale="en" />
        <h3>SidebarGroupContent</h3>
        <p>{`The group content container, usually holding a SidebarMenu.`}</p>
        <FieldTable fields={sidebarGroupContentFields} locale="en" />
        <h3>SidebarMenu</h3>
        <p>{`The sidebar menu list.`}</p>
        <FieldTable fields={sidebarMenuFields} locale="en" />
        <h3>SidebarMenuItem</h3>
        <p>{`A menu item container that can hold a menu button, action, badge, and submenu.`}</p>
        <FieldTable fields={sidebarMenuItemFields} locale="en" />
        <h3>SidebarMenuAction</h3>
        <p>{`A lightweight action button on the right side of a menu item.`}</p>
        <FieldTable fields={sidebarMenuActionFields} locale="en" />
        <h3>SidebarMenuSub</h3>
        <p>{`The submenu container inside a menu item.`}</p>
        <FieldTable fields={sidebarMenuSubFields} locale="en" />
        <h3>SidebarMenuSubItem</h3>
        <p>{`A submenu item container.`}</p>
        <FieldTable fields={sidebarMenuSubItemFields} locale="en" />
        <h3>SidebarMenuBadge</h3>
        <p>{`The badge on the right side of a menu item.`}</p>
        <FieldTable fields={sidebarMenuBadgeFields} locale="en" />
        <h3>SidebarMenuButton</h3>
        <p>{`[bgcolor=#eee] SidebarMenuButton [/bgcolor] does not register its own widget; it uses a class contract:`}</p>
        <ul>
          <li><Text>{`Add [bgcolor=#eee] sidebar-menu-button [/bgcolor] to a [bgcolor=#eee] button [/bgcolor], [bgcolor=#eee] a [/bgcolor], or [bgcolor=#eee] RouterLink [/bgcolor]`}</Text></li>
          <li><Text>{`Put the label in a [bgcolor=#eee] text [/bgcolor] child with the [bgcolor=#eee] sidebar-menu-label [/bgcolor] class`}</Text></li>
          <li><Text>{`Add the [bgcolor=#eee] active [/bgcolor] class to the current item`}</Text></li>
          <li><Text>{`In the future, [bgcolor=#eee] SidebarMenuButton [/bgcolor] in [bgcolor=#eee] @lcui/react [/bgcolor] will create a button with this class by default; when [bgcolor=#eee] render [/bgcolor] is specified, append the class to the rendered child`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] SidebarContent [/bgcolor] does not create a ScrollArea automatically. When scrolling is needed, compose [bgcolor=#eee] ScrollArea [/bgcolor] and [bgcolor=#eee] ScrollAreaContent [/bgcolor] inside [bgcolor=#eee] SidebarContent [/bgcolor].`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`Floating/inset variants, keyboard shortcuts, and cookie persistence are not supported in the current version. [bgcolor=#eee] SidebarInset [/bgcolor] is not implemented either.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
