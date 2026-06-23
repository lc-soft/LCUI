import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AnchorBasicDemo from "../../../examples/anchor-basic/index";
import FieldTable from "../../../components/field-table";
import { anchorFields } from "../../../widget-fields/anchor";

export default function AnchorDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Anchor</h1>
      <p className="doc-page-desc">{`链接部件，用法与 HTML 的 [bgcolor=#eee] <a> [/bgcolor] 标签相同，用于打开外部 URL。`}</p>
      <AnchorBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b] — 在页面中放置外部链接（点击后在系统浏览器中打开）。`}</Text></li>
          <li><Text>{`[b]不适用[/b] — 需要执行动作的按钮（用 [bgcolor=#eee] button [/bgcolor]）。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={anchorFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
