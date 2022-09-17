const fs = require("fs");
const path = require("path");

module.exports = {
  extends: ["@commitlint/config-conventional"],
  rules: {
    "header-min-length": [2, "always", 1],
    "header-max-length": [2, "always", 72],
    "subject-case": [0],
    "scope-enum": [
      2,
      "always",
      [...fs.readdirSync(path.resolve(__dirname, "lib")), "docs"],
    ],
  },
};
