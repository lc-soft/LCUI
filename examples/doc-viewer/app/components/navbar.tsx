import { Button, Text, Widget } from "@lcui/react";
import "./navbar.css";
import { repoStars } from "./repo-stars.generated";
import { accentNames } from "./accent-palette";

function formatStars(count: number): string {
  if (count < 1000) {
    return String(count);
  }
  return `${Math.floor(count / 1000)}k`;
}

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
          <Text className="lang en">Home</Text>
          <Text className="lang zh-CN">首页</Text>
        </Button>
        <Button
          variant="ghost"
          data-nav="docs"
          $ref="nav_docs"
          onClick="navbar_on_nav_click"
        >
          <Text className="lang en">Docs</Text>
          <Text className="lang zh-CN">文档</Text>
        </Button>
        <Button
          variant="ghost"
          data-nav="widgets"
          $ref="nav_widgets"
          onClick="navbar_on_nav_click"
        >
          <Text className="lang en">Widgets</Text>
          <Text className="lang zh-CN">部件</Text>
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
              <Text className="relative left-[3px] text-xs">文</Text>
              <Text className="relative bottom-[-5px] right-[3px] text-xs">
                A
              </Text>
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
        <Widget className="navbar-divider" />
        <Button
          variant="ghost"
          size="icon"
          $ref="theme_btn"
          onClick="navbar_on_theme_click"
        >
          <icon name="dark-theme" />
        </Button>
        <dropdown-menu $ref="accent_menu">
          <dropdown-menu-trigger>
            <Button variant="ghost" size="icon" $ref="accent_btn">
              <icon name="color" />
            </Button>
          </dropdown-menu-trigger>
          <dropdown-menu-content align="end">
            <Widget className="accent-palette">
              {accentNames.map((name) => (
                <Widget
                  key={name}
                  className={`accent-swatch accent-swatch-${name}`}
                  data-accent={name}
                  onClick="navbar_on_accent_click"
                >
                  <icon name="checkmark" className="accent-swatch-check" />
                </Widget>
              ))}
            </Widget>
          </dropdown-menu-content>
        </dropdown-menu>
        <Widget className="navbar-divider" />
        <a
          href="https://github.com/lc-soft/LCUI"
          className="navbar-link navbar-icon-link"
          title="GitHub"
        >
          <Widget className="navbar-icon navbar-icon-github" />
          {repoStars.github !== null && (
            <Text className="navbar-icon-count">
              {formatStars(repoStars.github)}
            </Text>
          )}
        </a>
        <Widget className="navbar-divider" />
        <a
          href="https://gitee.com/lc-soft/LCUI"
          className="navbar-link navbar-icon-link"
          title="Gitee"
        >
          <Widget className="navbar-icon navbar-icon-gitee" />
          {repoStars.gitee !== null && (
            <Text className="navbar-icon-count">
              {formatStars(repoStars.gitee)}
            </Text>
          )}
        </a>
      </div>
    </div>
  );
}
