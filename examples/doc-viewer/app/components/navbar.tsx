import { Button, Text } from "@lcui/react";
import "./navbar.css";
import packageJson from "../../../../package.json";

export default function Navbar() {
  return (
    <div className="navbar">
      <div className="navbar-left">
        <Button
          variant="ghost"
          data-nav="home"
          $ref="nav_home"
          onClick="navbar_on_nav_click"
        >
          Home
        </Button>
        <Button
          variant="ghost"
          data-nav="docs"
          $ref="nav_docs"
          onClick="navbar_on_nav_click"
        >
          Docs
        </Button>
        <Button
          variant="ghost"
          data-nav="widgets"
          $ref="nav_widgets"
          onClick="navbar_on_nav_click"
        >
          Widgets
        </Button>
      </div>
      <div className="navbar-center">
        <Text $ref="path" className="navbar-path">
          /
        </Text>
      </div>
      <div className="navbar-right">
        <dropdown-menu $ref="locale_menu">
          <dropdown-menu-trigger>
            <Button variant="ghost" size="icon" $ref="locale_btn">
              <icon name="local-language" />
            </Button>
          </dropdown-menu-trigger>
          <dropdown-menu-content align="end">
            <dropdown-menu-radio-group $ref="lang_group" value="zh-CN">
              <dropdown-menu-radio-item value="en">
                <Text>English</Text>
              </dropdown-menu-radio-item>
              <dropdown-menu-radio-item value="zh-CN">
                <Text>中文</Text>
              </dropdown-menu-radio-item>
            </dropdown-menu-radio-group>
          </dropdown-menu-content>
        </dropdown-menu>
        <Button
          variant="ghost"
          size="icon"
          $ref="theme_btn"
          onClick="navbar_on_theme_click"
        >
          <icon name="dark-theme" />
        </Button>
        <Text className="navbar-version">{packageJson.version}</Text>
        <a href="https://github.com/lc-soft/LCUI" className="navbar-link">
          <Text>GitHub</Text>
        </a>
      </div>
    </div>
  );
}
