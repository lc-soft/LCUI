#!/usr/bin/env node
// scripts/release-version.mjs
//
// npm "version" lifecycle script.  npm has already written the new version to
// package.json when this runs; this script syncs versions.json, the xmake
// descriptors, generated headers and the CI artifact version, then prepends
// the commit summary from conventional-changelog to CHANGELOG.md.  npm creates
// the release commit and tag after this script exits.
//
//   npm version 3.0.0-alpha.1

import { spawnSync } from "node:child_process";
import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const root = join(dirname(fileURLToPath(import.meta.url)), "..");
const SEMVER = /^\d+\.\d+\.\d+(?:-[0-9A-Za-z.-]+)?(?:\+[0-9A-Za-z.-]+)?$/;
const WORKFLOW = ".github/workflows/ccpp.yml";
const synced = [];

function quote(value) {
        return `"${value.replace(/"/g, '\\"')}"`;
}

function filePath(relative) {
        return join(root, relative);
}

function read(relative) {
        return readFileSync(filePath(relative), "utf8");
}

function write(relative, text) {
        writeFileSync(filePath(relative), text);
}

function run(command) {
        const result = spawnSync(command, {
                cwd: root,
                shell: true,
                stdio: "inherit",
        });

        if (result.error || result.status !== 0) {
                throw new Error(`command failed: ${command}`);
        }
}

function parseVersion(version) {
        const match = version.match(/^(\d+)\.(\d+)\.(\d+)/);

        if (!match) {
                throw new Error(`not a semantic version: ${version}`);
        }
        return {
                major: Number(match[1]),
                minor: Number(match[2]),
                patch: Number(match[3]),
        };
}

function topEntryVersion(text) {
        const line = text.split("\n").find((row) => /^# \[/.test(row));

        if (!line) {
                return null;
        }
        return (line.match(/^# \[([^\]]+)\]/) || [])[1];
}

function syncVersions(version, versions, versionsText) {
        const eol = versionsText.includes("\r\n") ? "\r\n" : "\n";

        versions.lcui = version;
        write(
                "versions.json",
                `${JSON.stringify(versions, null, 2).replace(/\n/g, eol)}${eol}`
        );
        synced.push("versions.json");
        run("node scripts/sync-versions.mjs --write");
        synced.push(
                "xmake.lua",
                "lib/i18n/include/i18n.h",
                "lib/thread/include/thread.h",
                "lib/router/include/router/version.h"
        );
        for (const name of Object.keys(versions)) {
                if (name !== "lcui") {
                        synced.push(`lib/${name}/xmake.lua`);
                }
        }
}

function syncWorkflow(version) {
        const { major, minor, patch } = parseVersion(version);
        const before = read(WORKFLOW);
        const after = before
                .replace(/^(\s*TARGET_MAJOR:\s*).*$/m, `$1${major}`)
                .replace(
                        /^(\s*TARGET_VERSION:\s*).*$/m,
                        `$1${major}.${minor}.${patch}`
                );

        if (after !== before) {
                write(WORKFLOW, after);
        }
        synced.push(WORKFLOW);
}

function generateChangelog(version) {
        run("conventional-changelog -p angular -i CHANGELOG.md -s");
        synced.push("CHANGELOG.md");
        const entryVersion = topEntryVersion(read("CHANGELOG.md"));

        if (entryVersion !== version) {
                throw new Error(
                        `CHANGELOG.md should start with "${version}", found ` +
                                `"${entryVersion || "no entry"}"`
                );
        }
}

function restoreSyncedFiles() {
        const files = [...synced, "package.json"];

        spawnSync(`git checkout -- ${files.map(quote).join(" ")}`, {
                cwd: root,
                shell: true,
                stdio: "inherit",
        });
}

function main() {
        const version = JSON.parse(read("package.json")).version;

        if (!SEMVER.test(version)) {
                throw new Error(`not a semantic version: ${version}`);
        }
        const versionsText = read("versions.json");
        const versions = JSON.parse(versionsText);

        if (versions.lcui === version) {
                throw new Error(`versions.json is already at ${version}`);
        }
        syncVersions(version, versions, versionsText);
        syncWorkflow(version);
        generateChangelog(version);
        run(`git add ${synced.map(quote).join(" ")}`);
        console.log(`release-version: ${version} prepared`);
}

try {
        main();
} catch (error) {
        restoreSyncedFiles();
        console.error(`release-version: ${error.message}`);
        console.error(
                "release-version: restored synced files and package.json"
        );
        process.exit(1);
}
