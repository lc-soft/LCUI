import { PropsWithChildren } from "@lcui/react";
import Navbar from "./components/navbar";
import "@lcui/fluent-icons/dist/icons.css";
import "@lcui/fluent-icons/dist/icons-filled.css";
import "./global.css";

export default function AppLayout({ children }: PropsWithChildren) {
  return (
    <div className="flex flex-col w-full h-full typeset">
      <Navbar />
      <div className="app-body flex-1 min-h-0 w-full">{children}</div>
    </div>
  );
}
