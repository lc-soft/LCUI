import { PropsWithChildren, ScrollArea, ScrollAreaContent, Scrollbar } from "@lcui/react";
import Navbar from "./components/navbar";
import Sidebar from "./components/sidebar";
import "@lcui/fluent-icons/dist/style.css";
import "./global.css";

export default function AppLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-col w-full h-full">
      <Navbar />
      <div className="flex flex-1">
        <Sidebar />
        <ScrollArea className="flex-1">
          <ScrollAreaContent>{children}</ScrollAreaContent>
          <Scrollbar direction="vertical" />
        </ScrollArea>
      </div>
    </div>
  );
}
