# coding=utf8
import re

# Assumes last entry is the version we want to release
def release_text(filename):
  with open(filename, "r") as rel_file:
    # First line will be the header and padding, we don't need it
    lines = rel_file.read().split("\n")[2:]
    for i, row in enumerate(lines):
      if row.startswith("# ["):
        return "\n".join(lines[0:i])
      
with open("RELEASE_NOTES.md", "w") as rel_notes:
  content = [
    "# Changelog",
    release_text("CHANGELOG.md"),
    "# 更新日志",
    release_text("CHANGELOG.zh-cn.md")
  ]
  rel_notes.write("\n".join(content))