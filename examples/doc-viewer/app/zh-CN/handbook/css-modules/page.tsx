import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CssModulesDocPage() {
  return (
    <Widget className="doc-page">
      <h1>CSS Modules</h1>
      <p className="doc-page-desc">{`CSS Modules 为 TSX 组件创建局部作用域的 CSS 类，避免命名冲突并提高可维护性。`}</p>
      <Widget className="doc-section">
        <h2>用法</h2>
        <p>{`创建 [bgcolor=#eee] .module.css [/bgcolor] 后缀的文件：`}</p>
        <Widget className="code-block" data-language="css" data-source="zh-CN/handbook/css-modules/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.module.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800].card[/color] {`}</Text>
            <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#eee[/color];`}</Text>
            <Text className="code-line">{`  border-radius: [color=#0550ae]4px[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`在 [bgcolor=#eee] .tsx [/bgcolor] 文件中导入：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/css-modules/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] styles [color=#cf222e]from[/color] [color=#0a3069]"./MyComponent.module.css"[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`在 JSX 中用 [bgcolor=#eee] styles [/bgcolor] 对象替代字符串类名：`}</p>
        <Widget className="code-block" data-language="diff" data-source="zh-CN/handbook/css-modules/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">DIFF</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`- <Widget className="card" />`}</Text>
            <Text className="code-line">{`+ <Widget className={styles.card} />`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] 编译时会将 [bgcolor=#eee] .module.css [/bgcolor] 转换为 C 标识符绑定，类名会自动生成唯一的 C 常量名，避免全局冲突。`}</p>
      </Widget>
    </Widget>
  );
}
