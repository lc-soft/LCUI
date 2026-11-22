#!/usr/bin/env node
// scripts/sync-versions.mjs
//
// Single source of truth for package versions is versions.json in the
// repository root.  This script keeps the values in build descriptors and in
// the generated public headers in sync with that file.
//
//   node scripts/sync-versions.mjs --check   verify, exit 1 on mismatch
//   node scripts/sync-versions.mjs --write   rewrite descriptors and headers
//
// The script deliberately does not decide version bumps.  Release automation
// updates versions.json first, then runs --write and commits the result.

import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const root = join(dirname(fileURLToPath(import.meta.url)), "..");
const versionsFile = join(root, "versions.json");
const versions = JSON.parse(readFileSync(versionsFile, "utf8"));

// package name in versions.json -> xmake descriptor
const TARGET_FILES = [
        ["lcui", "xmake.lua"],
        ["yutil", "lib/yutil/xmake.lua"],
        ["pandagl", "lib/pandagl/xmake.lua"],
        ["css", "lib/css/xmake.lua"],
        ["i18n", "lib/i18n/xmake.lua"],
        ["ptk", "lib/ptk/xmake.lua"],
        ["thread", "lib/thread/xmake.lua"],
        ["router", "lib/router/xmake.lua"],
        ["ui", "lib/ui/xmake.lua"],
        ["ui-server", "lib/ui-server/xmake.lua"],
        ["ui-xml", "lib/ui-xml/xmake.lua"],
        ["ctest", "lib/ctest/xmake.lua"],
];

// generated public headers that are tracked by git
const HEADERS = [
        ["lib/i18n/include/i18n.h", "i18n", "LIBI18N"],
        ["lib/thread/include/thread.h", "thread", "LIBTHREAD"],
        ["lib/router/include/router/version.h", "router", "LIBROUTER"],
];

function parseVersion(version) {
        const match = String(version).match(/^(\d+)\.(\d+)\.(\d+)/);
        if (!match) {
                throw new Error(`not a semantic version: ${version}`);
        }
        return {
                major: Number(match[1]),
                minor: Number(match[2]),
                patch: Number(match[3]),
        };
}

/* Keep the working-tree line endings stable so --check does not report every
 * CRLF checkout as out of sync. */
function normalizeEol(text) {
        return text.replace(/\r\n/g, "\n");
}

function restoreEol(text, eol) {
        return eol === "\r\n" ? text.replace(/\n/g, "\r\n") : text;
}

function syncXmake(text, version) {
        const lines = text.split("\n");
        const existing = lines.findIndex((line) =>
                /^\s*set_version\("[^"]*"\)/.test(line)
        );
        if (existing >= 0) {
                const indent = lines[existing].match(/^\s*/)[0];
                lines[existing] = `${indent}set_version("${version}")`;
                return lines.join("\n");
        }
        const target = lines.findIndex((line) => /^\s*target\("/.test(line));
        if (target < 0) {
                throw new Error("no target() declaration found");
        }
        lines.splice(target + 1, 0, `    set_version("${version}")`);
        return lines.join("\n");
}

function syncHeader(text, macro, version) {
        const { major, minor, patch } = parseVersion(version);
        const fields = [
                [`${macro}_VERSION`, `"${version}"`],
                [`${macro}_VERSION_MAJOR`, String(major)],
                [`${macro}_VERSION_MINOR`, String(minor)],
                [`${macro}_VERSION_ALTER`, String(patch)],
        ];
        for (const [name, value] of fields) {
                const pattern = new RegExp(`^(#define\\s+${name})\\s+.*$`, "m");
                if (!pattern.test(text)) {
                        throw new Error(`missing ${name}`);
                }
                text = text.replace(pattern, `$1 ${value}`);
        }
        return text;
}

function main() {
        const write = process.argv.includes("--write");
        const check = process.argv.includes("--check") || !write;
        const problems = [];
        let changed = 0;

        for (const [pkg, file] of TARGET_FILES) {
                const version = versions[pkg];
                if (!version) {
                        problems.push(`versions.json: missing "${pkg}"`);
                        continue;
                }
                const path = join(root, file);
                const before = readFileSync(path, "utf8");
                const eol = before.includes("\r\n") ? "\r\n" : "\n";
                const after = restoreEol(
                        syncXmake(normalizeEol(before), version),
                        eol
                );
                if (before !== after) {
                        changed++;
                        problems.push(
                                `${file}: expected set_version("${version}")`
                        );
                        if (write) {
                                writeFileSync(path, after);
                        }
                }
        }

        for (const [file, pkg, macro] of HEADERS) {
                const version = versions[pkg];
                const path = join(root, file);
                const before = readFileSync(path, "utf8");
                const eol = before.includes("\r\n") ? "\r\n" : "\n";
                const after = restoreEol(
                        syncHeader(normalizeEol(before), macro, version),
                        eol
                );
                if (before !== after) {
                        changed++;
                        problems.push(`${file}: expected ${macro} ${version}`);
                        if (write) {
                                writeFileSync(path, after);
                        }
                }
        }

        const packageJson = JSON.parse(
                readFileSync(join(root, "package.json"), "utf8")
        );
        if (packageJson.version !== versions.lcui) {
                changed++;
                problems.push(
                        `package.json: expected version ${versions.lcui}, found ${packageJson.version}`
                );
                if (write) {
                        packageJson.version = versions.lcui;
                        writeFileSync(
                                join(root, "package.json"),
                                `${JSON.stringify(packageJson, null, 2)}\n`
                        );
                }
        }

        if (changed === 0) {
                console.log("versions are in sync");
                return 0;
        }
        if (write) {
                console.log(`updated ${changed} file(s)`);
                return 0;
        }
        if (check) {
                for (const problem of problems) {
                        console.error(problem);
                }
                console.error(`\n${changed} file(s) out of sync with versions.json`);
                return 1;
        }
        return 0;
}

process.exit(main());
