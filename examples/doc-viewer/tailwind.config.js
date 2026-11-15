const { createConfig } = require("@lcui/tailwind-config");

/** @type {import('tailwindcss').Config} */
module.exports = createConfig({
  content: [
    "./app/**/*.{tsx,html}",
    "../../docs/examples/**/*-tsx/example.tsx",
  ],
});
