import { Fragment, Text, Widget } from "@lcui/react";
import { ChevronDown, ChevronUp } from "@lcui/fluent-icons";
import "./field-table.css";

export interface FieldData {
  name: string;
  type: string;
  default: string;
  description: { en: string; "zh-CN": string };
}

export interface FieldTableProps {
  fields: FieldData[];
  locale?: string;
}

const labels: Record<string, Record<string, string>> = {
  name: { en: "Prop", "zh-CN": "\u5C5E\u6027\u540D" },
  type: { en: "Type", "zh-CN": "\u7C7B\u578B" },
  default: { en: "Default", "zh-CN": "\u9ED8\u8BA4\u503C" },
  description: { en: "Description", "zh-CN": "\u63CF\u8FF0" },
};

function t(key: string, locale: string): string {
  return labels[key]?.[locale] ?? labels[key]?.en ?? key;
}

function FieldTableProvider({ children }: { children?: any }) {
  return <Widget className="field-table-provider">{children}</Widget>;
}

function FieldTable({ fields, locale = "en" }: FieldTableProps) {
  return (
    <FieldTableProvider>
      <Widget className="field-table">
        <Widget className="field-table-row field-table-header">
          <Widget className="field-table-cell cell-name">
            <Text>{t("name", locale)}</Text>
          </Widget>
          <Widget className="field-table-cell cell-type">
            <Text>{t("type", locale)}</Text>
          </Widget>
          <Widget className="field-table-cell cell-default">
            <Text>{t("default", locale)}</Text>
          </Widget>
        </Widget>
        {fields.map((field) => (
          <Fragment key={field.name}>
            <Widget className="field-table-row" key={field.name}>
              <Widget className="field-table-cell cell-name">
                <Text>{field.name}</Text>
              </Widget>
              <Widget className="field-table-cell cell-type">
                <Text>{field.type}</Text>
              </Widget>
              <Widget className="field-table-cell cell-default">
                <Text>{field.default}</Text>
              </Widget>
              <ChevronDown className="field-table-expand-icon" />
              <ChevronUp className="field-table-collapse-icon" />
            </Widget>
            <Widget className="field-table-details">
              <Widget className="field-table-detail-row">
                <Text className="field-table-detail-name">
                  {t("name", locale)}
                </Text>
                <Text className="field-table-detail-value">{field.name}</Text>
              </Widget>
              <Widget className="field-table-detail-row">
                <Text className="field-table-detail-name">
                  {t("type", locale)}
                </Text>
                <Text className="field-table-detail-value">{field.type}</Text>
              </Widget>
              <Widget className="field-table-detail-row">
                <Text className="field-table-detail-name">
                  {t("default", locale)}
                </Text>
                <Text className="field-table-detail-value">
                  {field.default}
                </Text>
              </Widget>
              <Widget className="field-table-detail-row">
                <Text className="field-table-detail-name">
                  {t("description", locale)}
                </Text>
                <Text className="field-table-detail-value">
                  {locale === "zh-CN"
                    ? field.description["zh-CN"]
                    : field.description.en}
                </Text>
              </Widget>
            </Widget>
          </Fragment>
        ))}
      </Widget>
    </FieldTableProvider>
  );
}

FieldTable.shouldPreRender = true;
export default FieldTable;
