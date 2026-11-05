import { PropsWithChildren, ScrollArea, ScrollAreaContent, Scrollbar } from "@lcui/react";
import DocsSidebar from "../../components/sidebar";

export default function ZhCnDocsLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-1 min-h-0 h-full">
      <DocsSidebar locale="zh-CN" />
      <ScrollArea className="flex-1">
        <ScrollAreaContent>{children}</ScrollAreaContent>
        <Scrollbar direction="vertical" />
      </ScrollArea>
    </div>
  );
}
