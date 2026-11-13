import { RouterLink, Text } from "@lcui/react";
import sidebarData from "../../../../../../docs/sidebars.json";

interface SidebarItemData {
  slug: string;
  label: { en: string; "zh-CN": string };
}

interface SidebarSection {
  slug: string;
  label: { en: string; "zh-CN": string };
  items: SidebarItemData[];
}

const sections = (sidebarData as { sidebar: SidebarSection[] }).sidebar;
const widgets = sections.find((section) => section.slug === "widgets");

const items = (widgets ? widgets.items : [])
  .slice()
  .sort((a, b) => a.label.en.localeCompare(b.label.en));

function chunk(source: SidebarItemData[], size: number): SidebarItemData[][] {
  const rows: SidebarItemData[][] = [];
  for (let i = 0; i < source.length; i += size) {
    rows.push(source.slice(i, i + size));
  }
  return rows;
}

export default function EnWidgetsPage() {
  return (
    <div className="doc-page widgets-index">
      <Text className="widgets-index-title">Widgets</Text>
      <Text className="widgets-index-desc">
        Here you can find all widgets available in the library.
      </Text>
      <Text className="widgets-index-heading">All Widgets</Text>
      {chunk(items, 3).map((row, index) => (
        <div className="widgets-index-row" key={index}>
          {row.map((item) => (
            <div className="widgets-index-cell" key={item.slug}>
              <RouterLink
                className="button button-link button-sm widgets-index-item"
                to={`/en/docs/widgets/${item.slug}`}
              >
                <Text>{item.label.en}</Text>
              </RouterLink>
            </div>
          ))}
        </div>
      ))}
    </div>
  );
}
