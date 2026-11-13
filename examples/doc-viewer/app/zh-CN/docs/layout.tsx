import {
  PropsWithChildren,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
} from "@lcui/react";
import DocsSidebar from "../../components/sidebar";

export default function ZhCnDocsLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-1 min-h-0 h-full pl-2">
      <DocsSidebar locale="zh-CN" />
      <ScrollArea className="flex-1">
        <ScrollAreaContent className="docs-scroll-content">
          {children}
        </ScrollAreaContent>
        <Scrollbar direction="vertical" />
      </ScrollArea>
    </div>
  );
}
