const DARK_IS = ":is(.dark *)";
const DARK_WHERE = ":where(.dark, .dark *)";

function rewriteSelector(selector) {
  if (!selector.includes(DARK_IS) && !selector.includes(DARK_WHERE)) {
    return selector;
  }
  const normalized = selector.replaceAll(DARK_WHERE, DARK_IS);
  return normalized
    .split(",")
    .map((part) => {
      const trimmed = part.trim();
      if (!trimmed.includes(DARK_IS)) {
        return trimmed;
      }
      const base = trimmed.replaceAll(DARK_IS, "").trim();
      if (!base || base === ".dark" || base.startsWith(".dark ")) {
        return base || ".dark";
      }
      return `.dark ${base}`;
    })
    .join(", ");
}

module.exports = () => ({
  postcssPlugin: "lcui-dark-selectors",
  Rule(rule) {
    const selector = rewriteSelector(rule.selector);
    if (selector !== rule.selector) {
      rule.selector = selector;
    }
  },
});

module.exports.rewriteSelector = rewriteSelector;
module.exports.postcss = true;
