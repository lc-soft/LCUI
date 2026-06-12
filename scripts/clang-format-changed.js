#!/usr/bin/env node
/*
 * scripts/clang-format-changed.js
 *
 * Run clang-format --dry-run --Werror on .c / .h files that differ from
 * a base ref. Designed for CI: it only inspects files actually touched by
 * the current PR / push, leaving pre-existing formatting issues alone.
 *
 * The clang-format binary is resolved from node_modules/.bin so that the
 * project-local version (installed from the `clang-format-node` devDependency)
 * is always used, regardless of system PATH.
 *
 * Usage:
 *   node scripts/clang-format-changed.js [base-ref]
 *
 * Defaults to "origin/develop" if no base ref is given.
 */

const fs = require("node:fs");
const path = require("node:path");
const { execFileSync, spawnSync } = require("node:child_process");

const baseRef = process.argv[2] || "origin/develop";

function resolveClangFormat() {
  const candidates = [
    path.join("node_modules", ".bin", "clang-format"),
    path.join("node_modules", ".bin", "clang-format.cmd"),
  ];
  for (const c of candidates) {
    if (fs.existsSync(c)) {
      return c;
    }
  }
  return null;
}

const clangFormat = resolveClangFormat();
if (!clangFormat) {
  console.error(
    "clang-format not found in node_modules/.bin. Run: npm install"
  );
  process.exit(1);
}

let version;
try {
  version = execFileSync(clangFormat, ["--version"], {
    encoding: "utf8",
    shell: true,
  }).trim();
} catch (err) {
  console.error(`failed to run ${clangFormat} --version:`, err.message);
  process.exit(1);
}

let files;
try {
  const out = execFileSync(
    "git",
    ["diff", "--name-only", "--diff-filter=ACMR", `${baseRef}...HEAD`],
    { encoding: "utf8" }
  );
  files = out
    .split(/\r?\n/)
    .filter((f) => /\.(c|h)$/i.test(f))
    .filter(Boolean);
} catch (err) {
  console.error(`failed to diff against ${baseRef}:`, err.message);
  process.exit(1);
}

if (files.length === 0) {
  console.log(`clang-format: ${version}`);
  console.log(`clang-format binary: ${clangFormat}`);
  console.log("no changed C/C++ files");
  process.exit(0);
}

console.log(`clang-format: ${version}`);
console.log(`clang-format binary: ${clangFormat}`);
console.log(`checking ${files.length} changed file(s) against ${baseRef}`);

const BATCH = 50;
let exitCode = 0;
for (let i = 0; i < files.length; i += BATCH) {
  const batch = files.slice(i, i + BATCH);
  const result = spawnSync(
    clangFormat,
    ["--dry-run", "--Werror", ...batch],
    { stdio: "inherit", shell: true }
  );
  if (result.error) {
    console.error(
      `failed to execute ${clangFormat}:`,
      result.error.message
    );
    process.exit(1);
  }
  if ((result.status ?? 0) !== 0) {
    exitCode = result.status ?? 1;
  }
}

process.exit(exitCode);
