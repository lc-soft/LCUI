import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import IconBrowser from "../../../components/icon-browser";

export default function IconLibraryDocPage() {
  return (
    <Widget className="doc-page">
      <h1>图标库</h1>
      <p className="doc-page-desc">{`[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 是专为 LCUI 适配的图标库，图标都来自 Microsoft 的 fluentui-system-icons 项目。`}</p>
      <Widget className="doc-section">
        <h2>安装</h2>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/devtools/icon-library/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/fluent-icons`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>选取图标</h2>
        <p>{`你可以在下面的图标库视图中搜索并选择图标。以放大图标为例，英文名通常是 Zoom In。`}</p>
        <IconBrowser locale="zh-CN" />
        <p>{`Fluent UI System Icons 提供方向相关的元数据，但图标数据没有分类字段，因此详情面板不显示分类标签。`}</p>
        <p>{`fluentui-system-icons 的图标有 16、20、24 等几种尺寸可选，命名方式是"图标名+尺寸+风格"。[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 的命名方式是"图标名+风格"，当风格为 Regular 时可以省略它。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/icon-library/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ZoomIn[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]ZoomIn[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`默认选择 regular 24 字形；如果没有 24，则选择最接近的可用 regular 尺寸，距离相同时选较小尺寸。[bgcolor=#eee] size [/bgcolor] 选择设计字形尺寸，并在显式设置时同步设置显示字号；[bgcolor=#eee] fontSize [/bgcolor] 只改变显示字号、不改变字形选择。未显式设置时图标会继承周围的文字字号，因此也可以通过 [bgcolor=#eee] className [/bgcolor] 或 [bgcolor=#eee] style [/bgcolor] 调整显示大小：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/icon-library/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]ZoomIn[/color] size={[color=#0550ae]32[/color]} />`}</Text>
          </Widget>
        </Widget>
        <p>{`C 代码可用 Fluent 快捷构造函数创建图标。XML 中省略 [bgcolor=#eee] library [/bgcolor] 时默认使用 Fluent；只有使用其它图标库时才需要指定该属性：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/devtools/icon-library/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#0a3069]"dismiss"[/color]);`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="xml" data-source="zh-CN/devtools/icon-library/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
