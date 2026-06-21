/**
 * page-generator.ts — Generate page.tsx, page.c, and demo sub-components
 * for a single doc page.
 */

import { writeFileSync, mkdirSync, existsSync } from "fs";
import { join, relative, posix, sep } from "path";
import hljs from "highlight.js";
import type { ParsedMdx, ParsedSection } from "./mdx-parser.ts";
import { markdownInlineToLcuiStyleTags } from "./mdx-parser.ts";
import type { CompiledExample } from "./example-compiler.ts";
import { highlightedHtmlToLines } from "./style-tag-renderer.ts";

// ---------------------------------------------------------------------------
// Path helpers
// ---------------------------------------------------------------------------

/** Convert any platform path to POSIX-style for use in import specifiers. */
function toPosix(p: string): string {
  return p.split(sep).join(posix.sep);
}

/** Compute a POSIX-style relative import path (always prefixed with `./` or `../`). */
function relativeImport(fromDir: string, toFile: string): string {
  const rel = toPosix(relative(fromDir, toFile));
  return rel.startsWith(".") ? rel : `./${rel}`;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

function camelCase(str: string): string {
  return str.replace(/-([a-z])/g, (_, c: string) => c.toUpperCase());
}

function pascalCase(str: string): string {
  const cc = camelCase(str);
  return cc.charAt(0).toUpperCase() + cc.slice(1);
}

function cIdent(str: string): string {
  return str.replace(/-/g, "_");
}

function escapeTsx(str: string): string {
  return str
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/\{/g, "&#123;")
    .replace(/\}/g, "&#125;");
}

function escapeTsxTemplateContent(str: string): string {
  // For use inside {`...`} template literals in TSX
  return str.replace(/\\/g, "\\\\").replace(/`/g, "\\`").replace(/\$/g, "\\$");
}

// LCUI's text widget collapses runs of ASCII whitespace at the start of
// a line during layout, so source-code indentation gets lost in code
// blocks.  Replace the leading run of ASCII spaces and tabs (a tab
// counts as 4 spaces) with U+00A0 (NO-BREAK SPACE) per character so the
// indentation is preserved as opaque text.  Whitespace inside the line
// is left untouched.
function preserveLeadingIndent(line: string): string {
  let i = 0;
  let spaces = 0;
  while (i < line.length) {
    if (line[i] === " ") {
      spaces += 1;
    } else if (line[i] === "\t") {
      spaces += 4;
    } else {
      break;
    }
    i += 1;
  }
  if (i === 0) {
    return line;
  }
  return "\u00a0".repeat(spaces) + line.slice(i);
}

// ---------------------------------------------------------------------------
// Section content renderer
//
// Walks the raw markdown lines captured in ParsedSection.content and emits
// LCUI/JSX nodes:
//
//   ## heading       →  <h2>…</h2>
//   ### heading      →  <h3>…</h3>
//   ```lang … ```    →  <Widget className="code-block">{ <Text /> per line }
//   <WidgetExample/> →  <NameDemo />
//   plain paragraph  →  <p>…</p>
//
// Inline markdown (**bold**, `code`, etc.) is passed through
// markdownInlineToLcuiStyleTags so the Text widgets keep emphasis using
// LCUI style tags (LCUI cannot render mixed text+element trees inside a
// single widget).
// ---------------------------------------------------------------------------

const HLJS_LANG_MAP: Record<string, string> = {
  c: "c",
  cpp: "cpp",
  "c++": "cpp",
  js: "javascript",
  javascript: "javascript",
  ts: "typescript",
  typescript: "typescript",
  tsx: "typescript",
  jsx: "javascript",
  xml: "xml",
  html: "xml",
  css: "css",
  json: "json",
  bash: "bash",
  sh: "bash",
  shell: "bash",
};

function highlightCode(source: string, lang: string): string {
  const mapped = HLJS_LANG_MAP[lang.toLowerCase()] ?? lang;
  try {
    return hljs.highlight(source, { language: mapped }).value;
  } catch {
    return hljs.highlightAuto(source).value;
  }
}

const FRIENDLY_LANG_NAME: Record<string, string> = {
  c: "C",
  cpp: "C++",
  "c++": "C++",
  ts: "TypeScript",
  typescript: "TypeScript",
  tsx: "TypeScript",
  js: "JavaScript",
  javascript: "JavaScript",
  jsx: "JavaScript",
  xml: "XML",
  html: "HTML",
  css: "CSS",
  json: "JSON",
  bash: "Bash",
  sh: "Shell",
  shell: "Shell",
};

function friendlyLangName(lang: string): string {
  return FRIENDLY_LANG_NAME[lang.toLowerCase()] ?? lang.toUpperCase();
}

// ---------------------------------------------------------------------------
// Snippet registry
//
// Every code block emitted into a page or demo gets its raw UTF-8
// source registered here under a stable id of the form
// "<scope>/<index>".  The registry is flushed to
// app/components/code-snippets.{c,h} at the end of compile.  The C
// table is consumed by app/components/code-block-copy.c so the click
// handler can resolve `data-source` attributes to the original source
// text without inlining multi-line literals into every page.tsx.h.
// ---------------------------------------------------------------------------

export interface Snippet {
  id: string;
  source: string;
}

export class SnippetRegistry {
  private snippets: Snippet[] = [];
  private counts: Map<string, number> = new Map();

  add(scope: string, source: string): string {
    const n = this.counts.get(scope) ?? 0;
    this.counts.set(scope, n + 1);
    const id = `${scope}/${n}`;
    this.snippets.push({ id, source });
    return id;
  }

  all(): Snippet[] {
    return this.snippets;
  }
}

function renderInlineLine(line: string, indent: string): string {
  const styled = markdownInlineToLcuiStyleTags(line);
  const escaped = escapeTsxTemplateContent(styled);
  return `${indent}<p>{\`${escaped}\`}</p>`;
}

// ---------------------------------------------------------------------------
// Markdown table → flexbox markup.
//
// LCUI does not support display: table, so we lay out every table as a
// column of flex rows.  For each column we compute the max number of
// printable characters across header + body.  Columns whose max length
// is below the threshold get a fixed pixel width (cellWidthPx); the
// rightmost long column stretches via flex-grow so long descriptions do
// not get forced into narrow slivers.  A minimum width is applied to
// every cell so short content does not collapse.
// ---------------------------------------------------------------------------

const MDX_TABLE_FLEX_THRESHOLD = 28;
const MDX_TABLE_CHAR_PX = 8;
const MDX_TABLE_PADDING_PX = 24;
const MDX_TABLE_MIN_WIDTH_PX = 48;

interface MdTableRow {
  cells: string[];
}

function parseMarkdownTableCells(line: string): string[] {
  // strip leading and trailing "|"
  const inner = line.replace(/^\s*\|/, "").replace(/\|\s*$/, "");
  return inner.split("|").map((c) => c.trim());
}

function isMarkdownTableSeparator(row: MdTableRow): boolean {
  return row.cells.every((c) => /^:?-+:?$/.test(c.trim()));
}

function renderMarkdownTable(
  rows: MdTableRow[],
  indent: string,
): string {
  if (rows.length < 2) return "";
  const header = rows[0];
  const body = rows.slice(2); // drop header + separator
  const numCols = Math.max(header.cells.length, ...body.map((r) => r.cells.length));

  const maxLen = new Array(numCols).fill(0);
  for (const row of [header, ...body]) {
    for (let c = 0; c < numCols; c++) {
      const val = row.cells[c] ?? "";
      maxLen[c] = Math.max(maxLen[c], val.length);
    }
  }

  const wideIdx = numCols - 1;
  const lastColWide = maxLen[wideIdx] >= MDX_TABLE_FLEX_THRESHOLD;

  function cellAttrs(
    c: number,
  ): { cls: string; styleObj: string } {
    const baseCls = "mdx-table-cell";
    if (c === wideIdx && lastColWide) {
      return { cls: baseCls + " flex-grow", styleObj: `{ minWidth: "${MDX_TABLE_MIN_WIDTH_PX}px" }` };
    }
    const w = Math.max(
      MDX_TABLE_MIN_WIDTH_PX,
      maxLen[c] * MDX_TABLE_CHAR_PX + MDX_TABLE_PADDING_PX,
    );
    return { cls: baseCls, styleObj: `{ width: "${w}px" }` };
  }

  function renderRow(row: MdTableRow, isHead: boolean): string {
    const children = [];
    for (let c = 0; c < numCols; c++) {
      const raw = row.cells[c] ?? "";
      const styled = markdownInlineToLcuiStyleTags(raw);
      const escaped = escapeTsxTemplateContent(styled);
      const { cls, styleObj } = cellAttrs(c);
      const headCls = isHead ? `${cls} mdx-table-head-cell` : cls;
      children.push(
        `${indent}    <Widget className="${headCls}" style={${styleObj}}>\n` +
        `${indent}      <Text>{\`${escaped}\`}</Text>\n` +
        `${indent}    </Widget>`,
      );
    }
    const rowCls = isHead ? "mdx-table-row mdx-table-head" : "mdx-table-row";
    return (
      `${indent}  <Widget className="${rowCls}">\n` +
      children.join("\n") + "\n" +
      `${indent}  </Widget>`
    );
  }

  const renderedRows = [
    renderRow(header, true),
    ...body.map((r) => renderRow(r, false)),
  ];
  return (
    `${indent}<Widget className="mdx-table">\n` +
    renderedRows.join("\n") + "\n" +
    `${indent}</Widget>`
  );
}

/**
 * Parse docusaurus-style fenced-code metadata (e.g. title="src/main.c").
 * Returns { title } if present, otherwise an empty object.
 */
function parseCodeMeta(meta: string | undefined): { title?: string } {
  if (!meta) return {};
  const m = meta.match(/title="([^"]+)"/);
  return m ? { title: m[1] } : {};
}

function renderCodeBlock(
  lang: string,
  sourceLines: string[],
  indent: string,
  snippets: SnippetRegistry,
  snippetScope: string,
  overrideTitle?: string,
  noHeader?: boolean,
): string {
  const source = sourceLines.join("\n");
  const html = highlightCode(source, lang);
  const lines = highlightedHtmlToLines(html);
  const inner = lines
    .map((l) => {
      const content = escapeTsxTemplateContent(preserveLeadingIndent(l));
      return `${indent}    <Text className="code-line">{\`${content}\`}</Text>`;
    })
    .join("\n");
  const id = snippets.add(snippetScope, source);
  if (noHeader) {
    return (
      `${indent}<Widget className="code-block" data-language="${lang}" data-source="${id}">\n` +
      `${indent}  <Widget className="code-block-body">\n` +
      `${indent}    <CodeBlockCopy />\n` +
      `${inner}\n` +
      `${indent}  </Widget>\n` +
      `${indent}</Widget>`
    );
  }
  const title = overrideTitle ?? friendlyLangName(lang);
  return (
    `${indent}<Widget className="code-block" data-language="${lang}" data-source="${id}">\n` +
    `${indent}  <Widget className="code-block-header">\n` +
    `${indent}    <Text className="code-block-title">${escapeTsx(title)}</Text>\n` +
    `${indent}  </Widget>\n` +
    `${indent}  <Widget className="code-block-body">\n` +
    `${indent}    <CodeBlockCopy />\n` +
    `${inner}\n` +
    `${indent}  </Widget>\n` +
    `${indent}</Widget>`
  );
}

function renderDescription(
  description: string,
  compiledExamples: CompiledExample[],
  snippets: SnippetRegistry,
  snippetScope: string,
  fieldTableNames: Set<string>,
  locale: string,
): string {
  if (!description) return "";

  const paragraphs = description.split(/\n\s*\n/).filter(p => p.trim());
  if (paragraphs.length === 0) return "";

  const out: string[] = [];

  // First paragraph: wrap with doc-page-desc class
  const firstPara = paragraphs[0].trim();
  const firstStyled = markdownInlineToLcuiStyleTags(firstPara);
  const firstEscaped = escapeTsxTemplateContent(firstStyled);
  out.push(`      <p className="doc-page-desc">{\`${firstEscaped}\`}</p>`);

  // Remaining paragraphs: render as section content
  if (paragraphs.length > 1) {
    const rest = paragraphs.slice(1).join("\n\n");
    const restContent = renderSectionContent(rest, compiledExamples, "      ", snippets, snippetScope, fieldTableNames, locale);
    if (restContent) {
      out.push(restContent);
    }
  }

  return out.join("\n");
}

function renderSectionContent(
  content: string,
  compiledExamples: CompiledExample[],
  indent: string,
  snippets: SnippetRegistry,
  snippetScope: string,
  fieldTableNames: Set<string>,
  locale: string,
): string {
  const lines = content.split("\n");
  const out: string[] = [];

  let i = 0;
  while (i < lines.length) {
    const line = lines[i];
    const trimmed = line.trim();

    if (trimmed === "") {
      i++;
      continue;
    }

    // Code fence: ```lang [meta] where meta can include title="..."
    const fenceMatch = line.match(/^```(\S*)(?:\s+(.+))?$/);
    if (fenceMatch) {
      const lang = fenceMatch[1] || "text";
      const meta = parseCodeMeta(fenceMatch[2]);
      const buf: string[] = [];
      i++;
      while (i < lines.length && !lines[i].startsWith("```")) {
        buf.push(lines[i]);
        i++;
      }
      i++; // skip closing ```
      out.push(
        renderCodeBlock(
          lang, buf, indent, snippets, snippetScope,
          meta.title,
          meta.title === "",
        ),
      );
      continue;
    }

    // Admonition containers: :::note|info|tip|caution|warning
    const admonMatch = line.match(/^:::(note|info|tip|caution|warning)\s*$/);
    if (admonMatch) {
      const type = admonMatch[1];
      const labels = { note: "提示", info: "信息", tip: "技巧", caution: "警告", warning: "注意" };
      const admonLines: string[] = [];
      i++;
      let depth = 1;
      while (i < lines.length) {
        const ln = lines[i];
        if (ln.match(/^:::(note|info|tip|caution|warning)\s*$/)) {
          depth++;
        } else if (ln.trim() === ":::") {
          depth--;
          if (depth === 0) {
            break;
          }
        }
        admonLines.push(ln);
        i++;
      }
      i++; // skip closing :::
      const admonInner = renderSectionContent(
        admonLines.join("\n"),
        compiledExamples,
        indent + "    ",
        snippets,
        snippetScope,
        fieldTableNames,
        locale,
      );
      out.push(
        `${indent}<Widget className="admonition admonition-${type}">\n` +
        `${indent}  <Text className="admonition-title">${escapeTsx(labels[type as keyof typeof labels])}</Text>\n` +
        `${indent}  <Widget className="admonition-content">\n` +
        `${admonInner}\n` +
        `${indent}  </Widget>\n` +
        `${indent}</Widget>`
      );
      continue;
    }

    // <WidgetExample name="..."/>
    const exMatch = line.match(/^<WidgetExample\s+name="([a-z0-9-]+)"\s*\/?>/);
    if (exMatch) {
      const name = exMatch[1];
      const found = compiledExamples.find((ex) => ex.name === name);
      if (found) {
        out.push(`${indent}<${pascalCase(name)}Demo />`);
      }
      i++;
      continue;
    }

    // <FieldTable name="..." />
    const ftMatch = line.match(/^<FieldTable\s+name="([a-z][a-z0-9-]*)"\s*\/?>/);
    if (ftMatch) {
      const name = ftMatch[1];
      const fieldsVar = camelCase(name) + "Fields";
      fieldTableNames.add(name);
      out.push(
        `${indent}<FieldTable fields={${fieldsVar}} locale="${locale}" />`,
      );
      i++;
      continue;
    }

    // ### heading inside section content
    const h3Match = line.match(/^### (.+)$/);
    if (h3Match) {
      out.push(`${indent}<h3>${escapeTsx(h3Match[1])}</h3>`);
      i++;
      continue;
    }

    const h4Match = line.match(/^#### (.+)$/);
    if (h4Match) {
      out.push(`${indent}<h4>${escapeTsx(h4Match[1])}</h4>`);
      i++;
      continue;
    }

    const h5Match = line.match(/^##### (.+)$/);
    if (h5Match) {
      out.push(`${indent}<h5>${escapeTsx(h5Match[1])}</h5>`);
      i++;
      continue;
    }

    const h6Match = line.match(/^###### (.+)$/);
    if (h6Match) {
      out.push(`${indent}<h6>${escapeTsx(h6Match[1])}</h6>`);
      i++;
      continue;
    }

    // Unordered list:  - foo / * foo
    // Ordered list:    1. foo
    const ulMatch = line.match(/^[-*]\s+(.+)$/);
    const olMatch = line.match(/^\d+\.\s+(.+)$/);
    if (ulMatch || olMatch) {
      const ordered = !!olMatch;
      const items: string[] = [];
      while (i < lines.length) {
        const cur = lines[i];
        const m = ordered
          ? cur.match(/^\d+\.\s+(.+)$/)
          : cur.match(/^[-*]\s+(.+)$/);
        if (!m) break;
        items.push(m[1]);
        i++;
      }
      const tag = ordered ? "ol" : "ul";
      const inner = items
        .map((it) => {
          const styled = markdownInlineToLcuiStyleTags(it);
          const escaped = escapeTsxTemplateContent(styled);
          return `${indent}  <li><Text>{\`${escaped}\`}</Text></li>`;
        })
        .join("\n");
      out.push(`${indent}<${tag}>\n${inner}\n${indent}</${tag}>`);
      continue;
    }

    // Markdown table: |col|col| followed by |---|---|.
    // Convert to a flex layout where the last wide column stretches
    // to fill the remaining space.
    if (
      trimmed.startsWith("|") &&
      trimmed.endsWith("|") &&
      i + 1 < lines.length &&
      /^[\s|:-]+$/.test(lines[i + 1]) &&
      lines[i + 1].includes("-")
    ) {
      const tableRows: MdTableRow[] = [];
      while (i < lines.length) {
        const ln = lines[i];
        if (!ln.trim().startsWith("|") || !ln.trim().endsWith("|")) break;
        tableRows.push({ cells: parseMarkdownTableCells(ln) });
        i++;
      }
      if (tableRows.length >= 2) {
        out.push(renderMarkdownTable(tableRows, indent));
      } else {
        // Malformed table — render as paragraph.
        const styled = markdownInlineToLcuiStyleTags(
          tableRows.map((r) => r.cells.join(" | ")).join(" "),
        );
        out.push(renderInlineLine(styled, indent));
      }
      continue;
    }

    // Coalesce consecutive non-empty plain-text lines into one paragraph.
    const paraBuf: string[] = [line];
    i++;
    while (i < lines.length) {
      const next = lines[i];
      const nextTrim = next.trim();
      if (nextTrim === "") break;
      if (next.startsWith("```")) break;
      if (next.match(/^<WidgetExample\s+/)) break;
      if (next.match(/^<FieldTable\s+/)) break;
      if (next.match(/^### /)) break;
      if (next.match(/^[-*]\s+/)) break;
      if (next.match(/^\d+\.\s+/)) break;
      if (nextTrim.startsWith("|") && nextTrim.endsWith("|")) break;
      paraBuf.push(next);
      i++;
    }
    const paraText = paraBuf.join(" ").replace(/\s+/g, " ").trim();
    out.push(renderInlineLine(paraText, indent));
  }

  return out.join("\n");
}

function renderSections(
  sections: ParsedSection[],
  compiledExamples: CompiledExample[],
  snippets: SnippetRegistry,
  snippetScope: string,
  fieldTableNames: Set<string>,
  locale: string,
): string {
  const blocks: string[] = [];
  for (const section of sections) {
    if (section.level !== 2) continue;
    const inner = renderSectionContent(
      section.content, compiledExamples, "        ", snippets, snippetScope,
      fieldTableNames,
      locale,
    );
    blocks.push(
      `      <Widget className="doc-section">\n` +
        `        <h2>${escapeTsx(section.heading)}</h2>\n` +
        (inner ? `${inner}\n` : "") +
        `      </Widget>`,
    );
  }
  return blocks.join("\n");
}

// ---------------------------------------------------------------------------
// Demo sub-component generator (index.tsx + index.c)
// ---------------------------------------------------------------------------

function buildDemoIndexTsx(ex: CompiledExample, snippets: SnippetRegistry): string {
  const componentName = pascalCase(ex.name) + "Demo";

  // Sort variants so tsx comes first — on_ready picks the first
  // language tab as the default, and tsx is the primary language for
  // this doc-viewer.  Other variants keep their original relative
  // order after tsx.
  //
  // Within each variant, sort files so that `.tsx` (and `.ts`/`.jsx`)
  // files come before `.c`/`.h`/`.xml` — the first file of the
  // default tsx language is the one auto-selected on mount, and
  // framework code is more interesting than the surrounding boilerplate.
  const frameworkExt = /\.(tsx|ts|jsx)$/i;
  const sortedVariants = ex.variants.map((v) => ({
    ...v,
    files: [...v.files].sort((a, b) => {
      const aF = frameworkExt.test(a.filename) ? 0 : 1;
      const bF = frameworkExt.test(b.filename) ? 0 : 1;
      return aF - bF;
    }),
  })).sort((a, b) => {
    if (a.id === "tsx") return -1;
    if (b.id === "tsx") return 1;
    return 0;
  });

  // Default language is the first in sortedVariants (tsx).
  // Default file is the first file of the default variant.
  const defaultLanguage = sortedVariants[0].id;
  const defaultFile = sortedVariants[0].files[0].filename;

  // Language tabs.  The default language tab gets `active` immediately
  // so it renders visibly on mount.  Other language tabs are always
  // visible (no display:none) — only the active one is styled.
  const langTabs = sortedVariants
    .map(
      (v) =>
        `            <Text className="demo-tab${v.id === defaultLanguage ? " active" : ""}" data-value="${v.id}">${v.id.toUpperCase()}</Text>`,
    )
    .join("\n");

  // File tabs.  Only the default (language, file) pair is visible on
  // mount; all others carry `display: none` in inline style.
  // demo-provider.c uses ui_widget_show (which strips inline
  // display:none) and ui_widget_hide (which sets inline display:none)
  // so the inline style here is the starting point, not a permanent
  // override.
  const fileTabs: string[] = [];
  for (const variant of sortedVariants) {
    for (const file of variant.files) {
      const isDefault =
        variant.id === defaultLanguage && file.filename === defaultFile;
      const cls = isDefault ? "demo-tab active" : "demo-tab";
      const style = isDefault ? "" : ' style={{ display: "none" }}';
      fileTabs.push(
        `            <Text className="${cls}" data-language="${variant.id}" data-value="${file.filename}"${style}>${file.filename}</Text>`,
      );
    }
  }

  // Code block bodies: one per (language, file), flat under the
  // wrapper .code-block.  Each body carries data-language + data-file
  // + data-source (snippet id) directly.  The wrapper .code-block
  // provides the card chrome (border, rounding, background), so the
  // bodies themselves only render their content.  Default body is
  // shown immediately with `active`; others carry inline
  // `display: none` to avoid a flash of all bodies on mount.
  const snippetScope = `examples/${ex.name}`;
  const codeBodies: string[] = [];
  for (const variant of sortedVariants) {
    for (const file of variant.files) {
      const isDefault =
        variant.id === defaultLanguage && file.filename === defaultFile;
      const cls = isDefault ? "code-block-body active" : "code-block-body";
      const style = isDefault ? "" : ' style={{ display: "none" }}';
      const lineWidgets = file.lines
        .map((line) => {
          const content = escapeTsxTemplateContent(preserveLeadingIndent(line));
          return `          <Text className="code-line">{\`${content}\`}</Text>`;
        })
        .join("\n");
      const id = snippets.add(snippetScope, file.source);
      codeBodies.push(
        `        <Widget className="${cls}" data-language="${variant.id}" data-file="${file.filename}" data-source="${id}"${style}>\n` +
        `          <CodeBlockCopy />\n` +
        `${lineWidgets}\n` +
        `        </Widget>`,
      );
    }
  }

  // Header groups file tabs (.demo-files) and language tabs
  // (.demo-languages) in a single flex-row widget.  The code-block-copy
  // button lives inside each code-block-body (see codeBodies above).
  return `import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ${componentName}() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
${fileTabs.join("\n")}
          </Widget>
          <Widget className="demo-languages">
${langTabs}
          </Widget>
        </Widget>
${codeBodies.join("\n")}
      </Widget>
    </DemoProvider>
  );
}
`;
}

function buildDemoIndexC(
  ex: CompiledExample,
  demoDir: string,
  repoRoot: string,
): string {
  const ns = `${cIdent(ex.name)}_demo`;
  const examplesRelPath = toPosix(
    relative(demoDir, join(repoRoot, "docs", "examples", ex.name, "main.c")),
  );

  const localSymbols = ex.localSymbols ?? [];
  let includeBlock = "";
  if (localSymbols.length > 0) {
    const prefix = ex.name.replace(/-/g, "_");
    includeBlock += localSymbols
      .map((sym) => `#define ${sym} ${prefix}__${sym}`)
      .join("\n");
    includeBlock += "\n";
    includeBlock += `#include "${examplesRelPath}"\n`;
    includeBlock += localSymbols.map((sym) => `#undef ${sym}`).join("\n");
  } else {
    includeBlock = `#include "${examplesRelPath}"`;
  }

  return `#include "index.tsx.h"
#include "index.h"

${includeBlock}

typedef struct {
        ${ns}_react_t base;
} ${ns}_t;

static void ${ns}_init(ui_widget_t *w)
{
        ${ns}_t *_that = ui_widget_add_data(
            w, ${ns}_proto, sizeof(${ns}_t));
        ${ns}_proto->proto->init(w);
        ${ns}_react_init(w);
        ${ex.funcName}(_that->base.refs.preview);
        ${ns}_update(w);
}

static void ${ns}_destroy(ui_widget_t *w)
{
        ${ns}_proto->proto->destroy(w);
        ${ns}_react_destroy(w);
}

static void ${ns}_update(ui_widget_t *w)
{
        ${ns}_react_update(w);
}

ui_widget_t *ui_create_${ns}(void)
{
        return ui_create_widget_with_prototype(${ns}_proto);
}

void ui_register_${ns}(void)
{
        ${ns}_init_prototype();
        ${ns}_proto->init = ${ns}_init;
        ${ns}_proto->destroy = ${ns}_destroy;
}
`;
}

// ---------------------------------------------------------------------------
// page.tsx generator
// ---------------------------------------------------------------------------

function buildComponentPageTsx(
  parsed: ParsedMdx,
  compiledExamples: CompiledExample[],
  slug: string,
  pageDir: string,
  examplesOutDir: string,
  snippets: SnippetRegistry,
  snippetScope: string,
  locale: string,
): string {
  const componentName = pascalCase(slug) + "DocPage";

  // Each demo is emitted under examples/doc-viewer/app/examples/<name>/ so
  // the directory layout is shared across widget pages.
  //
  // NOTE: the trailing "/index" suffix is required by the current
  // @lcui/cli module resolver, which only tries the file paths listed
  // in resolve.extensions and does not fall back to "<name>/index.tsx"
  // when "<name>" resolves to a directory.  Drop the "/index" once
  // lcui-cli supports directory imports.
  const importPathFor = (name: string) =>
    relativeImport(pageDir, join(examplesOutDir, name, "index"));

  const fieldTableNames = new Set<string>();

  const demoImports = compiledExamples
    .map((ex) => {
      const demoName = pascalCase(ex.name) + "Demo";
      return `import ${demoName} from "${importPathFor(ex.name)}";`;
    })
    .join("\n");

  const basicDemoName = parsed.basicExample
    ? pascalCase(parsed.basicExample.name) + "Demo"
    : null;
  const basicDemoMarkup = basicDemoName
    ? `      <${basicDemoName} />\n`
    : "";

  const sectionBlocks = renderSections(
    parsed.sections, compiledExamples, snippets, snippetScope, fieldTableNames,
    locale,
  );

  const descContent = renderDescription(
    parsed.description, compiledExamples, snippets, snippetScope, fieldTableNames,
    locale,
  );

  const componentsDir = join(examplesOutDir, "..", "components");
  const ccPath = relativeImport(pageDir, join(componentsDir, "code-block-copy"));
  const ftablePath = relativeImport(pageDir, join(componentsDir, "field-table"));

  const fieldTableImports = Array.from(fieldTableNames).map((name) => {
    const fieldsVarName = camelCase(name) + "Fields";
    const fieldsPath = relativeImport(pageDir, join(examplesOutDir, "..", "widget-fields", name));
    return `import { ${fieldsVarName} } from "${fieldsPath}";`;
  }).join("\n");

  let ftImportBlock = "";
  if (fieldTableNames.size > 0) {
    ftImportBlock = `import FieldTable from "${ftablePath}";\n${fieldTableImports}`;
  }

  const allImports = [demoImports, ftImportBlock].filter(Boolean).join("\n");

  return `import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "${ccPath}";
${allImports}

export default function ${componentName}() {
  return (
    <Widget className="doc-page">
      <h1>${escapeTsx(parsed.title)}</h1>
${descContent}
${basicDemoMarkup}${sectionBlocks}
    </Widget>
  );
}
`;
}

function buildContentPageTsx(
  parsed: ParsedMdx,
  slug: string,
  pageDir: string,
  examplesOutDir: string,
  snippets: SnippetRegistry,
  snippetScope: string,
  locale: string,
): string {
  const componentName = pascalCase(slug) + "DocPage";
  const fieldTableNames = new Set<string>();
  const sectionBlocks = renderSections(
    parsed.sections, [], snippets, snippetScope, fieldTableNames, locale,
  );
  const ccPath = relativeImport(pageDir, join(examplesOutDir, "..", "components", "code-block-copy"));

  const componentsDir = join(examplesOutDir, "..", "components");
  const ftablePath = relativeImport(pageDir, join(componentsDir, "field-table"));

  const fieldTableImports = Array.from(fieldTableNames).map((name) => {
    const fieldsVarName = camelCase(name) + "Fields";
    const fieldsPath = relativeImport(pageDir, join(examplesOutDir, "..", "widget-fields", name));
    return `import { ${fieldsVarName} } from "${fieldsPath}";`;
  }).join("\n");

  let ftImportBlock = "";
  if (fieldTableNames.size > 0) {
    ftImportBlock = `import FieldTable from "${ftablePath}";\n${fieldTableImports}`;
  }

  const allImports = [ftImportBlock].filter(Boolean).join("\n");
  const importsBlock = allImports ? `${allImports}\n` : "";

  return `import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "${ccPath}";
${importsBlock}
export default function ${componentName}() {
  return (
    <Widget className="doc-page">
      <h1>${escapeTsx(parsed.title)}</h1>
${renderDescription(parsed.description, [], snippets, snippetScope, fieldTableNames, locale)}
${sectionBlocks}
    </Widget>
  );
}
`;
}

// ---------------------------------------------------------------------------
// Public API
//
// NOTE: page.c is intentionally NOT generated here.  `lcui build` emits a
// per-page page.c skeleton on first build (one-shot) with the correct
// widget prototype boilerplate.  See .opencode/skills/lcui-cli/SKILL.md
// for the convention.  We only generate page.tsx (regenerated every
// compile) and the demo sub-component files.
// ---------------------------------------------------------------------------

export interface GeneratePageFilesOptions {
  /** Absolute path to the repository root (used to resolve docs/examples). */
  repoRoot: string;
  outDir: string;
  /**
   * Root directory under which per-example demo widgets are emitted.
   * Each compiled example is written to `${examplesOutDir}/<name>/`.
   */
  examplesOutDir: string;
  parsed: ParsedMdx;
  compiledExamples: CompiledExample[];
  locale: string;
  section: string;
  slug: string;
  snippets: SnippetRegistry;
}

export async function generatePageFiles(
  opts: GeneratePageFilesOptions,
): Promise<void> {
  const { repoRoot, outDir, examplesOutDir, parsed, compiledExamples,
          section, snippets } = opts;

  const isWidgetPage = section === "widgets";
  const hasExamples = compiledExamples.length > 0;
  const snippetScope = `${opts.locale}/${opts.section}/${opts.slug}`;

  let tsxContent: string;

  if (isWidgetPage && hasExamples) {
    tsxContent = buildComponentPageTsx(
      parsed,
      compiledExamples,
      opts.slug,
      outDir,
      examplesOutDir,
      snippets,
      snippetScope,
      opts.locale,
    );

    // Generate demo sub-components under app/examples/<name>/.  The
    // directory is shared across widget pages, so the same example can
    // be referenced from more than one page without duplication.
    for (const ex of compiledExamples) {
      const demoDir = join(examplesOutDir, ex.name);
      mkdirSync(demoDir, { recursive: true });
      // index.tsx is regenerated on every compile (derived from MDX +
      // docs/examples sources).  index.c is treated as an editable
      // skeleton: written only when the file does not yet exist, so a
      // developer's hand-written event handlers are preserved across
      // recompiles.  This mirrors how @lcui/cli emits its own *.c/*.h
      // skeletons during `lcui build`.
      writeFileSync(
        join(demoDir, "index.tsx"),
        buildDemoIndexTsx(ex, snippets),
        "utf8",
      );
      const indexCPath = join(demoDir, "index.c");
      if (!existsSync(indexCPath)) {
        writeFileSync(
          indexCPath,
          buildDemoIndexC(ex, demoDir, repoRoot),
          "utf8",
        );
      }
    }
  } else {
    tsxContent = buildContentPageTsx(
      parsed, opts.slug, outDir, examplesOutDir, snippets, snippetScope,
      opts.locale,
    );
  }

  writeFileSync(join(outDir, "page.tsx"), tsxContent, "utf8");
}

// ---------------------------------------------------------------------------
// Route titles header generator
//
// Emits app/components/route-titles.h: a static table mapping every
// "/<locale>/<section>/<slug>" path to a wide-character page title.
// Consumed by navbar.c on every router_watch callback so the navbar
// address bar text and the OS window title stay in sync with the
// current route.
//
// The header is fully ASCII -- non-ASCII title characters (e.g. zh-CN
// labels) are emitted as \uXXXX escapes so the file can be compiled
// without /utf-8 or a UTF-8 BOM and never trips MSVC C4819.
// ---------------------------------------------------------------------------

interface RouteSidebarItem {
  slug: string;
  label: { en: string; "zh-CN": string };
}
interface RouteSidebarSection {
  slug: string;
  label: { en: string; "zh-CN": string };
  items: RouteSidebarItem[];
}

function wideStringLiteral(s: string): string {
  let out = 'L"';
  for (const ch of s) {
    const code = ch.codePointAt(0)!;
    if (code === 0x22) out += '\\"';
    else if (code === 0x5c) out += "\\\\";
    else if (code >= 0x20 && code < 0x7f) out += ch;
    else if (code <= 0xffff) {
      out += "\\u" + code.toString(16).padStart(4, "0");
    } else {
      out += "\\U" + code.toString(16).padStart(8, "0");
    }
  }
  return out + '"';
}

export interface GenerateRouteTitlesOptions {
  outFile: string;
  sidebar: RouteSidebarSection[];
  locales: string[];
}

export async function generateRouteTitles(
  opts: GenerateRouteTitlesOptions,
): Promise<void> {
  const rows: string[] = [];
  for (const locale of opts.locales) {
    const localeKey = locale as "en" | "zh-CN";
    for (const section of opts.sidebar) {
      for (const item of section.items) {
        const path = `/${locale}/${section.slug}/${item.slug}`;
        const title = item.label[localeKey];
        rows.push(
          `    { "${path}", ${wideStringLiteral(title)} },`,
        );
      }
    }
  }
  const body = `/* Auto-generated by scripts/compiler/page-generator.ts.  Do not edit. */
#ifndef DOC_VIEWER_ROUTE_TITLES_H
#define DOC_VIEWER_ROUTE_TITLES_H

#include <string.h>
#include <wchar.h>

typedef struct {
        const char *path;
        const wchar_t *title;
} route_title_t;

static const route_title_t route_titles[] = {
${rows.join("\n")}
    { NULL, NULL }
};

static inline const wchar_t *route_title_for(const char *path)
{
        const route_title_t *r;
        if (!path) {
                return L"";
        }
        for (r = route_titles; r->path; ++r) {
                if (strcmp(r->path, path) == 0) {
                        return r->title;
                }
        }
        return L"";
}

#endif
`;
  writeFileSync(opts.outFile, body, "utf8");
}

// ---------------------------------------------------------------------------
// Code snippets table generator
//
// Emits app/components/code-snippets.{c,h}: a static lookup table
// mapping every registered snippet id to its raw UTF-8 source.
// Consumed by app/components/code-block-copy.c so the click handler
// can resolve a code block's `data-source` attribute to the source
// text without inlining multi-line literals into every page.tsx.h.
// ---------------------------------------------------------------------------

function cStringLiteral(s: string): string {
  let out = '"';
  let prevWasHex = false;
  for (const ch of s) {
    const code = ch.codePointAt(0)!;
    if (code === 0x22) {
      out += '\\"';
      prevWasHex = false;
    } else if (code === 0x5c) {
      out += "\\\\";
      prevWasHex = false;
    } else if (code === 0x0a) {
      out += "\\n";
      prevWasHex = false;
    } else if (code === 0x0d) {
      out += "\\r";
      prevWasHex = false;
    } else if (code === 0x09) {
      out += "\\t";
      prevWasHex = false;
    } else if (code >= 0x20 && code < 0x7f) {
      // If the previous octet was a \xNN escape and the next plain
      // character is itself a hex digit, MSVC would greedily merge
      // it into the previous escape.  Break the literal in that
      // case using adjacent-string concatenation.
      if (prevWasHex && /[0-9a-fA-F]/.test(ch)) {
        out += '" "' + ch;
      } else {
        out += ch;
      }
      prevWasHex = false;
    } else {
      // Encode any other character (including non-ASCII) as UTF-8
      // octets via \xNN.  Keeps the source file ASCII so MSVC does
      // not warn C4819, and the resulting bytes are still valid
      // UTF-8 at runtime.  C standard says \x consumes ALL adjacent
      // hex digits, so two adjacent UTF-8 octets like \xe4\xb8 would
      // merge into a single out-of-range value -- break the literal
      // between every byte using adjacent string concatenation.
      const buf = Buffer.from(ch, "utf8");
      for (const b of buf) {
        if (prevWasHex) {
          out += '" "';
        }
        out += "\\x" + b.toString(16).padStart(2, "0");
        prevWasHex = true;
      }
    }
  }
  return out + '"';
}

export interface GenerateCodeSnippetsOptions {
  outHFile: string;
  outCFile: string;
  snippets: Snippet[];
}

export async function generateCodeSnippets(
  opts: GenerateCodeSnippetsOptions,
): Promise<void> {
  const rows = opts.snippets
    .map((s) => `    { "${s.id}", ${cStringLiteral(s.source)} },`)
    .join("\n");

  const header = `/* Auto-generated by scripts/compiler/page-generator.ts.  Do not edit. */
#ifndef DOC_VIEWER_CODE_SNIPPETS_H
#define DOC_VIEWER_CODE_SNIPPETS_H

const char *code_snippet_for(const char *id);

#endif
`;
  const cBody = `/* Auto-generated by scripts/compiler/page-generator.ts.  Do not edit. */
#include <string.h>
#include "code-snippets.h"

typedef struct {
        const char *id;
        const char *source;
} code_snippet_t;

static const code_snippet_t code_snippets[] = {
${rows}
    { 0, 0 }
};

const char *code_snippet_for(const char *id)
{
        const code_snippet_t *s;
        if (!id) {
                return 0;
        }
        for (s = code_snippets; s->id; ++s) {
                if (strcmp(s->id, id) == 0) {
                        return s->source;
                }
        }
        return 0;
}
`;
  writeFileSync(opts.outHFile, header, "utf8");
  writeFileSync(opts.outCFile, cBody, "utf8");
}
