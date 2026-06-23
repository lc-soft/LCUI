import { Button, Text } from "@lcui/react";
import "./navbar.css";
import packageJson from "../../../../package.json";

export default function Navbar() {
  return (
    <div className="navbar">
      <Text className="navbar-logo">LCUI</Text>
      <Text $ref="path" className="navbar-path">
        /
      </Text>
      <div className="navbar-links">
        <div className="flex gap-2">
          <Button
            className="navbar-btn"
            data-locale="en"
            $ref="locale_en"
            onClick="navbar_on_locale_click"
          >
            EN
          </Button>
          <Button
            className="navbar-btn"
            data-locale="zh-CN"
            $ref="locale_zh"
            onClick="navbar_on_locale_click"
          >
            中文
          </Button>
        </div>
        <Text className="navbar-version">{packageJson.version}</Text>
        <a href="https://github.com/lc-soft/LCUI" className="navbar-btn">
          <Text>GitHub</Text>
        </a>
      </div>
    </div>
  );
}
