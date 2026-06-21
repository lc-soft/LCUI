import { Widget, type WidgetProps } from "@lcui/react";

/**
 * DemoProvider — wrapper for code-demo widgets.
 *
 * Renders a plain `<widget class="demo">`.  All interactive behaviour
 * lives in `demo-provider.c`, which lcui-cli emits as a one-shot
 * skeleton from this file:
 *
 *   - on mount, pick the first `.demo-languages > .demo-tab` as active,
 *     then the first `.demo-files > .demo-tab[data-language=…]` whose
 *     language matches, then the matching `.code-block` block, and add
 *     the `active` class to each;
 *   - on click within `.demo-languages` or `.demo-files`, update the
 *     active selection and show/hide siblings accordingly.
 *
 * The lcui-cli compiler walks the JSX tree on the call site, so children
 * of `<DemoProvider>…</DemoProvider>` are appended to this widget even
 * though the function body itself does not reference `props.children`.
 * The `WidgetProps` annotation is purely a TypeScript-level declaration
 * so consumers can nest tabs/code-blocks (and pass any standard widget
 * attribute) inside this provider; at runtime lcui-cli ignores
 * `children` here.
 *
 * No other props are passed: the default selection is derived from the
 * children's source order at runtime.  This avoids relying on lcui-cli's
 * (currently unverified) propagation of custom-component props.
 *
 * Consumers (each `app/examples/<name>-tsx/index.tsx`) wrap their tabs,
 * code blocks, and preview placeholder in this provider:
 *
 *   <DemoProvider>
 *     <Widget $ref="preview" className="demo-preview" />
 *     <Widget className="demo-languages"> ... .demo-tab[data-value] ... </Widget>
 *     <Widget className="demo-files"> ... .demo-tab[data-language][data-value] ... </Widget>
 *     <Widget className="demo-code-blocks"> ... .code-block[data-language][data-file] ... </Widget>
 *   </DemoProvider>
 */
export default function DemoProvider(_props: WidgetProps) {
  return <Widget className="demo" />;
}
