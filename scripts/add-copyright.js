const fs = require("fs");
const path = require("path");

const bom = "\ufeff";
const cwd = process.cwd();
const project = path.basename(cwd);
const includes = ["lib", "include", "src"].map((file) =>
  path.resolve(cwd, file)
);
const excludes = ["lib/yutil"].map((file) => path.resolve(cwd, file));
const extensions = [".c", ".h"];
const copyrightHolder = "Liu Chao <i@lc-soft.io>";
const copyright = `/*
 * {{filePath}}{{summary}}
 *
 * {{copyright}}
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ${project}, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */
`;

function isCurrentHolder(holder) {
  const name = holder.split(" <")[0].toLowerCase();
  return copyrightHolder.toLowerCase().startsWith(name);
}

function parseCopyrightComment(content) {
  const result = { comment: "", exists: false, holders: [] };
  if (!content.startsWith("/*")) {
    return result;
  }
  let index = -1;
  const hasComment = [" */", "#", "\n\n"].some((prefix) => {
    index = content.indexOf(prefix);
    if (index > 0) {
      index += prefix.length;
      return true;
    }
    return false;
  });

  if (!hasComment) {
    return result;
  }
  result.comment = content.substring(0, index);
  result.comment.split("\n").forEach((line) => {
    if (!line.includes("Copyright")) {
      return;
    }
    const [year, holder] = line
      .substring(3)
      .replace("Copyright (c)", "")
      .replace("All rights reserved.", "")
      .split(", ")
      .map((str) => str.trim());
    result.holders.push({ year, holder });
    result.exists = true;
  });
  return result;
}

function parseFile(filePath) {
  let content = fs.readFileSync(filePath, "utf8");
  if (content[0] === bom) {
    content = content.slice(1);
  }
  const copyright = parseCopyrightComment(content);
  if (
    !copyright.exists ||
    !copyright.holders.some(({ holder }) => isCurrentHolder(holder))
  ) {
    copyright.holders.push({ year: null, holder: copyrightHolder });
  }
  const result = {
    summary: "",
    createdAt: fs.statSync(filePath).ctime,
    copyright,
    content,
  };
  if (copyright.exists) {
    copyright.comment
      .split("\n")
      .slice(0, 3)
      .some((line) => {
        if (line.includes("Copyright")) {
          return true;
        }
        line = line.substring(3);
        if (line) {
          index = line.indexOf(" ");
          if (index > 0) {
            result.summary = line.substring(index + 1);
          }
        }
        return false;
      });
    result.content = result.content.slice(copyright.comment.length);
  }
  result.content = result.content.trimStart();
  return result;
}

function generateCopyright(holders, fileCreatedAt) {
  const currentYear = new Date().getFullYear();
  const lines = holders.map(({ year, holder }) => {
    let copyrightYear = year;

    if (isCurrentHolder(holder)) {
      if (!year) {
        copyrightYear = fileCreatedAt.getFullYear();
      }
      const startYear = `${copyrightYear}`.split('-')[0];
      if (startYear !== `${currentYear}`) {
        copyrightYear = `${startYear}-${currentYear}`;
      }
    }
    return `Copyright (c) ${copyrightYear}, ${holder}`;
  });
  const result = lines.join("\n * ");
  return `${
    lines.length > 1 ? `${result}\n * ` : `${result} `
  }All rights reserved.`;
}

function insertCopyright(filePath) {
  const result = parseFile(filePath);
  const header = copyright
    .replace("{{filePath}}", path.relative(cwd, filePath).replace(/\\/g, "/"))
    .replace("{{summary}}", result.summary ? `: ${result.summary}` : "")
    .replace(
      "{{copyright}}",
      generateCopyright(result.copyright.holders, result.createdAt)
    );

  fs.writeFileSync(filePath, `${bom}${header}\n${result.content}`, "utf-8");
}

function traverseFolder(folderPath) {
  fs.readdirSync(folderPath).forEach((file) => {
    const filePath = path.join(folderPath, file);
    const stats = fs.statSync(filePath);
    if (stats.isDirectory()) {
      traverseFolder(filePath);
      return;
    }
    if (
      excludes.some((item) => filePath.startsWith(item)) ||
      !includes.some((item) => filePath.startsWith(item))
    ) {
      return;
    }
    if (extensions.includes(path.extname(filePath))) {
      console.log(filePath);
      insertCopyright(filePath);
    }
  });
}

traverseFolder(cwd);
