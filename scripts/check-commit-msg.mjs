#!/usr/bin/env node
// scripts/check-commit-msg.mjs
//
// Commit-msg hook.  The scope is optional; when present it must be a
// directory name under lib/, or "styles"/"widgets".  Merge, revert, fixup,
// squash and amend commits are always accepted.

import { readdirSync, readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const root = join(dirname(fileURLToPath(import.meta.url)), "..");
const SCOPES = readdirSync(join(root, "lib"), { withFileTypes: true })
        .filter((entry) => entry.isDirectory())
        .map((entry) => entry.name)
        .concat(["styles", "widgets", "release"])
        .sort();

const file = process.argv[2];

if (!file) {
        console.error(
                "usage: node scripts/check-commit-msg.mjs <commit-msg-file>"
        );
        process.exit(2);
}

const subject = readFileSync(file, "utf8")
        .replace(/\r\n/g, "\n")
        .split("\n")[0]
        .trim();

if (
        !subject ||
        /^(Merge |Revert "|fixup! |squash! |amend! )/.test(subject)
) {
        process.exit(0);
}

const match = subject.match(/^[a-z]+\(([^)]+)\)/);

if (!match) {
        process.exit(0);
}

const invalid = match[1]
        .split(",")
        .map((scope) => scope.trim())
        .filter((scope) => !SCOPES.includes(scope));

if (invalid.length === 0) {
        process.exit(0);
}

console.error(`Invalid commit scope: ${invalid.join(", ")}`);
console.error("");
console.error(`Allowed scopes: ${SCOPES.join(", ")}`);
console.error("");
console.error(
        'Scope can be omitted; otherwise use a lib/ directory name, "styles" or "widgets".'
);
process.exit(1);
