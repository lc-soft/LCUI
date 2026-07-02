import { RouterLink, ScrollArea, ScrollAreaContent, Scrollbar, Text } from "@lcui/react";
import  "./sidebar.css";
import sidebarData from "../../../../docs/sidebars.json";

interface SidebarItemData {
  slug: string;
  label: { en: string; "zh-CN": string };
}

interface SidebarSection {
  slug: string;
  label: { en: string; "zh-CN": string };
  items: SidebarItemData[];
}

export default function Sidebar() {
  return (
    <ScrollArea className="sidebar-container">
      {(["zh-CN", "en"] as const).map((locale) => (
        <ScrollAreaContent key={locale} className={`sidebar lang ${locale}`}>
          {(sidebarData as { sidebar: SidebarSection[] }).sidebar.map(
            (section: SidebarSection) => (
              <div key={section.slug} className="sidebar-section">
                <Text className="sidebar-heading">
                  {section.label[locale]}
                </Text>
                <div className="sidebar-list">
                  {section.items.map((item: SidebarItemData) => (
                    <RouterLink
                      key={item.slug}
                      to={`/${locale}/${section.slug}/${item.slug}`}
                      className="sidebar-item"
                      activeClass="active"
                    >
                      {item.label[locale]}
                    </RouterLink>
                  ))}
                </div>
              </div>
            )
          )}
        </ScrollAreaContent>
      ))}
      <Scrollbar direction="vertical" />
    </ScrollArea>
  );
}
