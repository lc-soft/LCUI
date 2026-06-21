#!/usr/bin/env bun
/**
 * LCUI doc-compiler — main entry point
 *
 * Usage:
 *   bun run scripts/compiler/index.ts                  compile docs/ → app/
 *   bun run scripts/compiler/index.ts --watch          recompile on source changes
 *   bun run scripts/compiler/index.ts --css-only       extract CSS from src/widgets/*.c only
 *
 * Output: app/  (App Router layout, one dir per page)
 */

import { readFileSync, writeFileSync, mkdirSync, existsSync, copyFileSync, readdirSync } from "fs";
import { resolve, dirname, join } from "path";
import { fileURLToPath } from "url";

import { parseMdx } from "./mdx-parser.ts";
import { compileExamples, type CompiledExample } from "./example-compiler.ts";
import { generatePageFiles, generateRouteTitles, generateCodeSnippets, SnippetRegistry } from "./page-generator.ts";
import { extractWidgetCss } from "./css-extractor.ts";

// ---------------------------------------------------------------------------
// Sidebar shape (mirrors docs/sidebars.json)
// ---------------------------------------------------------------------------

export interface SidebarSection {
  slug: string;
  label: { en: string; "zh-CN": string };
  items: Array<{ slug: string; label: { en: string; "zh-CN": string } }>;
}

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------

const __dirname = dirname(fileURLToPath(import.meta.url));
const ROOT = resolve(__dirname, "../../../..");
const DOCS_DIR = join(ROOT, "docs");
const APP_DIR = join(ROOT, "examples", "doc-viewer", "app");
const SIDEBARS_PATH = join(DOCS_DIR, "sidebars.json");
const EXAMPLES_DIR = join(DOCS_DIR, "examples");
const WIDGETS_SRC_DIR = join(ROOT, "src", "widgets");
const WIDGET_FIELDS_SRC = join(DOCS_DIR, "widget-fields");

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

function ensureDir(dir: string): void {
  if (!existsSync(dir)) {
    mkdirSync(dir, { recursive: true });
  }
}

function copyWidgetFields(srcDir: string, destDir: string): void {
  if (!existsSync(srcDir)) return;
  ensureDir(destDir);
  for (const entry of readdirSync(srcDir)) {
    if (!entry.endsWith(".ts")) continue;
    copyFileSync(join(srcDir, entry), join(destDir, entry));
  }
}

function loadSidebars(): { sidebar: SidebarSection[] } {
  const raw = readFileSync(SIDEBARS_PATH, "utf8");
  return JSON.parse(raw) as { sidebar: SidebarSection[] };
}

// ---------------------------------------------------------------------------
// Compile a single MDX page
// ---------------------------------------------------------------------------

async function compilePage(
  locale: string,
  section: SidebarSection,
  itemSlug: string,
  localeAppDir: string,
  examplesOutDir: string,
  snippets: SnippetRegistry,
): Promise<CompiledExample[]> {
  const mdxPath = join(DOCS_DIR, locale, section.slug, `${itemSlug}.mdx`);

  if (!existsSync(mdxPath)) {
    console.warn(`  [warn] MDX not found: ${mdxPath}`);
    return [];
  }

  const mdxSource = readFileSync(mdxPath, "utf8");
  const parsed = parseMdx(mdxSource);

  // Collect every example referenced by the page (basic + in-section).
  const allExamples = parsed.basicExample
    ? [parsed.basicExample, ...parsed.examples]
    : parsed.examples;
  const compiledExamples = await compileExamples(allExamples, EXAMPLES_DIR);

  const outDir = join(localeAppDir, section.slug, itemSlug);
  ensureDir(outDir);

  await generatePageFiles({
    repoRoot: ROOT,
    outDir,
    examplesOutDir,
    parsed,
    compiledExamples,
    locale,
    section: section.slug,
    slug: itemSlug,
    snippets,
  });

  console.log(`  [ok] ${locale}/${section.slug}/${itemSlug}`);
  return compiledExamples;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

async function main(): Promise<void> {
  const args = process.argv.slice(2);
  const watchMode = args.includes("--watch");
  const cssOnly = args.includes("--css-only");

  ensureDir(join(DOCS_DIR, "build", "public"));
  await extractWidgetCss(WIDGETS_SRC_DIR, join(DOCS_DIR, "build", "public"));
  console.log(
    "[css] Widget CSS extracted → docs/build/public/component-styles.css",
  );

  if (cssOnly) return;

  const { sidebar } = loadSidebars();
  const locales = ["en", "zh-CN"];

  ensureDir(APP_DIR);

  // Copy sidebar.json to app/components/sidebar.json for the Sidebar component
  const sidebarDestDir = join(APP_DIR, "components");
  ensureDir(sidebarDestDir);
  writeFileSync(
    join(sidebarDestDir, "sidebar.json"),
    readFileSync(SIDEBARS_PATH, "utf8"),
    "utf8",
  );

  // Copy widget-fields data (widget.ts + per-component *.ts) into
  // app/widget-fields/ so generated page.tsx files can import them
  // at compile time.
  copyWidgetFields(WIDGET_FIELDS_SRC, join(APP_DIR, "widget-fields"));

  // Per-example demos are emitted to a single shared directory so the
  // layout mirrors docs/examples/<name>-tsx/ in the source tree.
  const examplesOutDir = join(APP_DIR, "examples");
  ensureDir(examplesOutDir);

  const snippets = new SnippetRegistry();

  for (const locale of locales) {
    console.log(`\n[locale] ${locale}`);
    const localeAppDir = join(APP_DIR, locale);
    ensureDir(localeAppDir);
    for (const section of sidebar) {
      for (const item of section.items) {
        await compilePage(
          locale,
          section,
          item.slug,
          localeAppDir,
          examplesOutDir,
          snippets,
        );
      }
    }
  }

  await generateRouteTitles({
    outFile: join(APP_DIR, "components", "route-titles.h"),
    sidebar,
    locales,
  });
  console.log("[routes] route-titles.h generated");

  await generateCodeSnippets({
    outHFile: join(APP_DIR, "components", "code-snippets.h"),
    outCFile: join(APP_DIR, "components", "code-snippets.c"),
    snippets: snippets.all(),
  });
  console.log(`[snippets] code-snippets.c generated (${snippets.all().length} entries)`);

  if (watchMode) {
    console.log("\n[watch] Watching docs/ for changes… (Ctrl+C to stop)");
    const { watch } = await import("fs");
    watch(DOCS_DIR, { recursive: true }, async (event, filename) => {
      if (!filename || !filename.endsWith(".mdx")) return;
      console.log(`\n[change] ${filename}`);
      const parts = filename.replace(/\\/g, "/").split("/");
      if (parts.length >= 3) {
        const locale = parts[0];
        const sectionDir = parts[1];
        const slug = parts[2].replace(".mdx", "");
        const section = sidebar.find((s) => s.slug === sectionDir);
        if (section) {
          await compilePage(
            locale, section, slug, join(APP_DIR, locale),
            examplesOutDir, snippets);
        }
      }
    });
  } else {
    console.log("\n[done] Build complete → tools/doc-viewer/app/");
  }
}

main().catch((err) => {
  console.error("[error]", err);
  process.exit(1);
});
