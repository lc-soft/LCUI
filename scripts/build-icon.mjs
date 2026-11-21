import { existsSync } from "node:fs";
import { execFileSync } from "node:child_process";

const sizes = [16, 24, 32, 48, 64, 128, 256];

for (const size of sizes) {
  const input =
    size === 256
      ? "resources/lcui-icon.png"
      : `resources/lcui-icon-${size}.png`;
  const output = `src/resources/lcui-icon-${size}.png.inc`;

  if (!existsSync(input)) {
    console.error(`missing icon source: ${input}`);
    process.exit(1);
  }

  execFileSync(
    "xmake",
    [
      "l",
      "cli.binutils.bin2c",
      "-i",
      input,
      "-o",
      output,
      "--nozeroend",
    ],
    { stdio: "inherit" }
  );
}
