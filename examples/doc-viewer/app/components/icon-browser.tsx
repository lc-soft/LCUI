import {
  Button,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Text,
  TextInput,
  Widget,
} from "@lcui/react";
import { Dismiss, DismissCircle, Icon, iconMap } from "@lcui/fluent-icons";
import "./icon-browser.css";

const iconNames = Object.keys(iconMap).sort();

function getFilledPreviewSize(name: keyof typeof iconMap) {
  const sizes = Object.keys(iconMap[name].filled).map(Number);
  return sizes.reduce((best, size) => {
    const distance = Math.abs(size - 24);
    const bestDistance = Math.abs(best - 24);
    return distance < bestDistance || (distance === bestDistance && size < best)
      ? size
      : best;
  });
}

function getIconComponentName(name: keyof typeof iconMap) {
  const componentName = name
    .split("-")
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join("");
  return Object.keys(iconMap[name].regular).length > 0
    ? componentName
    : `${componentName}Filled`;
}

function hasRegularStyle(name: keyof typeof iconMap) {
  return Object.keys(iconMap[name].regular).length > 0;
}

interface IconBrowserProps {
  locale?: "en" | "zh-CN";
}

export default function IconBrowser(_props: IconBrowserProps) {
  return (
    <Widget className="icon-library-browser">
      <TextInput
        $ref="search"
        className="icon-library-search"
        placeholder="搜索图标名称..."
        onChange="icon_browser_on_search_change"
      />
      <ScrollArea className="icon-library-scroll-area">
        <ScrollAreaContent>
          <Widget $ref="icon_list" className="icon-library-list">
            {iconNames.map((name) => (
              <Widget
                key={name}
                className="icon-library-item"
                name={name}
                data-filled-only={hasRegularStyle(name) ? "false" : "true"}
                data-component-name={getIconComponentName(name)}
                data-preview-size={
                  hasRegularStyle(name) ? 24 : getFilledPreviewSize(name)
                }
                onClick="icon_browser_on_select"
              >
                <Icon
                  name={name}
                  filled={!hasRegularStyle(name)}
                  size={hasRegularStyle(name) ? 24 : getFilledPreviewSize(name)}
                  fontSize={24}
                />
              </Widget>
            ))}
          </Widget>
        </ScrollAreaContent>
        <Scrollbar />
      </ScrollArea>
      <Widget $ref="detail" className="icon-library-detail">
        <Widget className="icon-library-preview">
          <Icon
            $ref="detail_icon"
            name="checkmark"
            className="icon-library-detail-icon"
          />
        </Widget>
        <Widget className="icon-library-detail-content">
          <Widget className="icon-library-detail-header">
            <Text $ref="detail_name" className="icon-library-detail-name" />
            <Button
              className="icon-library-close"
              onClick="icon_browser_on_close"
            >
              <Dismiss fontSize={16} />
            </Button>
          </Widget>
          <Widget className="icon-library-actions">
            <Button
              $ref="copy_c"
              className="icon-library-copy-button"
              data-copy-format="c"
              onClick="icon_browser_on_copy"
            >
              复制 C 代码
            </Button>
            <Button
              $ref="copy_jsx"
              className="icon-library-copy-button"
              data-copy-format="jsx"
              onClick="icon_browser_on_copy"
            >
              复制 JSX
            </Button>
            <Button
              $ref="copy_xml"
              className="icon-library-copy-button"
              data-copy-format="xml"
              onClick="icon_browser_on_copy"
            >
              复制 XML
            </Button>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
