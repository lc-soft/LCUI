/**
 * style-tag-renderer.ts — Convert highlight.js HTML output into
 * LCUI text style tag strings, split per line.
 *
 * highlight.js emits nested <span class="hljs-xxx">…</span> wrappers
 * around source tokens.  LCUI cannot render mixed text+element nodes
 * inside a single widget, so each source line is rendered as one
 * <Text> element whose content uses LCUI style tag syntax for colour
 * and weight:
 *
 *   [color=#569cd6][b]int[/b][/color] main()
 *
 * This module returns an array of such strings — one per source line.
 */

// ---------------------------------------------------------------------------
// hljs class → LCUI style tag mapping
// ---------------------------------------------------------------------------

interface StyleTagSet {
  color?: string;
  bold?: boolean;
  italic?: boolean;
  bgcolor?: string;
}

/**
 * Maps highlight.js token classes to LCUI text style tags.
 *
 * The colour palette below targets a light-themed code block (white
 * background, dark text), mirroring the GitHub Light syntax theme so
 * an MDX-derived page widget reads cleanly on the doc-viewer's light
 * surfaces.  LCUI's `[color=...]` markup only accepts hex literals,
 * so the comments next to each entry name the GitHub-Light role for
 * future maintenance.
 *
 * Only classes that start with "hljs-" are recognised; other sub-tokens
 * such as "function_", "class_" or "language-xml" that highlight.js
 * emits alongside "hljs-title" are ignored.
 */
const HLJS_CLASS_STYLES: Record<string, StyleTagSet> = {
  "hljs-keyword":        { color: "#cf222e" }, // red-700  -- keyword
  "hljs-built_in":       { color: "#953800" }, // orange-800 -- builtin
  "hljs-type":           { color: "#953800" }, // orange-800 -- type
  "hljs-literal":        { color: "#0550ae" }, // blue-700 -- literal
  "hljs-number":         { color: "#0550ae" }, // blue-700 -- number
  "hljs-string":         { color: "#0a3069" }, // blue-900 -- string
  "hljs-comment":        { color: "#6e7781" }, // gray-500 -- comment
  "hljs-doctag":         { color: "#6e7781" }, // gray-500 -- doctag
  "hljs-title":          { color: "#8250df" }, // purple-700 -- function name
  "hljs-attr":           { color: "#0550ae" }, // blue-700 -- attr
  "hljs-variable":       { color: "#0550ae" }, // blue-700 -- variable
  "hljs-tag":            { color: "#116329" }, // green-800 -- tag
  "hljs-name":           { color: "#0550ae" }, // blue-700 -- name
  "hljs-meta":           { color: "#6e7781" }, // gray-500 -- meta
  "hljs-meta-string":    { color: "#0a3069" }, // blue-900 -- meta-string
  "hljs-params":         { color: "#0550ae" }, // blue-700 -- params
  "hljs-punctuation":    { color: "#1f2328" }, // gray-900 -- punctuation
  "hljs-symbol":         { color: "#0550ae" }, // blue-700 -- symbol
  "hljs-property":       { color: "#0550ae" }, // blue-700 -- property
  "hljs-selector-tag":   { color: "#116329" }, // green-800 -- selector-tag
  "hljs-selector-class": { color: "#953800" }, // orange-800 -- selector-class
  "hljs-selector-id":    { color: "#953800" }, // orange-800 -- selector-id
  "hljs-regexp":         { color: "#0a3069" }, // blue-900 -- regexp
  "hljs-link":           { color: "#0550ae" }, // blue-700 -- link
};

function styleSetForClasses(classes: string[]): StyleTagSet | null {
  for (const cls of classes) {
    if (!cls.startsWith("hljs-")) continue;
    const set = HLJS_CLASS_STYLES[cls];
    if (set) return set;
  }
  return null;
}

function openTags(set: StyleTagSet): string {
  let out = "";
  if (set.color) out += `[color=${set.color}]`;
  if (set.bgcolor) out += `[bgcolor=${set.bgcolor}]`;
  if (set.bold) out += "[b]";
  if (set.italic) out += "[i]";
  return out;
}

function closeTags(set: StyleTagSet): string {
  let out = "";
  if (set.italic) out += "[/i]";
  if (set.bold) out += "[/b]";
  if (set.bgcolor) out += "[/bgcolor]";
  if (set.color) out += "[/color]";
  return out;
}

// ---------------------------------------------------------------------------
// HTML entity decoding
// ---------------------------------------------------------------------------

const ENTITY_MAP: Record<string, string> = {
  "&amp;": "&",
  "&lt;": "<",
  "&gt;": ">",
  "&quot;": '"',
  "&#39;": "'",
  "&apos;": "'",
  "&nbsp;": " ",
};

function decodeEntities(s: string): string {
  return s.replace(/&(?:amp|lt|gt|quot|apos|nbsp|#39);/g, (m) => ENTITY_MAP[m] ?? m);
}

// ---------------------------------------------------------------------------
// LCUI style tag escaping (only needed for literal '[' / ']' in source text)
// ---------------------------------------------------------------------------

function escapeStyleTagText(s: string): string {
  // LCUI parses [tag][/tag] markers; if user source contains literal
  // brackets, escape them by inserting a zero-width separator.  In
  // practice LCUI's text renderer treats only well-known tag names as
  // markup, so unmatched [foo] is shown as-is, but to be safe we
  // prefix '[' with a backslash which the renderer ignores.
  return s.replace(/\[/g, "\\[");
}

// ---------------------------------------------------------------------------
// HTML → LCUI style-tagged lines
// ---------------------------------------------------------------------------

/**
 * Convert highlight.js HTML output into an array of LCUI style-tagged
 * strings, one per source line.
 *
 * Each line is independently balanced: any style tag opened on the
 * line is closed on the same line, so an LCUI <Text> element can
 * render it without leaking styles across element boundaries.
 *
 * Empty source lines become a single space, so the line height stays
 * visible in the demo viewer.
 */
export function highlightedHtmlToLines(html: string): string[] {
  // Normalise CRLF/CR to LF so line splitting is consistent regardless
  // of the source file's line endings.
  html = html.replace(/\r\n?/g, "\n");

  const lines: string[] = [];
  let current = "";
  const stack: StyleTagSet[] = [];

  function reopenStack(): string {
    return stack.map((s) => openTags(s)).join("");
  }

  function closeStack(): string {
    return [...stack].reverse().map((s) => closeTags(s)).join("");
  }

  function appendText(text: string): void {
    const parts = text.split("\n");
    for (let i = 0; i < parts.length; i++) {
      if (i > 0) {
        // Flush current line with all active tags closed.
        current += closeStack();
        lines.push(current);
        // Start the next line with active tags re-opened.
        current = reopenStack();
      }
      current += escapeStyleTagText(parts[i]);
    }
  }

  let i = 0;
  while (i < html.length) {
    if (html.startsWith("<span", i)) {
      const tagEnd = html.indexOf(">", i);
      if (tagEnd < 0) break;
      const tag = html.slice(i, tagEnd + 1);
      const classMatch = tag.match(/class="([^"]+)"/);
      const classes = classMatch ? classMatch[1].split(/\s+/) : [];
      const set = styleSetForClasses(classes) ?? {};
      stack.push(set);
      current += openTags(set);
      i = tagEnd + 1;
      continue;
    }
    if (html.startsWith("</span>", i)) {
      const set = stack.pop();
      if (set) current += closeTags(set);
      i += "</span>".length;
      continue;
    }
    // Plain text up to next '<'
    const next = html.indexOf("<", i);
    const chunk = next < 0 ? html.slice(i) : html.slice(i, next);
    appendText(decodeEntities(chunk));
    i = next < 0 ? html.length : next;
  }

  // Flush trailing line.
  current += closeStack();
  lines.push(current);

  // Trim a trailing empty line that highlight.js often produces from a
  // final '\n' in the source.
  if (lines.length > 0 && lines[lines.length - 1] === "") {
    lines.pop();
  }

  // Replace empty lines with a single space so the line widget keeps
  // its natural height in the doc-viewer.
  return lines.map((l) => (l === "" ? " " : l));
}
