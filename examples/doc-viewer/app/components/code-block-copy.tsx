import { Widget } from "@lcui/react";
import { Checkmark, Copy } from "@lcui/fluent-icons";

export default function CodeBlockCopy() {
  return (
    <Widget
      className="code-block-copy"
      onClick="code_block_copy_on_click"
    >
      <Copy className="copy-icon text-base" />
      <Checkmark className="check-icon text-base" />
    </Widget>
  );
}
