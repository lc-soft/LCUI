/**
 * mdx-parser.ts — Parse an MDX source file into structured data.
 *
 * Extracts:
 *   - title         (first # heading)
 *   - description   (first paragraph after title, before first ## heading)
 *   - sections      (## / ### headings with their content blocks)
 *   - examples      (list of { name, heading } from <WidgetExample> tags)
 *
 * Also exposes markdownInlineToLcuiStyleTags() for converting parsed
 * markdown inline content to LCUI-compatible style tags.
 *
 * LCUI does not support mixed text+element nodes.  Inline markup inside a
 * paragraph (e.g. <b>Note:</b> Run <code>ls -a</code>) is flattened into a
 * single <text> element using LCUI style tags:
 *   [b]Note:[/b] Run [bgcolor=#eee]ls -a[/bgcolor]
 */

import { fromMarkdown } from "mdast-util-from-markdown";
import type { PhrasingContent } from "mdast";

export interface ParsedExample {
  name: string;
  heading: string;
}

export interface ParsedSection {
  level: number;
  heading: string;
  content: string;
}

export interface ParsedMdx {
  title: string;
  description: string;
  /**
   * The first `<WidgetExample/>` that appears between the description
   * paragraph and the first `## ` heading.  Conventionally a widget's
   * "basic" demo, rendered immediately under the page header.  Absent
   * for non-widget pages.
   */
  basicExample?: ParsedExample;
  /**
   * Examples that appear inside `## ` sections (e.g. an `## Examples`
   * block).  Does NOT include `basicExample`.
   */
  examples: ParsedExample[];
  sections: ParsedSection[];
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

function trimLines(str: string): string {
  return str
    .split("\n")
    .map((l) => l.trimEnd())
    .join("\n")
    .trim();
}

// ---------------------------------------------------------------------------
// Token types
// ---------------------------------------------------------------------------

const T = {
  H1: "h1",
  H2: "h2",
  H3: "h3",
  CODE_FENCE: "code_fence",
  COMPONENT: "component",
  TEXT: "text",
  EMPTY: "empty",
} as const;

type TokenType = (typeof T)[keyof typeof T];

interface Token {
  type: TokenType;
  text?: string;
  lang?: string;
  name?: string;
}

function tokenizeLine(line: string): Token {
  if (/^# /.test(line)) return { type: T.H1, text: line.slice(2).trim() };
  if (/^## /.test(line)) return { type: T.H2, text: line.slice(3).trim() };
  if (/^### /.test(line)) return { type: T.H3, text: line.slice(4).trim() };
  if (/^```/.test(line)) {
    return { type: T.CODE_FENCE, lang: line.slice(3).trim() };
  }
  const m = line.match(/^<WidgetExample\s+name="([a-z0-9-]+)"\s*\/?>/);
  if (m) return { type: T.COMPONENT, name: m[1] };
  if (line.trim() === "") return { type: T.EMPTY };
  return { type: T.TEXT, text: line };
}

// ---------------------------------------------------------------------------
// Inline markdown → LCUI style tags
// ---------------------------------------------------------------------------

/**
 * Convert inline markdown markup to LCUI text style tags.
 *
 * Uses a markdown AST parser (micromark + mdast-util-from-markdown) to
 * handle emphasis/strong/code nodes correctly including edge cases that
 * regex-based parsers fail on (e.g. multiple `*` pairs, nested emphasis).
 *
 * Supported conversions:
 *   **text** / __text__  →  [b]text[/b]
 *   *text*  / _text_     →  [i]text[/i]
 *   `code`               →  [bgcolor=#eee] code [/bgcolor]  (spaces act as padding)
 *   [text](url)          →  text  (link text only — LCUI has no hyperlink)
 */
export function markdownInlineToLcuiStyleTags(text: string): string {
  const root = fromMarkdown(text);
  const paragraph = root.children[0];
  if (!paragraph || paragraph.type !== "paragraph") {
    return text;
  }
  return renderPhrasingChildren(paragraph.children);
}

function renderPhrasingChildren(nodes: PhrasingContent[]): string {
  const out: string[] = [];
  for (const node of nodes) {
    switch (node.type) {
      case "text":
        out.push(node.value);
        break;
      case "strong":
        out.push(`[b]${renderPhrasingChildren(node.children)}[/b]`);
        break;
      case "emphasis":
        out.push(`[i]${renderPhrasingChildren(node.children)}[/i]`);
        break;
      case "inlineCode":
        out.push(`[bgcolor=#eee] ${node.value} [/bgcolor]`);
        break;
      case "link":
        out.push(renderPhrasingChildren(node.children));
        break;
      case "break":
        out.push(" ");
        break;
      default:
        if ("value" in node) {
          out.push((node as { value: string }).value);
        } else if ("children" in node) {
          out.push(renderPhrasingChildren((node as { children: PhrasingContent[] }).children));
        }
    }
  }
  return out.join("");
}

// ---------------------------------------------------------------------------
// Main parser
// ---------------------------------------------------------------------------

interface SectionBuilder {
  level: number;
  heading: string;
  lines: string[];
}

export function parseMdx(source: string): ParsedMdx {
  const lines = source.split("\n");
  let title = "";
  let description = "";
  const examples: ParsedExample[] = [];
  const sections: ParsedSection[] = [];

  let inCode = false;
  let currentBuilder: SectionBuilder | null = null;
  let phase: "before_title" | "desc" | "sections" = "before_title";
  let lastExampleHeading = "";
  let descLines: string[] = [];
  let basicExample: ParsedExample | undefined;

  function flushSection() {
    if (currentBuilder) {
      sections.push({
        level: currentBuilder.level,
        heading: currentBuilder.heading,
        content: trimLines(currentBuilder.lines.join("\n")),
      });
      currentBuilder = null;
    }
  }

  function appendToCurrentSection(line: string) {
    if (currentBuilder) {
      currentBuilder.lines.push(line);
    }
  }

  for (const line of lines) {
    if (inCode) {
      if (line.startsWith("```")) {
        inCode = false;
      }
      appendToCurrentSection(line);
      continue;
    }

    const tok = tokenizeLine(line);

    if (tok.type === T.CODE_FENCE) {
      inCode = true;
      appendToCurrentSection(line);
      continue;
    }

    switch (phase) {
      case "before_title":
        if (tok.type === T.H1) {
          title = tok.text!;
          phase = "desc";
        }
        break;

      case "desc":
        if (tok.type === T.H2) {
          description = trimLines(descLines.join("\n"));
          phase = "sections";
          flushSection();
          currentBuilder = { level: 2, heading: tok.text!, lines: [] };
          lastExampleHeading = "";
        } else if (tok.type === T.COMPONENT) {
          // A `<WidgetExample />` between the description and the
          // first `## ` heading is the page's "basic" example.  Stop
          // accumulating description text, store the example, and
          // wait for the first `## ` to actually open a section.
          if (!basicExample) {
            basicExample = { name: tok.name!, heading: title };
          }
          description = trimLines(descLines.join("\n"));
          phase = "sections";
        } else if (tok.type !== T.EMPTY || descLines.length > 0) {
          descLines.push(line);
        }
        break;

      case "sections":
        if (tok.type === T.H2) {
          flushSection();
          currentBuilder = { level: 2, heading: tok.text!, lines: [] };
          lastExampleHeading = "";
        } else if (tok.type === T.H3) {
          appendToCurrentSection(line);
          lastExampleHeading = tok.text!;
        } else if (tok.type === T.COMPONENT) {
          examples.push({ name: tok.name!, heading: lastExampleHeading });
          appendToCurrentSection(line);
        } else {
          appendToCurrentSection(line);
        }
        break;
    }
  }

  if (phase === "desc") {
    description = trimLines(descLines.join("\n"));
  }
  flushSection();

  return {
    title,
    description,
    basicExample,
    examples,
    sections,
  };
}
