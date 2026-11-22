const fs = require("node:fs");
const path = require("node:path");
const postcss = require("postcss");
const {
  accents,
  deriveTokens,
  foregroundFor,
  rgbaString,
  toHex,
} = require("./accent-colors");

const ROOT = path.resolve(__dirname, "..");
const STYLE_NOVA = path.join(ROOT, "src/styles/style-nova.css");
const ACCENTS_DIR = path.join(ROOT, "src/styles/accents");
const PALETTE_HEADER = path.join(ROOT, "src/styles/accent-palette.h");
const RULES_HEADER = path.join(ROOT, "src/styles/accent-rules.h");
const SWATCHES_CSS = path.join(ROOT, "src/styles/accent-swatches.css");
const THEMES_API_HEADER = path.join(ACCENTS_DIR, "accent-themes-api.h");
const TSX_PALETTE = path.join(
  ROOT,
  "examples/doc-viewer/app/components/accent-palette.ts"
);

const TOKEN_MAP = {
  primary: "primary",
  "primary-foreground": "primaryForeground",
  accent: "accent",
  "accent-foreground": "accentForeground",
  "sidebar-primary": "sidebarPrimary",
  "sidebar-primary-foreground": "sidebarPrimaryForeground",
  "sidebar-accent": "sidebarAccent",
  "sidebar-accent-foreground": "sidebarAccentForeground",
};

const PROPERTY_MAP = {
  bg: "background-color",
  text: "color",
  border: "border-color",
  "border-t": "border-top-color",
  "border-b": "border-bottom-color",
  "border-l": "border-left-color",
  "border-r": "border-right-color",
};

const TOKEN_ENUM = {
  primary: "UI_THEME_TOKEN_PRIMARY",
  primaryForeground: "UI_THEME_TOKEN_PRIMARY_FOREGROUND",
  accent: "UI_THEME_TOKEN_ACCENT",
  accentForeground: "UI_THEME_TOKEN_ACCENT_FOREGROUND",
  sidebarPrimary: "UI_THEME_TOKEN_SIDEBAR_PRIMARY",
  sidebarPrimaryForeground: "UI_THEME_TOKEN_SIDEBAR_PRIMARY_FOREGROUND",
  sidebarAccent: "UI_THEME_TOKEN_SIDEBAR_ACCENT",
  sidebarAccentForeground: "UI_THEME_TOKEN_SIDEBAR_ACCENT_FOREGROUND",
};

function parseApplyClass(cls) {
  const dark = cls.startsWith("dark:");
  const body = dark ? cls.slice(5) : cls;
  const match = body.match(/^(bg|text|border(?:-[tblr])?)-(.+)$/);
  if (!match) {
    return null;
  }
  let rest = match[2];
  let alpha;
  const alphaMatch = rest.match(/^(.*)\/(\d+)$/);
  if (alphaMatch) {
    rest = alphaMatch[1];
    alpha = Number(alphaMatch[2]) / 100;
  }
  if (rest.endsWith("-dark")) {
    rest = rest.slice(0, -5);
  }
  const token = TOKEN_MAP[rest];
  if (!token) {
    return null;
  }
  return {
    property: PROPERTY_MAP[match[1]],
    token,
    dark,
    alpha,
  };
}

function extractRules(css) {
  const rules = [];
  const seen = new Set();
  const root = postcss.parse(css);

  root.walkRules((rule) => {
    /* Badges keep their own semantic colors (secondary/outline/ghost and
     * the custom color variants); only the explicit default variant follows
     * the accent. */
    if (
      /\bbadge\b/.test(rule.selector) &&
      !/badge:default\b/.test(rule.selector) &&
      !/\.badge-default\b/.test(rule.selector)
    ) {
      return;
    }
    rule.walkAtRules("apply", (at) => {
      for (const cls of at.params.split(/\s+/)) {
        const parsed = parseApplyClass(cls);
        if (!parsed) {
          continue;
        }
        const key = [
          rule.selector,
          parsed.property,
          parsed.token,
          parsed.dark,
          parsed.alpha,
        ].join("|");
        if (seen.has(key)) {
          continue;
        }
        seen.add(key);
        rules.push({ selector: rule.selector, ...parsed });
      }
    });
    /* Focus rings are box-shadows; override their color so the ring follows
     * the accent instead of the fixed neutral `boxShadow.focus*` values. */
    rule.walkDecls("box-shadow", (decl) => {
      const match = decl.value.match(
        /theme\("boxShadow\.(focus(?:-sm)?)(-dark)?"\)/
      );
      if (!match) {
        return;
      }
      for (const part of rule.selector.split(",")) {
        let selector = part.trim();
        let dark = !!match[2];
        if (selector.startsWith(".dark ")) {
          selector = selector.slice(6).trim();
          dark = true;
        }
        const key = [selector, "box-shadow", "primary", dark, match[1]].join(
          "|"
        );
        if (seen.has(key)) {
          continue;
        }
        seen.add(key);
        rules.push({
          selector,
          property: "box-shadow",
          token: "primary",
          dark,
          alpha: dark ? 0.3 : 0.2,
          shadow: match[1] === "focus-sm" ? 2 : 3,
        });
      }
    });
  });
  return rules;
}

function scopeSelector(selector, name, dark) {
  const prefix = dark ? `.accent-${name}.dark` : `.accent-${name}`;
  return selector
    .split(",")
    .map((part) => `${prefix} ${part.trim()}`)
    .join(", ");
}

function tokenValue(tokens, rule) {
  const entry = tokens[rule.dark ? "dark" : "light"][rule.token];
  const alpha = rule.alpha !== undefined ? rule.alpha : entry.alpha;
  return rgbaString(entry, alpha);
}

function cssForAccent(accent, rules) {
  const tokens = deriveTokens(accent);
  return rules
    .map((rule) => {
      const selector = scopeSelector(rule.selector, accent.name, rule.dark);
      const color = tokenValue(tokens, rule);
      const value = rule.shadow ? `0 0 0 ${rule.shadow}px ${color}` : color;
      return `${selector} {\n  ${rule.property}: ${value};\n}\n`;
    })
    .join("\n");
}

function hexPart(value) {
  return value.toString(16).padStart(2, "0");
}

function colorLiteral(color) {
  return `{ .b = 0x${hexPart(color.b)}, .g = 0x${hexPart(color.g)}, .r = 0x${hexPart(color.r)}, .a = 255 }`;
}

function paletteHeader() {
  const rows = accents
    .map(
      (accent) =>
        `        { "${accent.name}", ${colorLiteral(accent.light)}, ${colorLiteral(accent.dark)} },`
    )
    .join("\n");
  return `/* Generated by scripts/generate-accent-themes.js */
/* clang-format off */
static const ui_theme_accent_t ui_theme_accents[] = {
${rows}
};
/* clang-format on */
`;
}

function rulesHeader(rules) {
  const rows = [];
  for (const rule of rules) {
    const alpha =
      rule.alpha === undefined ? -1 : Math.round(rule.alpha * 255);
    for (const selector of rule.selector.split(",").map((part) => part.trim())) {
      rows.push(
        `        { "${selector}", "${rule.property}", ${TOKEN_ENUM[rule.token]}, ${rule.dark ? 1 : 0}, ${alpha}, ${rule.shadow || 0} },`
      );
    }
  }
  return `/* Generated by scripts/generate-accent-themes.js */
/* clang-format off */
static const ui_theme_rule_t ui_theme_rules[] = {
${rows.join("\n")}
};
/* clang-format on */
`;
}

function swatchesCss() {
  return accents
    .map((accent) => {
      const lightForeground = toHex(foregroundFor(accent.light));
      const darkForeground = toHex(foregroundFor(accent.dark));
      return `.accent-swatch-${accent.name} {\n  background-color: ${toHex(accent.light)};\n}\n.accent-swatch-${accent.name} .accent-swatch-check {\n  color: ${lightForeground};\n}\n.dark .accent-swatch-${accent.name} {\n  background-color: ${toHex(accent.dark)};\n}\n.dark .accent-swatch-${accent.name} .accent-swatch-check {\n  color: ${darkForeground};\n}\n`;
    })
    .join("\n");
}

function tsxPalette() {
  return `// Generated by scripts/generate-accent-themes.js
export const accentNames = [
${accents.map((accent) => `  "${accent.name}",`).join("\n")}
] as const;
`;
}

function themesHeader() {
  const includes = accents
    .map((accent) => `#include "${accent.name}.css.h"`)
    .join("\n");
  const branches = accents
    .map(
      (accent) =>
        `        if (strcmp(name, "${accent.name}") == 0) {\n                ui_load_css_string_${accent.name}_chunks();\n                return true;\n        }`
    )
    .join("\n");
  return `/* Generated by scripts/generate-accent-themes.js */
#include "accent-themes-api.h"
${includes}
#include "../accent-swatches.css.h"

void ui_load_accent_swatches(void)
{
        ui_load_css_string_accent_swatches_chunks();
}

bool ui_load_accent_theme(const char *name)
{
        if (!name) {
                return false;
        }
${branches}
        return false;
}
`;
}

function themesApiHeader() {
  return `/* Generated by scripts/generate-accent-themes.js */
#ifndef LCUI_GENERATED_ACCENT_THEMES_API_H
#define LCUI_GENERATED_ACCENT_THEMES_API_H

#include <stdbool.h>

void ui_load_accent_swatches(void);
bool ui_load_accent_theme(const char *name);

#endif
`;
}

function generate() {
  const source = fs.readFileSync(STYLE_NOVA, "utf8");
  const rules = extractRules(source);

  fs.mkdirSync(ACCENTS_DIR, { recursive: true });
  for (const file of fs.readdirSync(ACCENTS_DIR)) {
    const match = file.match(/^(.+?)\.css(\.h)?$/);
    if (!match) {
      continue;
    }
    if (!accents.some((accent) => accent.name === match[1])) {
      fs.unlinkSync(path.join(ACCENTS_DIR, file));
    }
  }
  for (const accent of accents) {
    fs.writeFileSync(
      path.join(ACCENTS_DIR, `${accent.name}.css`),
      cssForAccent(accent, rules)
    );
  }
  fs.writeFileSync(PALETTE_HEADER, paletteHeader());
  fs.writeFileSync(RULES_HEADER, rulesHeader(rules));
  fs.writeFileSync(SWATCHES_CSS, swatchesCss());
  fs.writeFileSync(path.join(ACCENTS_DIR, "accent-themes.h"), themesHeader());
  fs.writeFileSync(THEMES_API_HEADER, themesApiHeader());
  fs.mkdirSync(path.dirname(TSX_PALETTE), { recursive: true });
  fs.writeFileSync(TSX_PALETTE, tsxPalette());
  return rules;
}

if (require.main === module) {
  const rules = generate();
  console.log(
    `generated ${accents.length} accent themes with ${rules.length} rules`
  );
}

module.exports = { extractRules, generate, cssForAccent, parseApplyClass };
