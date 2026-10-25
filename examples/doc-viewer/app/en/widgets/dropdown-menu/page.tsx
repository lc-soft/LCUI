import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import DropdownMenuDemoDemo from "../../../examples/dropdown-menu-demo/index";
import DropdownMenuBasicDemoDemo from "../../../examples/dropdown-menu-basic-demo/index";
import DropdownMenuSubmenuDemo from "../../../examples/dropdown-menu-submenu/index";
import DropdownMenuShortcutsDemo from "../../../examples/dropdown-menu-shortcuts/index";
import DropdownMenuIconsDemo from "../../../examples/dropdown-menu-icons/index";
import DropdownMenuCheckboxesDemo from "../../../examples/dropdown-menu-checkboxes/index";
import DropdownMenuCheckboxesIconsDemo from "../../../examples/dropdown-menu-checkboxes-icons/index";
import DropdownMenuRadioGroupDemo from "../../../examples/dropdown-menu-radio-group/index";
import DropdownMenuRadioIconsDemo from "../../../examples/dropdown-menu-radio-icons/index";
import DropdownMenuDestructiveDemo from "../../../examples/dropdown-menu-destructive/index";
import DropdownMenuAvatarDemo from "../../../examples/dropdown-menu-avatar/index";
import DropdownMenuComplexDemo from "../../../examples/dropdown-menu-complex/index";
import FieldTable from "../../../components/field-table";
import { dropdownMenuFields } from "../../../widget-fields/dropdown-menu";
import { dropdownMenuTriggerFields } from "../../../widget-fields/dropdown-menu-trigger";
import { dropdownMenuContentFields } from "../../../widget-fields/dropdown-menu-content";
import { dropdownMenuGroupFields } from "../../../widget-fields/dropdown-menu-group";
import { dropdownMenuLabelFields } from "../../../widget-fields/dropdown-menu-label";
import { dropdownMenuItemFields } from "../../../widget-fields/dropdown-menu-item";
import { dropdownMenuCheckboxItemFields } from "../../../widget-fields/dropdown-menu-checkbox-item";
import { dropdownMenuRadioGroupFields } from "../../../widget-fields/dropdown-menu-radio-group";
import { dropdownMenuRadioItemFields } from "../../../widget-fields/dropdown-menu-radio-item";
import { dropdownMenuSeparatorFields } from "../../../widget-fields/dropdown-menu-separator";
import { dropdownMenuShortcutFields } from "../../../widget-fields/dropdown-menu-shortcut";
import { dropdownMenuSubFields } from "../../../widget-fields/dropdown-menu-sub";
import { dropdownMenuSubTriggerFields } from "../../../widget-fields/dropdown-menu-sub-trigger";
import { dropdownMenuSubContentFields } from "../../../widget-fields/dropdown-menu-sub-content";

export default function DropdownMenuDocPage() {
  return (
    <Widget className="doc-page">
      <h1>DropdownMenu</h1>
      <p className="doc-page-desc">{`A dropdown menu triggers a single command from a set of actions, with support for groups, submenus, checkbox items, radio items, shortcut hints, and destructive actions.`}</p>
      <DropdownMenuDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: User menus, inline action menus, and command lists that need groups or submenus`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Settings that show checked or selected state inside the menu`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Overlays that need large text input or complex forms (use [bgcolor=#eee] dialog [/bgcolor] or [bgcolor=#eee] popover [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Cases that only need a regular button (use [bgcolor=#eee] button [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/dropdown-menu/0">
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
                  <Text className="code-line">{`  [color=#8250df]DropdownMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSeparator[/color],`}</Text>
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
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/dropdown-menu/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]DropdownMenu[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color]>[/color]Open[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]DropdownMenuContent[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]My Account[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]Profile[color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]Settings[color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]Log out[color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`</[color=#8250df]DropdownMenu[/color]>`}</Text>
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
        <p>{`Use the following composition to build a [bgcolor=#eee] DropdownMenu [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/dropdown-menu/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`DropdownMenu`}</Text>
                <Text className="code-line">{`├── DropdownMenuTrigger`}</Text>
                <Text className="code-line">{`└── DropdownMenuContent (Portal)`}</Text>
                <Text className="code-line">{`    ├── DropdownMenuLabel`}</Text>
                <Text className="code-line">{`    ├── DropdownMenuGroup`}</Text>
                <Text className="code-line">{`    │   ├── DropdownMenuItem`}</Text>
                <Text className="code-line">{`    │   └── DropdownMenuCheckboxItem`}</Text>
                <Text className="code-line">{`    ├── DropdownMenuSeparator`}</Text>
                <Text className="code-line">{`    ├── DropdownMenuGroup`}</Text>
                <Text className="code-line">{`    │   └── DropdownMenuRadioGroup`}</Text>
                <Text className="code-line">{`    │       ├── DropdownMenuRadioItem`}</Text>
                <Text className="code-line">{`    │       └── DropdownMenuRadioItem`}</Text>
                <Text className="code-line">{`    └── DropdownMenuSub`}</Text>
                <Text className="code-line">{`        ├── DropdownMenuSubTrigger`}</Text>
                <Text className="code-line">{`        └── DropdownMenuSubContent (Portal)`}</Text>
                <Text className="code-line">{`            └── DropdownMenuItem`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Basic</h3>
        <p>{`A basic menu with a group label, a separator, and a disabled item at the end.`}</p>
        <DropdownMenuBasicDemoDemo />
        <h3>Submenu</h3>
        <p>{`Place [bgcolor=#eee] DropdownMenuSubTrigger [/bgcolor] and [bgcolor=#eee] DropdownMenuSubContent [/bgcolor] inside [bgcolor=#eee] DropdownMenuSub [/bgcolor]. Hovering or clicking the trigger opens the submenu. Submenu content is also rendered through a Portal.`}</p>
        <DropdownMenuSubmenuDemo />
        <h3>Shortcuts</h3>
        <p>{`[bgcolor=#eee] DropdownMenuShortcut [/bgcolor] only displays a shortcut hint; it does not bind a real shortcut. While the menu is open, typing a letter triggers typeahead: for example, pressing [bgcolor=#eee] s [/bgcolor] moves to the menu item that starts with S. It only changes the highlight and never runs the command. Pressing the same letter repeatedly cycles through items with the same initial.`}</p>
        <DropdownMenuShortcutsDemo />
        <h3>Icons</h3>
        <p>{`Place an icon before the item label to make the action easier to recognize.`}</p>
        <DropdownMenuIconsDemo />
        <h3>Checkboxes</h3>
        <p>{`[bgcolor=#eee] DropdownMenuCheckboxItem [/bgcolor] toggles its [bgcolor=#eee] checked [/bgcolor] status when activated and keeps the menu open so several items can be toggled in a row. Set the initial state with the [bgcolor=#eee] checked [/bgcolor] attribute.`}</p>
        <DropdownMenuCheckboxesDemo />
        <h3>Checkboxes with icons</h3>
        <p>{`Add icons before the checkbox labels to make each option easier to recognize. The check indicator is still inserted automatically at the right side by the widget.`}</p>
        <DropdownMenuCheckboxesIconsDemo />
        <h3>Radio group</h3>
        <p>{`When the [bgcolor=#eee] value [/bgcolor] of [bgcolor=#eee] DropdownMenuRadioGroup [/bgcolor] matches the [bgcolor=#eee] value [/bgcolor] of a [bgcolor=#eee] DropdownMenuRadioItem [/bgcolor], that item enters the selected state. Switching options keeps the menu open.`}</p>
        <DropdownMenuRadioGroupDemo />
        <h3>Radio with icons</h3>
        <p>{`Radio items can also show icons before their labels, which is useful for choices such as payment methods.`}</p>
        <DropdownMenuRadioIconsDemo />
        <h3>Disabled and destructive</h3>
        <p>{`Setting [bgcolor=#eee] disabled [/bgcolor] on an item prevents pointer and keyboard activation. Setting [bgcolor=#eee] variant="destructive" [/bgcolor] renders the item in the danger color.`}</p>
        <DropdownMenuDestructiveDemo />
        <h3>Avatar menu</h3>
        <p>{`Use a circular avatar button as the trigger to build an account switcher menu.`}</p>
        <DropdownMenuAvatarDemo />
        <h3>Complex</h3>
        <p>{`Combine icons, groups, separators, checkbox items, submenus, and shortcut hints to build a complete account menu.`}</p>
        <DropdownMenuComplexDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>DropdownMenu</h3>
        <p>{`The menu state container that manages open state, focus, and keyboard navigation.`}</p>
        <FieldTable fields={dropdownMenuFields} locale="en" />
        <h3>DropdownMenuTrigger</h3>
        <p>{`The trigger area that toggles the menu open state when clicked.`}</p>
        <FieldTable fields={dropdownMenuTriggerFields} locale="en" />
        <h3>DropdownMenuContent</h3>
        <p>{`The menu panel rendered through a Portal. It handles positioning, outside-click closing, and item layout.`}</p>
        <FieldTable fields={dropdownMenuContentFields} locale="en" />
        <h3>DropdownMenuGroup</h3>
        <p>{`A container that groups related menu items.`}</p>
        <FieldTable fields={dropdownMenuGroupFields} locale="en" />
        <h3>DropdownMenuLabel</h3>
        <p>{`A menu group heading that is skipped by keyboard navigation.`}</p>
        <FieldTable fields={dropdownMenuLabelFields} locale="en" />
        <h3>DropdownMenuItem</h3>
        <p>{`A regular command item that closes the menu and emits [bgcolor=#eee] select [/bgcolor] when activated.`}</p>
        <FieldTable fields={dropdownMenuItemFields} locale="en" />
        <h3>DropdownMenuCheckboxItem</h3>
        <p>{`A menu item with a checked state. Activating it toggles the state and emits [bgcolor=#eee] checked-change [/bgcolor].`}</p>
        <FieldTable fields={dropdownMenuCheckboxItemFields} locale="en" />
        <h3>DropdownMenuRadioGroup</h3>
        <p>{`A container that manages a mutually exclusive radio value. The checked state of its items is synchronized from [bgcolor=#eee] value [/bgcolor].`}</p>
        <FieldTable fields={dropdownMenuRadioGroupFields} locale="en" />
        <h3>DropdownMenuRadioItem</h3>
        <p>{`A radio menu item. Activating it updates the value of its radio group and emits [bgcolor=#eee] value-change [/bgcolor].`}</p>
        <FieldTable fields={dropdownMenuRadioItemFields} locale="en" />
        <h3>DropdownMenuSeparator</h3>
        <p>{`A thin divider between menu groups.`}</p>
        <FieldTable fields={dropdownMenuSeparatorFields} locale="en" />
        <h3>DropdownMenuShortcut</h3>
        <p>{`Shortcut hint text aligned to the right side of a menu item.`}</p>
        <FieldTable fields={dropdownMenuShortcutFields} locale="en" />
        <h3>DropdownMenuSub</h3>
        <p>{`The submenu state container.`}</p>
        <FieldTable fields={dropdownMenuSubFields} locale="en" />
        <h3>DropdownMenuSubTrigger</h3>
        <p>{`The item that opens the submenu. Hovering or clicking it opens the submenu.`}</p>
        <FieldTable fields={dropdownMenuSubTriggerFields} locale="en" />
        <h3>DropdownMenuSubContent</h3>
        <p>{`The submenu panel rendered through a Portal. It opens to the right of the trigger by default.`}</p>
        <FieldTable fields={dropdownMenuSubContentFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]Menu content is managed by Portal.[/b] Put [bgcolor=#eee] DropdownMenuContent [/bgcolor] inside [bgcolor=#eee] DropdownMenu [/bgcolor]; do not mount it to [bgcolor=#eee] ui_root [/bgcolor] manually.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]Keyboard navigation is built in.[/b] Once the menu is open, use [bgcolor=#eee] ArrowUp [/bgcolor] / [bgcolor=#eee] ArrowDown [/bgcolor] to cycle the highlight, [bgcolor=#eee] Home [/bgcolor] / [bgcolor=#eee] End [/bgcolor] to jump to the first or last item, [bgcolor=#eee] Enter [/bgcolor] / [bgcolor=#eee] Space [/bgcolor] to activate, [bgcolor=#eee] ArrowRight [/bgcolor] to enter a submenu, [bgcolor=#eee] ArrowLeft [/bgcolor] to return to the parent, and [bgcolor=#eee] Escape [/bgcolor] to close the submenu and then the root menu.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Automatic collision flipping is not supported yet.[/b] The menu does not flip or clamp itself against window edges. Set [bgcolor=#eee] side [/bgcolor] / [bgcolor=#eee] align [/bgcolor] manually when needed.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]RTL is not implemented yet.[/b] The current layout and arrow-key behavior assume LTR.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
