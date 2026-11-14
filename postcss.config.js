const formatLcuiCss = {
  postcssPlugin: "format-lcui-css",
  OnceExit(root) {
    root.raws.after = "\n";
    root.each((node) => {
      node.raws.before = "\n";
      node.raws.after = "\n";
    });
  },
};

module.exports = {
  plugins: [
    require("tailwindcss"),
    require("@thedutchcoder/postcss-rem-to-px"),
    require("./scripts/postcss-lcui-dark"),
    formatLcuiCss,
  ],
};