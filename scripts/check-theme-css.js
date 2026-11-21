const fs = require("node:fs");
const path = require("node:path");

const root = path.resolve(__dirname, "..");
const sourceFiles = [
  "src/styles/style-nova.css",
  "examples/doc-viewer/app/global.css",
  "examples/doc-viewer/app/components/navbar.css",
];
const generatedFiles = [
  "src/styles/style-nova.css.h",
  "examples/doc-viewer/app/global.css.h",
  "examples/doc-viewer/app/components/navbar.css.h",
];

const colorPattern = /#[0-9a-fA-F]{3,8}\b|rgba?\(/g;
let failed = false;

for (const relative of sourceFiles) {
  const file = path.join(root, relative);
  if (!fs.existsSync(file)) {
    continue;
  }
  const lines = fs.readFileSync(file, "utf8").split(/\r?\n/);
  lines.forEach((line, index) => {
    if (colorPattern.test(line)) {
      failed = true;
      console.error(`${relative}:${index + 1}: raw color literal: ${line.trim()}`);
    }
    colorPattern.lastIndex = 0;
  });
}

for (const relative of generatedFiles) {
  const file = path.join(root, relative);
  if (!fs.existsSync(file)) {
    continue;
  }
  const text = fs.readFileSync(file, "utf8");
  if (text.includes(":is(.dark") || text.includes(":where(.dark")) {
    failed = true;
    console.error(`${relative}: contains an unsupported dark selector`);
  }
}

if (failed) {
  process.exitCode = 1;
} else {
  console.log("theme css checks passed");
}
