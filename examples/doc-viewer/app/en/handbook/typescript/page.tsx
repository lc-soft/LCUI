import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TypescriptDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TypeScript &amp; TSX Development</h1>
      <p className="doc-page-desc">{`LCUI supports a React-like development experience using TypeScript and JSX syntax, compiled by [bgcolor=#eee] @lcui/cli [/bgcolor].`}</p>
      <Widget className="doc-section">
        <h2>Setup</h2>
        <p>{`Install the CLI:`}</p>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/typescript/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install -g @lcui/cli`}</Text>
          </Widget>
        </Widget>
        <p>{`Create a new project:`}</p>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/typescript/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`lcui create my-app`}</Text>
            <Text className="code-line">{`[color=#953800]cd[/color] my-app`}</Text>
          </Widget>
        </Widget>
        <p>{`The generated project includes:`}</p>
        <ul>
          <li><Text>{`[bgcolor=#eee] src/app.tsx [/bgcolor] — Root component`}</Text></li>
          <li><Text>{`[bgcolor=#eee] xmake.lua [/bgcolor] — Build configuration`}</Text></li>
          <li><Text>{`[bgcolor=#eee] package.json [/bgcolor] — Node dependencies`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Writing components</h2>
        <p>{`Components are plain TypeScript functions that return JSX:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/typescript/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]interface[/color] [color=#8250df]GreetingProps[/color] {`}</Text>
            <Text className="code-line">{`  [color=#0550ae]name[/color]: [color=#953800]string[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]function[/color] [color=#8250df]Greeting[/color]([color=#0550ae]{ name }: [color=#8250df]GreetingProps[/color][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]padding:[/color] [color=#0550ae]16[/color] }}>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Text[/color]>[/color]Hello, {name}![color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] [color=#116329]<[color=#0550ae]Greeting[/color] [color=#0550ae]name[/color]=[color=#0a3069]"World"[/color] />[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>State management</h2>
        <p>{`Use the [bgcolor=#eee] useState [/bgcolor] hook:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/typescript/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color], [color=#8250df]Text[/color], useState } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Counter[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[count, setCount] = [color=#8250df]useState[/color]([color=#0550ae]0[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Text[/color]>[/color]Count: {count}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]onClick[/color]=[color=#0a3069]{()[/color] =>[/color] setCount(count + 1)}>Increment[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Compiling TSX to C</h2>
        <p>{`The CLI compiles TSX to a C header at build time:`}</p>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/typescript/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`lcui build`}</Text>
          </Widget>
        </Widget>
        <p>{`This generates [bgcolor=#eee] ui.h [/bgcolor] from [bgcolor=#eee] ui.tsx [/bgcolor], which you include in your C code:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/typescript/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"ui.h"[/color][/color]`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        lcui_init();`}</Text>
            <Text className="code-line">{`        ui_load();   [color=#6e7781]/* calls the generated widget setup code */[/color]`}</Text>
            <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Available packages</h2>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "136px" }}>
              <Text>{`Package`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow mdx-table-head-cell" style={{ minWidth: "48px" }}>
              <Text>{`Description`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "136px" }}>
              <Text>{`[bgcolor=#eee] @lcui/react [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Core React-like runtime, hooks, and built-in widget bindings (Button, Text, TextInput, …)`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "136px" }}>
              <Text>{`[bgcolor=#eee] @lcui/router [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`File-system based page router`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>CSS Modules</h2>
        <p>{`You can import [bgcolor=#eee] .module.css [/bgcolor] files in your TSX components:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/typescript/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] styles [color=#cf222e]from[/color] [color=#0a3069]"./App.module.css"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]{styles.container}[/color]>[/color]Hello[color=#116329]</[color=#0550ae]div[/color]>[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Tailwind CSS</h2>
        <p>{`Tailwind CSS class names are supported out of the box in LCUI CLI projects:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/typescript/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col p-4 gap-2"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]span[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-lg font-bold"[/color]>[/color]Title[color=#116329]</[color=#0550ae]span[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"bg-blue-500 text-white px-4 py-2 rounded"[/color]>[/color]`}</Text>
            <Text className="code-line">{`        Click me`}</Text>
            <Text className="code-line">{`      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
