import { PropsWithChildren, ScrollArea, ScrollAreaContent, Scrollbar } from "@lcui/react";
import Navbar from "./components/navbar";
import DocsSidebar from "./components/sidebar";
import "@lcui/fluent-icons/dist/icons.css";
import "@lcui/fluent-icons/dist/icons-filled.css";
import "./global.css";

export default function AppLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-col w-full h-full typeset">
      <Navbar />
      <div className="flex flex-1">
        <sidebar className="bg-transparent h-full">
          <DocsSidebar />
        </sidebar>
        <ScrollArea className="flex-1">
          <ScrollAreaContent>{children}</ScrollAreaContent>
          <Scrollbar direction="vertical" />
        </ScrollArea>
      </div>
    </div>
  );
}
