import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TsxAndDevToolsDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TSX 与开发工具</h1>
      <p className="doc-page-desc">{`配合 [bgcolor=#eee] @lcui/cli [/bgcolor]，你可以用 TypeScript 搭配 JSX 语法编写声明式界面。CLI 会编译 TSX 为 C 代码，你无需手动编写部件树构建逻辑。`}</p>
      <p>{`这样做的好处是：声明式编程方式使得状态绑定、事件绑定和资源引入更加简洁直观，在同一份文件中处理逻辑和视图可以减少上下文切换。`}</p>
      <p>{`不过，[bgcolor=#eee] @lcui/react [/bgcolor] 目前功能有限，你只能声明组件的状态、数据绑定和事件绑定。对于条件渲染、列表渲染等复杂操作，你仍需要编写 C 代码。`}</p>
      <Widget className="doc-section">
        <h2>安装开发工具</h2>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] 是一个命令行工具，集成了 TypeScript 编译器、Sass 预处理器、资源文件加载器等功能。它依赖 Node.js 运行时环境：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/devtools/tsx-and-dev-tools/0">
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
        <h2>预处理器工作原理</h2>
        <p>{`LCUI 采用预处理器方案：TSX 文件只在预处理阶段执行，不会成为运行时代码。你可以理解为 TSX 文件是配置文件，其中的 TypeScript 代码都是预处理指令。`}</p>
        <p>{`预处理器按文件后缀名匹配合适的加载器，解析 TSX 代码、收集依赖、执行组件函数，然后根据返回的 JSX 元素和 Hook 调用结果生成 C 源文件。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>用法概览</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/1">
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
        <h2>React 库</h2>
        <p>{`[bgcolor=#eee] @lcui/react [/bgcolor] 是针对 LCUI 特性和预处理器工作模式的用户界面库，提供预置组件、工具函数和 Hook 函数。`}</p>
        <p>{`安装方法：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/devtools/tsx-and-dev-tools/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/react`}</Text>
          </Widget>
        </Widget>
        <h3>组件函数</h3>
        <p>{`预处理器会执行组件函数，收集 [bgcolor=#eee] useState [/bgcolor]、[bgcolor=#eee] useRef [/bgcolor] 等产出的数据和返回的 JSX 元素，然后转换成 C 代码。`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <ul>
              <li><Text>{`组件函数返回值必须是一个 JSX 元素，不能是 [bgcolor=#eee] <Fragment> [/bgcolor]、null、undefined、字符串、数字等对象。`}</Text></li>
              <li><Text>{`暂不支持声明和传递组件 props 参数。`}</Text></li>
            </ul>
          </Widget>
        </Widget>
        <h3>状态管理</h3>
        <p>{`[bgcolor=#eee] useState [/bgcolor] 为组件声明状态变量：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/3">
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
        <p>{`[bgcolor=#eee] useState [/bgcolor] 返回一个数组：`}</p>
        <ul>
          <li><Text>{`状态变量，初始值为你传给 [bgcolor=#eee] useState [/bgcolor] 的值。`}</Text></li>
          <li><Text>{`set 函数，允许你在响应交互时更改状态变量。`}</Text></li>
        </ul>
        <p>{`[bgcolor=#eee] useState [/bgcolor] 会为组件的状态结构体添加成员，并在初始化函数中添加初始化代码：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/devtools/tsx-and-dev-tools/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]/* 组件状态结构体 */[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]struct[/color] [color=#8250df]MyComponent_state[/color] {`}</Text>
            <Text className="code-line">{`  [color=#953800]int[/color] age;`}</Text>
            <Text className="code-line">{`  [color=#953800]char[/color] *name;`}</Text>
            <Text className="code-line">{`};`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#6e7781]/* 组件初始化函数中的代码 */[/color]`}</Text>
            <Text className="code-line">{`_that->state.age = [color=#0550ae]23[/color];`}</Text>
            <Text className="code-line">{`_that->state.name = strdup2([color=#0a3069]"Taylor"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`状态的 set 函数会在当前作用域中插入 C 代码，例如 [bgcolor=#eee] setAge(30) [/bgcolor] 会生成 [bgcolor=#eee] _that->state.age = 30; [/bgcolor]。`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] useState [/bgcolor] 的参数只能是 string、number 类型。`}</p>
          </Widget>
        </Widget>
        <h3>引用</h3>
        <p>{`用 [bgcolor=#eee] \$ref [/bgcolor] 属性引用部件对象：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]TextInput[/color] \$ref=[color=#0a3069]"input"[/color] />`}</Text>
          </Widget>
        </Widget>
        <p>{`之后在 C 代码中操作它：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/devtools/tsx-and-dev-tools/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_textinput_set_content(_that->refs.input, [color=#0a3069]"hello"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] \$ref [/bgcolor] 的值也可以是 [bgcolor=#eee] useRef [/bgcolor] 返回的引用对象：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/7">
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
        <p>{`[bgcolor=#eee] useRef [/bgcolor] 目前只实现了 TextInput 的 value 属性读写绑定：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/8">
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
            <p>{`[bgcolor=#eee] @lcui/react [/bgcolor] 的 [bgcolor=#eee] useRef [/bgcolor] 和 React 的 [bgcolor=#eee] useRef [/bgcolor] 不同，仅用于引用部件对象。`}</p>
          </Widget>
        </Widget>
        <h3>响应事件</h3>
        <p>{`通过 [bgcolor=#eee] on + 事件名 [/bgcolor] 属性声明事件处理函数：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/9">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Button[/color] onClick=[color=#0a3069]"handleClick"[/color]>点我</[color=#8250df]Button[/color]>`}</Text>
          </Widget>
        </Widget>
        <p>{`预处理器会生成 C 代码绑定事件：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/devtools/tsx-and-dev-tools/10">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_widget_on(_that->refs.ref_0, [color=#0a3069]"click"[/color], handleClick, w);`}</Text>
          </Widget>
        </Widget>
        <p>{`也可以将 JavaScript 函数与事件绑定：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/tsx-and-dev-tools/11">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { useState, [color=#8250df]Text[/color], [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Counter[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]const[/color] \\[text, setText] = [color=#8250df]useState[/color]([color=#0a3069]"点我"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`  [color=#cf222e]function[/color] [color=#8250df]handleClick[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`    [color=#8250df]setText[/color]([color=#0a3069]"你已点击"[/color]);`}</Text>
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
        <p>{`预处理器会根据事件处理函数内部的代码执行结果，为其生成 C 语言版本的事件处理函数。`}</p>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`暂时只支持在事件处理函数中执行状态变量的 set 函数，不支持访问事件对象，不支持执行其它语句。`}</p>
          </Widget>
        </Widget>
        <h3>条件渲染</h3>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`暂不支持。 我们正考虑使用 [bgcolor=#eee] <Show> [/bgcolor] 组件实现条件渲染，参考 Solid.js 的 [bgcolor=#eee] <Show> [/bgcolor]。`}</p>
          </Widget>
        </Widget>
        <h3>渲染列表</h3>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`暂不支持。 我们正考虑使用 [bgcolor=#eee] <For> [/bgcolor] 组件实现数组遍历，参考 Solid.js 的 [bgcolor=#eee] <For> [/bgcolor]。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
