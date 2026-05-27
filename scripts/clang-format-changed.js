#!/usr/bin/env node
/*
 * scripts/clang-format-changed.js
 *
 * Run clang-format --dry-run --Werror on .c / .h files that differ from
 * a base ref. Designed for CI: it only inspects files actually touched by
 * the current PR / push, leaving pre-existing formatting issues alone.
 *
 * Usage:
 *   node scripts/clang-format-changed.js [base-ref]
 *
 * Defaults to "origin/develop" if no base ref is given.
 */

const { execFileSync, spawnSync } = require("node:child_process");

const baseRef = process.argv[2] || "origin/develop";

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
  console.log("no changed C/C++ files");
  process.exit(0);
}

console.log(`checking ${files.length} changed file(s) against ${baseRef}`);

const BATCH = 50;
let exitCode = 0;
for (let i = 0; i < files.length; i += BATCH) {
  const batch = files.slice(i, i + BATCH);
  const result = spawnSync(
    "clang-format",
    ["--dry-run", "--Werror", ...batch],
    { stdio: "inherit", shell: true }
  );
  if (result.error) {
    console.error("clang-format not found in PATH. Install LLVM clang-format.");
    process.exit(1);
  }
  if ((result.status ?? 0) !== 0) {
    exitCode = result.status ?? 1;
  }
}

process.exit(exitCode);
