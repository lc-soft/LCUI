import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TsxAndDevToolsDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TSX &amp; Dev Tools</h1>
      <p className="doc-page-desc">{`With [bgcolor=#eee] @lcui/cli [/bgcolor], you can write declarative UIs using TypeScript with JSX syntax. The CLI compiles TSX into C code, so you don't need to manually write widget tree construction logic.`}</p>
      <p>{`The benefit is that the declarative approach makes state binding, event binding, and resource imports more concise and intuitive. Handling logic and view in the same file reduces context switching.`}</p>
      <p>{`However, [bgcolor=#eee] @lcui/react [/bgcolor] currently has limited capabilities — you can only declare component state, data bindings, and event bindings. For complex operations like conditional rendering or list rendering, you still need to write C code.`}</p>
      <Widget className="doc-section">
        <h2>Installing dev tools</h2>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] is a command-line tool that integrates a TypeScript compiler, Sass preprocessor, resource file loader, and more. It requires a Node.js runtime:`}</p>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/tsx-and-dev-tools/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install -g @lcui/cli`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>How the preprocessor works</h2>
        <p>{`LCUI uses a preprocessor approach: TSX files are only executed during preprocessing and do not become runtime code. You can think of TSX files as configuration files, where the TypeScript code acts as preprocessing directives.`}</p>
        <p>{`The preprocessor matches the appropriate loader by file extension, parses the TSX code, collects dependencies, executes component functions, and then generates C source files based on the returned JSX elements and Hook call results.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage overview</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/App.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { useState, [color=#8250df]TextInput[/color], [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] inputRef = { [color=#0550ae]current[/color]: { [color=#0550ae]value[/color]: [color=#0a3069]""[/color] } };`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[name, setName] = [color=#8250df]useState[/color]([color=#0a3069]"LCUI"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"container"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col items-center pt-6"[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"my-4 text-2xl text-center"[/color]>[/color]Hello, {name}![color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]          \$[color=#0550ae]ref[/color]=[color=#0a3069]{inputRef}[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]          [color=#0550ae]className[/color]=[color=#0a3069]"mt-2"[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]          [color=#0550ae]placeholder[/color]=[color=#0a3069]"Please input..."[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]        />[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]          [color=#0550ae]className[/color]=[color=#0a3069]"mt-2"[/color][/color]`}</Text>
            <Text className="code-line">{`[color=#116329]          [color=#0550ae]onClick[/color]=[color=#0a3069]{()[/color] =>[/color] setName(inputRef.current.value)}`}</Text>
            <Text className="code-line">{`        >`}</Text>
            <Text className="code-line">{`          Change`}</Text>
            <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>React library</h2>
        <p>{`[bgcolor=#eee] @lcui/react [/bgcolor] is a UI library tailored for LCUI's features and preprocessor workflow, providing built-in components, utility functions, and hooks.`}</p>
        <p>{`Installation:`}</p>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/tsx-and-dev-tools/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/react`}</Text>
          </Widget>
        </Widget>
        <h3>Component functions</h3>
        <p>{`The preprocessor executes component functions, collects data from [bgcolor=#eee] useState [/bgcolor], [bgcolor=#eee] useRef [/bgcolor], etc. and the returned JSX elements, then converts them into C code.`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <ul>
              <li><Text>{`Component function return values must be a single JSX element — [bgcolor=#eee] <Fragment> [/bgcolor], null, undefined, strings, numbers, or other objects are not supported.`}</Text></li>
              <li><Text>{`Declaring and passing component props is not yet supported.`}</Text></li>
            </ul>
          </Widget>
        </Widget>
        <h3>State management</h3>
        <p>{`[bgcolor=#eee] useState [/bgcolor] declares a state variable for a component:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { useState } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]function[/color] [color=#8250df]MyComponent[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[age, setAge] = [color=#8250df]useState[/color]([color=#0550ae]23[/color]);`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[name, setName] = [color=#8250df]useState[/color]([color=#0a3069]"Taylor"[/color]);`}</Text>
            <Text className="code-line">{`  [color=#6e7781]// ...[/color]`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] useState [/bgcolor] returns an array:`}</p>
        <ul>
          <li><Text>{`The state variable, initialized to the value passed to [bgcolor=#eee] useState [/bgcolor].`}</Text></li>
          <li><Text>{`A set function, which lets you change the state variable in response to interactions.`}</Text></li>
        </ul>
        <p>{`[bgcolor=#eee] useState [/bgcolor] adds a member to the component's state struct and generates initialization code in the init function:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/tsx-and-dev-tools/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]/* Component state struct */[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]struct[/color] [color=#8250df]MyComponent_state[/color] {`}</Text>
            <Text className="code-line">{`  [color=#953800]int[/color] age;`}</Text>
            <Text className="code-line">{`  [color=#953800]char[/color] *name;`}</Text>
            <Text className="code-line">{`};`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#6e7781]/* Code in component init function */[/color]`}</Text>
            <Text className="code-line">{`_that->state.age = [color=#0550ae]23[/color];`}</Text>
            <Text className="code-line">{`_that->state.name = strdup2([color=#0a3069]"Taylor"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`The state's set function inserts C code in the current scope, e.g. [bgcolor=#eee] setAge(30) [/bgcolor] generates [bgcolor=#eee] _that->state.age = 30; [/bgcolor].`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] useState [/bgcolor] parameters can only be string or number types.`}</p>
          </Widget>
        </Widget>
        <h3>Refs</h3>
        <p>{`Use the [bgcolor=#eee] \$ref [/bgcolor] attribute to reference a widget object:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]TextInput[/color] \$ref=[color=#0a3069]"input"[/color] />`}</Text>
          </Widget>
        </Widget>
        <p>{`Then operate on it in C code:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/tsx-and-dev-tools/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_textinput_set_content(_that->refs.input, [color=#0a3069]"hello"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`The [bgcolor=#eee] \$ref [/bgcolor] value can also be a reference object returned by [bgcolor=#eee] useRef [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { useRef } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]function[/color] [color=#8250df]MyComponent[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] inputRef = [color=#8250df]useRef[/color]();`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] [color=#116329]<[color=#0550ae]TextInput[/color] \$[color=#0550ae]ref[/color]=[color=#0a3069]{inputRef}[/color] />[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] useRef [/bgcolor] currently only implements value property read/write binding for TextInput:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/8">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`inputRef.[color=#0550ae]current[/color].[color=#0550ae]value[/color] = [color=#0a3069]"World"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]onClick[/color]=[color=#0a3069]{()[/color] =>[/color] setName(inputRef.current.value)}>Click[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] @lcui/react [/bgcolor]'s [bgcolor=#eee] useRef [/bgcolor] is different from React's [bgcolor=#eee] useRef [/bgcolor] — it is only for referencing widget objects.`}</p>
          </Widget>
        </Widget>
        <h3>Responding to events</h3>
        <p>{`Declare event handlers using the [bgcolor=#eee] on + event name [/bgcolor] attribute:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/9">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Button[/color] onClick=[color=#0a3069]"handleClick"[/color]>[color=#8250df]Click[/color] me</[color=#8250df]Button[/color]>`}</Text>
          </Widget>
        </Widget>
        <p>{`The preprocessor generates C code to bind the event:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/tsx-and-dev-tools/10">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_widget_on(_that->refs.ref_0, [color=#0a3069]"click"[/color], handleClick, w);`}</Text>
          </Widget>
        </Widget>
        <p>{`You can also bind JavaScript functions to events:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/tsx-and-dev-tools/11">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { useState, [color=#8250df]Text[/color], [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Counter[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[text, setText] = [color=#8250df]useState[/color]([color=#0a3069]"Click me"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]function[/color] [color=#8250df]handleClick[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`    [color=#8250df]setText[/color]([color=#0a3069]"You clicked"[/color]);`}</Text>
            <Text className="code-line">{`  }`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]onClick[/color]=[color=#0a3069]{handleClick}[/color]>[/color]`}</Text>
            <Text className="code-line">{`      {text}`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`The preprocessor generates a C-language version of the event handler based on the code executed inside the handler function.`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`Currently, only state variable set functions can be called inside event handlers. Accessing the event object or executing other statements is not supported.`}</p>
          </Widget>
        </Widget>
        <h3>Conditional rendering</h3>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`Not yet supported. We are considering implementing conditional rendering with a [bgcolor=#eee] <Show> [/bgcolor] component, inspired by Solid.js's [bgcolor=#eee] <Show> [/bgcolor].`}</p>
          </Widget>
        </Widget>
        <h3>Rendering lists</h3>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`Not yet supported. We are considering implementing array iteration with a [bgcolor=#eee] <For> [/bgcolor] component, inspired by Solid.js's [bgcolor=#eee] <For> [/bgcolor].`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
