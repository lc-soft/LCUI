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
      <p className="doc-page-desc">{`下拉菜单用于从一组操作中触发单个命令，支持分组、子菜单、复选项、单选项、快捷键提示和危险操作。`}</p>
      <DropdownMenuDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：用户菜单、行内操作菜单、需要分组或子菜单的命令列表`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要在菜单内展示勾选状态或单选状态的配置项`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要输入大量文本或复杂表单的浮层（使用 [bgcolor=#eee] dialog [/bgcolor]、[bgcolor=#eee] popover [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：只需要一个普通按钮（使用 [bgcolor=#eee] button [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/dropdown-menu/0">
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
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/dropdown-menu/1">
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
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] DropdownMenu [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/dropdown-menu/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`DropdownMenu`}</Text>
                <Text className="code-line">{`├── DropdownMenuTrigger`}</Text>
                <Text className="code-line">{`└── DropdownMenuContent（Portal）`}</Text>
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
                <Text className="code-line">{`        └── DropdownMenuSubContent（Portal）`}</Text>
                <Text className="code-line">{`            └── DropdownMenuItem`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>基础</h3>
        <p>{`带分组标题和分隔线的基础菜单，末尾包含一个禁用项。`}</p>
        <DropdownMenuBasicDemoDemo />
        <h3>子菜单</h3>
        <p>{`[bgcolor=#eee] DropdownMenuSub [/bgcolor] 内放置 [bgcolor=#eee] DropdownMenuSubTrigger [/bgcolor] 和 [bgcolor=#eee] DropdownMenuSubContent [/bgcolor]，悬停或点击触发项展开子菜单。子菜单内容同样通过 Portal 渲染。`}</p>
        <DropdownMenuSubmenuDemo />
        <h3>快捷键</h3>
        <p>{`[bgcolor=#eee] DropdownMenuShortcut [/bgcolor] 只负责展示快捷键提示，不会绑定真实快捷键。菜单打开后直接键入字母会触发 typeahead：例如按 [bgcolor=#eee] s [/bgcolor] 会移动到以 S 开头的菜单项，只改变高亮，不会执行命令；快速重复同一个字母可以在同首字母的选项之间循环。`}</p>
        <DropdownMenuShortcutsDemo />
        <h3>图标</h3>
        <p>{`在菜单项文字前放置图标，用于快速识别操作类别。`}</p>
        <DropdownMenuIconsDemo />
        <h3>复选项</h3>
        <p>{`[bgcolor=#eee] DropdownMenuCheckboxItem [/bgcolor] 激活后切换 [bgcolor=#eee] checked [/bgcolor] status，并保持菜单打开，方便继续勾选其它项。初始勾选通过 [bgcolor=#eee] checked [/bgcolor] 属性设置。`}</p>
        <DropdownMenuCheckboxesDemo />
        <h3>复选项（带图标）</h3>
        <p>{`在复选项文字前放置图标，方便快速识别选项类别；勾选指示器仍由部件自动放在右侧。`}</p>
        <DropdownMenuCheckboxesIconsDemo />
        <h3>单选项</h3>
        <p>{`[bgcolor=#eee] DropdownMenuRadioGroup [/bgcolor] 的 [bgcolor=#eee] value [/bgcolor] 与子项 [bgcolor=#eee] value [/bgcolor] 匹配时，对应 [bgcolor=#eee] DropdownMenuRadioItem [/bgcolor] 进入选中状态。切换选项时菜单保持打开。`}</p>
        <DropdownMenuRadioGroupDemo />
        <h3>单选项（带图标）</h3>
        <p>{`单选项同样可以在文字前放置图标，用于支付方式等需要视觉区分的场景。`}</p>
        <DropdownMenuRadioIconsDemo />
        <h3>禁用与危险操作</h3>
        <p>{`为菜单项设置 [bgcolor=#eee] disabled [/bgcolor] 后不再响应点击和键盘激活；设置 [bgcolor=#eee] variant="destructive" [/bgcolor] 使用危险色显示。`}</p>
        <DropdownMenuDestructiveDemo />
        <h3>头像菜单</h3>
        <p>{`使用圆形头像按钮作为触发器，构建账户切换菜单。`}</p>
        <DropdownMenuAvatarDemo />
        <h3>复杂菜单</h3>
        <p>{`将图标、分组、分隔线、复选项、子菜单和快捷键组合起来，可以构建完整的账户菜单。`}</p>
        <DropdownMenuComplexDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>DropdownMenu</h3>
        <p>{`菜单状态容器，负责打开/关闭、焦点和键盘导航。`}</p>
        <FieldTable fields={dropdownMenuFields} locale="zh-CN" />
        <h3>DropdownMenuTrigger</h3>
        <p>{`点击切换菜单打开状态的触发区域。`}</p>
        <FieldTable fields={dropdownMenuTriggerFields} locale="zh-CN" />
        <h3>DropdownMenuContent</h3>
        <p>{`通过 Portal 渲染的菜单面板，负责定位、外部点击关闭和菜单项布局。`}</p>
        <FieldTable fields={dropdownMenuContentFields} locale="zh-CN" />
        <h3>DropdownMenuGroup</h3>
        <p>{`将一组相关菜单项组织在一起的容器。`}</p>
        <FieldTable fields={dropdownMenuGroupFields} locale="zh-CN" />
        <h3>DropdownMenuLabel</h3>
        <p>{`菜单分组标题，不参与键盘导航。`}</p>
        <FieldTable fields={dropdownMenuLabelFields} locale="zh-CN" />
        <h3>DropdownMenuItem</h3>
        <p>{`普通命令项，激活后关闭菜单并触发 [bgcolor=#eee] select [/bgcolor] 事件。`}</p>
        <FieldTable fields={dropdownMenuItemFields} locale="zh-CN" />
        <h3>DropdownMenuCheckboxItem</h3>
        <p>{`带勾选状态的菜单项，激活后切换状态并触发 [bgcolor=#eee] checked-change [/bgcolor] 事件。`}</p>
        <FieldTable fields={dropdownMenuCheckboxItemFields} locale="zh-CN" />
        <h3>DropdownMenuRadioGroup</h3>
        <p>{`管理互斥单选值的容器，子项选中状态由 [bgcolor=#eee] value [/bgcolor] 自动同步。`}</p>
        <FieldTable fields={dropdownMenuRadioGroupFields} locale="zh-CN" />
        <h3>DropdownMenuRadioItem</h3>
        <p>{`单选菜单项，激活后更新父级单选组的值并触发 [bgcolor=#eee] value-change [/bgcolor] 事件。`}</p>
        <FieldTable fields={dropdownMenuRadioItemFields} locale="zh-CN" />
        <h3>DropdownMenuSeparator</h3>
        <p>{`分隔不同菜单分组的一条细线。`}</p>
        <FieldTable fields={dropdownMenuSeparatorFields} locale="zh-CN" />
        <h3>DropdownMenuShortcut</h3>
        <p>{`菜单项右侧的快捷键提示文本。`}</p>
        <FieldTable fields={dropdownMenuShortcutFields} locale="zh-CN" />
        <h3>DropdownMenuSub</h3>
        <p>{`子菜单状态容器。`}</p>
        <FieldTable fields={dropdownMenuSubFields} locale="zh-CN" />
        <h3>DropdownMenuSubTrigger</h3>
        <p>{`展开子菜单的触发项，悬停或点击均可打开。`}</p>
        <FieldTable fields={dropdownMenuSubTriggerFields} locale="zh-CN" />
        <h3>DropdownMenuSubContent</h3>
        <p>{`通过 Portal 渲染的子菜单面板，默认在触发项右侧展开。`}</p>
        <FieldTable fields={dropdownMenuSubContentFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]菜单内容由 Portal 管理。[/b] 业务代码只需把 [bgcolor=#eee] DropdownMenuContent [/bgcolor] 放进 [bgcolor=#eee] DropdownMenu [/bgcolor]，不要手动挂载到 [bgcolor=#eee] ui_root [/bgcolor]。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b]键盘导航已内置。[/b] 打开菜单后可用 [bgcolor=#eee] ArrowUp [/bgcolor] / [bgcolor=#eee] ArrowDown [/bgcolor] 循环高亮，[bgcolor=#eee] Home [/bgcolor] / [bgcolor=#eee] End [/bgcolor] 跳转首尾，[bgcolor=#eee] Enter [/bgcolor] / [bgcolor=#eee] Space [/bgcolor] 激活，[bgcolor=#eee] ArrowRight [/bgcolor] 进入子菜单，[bgcolor=#eee] ArrowLeft [/bgcolor] 返回父级，[bgcolor=#eee] Escape [/bgcolor] 依次关闭子菜单和根菜单。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]暂不支持自动碰撞翻转。[/b] 当前版本不会根据窗口边缘自动翻转菜单方向或裁剪可用高度；需要时请通过 [bgcolor=#eee] side [/bgcolor] / [bgcolor=#eee] align [/bgcolor] 手动指定。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]RTL 暂未实现。[/b] 当前菜单布局与方向键行为按 LTR 设计。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
