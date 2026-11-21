const tailwindColors = require("tailwindcss/colors");

const NEUTRAL_NAMES = ["neutral"];

/* Tailwind v4.1 neutral palettes, kept for reference. */
const V4_NEUTRALS = {
  taupe: {
    100: [0.96, 0.002, 17.2],
    900: [0.214, 0.009, 43.1],
  },
  mauve: {
    100: [0.96, 0.003, 325.6],
    900: [0.212, 0.019, 322.12],
  },
  mist: {
    100: [0.963, 0.002, 197.1],
    900: [0.218, 0.008, 223.9],
  },
  olive: {
    100: [0.966, 0.005, 106.5],
    900: [0.228, 0.013, 107.4],
  },
};

const ACCENT_ORDER = [
  "neutral",
  "red",
  "orange",
  "amber",
  "yellow",
  "lime",
  "green",
  "emerald",
  "teal",
  "cyan",
  "sky",
  "blue",
  "indigo",
  "violet",
  "purple",
  "fuchsia",
  "pink",
  "rose",
];

function clamp255(value) {
  return Math.max(0, Math.min(255, Math.round(value * 255)));
}

function encodeSrgb(value) {
  if (value <= 0.0031308) {
    return 12.92 * value;
  }
  return 1.055 * Math.pow(value, 1 / 2.4) - 0.055;
}

function oklchToRgb(oklch) {
  const [l, c, h] = oklch;
  const radians = (h * Math.PI) / 180;
  const a = c * Math.cos(radians);
  const b = c * Math.sin(radians);
  const lPrime = l + 0.3963377774 * a + 0.2158037573 * b;
  const mPrime = l - 0.1055613458 * a - 0.0638541728 * b;
  const sPrime = l - 0.0894841775 * a - 1.291485548 * b;
  const l3 = lPrime ** 3;
  const m3 = mPrime ** 3;
  const s3 = sPrime ** 3;
  const r = 4.0767416621 * l3 - 3.3077115913 * m3 + 0.2309699292 * s3;
  const g = -1.2684380046 * l3 + 2.6097574011 * m3 - 0.3413193965 * s3;
  const blue = -0.0041960863 * l3 - 0.7034186147 * m3 + 1.707614701 * s3;
  return {
    r: clamp255(encodeSrgb(r)),
    g: clamp255(encodeSrgb(g)),
    b: clamp255(encodeSrgb(blue)),
  };
}

function parseHex(hex) {
  const value = hex.replace("#", "");
  return {
    r: parseInt(value.slice(0, 2), 16),
    g: parseInt(value.slice(2, 4), 16),
    b: parseInt(value.slice(4, 6), 16),
  };
}

function toHex({ r, g, b }) {
  const part = (value) => value.toString(16).padStart(2, "0");
  return `#${part(r)}${part(g)}${part(b)}`;
}

function colorFromScale(name, scale) {
  if (V4_NEUTRALS[name]) {
    return oklchToRgb(V4_NEUTRALS[name][scale]);
  }
  const palette = tailwindColors[name];
  return parseHex(palette[scale]);
}

function buildAccents() {
  const accents = [];
  for (const name of ACCENT_ORDER) {
    const neutral = NEUTRAL_NAMES.includes(name);
    accents.push({
      name,
      neutral,
      light: colorFromScale(name, neutral ? 900 : 600),
      dark: colorFromScale(name, neutral ? 100 : 400),
    });
  }
  return accents;
}

function channelLuminance(channel) {
  const value = channel / 255;
  return value <= 0.04045
    ? value / 12.92
    : Math.pow((value + 0.055) / 1.055, 2.4);
}

function relativeLuminance({ r, g, b }) {
  return (
    0.2126 * channelLuminance(r) +
    0.7152 * channelLuminance(g) +
    0.0722 * channelLuminance(b)
  );
}

function foregroundFor(color) {
  return relativeLuminance(color) > 0.5
    ? { r: 0x17, g: 0x17, b: 0x17 }
    : { r: 0xfa, g: 0xfa, b: 0xfa };
}

function rgbaString(color, alpha) {
  return `rgba(${color.r}, ${color.g}, ${color.b}, ${alpha})`;
}

function deriveTokens({ light, dark }) {
  const lightForeground = foregroundFor(light);
  const darkForeground = foregroundFor(dark);

  return {
    light: {
      primary: { ...light, alpha: 1 },
      primaryForeground: { ...lightForeground, alpha: 1 },
      accent: { ...light, alpha: 0.1 },
      accentForeground: { ...light, alpha: 1 },
      sidebarPrimary: { ...light, alpha: 1 },
      sidebarPrimaryForeground: { ...lightForeground, alpha: 1 },
      sidebarAccent: { ...light, alpha: 0.1 },
      sidebarAccentForeground: { ...light, alpha: 1 },
    },
    dark: {
      primary: { ...dark, alpha: 1 },
      primaryForeground: { ...darkForeground, alpha: 1 },
      accent: { ...dark, alpha: 0.15 },
      accentForeground: { ...dark, alpha: 1 },
      sidebarPrimary: { ...dark, alpha: 1 },
      sidebarPrimaryForeground: { ...darkForeground, alpha: 1 },
      sidebarAccent: { ...dark, alpha: 0.15 },
      sidebarAccentForeground: { ...dark, alpha: 1 },
    },
  };
}

module.exports = {
  accents: buildAccents(),
  deriveTokens,
  foregroundFor,
  relativeLuminance,
  rgbaString,
  toHex,
  parseHex,
  oklchToRgb,
};
