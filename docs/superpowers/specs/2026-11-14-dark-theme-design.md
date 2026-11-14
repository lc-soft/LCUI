# Dark Theme Design

- Status: proposed
- Date: 2026-11-14
- Scope: LCUI style-nova, doc-viewer app, demos, generated code blocks

## Goal

Add a complete dark theme for the project, with shadcn neutral as the dark
palette and GitHub Dark for syntax highlighting. Remove raw color literals from
theme-bearing CSS and express colors through Tailwind color utilities.

The existing `app/theme.c` persistence and `.dark` class toggle stay as the
theme switch mechanism.

## Constraints

- LCUI CSS does not support `var()`, `:is()`, `:where()`, media queries, or
  attribute selectors.
- Tailwind 3.4 dark variants emit `:is(.dark *)` / `:where(.dark, .dark *)`.
  These selectors must be rewritten before LCUI consumes the CSS.
- The doc-viewer code blocks use compiler-generated `[color=#hex]` tags, so
  syntax colors cannot be switched by CSS alone.

## Architecture

### 1. Shared semantic tokens

Create a shared CommonJS token module consumed by both Tailwind configs:

- `tailwind.config.js` (style-nova)
- `examples/doc-viewer/tailwind.config.js`

Each semantic token has a light value and a `-dark` variant. Tailwind classes
therefore use pairs such as:

```text
bg-background dark:bg-background-dark
text-foreground dark:text-foreground-dark
border-border dark:border-border-dark
```

Set `darkMode: "class"` in both configs.

### 2. Dark selector rewrite

Add a PostCSS plugin, `scripts/postcss-lcui-dark.js`, and register it after
Tailwind in both PostCSS configs. It rewrites:

```css
.foo:is(.dark *)
.foo:where(.dark, .dark *)
```

into:

```css
.dark .foo
```

It must handle comma-separated selector lists and preserve the rest of the
selector, including pseudo-classes and escaped Tailwind class names.

A small Node check validates that generated CSS contains no remaining
`:is(.dark` or `:where(.dark`.

### 3. Color tokens

#### Core

| Token | Light | Dark |
|---|---|---|
| `background` | `#ffffff` | `#0a0a0a` |
| `foreground` | `#0a0a0a` | `#fafafa` |
| `card` | `#ffffff` | `#171717` |
| `card-foreground` | `#0a0a0a` | `#fafafa` |
| `popover` | `#ffffff` | `#171717` |
| `popover-foreground` | `#0a0a0a` | `#fafafa` |
| `primary` | `#171717` | `#fafafa` |
| `primary-foreground` | `#fafafa` | `#171717` |
| `secondary` | `#f5f5f5` | `#262626` |
| `secondary-foreground` | `#171717` | `#fafafa` |
| `muted` | `#f5f5f5` | `#262626` |
| `muted-foreground` | `#737373` | `#a1a1a1` |
| `accent` | `#f5f5f5` | `#262626` |
| `accent-foreground` | `#171717` | `#fafafa` |
| `destructive` | `#e7000b` | `#ff6467` |
| `destructive-muted` | `#ffe8e9` | `#4c0519` |
| `border` | `#e5e5e5` | `rgba(255,255,255,0.1)` |
| `input` | `#e5e5e5` | `rgba(255,255,255,0.15)` |
| `ring` | `#a1a1a1` | `#d4d4d4` |

#### Sidebar

| Token | Light | Dark |
|---|---|---|
| `sidebar` | `#fafafa` | `#171717` |
| `sidebar-foreground` | `#0a0a0a` | `#fafafa` |
| `sidebar-primary` | `#171717` | `#fafafa` |
| `sidebar-primary-foreground` | `#fafafa` | `#171717` |
| `sidebar-accent` | `#f5f5f5` | `#262626` |
| `sidebar-accent-foreground` | `#171717` | `#fafafa` |
| `sidebar-border` | `#e5e5e5` | `rgba(255,255,255,0.1)` |

#### Status and code

| Token | Light | Dark |
|---|---|---|
| `success` | `#16a34a` | `#4ade80` |
| `warning` | `#f5b301` | `#facc15` |
| `code` | `#f7f7f7` | `#171717` |
| `code-foreground` | `#1f2328` | `#d4d4d4` |
| `code-header` | `#f5f5f5` | `#1f1f1f` |
| `code-border` | `#e5e5e5` | `rgba(255, 255, 255, 0.1)` |

The dark code surface uses the same neutral surface as `card` / `sidebar`
instead of GitHub's blue-tinted `#0d1117`, so code blocks sit on the page
background without a cold cast. Only the syntax palette keeps GitHub Dark
colors; the container itself stays neutral.

#### Admonitions

| Token | Light | Dark |
|---|---|---|
| `admonition-note` | `#3b82f6` | `#58a6ff` |
| `admonition-note-foreground` | `#1d4ed8` | `#79c0ff` |
| `admonition-info` | `#06b6d4` | `#39c5cf` |
| `admonition-info-foreground` | `#0891b2` | `#56d4dd` |
| `admonition-tip` | `#10b981` | `#56d364` |
| `admonition-tip-foreground` | `#047857` | `#7ee787` |
| `admonition-caution` | `#eab308` | `#e3b341` |
| `admonition-caution-foreground` | `#ca8a04` | `#f2cc60` |
| `admonition-warning` | `#ef4444` | `#f85149` |
| `admonition-warning-foreground` | `#b91c1c` | `#ff7b72` |

#### Demo and effects

| Token | Light | Dark |
|---|---|---|
| `overlay` | `rgba(0,0,0,0.35)` | `rgba(0,0,0,0.55)` |
| `card-image-cover` | `#94a3b8` | `#475569` |
| `card-image-overlay` | `rgba(0,0,0,0.35)` | `rgba(0,0,0,0.55)` |

Add Tailwind `boxShadow` utilities for the existing card, popover, menu,
dialog, toast, and select shadows. Use `shadow-*` instead of raw
`box-shadow` color literals where the geometry matches.

### 4. CSS migration

Replace raw `#hex` / `rgb()` / `rgba()` color declarations in:

- `src/styles/style-nova.css`
- `examples/doc-viewer/app/global.css`
- `examples/doc-viewer/app/components/navbar.css`

with semantic Tailwind utilities and paired dark variants. Examples:

```css
item {
  @apply bg-card text-card-foreground border-border;
  @apply dark:bg-card-dark dark:text-card-foreground-dark dark:border-border-dark;
}
```

Hardcoded colors in demo-only decorations are also mapped to tokens. Syntax
highlighting is the only exception because it is generated text, handled below.

### 5. Dual syntax highlighting

`examples/doc-viewer/scripts/compiler/style-tag-renderer.ts` gets two palettes:

- `LIGHT`: existing GitHub Light colors
- `DARK`: GitHub Dark colors

Dark palette:

```text
keyword     #ff7b72
builtin     #ffa657
literal     #79c0ff
string      #a5d6ff
comment     #8b949e
title       #d2a8ff
tag         #7ee787
punctuation #c9d1d9
```

`example-compiler.ts` and `page-generator.ts` emit both `lines` and
`darkLines` for every code block. The C page generator emits two line
containers; CSS shows the light container by default and the dark container
under `.dark`.

This intentionally doubles generated code-line strings/widgets for code
blocks; other page content is not duplicated.

## Migration order

1. Add shared tokens, `darkMode: "class"`, PostCSS plugin, and selector check.
2. Migrate `style-nova.css` to tokens and dark variants.
3. Migrate doc-viewer `global.css`, `navbar.css`, Home cards, docs, demos.
4. Add dual syntax highlighting to the compiler and code block renderer.
5. Add theme tests and run the full verification matrix.

## Verification

- Token lint: no raw `#hex` / `rgb()` / `rgba()` in theme-bearing CSS.
- PostCSS check: generated CSS contains no `:is(.dark` or `:where(.dark`.
- C theme tests: toggle `.dark`, check computed colors for button, card,
  badge, input, dropdown/select, sidebar, dialog, toast.
- Screenshot matrix: Home, Docs, Widgets, dialog, dropdown in light and dark.
- `npm run build-styles`, `xmake build doc-viewer`, full `xmake test`.
- Verify code blocks have readable light and dark syntax colors.

## Non-goals

- Theme-aware images or charts.
- Animated theme transitions.
- User-defined palettes.
- Reworking layout or component structure.
