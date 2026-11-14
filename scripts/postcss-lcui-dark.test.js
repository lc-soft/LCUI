const assert = require("node:assert/strict");
const test = require("node:test");
const postcss = require("postcss");
const plugin = require("./postcss-lcui-dark");

async function transform(css) {
  const result = await postcss([plugin()]).process(css, { from: undefined });
  return result.css;
}

test("rewrites :is dark selectors to a descendant selector", async () => {
  assert.equal(
    await transform(".foo:is(.dark *) { color: red }"),
    ".dark .foo { color: red }",
  );
});

test("rewrites comma-separated selectors independently", async () => {
  assert.equal(
    await transform(".a, .b:where(.dark, .dark *) { color: red }"),
    ".a, .dark .b { color: red }",
  );
});

test("preserves escaped tailwind class names", async () => {
  assert.equal(
    await transform(".dark\\:bg-card:is(.dark *) { color: red }"),
    ".dark .dark\\:bg-card { color: red }",
  );
});

test("rewrites pseudo-class variants", async () => {
  assert.equal(
    await transform(".button:hover:is(.dark *) { color: red }"),
    ".dark .button:hover { color: red }",
  );
});
