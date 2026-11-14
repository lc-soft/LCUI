const { colors, boxShadow } = require("./scripts/theme-colors");

/** @type {import('tailwindcss').Config} */
module.exports = {
  darkMode: "class",
  content: ["./src/**/*.{c,h,css}"],
  theme: {
    extend: {
      colors,
      boxShadow,
    },
  },
  plugins: [
    require("./scripts/tailwind-shadow-plugin")({ shadows: boxShadow }),
  ],
  corePlugins: [
    "display",
    "flexDirection",
    "flex",
    "alignItems",
    "justifyContent",
    "width",
    "height",
    "padding",
    "margin",
    "gap",
    "borderRadius",
    "borderWidth",
    "borderColor",
    "backgroundColor",
    "textColor",
    "fontSize",
    "fontWeight",
    "lineHeight",
    "textAlign",
    "position",
    "inset",
    "pointerEvents",
    "textDecoration",
    "textUnderlineOffset",
    "opacity",
  ],
};
