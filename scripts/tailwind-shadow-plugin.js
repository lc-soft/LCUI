const plugin = require("tailwindcss/plugin");

/**
 * Box-shadow utilities that inline the configured shadow value.
 *
 * Tailwind's built-in `box-shadow` plugin emits `--tw-shadow` /
 * `--tw-shadow-colored` custom properties and a `var(...)` chain. LCUI's CSS
 * parser has no custom-property support, so those declarations are ignored and
 * the utilities would silently do nothing. This plugin keeps the familiar
 * `shadow-<name>` class names while writing the literal value instead.
 *
 * Pass the shadow map explicitly so only the values this project defines are
 * emitted; otherwise Tailwind's default theme would also expose `shadow`,
 * `shadow-xl`, `shadow-inner` and friends.
 *
 * Only utilities that actually appear in the scanned content are emitted,
 * because Tailwind still applies its candidate-based tree shaking.
 */
module.exports = plugin.withOptions((options = {}) => {
  return ({ addUtilities, theme }) => {
    const shadows = options.shadows || theme("boxShadow") || {};
    const utilities = {};

    for (const [name, value] of Object.entries(shadows)) {
      if (typeof value !== "string" || value.length === 0) {
        continue;
      }
      utilities[name === "DEFAULT" ? ".shadow" : `.shadow-${name}`] = {
        boxShadow: value,
      };
    }
    addUtilities(utilities);
  };
});
