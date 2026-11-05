import {
  FunctionWidget,
  RouterLink,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Text,
} from "@lcui/react";
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

/**
 * Widget items in the Chinese sidebar show the Chinese name first, followed by
 * the English name in a muted color. Other sections and locales keep the label
 * from sidebars.json unchanged.
 */
function getItemLabel(
  section: SidebarSection,
  item: SidebarItemData,
  locale: "zh-CN" | "en",
): string {
  if (locale !== "zh-CN" || section.slug !== "widgets") {
    return item.label[locale];
  }
  const en = item.label.en;
  const zhLabel = item.label["zh-CN"];
  const zh = zhLabel.startsWith(en) ? zhLabel.slice(en.length).trim() : zhLabel;
  return (
    <>
      <Text className="mr-2">{zh}</Text>
      <Text className="text-[#999999]">{en}</Text>
    </>
  );
}

const DocsSidebar: FunctionWidget = ({
  locale,
}: {
  locale: "zh-CN" | "en";
}) => {
  return (
    <sidebar className="bg-transparent h-full">
      <ScrollArea className="flex-1 min-h-0 w-full">
        {/* ScrollArea only tracks its first <ScrollAreaContent>, so both locales
         * live inside one content and CSS toggles which language block is shown. */}
        <ScrollAreaContent>
          {(sidebarData as { sidebar: SidebarSection[] }).sidebar.map(
            (section: SidebarSection) => (
              <sidebar-group key={section.slug}>
                <sidebar-group-label>
                  {section.label[locale]}
                </sidebar-group-label>
                <sidebar-group-content>
                  <sidebar-menu>
                    {section.items.map((item: SidebarItemData) => (
                      <sidebar-menu-item key={item.slug}>
                        <RouterLink
                          to={`/${locale}/docs/${section.slug}/${item.slug}`}
                          className="sidebar-menu-button w-fit gap-0 font-medium leading-[30px]"
                          activeClass="active"
                        >
                          {getItemLabel(section, item, locale)}
                        </RouterLink>
                      </sidebar-menu-item>
                    ))}
                  </sidebar-menu>
                </sidebar-group-content>
              </sidebar-group>
            ),
          )}
        </ScrollAreaContent>
        <Scrollbar direction="vertical" />
      </ScrollArea>
    </sidebar>
  );
};

DocsSidebar.shouldPreRender = true;

export default DocsSidebar;
