/**
 * example-compiler.ts — Resolve example source files and apply syntax
 * highlighting so that page-generator can embed them in generated code.
 *
 * For each example name:
 *   - Check which variants exist: plain C (no suffix), -xml, -tsx
 *   - Read main.c (+ ui.xml / ui.tsx where present)
 *   - Apply highlight.js to produce highlighted HTML
 */

import { readFileSync, existsSync } from "fs";
import { join } from "path";
import hljs from "highlight.js";
import { highlightedHtmlToLines } from "./style-tag-renderer.ts";

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

export interface CompiledVariantFile {
  lang: string;
  filename: string;
  source: string;
  highlighted: string;
  /**
   * Source-code lines with highlight.js colours translated into LCUI
   * style tags.  Each entry is one balanced (self-closing) line ready
   * to be embedded in a <Text> widget.  Empty lines become a single
   * space so the rendered widget keeps a non-zero height.
   */
  lines: string[];
}

export interface CompiledVariant {
  id: "c" | "xml" | "tsx";
  files: CompiledVariantFile[];
}

export interface CompiledExample {
  name: string;
  heading: string;
  funcName: string;
  localSymbols: string[];
  variants: CompiledVariant[];
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

const C_KEYWORDS = new Set([
  "void", "int", "char", "float", "double", "long", "short",
  "unsigned", "signed", "const", "volatile", "inline", "extern",
  "static", "register", "auto", "struct", "union", "enum",
  "typedef", "sizeof", "return", "if", "else", "for", "while",
  "do", "switch", "case", "break", "continue", "goto", "default",
  "bool", "true", "false", "NULL", "size_t", "ssize_t",
  "uint8_t", "uint16_t", "uint32_t", "uint64_t",
  "int8_t", "int16_t", "int32_t", "int64_t",
  "ui_widget_t", "ui_event_t", "wchar_t",
]);

function toFuncName(exampleName: string): string {
  return exampleName.replace(/-/g, "_") + "_init";
}

function toPrefix(exampleName: string): string {
  return exampleName.replace(/-/g, "_");
}

export function findLocalSymbols(source: string, exampleName: string): string[] {
  const prefix = toPrefix(exampleName);
  const found = new Set<string>();

  const staticFuncRe = /\bstatic\b[^(;{]*?\b(\w+)\s*\(/g;
  let m: RegExpExecArray | null;
  while ((m = staticFuncRe.exec(source)) !== null) {
    const sym = m[1];
    if (!C_KEYWORDS.has(sym)) found.add(sym);
  }

  const typedefStructRe = /\btypedef\s+struct\b[^;]*?\}\s*(\w+)\s*;/gs;
  while ((m = typedefStructRe.exec(source)) !== null) {
    const sym = m[1];
    if (!C_KEYWORDS.has(sym)) found.add(sym);
  }

  const typedefAliasRe = /\btypedef\s+(?!struct\b)[\w\s*]+\s+(\w+)\s*;/g;
  while ((m = typedefAliasRe.exec(source)) !== null) {
    const sym = m[1];
    if (!C_KEYWORDS.has(sym)) found.add(sym);
  }

  const initFunc = prefix + "_init";
  return [...found].filter(
    (sym) => sym !== initFunc && !sym.startsWith(prefix + "_"),
  );
}

function highlight(source: string, lang: string): string {
  try {
    return hljs.highlight(source, { language: lang }).value;
  } catch {
    return hljs.highlightAuto(source).value;
  }
}

const VARIANT_FILES: Record<string, string[]> = {
  c: ["main.c"],
  xml: ["main.c", "ui.xml"],
  tsx: ["main.c", "example.tsx", "example.css"],
};

const FILE_LANG: Record<string, string> = {
  "main.c": "c",
  "ui.xml": "xml",
  "example.tsx": "typescript",
  "example.css": "css",
};

// ---------------------------------------------------------------------------
// Compile a single example
// ---------------------------------------------------------------------------

function loadVariantFiles(dir: string, filenames: string[]): CompiledVariantFile[] {
  const files: CompiledVariantFile[] = [];
  for (const filename of filenames) {
    const filePath = join(dir, filename);
    if (!existsSync(filePath)) continue;
    const source = readFileSync(filePath, "utf8");
    const lang = FILE_LANG[filename];
    const highlighted = highlight(source, lang);
    files.push({
      lang,
      filename,
      source,
      highlighted,
      lines: highlightedHtmlToLines(highlighted),
    });
  }
  return files;
}

function compileSingleExample(
  example: { name: string; heading: string },
  examplesDir: string,
): CompiledExample {
  const { name, heading } = example;
  const variants: CompiledVariant[] = [];

  const dirs: Array<{ id: "c" | "xml" | "tsx"; dir: string }> = [
    { id: "c", dir: join(examplesDir, name) },
    { id: "xml", dir: join(examplesDir, `${name}-xml`) },
    { id: "tsx", dir: join(examplesDir, `${name}-tsx`) },
  ];

  for (const { id, dir } of dirs) {
    if (!existsSync(join(dir, "main.c"))) continue;
    variants.push({ id, files: loadVariantFiles(dir, VARIANT_FILES[id]) });
  }

  let cMainSource: string | null = null;
  const cMainPath = join(examplesDir, name, "main.c");
  if (existsSync(cMainPath)) {
    cMainSource = readFileSync(cMainPath, "utf8");
  }

  return {
    name,
    heading,
    funcName: toFuncName(name),
    localSymbols: cMainSource ? findLocalSymbols(cMainSource, name) : [],
    variants,
  };
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

export async function compileExamples(
  examples: Array<{ name: string; heading: string }>,
  examplesDir: string,
): Promise<CompiledExample[]> {
  return examples.map((ex) => compileSingleExample(ex, examplesDir));
}
