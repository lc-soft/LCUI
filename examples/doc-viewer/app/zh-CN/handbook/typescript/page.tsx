import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TypescriptDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TypeScript</h1>
      <p className="doc-page-desc">{`在 Web 前端领域，有很多用于构建用户界面的库和框架，如 React、Vue.js、Solid.js 和 Svelte。其中的 React 是一个比较流行的库，它使用 JSX 作为语法扩展，通过组件化方式，允许开发者将用户界面拆分成独立的、可重用的部分，简化了开发和维护的过程。此外，CSS Modules、Sass 和 Tailwind CSS 也被广泛使用，以帮助开发者更好地管理和组织样式。`}</p>
      <p>{`TypeScript 是 JavaScript 的超集，增加了可选的静态类型和基于类的面向对象编程。选择 TypeScript 可以更好地融合 Web 前端的这些技术，而且借助 JavaScript 的生态系统，我们还可以开发一些工具和库来增强 UI 开发体验，包括但不限于：`}</p>
      <ul>
        <li><Text>{`集成 CSS Modules、Sass 和 Tailwind CSS 等方案解决 CSS 样式问题`}</Text></li>
        <li><Text>{`提供类似于 Webpack 的能力来组织、加载和打包资源文件`}</Text></li>
        <li><Text>{`添加图标库、组件库、工具库来简化界面开发`}</Text></li>
      </ul>
      <p>{`这样一来，我们就可以用 TypeScript、JSX、React 再搭配各种工具和库来开发用户界面；相比使用 C 搭配 XML 和 CSS 来编写界面，声明式编程方式使得状态绑定、事件绑定、条件渲染和资源引入更加简洁和直观。使用 JSX 可以让开发者在同一文件中处理逻辑和视图，减少上下文切换。`}</p>
      <Widget className="doc-section">
        <h2>用法概览</h2>
        <p>{`不用担心，你无需花费大量时间去深入学习 TypeScript 语言，因为用户界面相关的代码仅涉及一些基本且常见的语法，所以大多数情况下你只需要参考以下代码片段来编写即可：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/0">
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
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"container"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col items-center pt-6"[/color]>[/color]`}</Text>
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
            <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>常用语法</h2>
        <h3>`import`</h3>
        <p>{`导入由另一个模块导出的绑定。你可以使用 [bgcolor=#eee] import [/bgcolor] 语句引入 JavaScript 模块、图片、CSS 样式文件、字体文件等文件。`}</p>
        <h3>`export`</h3>
        <p>{`导出值。与 default 组合使用时表示声明为默认导出。`}</p>
        <h3>`const`</h3>
        <p>{`声明用于声明块作用域的局部变量。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>开发工具</h2>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] 是一个命令行工具，集成了包括 TypeScript 编译器、Sass 预处理器、资源文件加载器等在内的一些与 LCUI 应用程序开发相关的功能。`}</p>
        <p>{`它依赖 JavaScript 运行时环境，你需要先安装 Node.js，然后运行以下命令安装它：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/handbook/typescript/1">
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
        <h2>预处理</h2>
        <p>{`LCUI 所采用的 TypeScript 支持方案是预处理器，相较于内置 JavaScript 引擎和 JavaScript 到 C 的编译器这两种方案，它的实现成本和复杂度都比较低，而且未来也可以增加对部分 JavaScript 语法的编译能力。`}</p>
        <p>{`预处理器的文件加载方式参考了 Webpack，由加载器（Loader）提供特定类型文件的加载和编译能力，根据文件后缀名来匹配合适的加载器。预处理器在处理 TypeScript 文件时，会先调用 TypeScript 加载器解析文件内的代码，获取其依赖的模块并加载它们，然后将该文件转换为 JavaScript 模块，之后执行它，根据组件函数内部的函数调用行为和返回结果，生成相应的 C 源文件。`}</p>
        <p>{`这一处理过程与 C 编译器的预处理类似，你可以理解为你写的 tsx 文件是配置文件，里面的 TypeScript 代码都是预处理指令，它只在预处理阶段执行，并不会成为运行时代码。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>React 库</h2>
        <p>{`[bgcolor=#eee] @lcui/react [/bgcolor] 是一个针对 LCUI 的特性和预处理器工作模式的用户界面库，提供了一些预置组件、工具函数以及 Hook 函数。它相当于 LCUI 版的 React 库，让你可以像写 React 组件一样写 LCUI 的组件。不过，它目前提供的功能很有限，你只能声明组件的状态、数据绑定和事件绑定，对于条件渲染、列表渲染等复杂的操作，你仍需要编写 C 代码。`}</p>
        <p>{`安装方法：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/handbook/typescript/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/react`}</Text>
          </Widget>
        </Widget>
        <h3>组件函数</h3>
        <p>{`预处理器的 TypeScript 加载器会调用 [bgcolor=#eee] @lcui/react [/bgcolor] 库的 [bgcolor=#eee] render [/bgcolor] 函数来渲染组件，其内部实现就是直接执行组件函数，收集组件函数内执行的 [bgcolor=#eee] useState [/bgcolor]、[bgcolor=#eee] useRef [/bgcolor] 等函数产出的数据和组件函数返回的 JSX 元素，然后转换成 C 代码。`}</p>
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
        <p>{`[bgcolor=#eee] useState [/bgcolor] 函数用于为组件声明一个状态变量，用法如下：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/3">
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
        <p>{`按照惯例使用数组解构来命名状态变量，例如 [bgcolor=#eee] [something, setSomething] [/bgcolor]。`}</p>
        <p>{`[bgcolor=#eee] useState [/bgcolor] 返回一个只包含两个项的数组：`}</p>
        <ul>
          <li><Text>{`该状态变量，初始值为你传给 [bgcolor=#eee] useState [/bgcolor] 的值。`}</Text></li>
          <li><Text>{`set 函数，它允许你在响应交互时将状态变量更改为任何其他值。`}</Text></li>
        </ul>
        <p>{`[bgcolor=#eee] useState [/bgcolor] 函数会为组件的状态结构体添加一个成员，然后在组件的状态初始化函数中添加该成员的初始化代码。以上面的代码片段为例，[bgcolor=#eee] useState [/bgcolor] 函数会生成以下 C 代码：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/typescript/4">
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
        <p>{`状态的 set 函数会在当前函数作用域中插入用于变更状态的 C 代码，例如执行 [bgcolor=#eee] setAge(30) [/bgcolor]，会生成：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/typescript/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`_that->state.age = [color=#0550ae]30[/color];`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] useState [/bgcolor] 的参数只能是 string、number 类型。`}</p>
          </Widget>
        </Widget>
        <h3>引用</h3>
        <p>{`为了操作特定的组件，我们通常会创建一个变量来引用该组件对象。在 JSX 中这一创建操作可以靠设置 [bgcolor=#eee] \$ref [/bgcolor] 属性来完成：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]TextInput[/color] \$ref=[color=#0a3069]"input"[/color] />`}</Text>
          </Widget>
        </Widget>
        <p>{`之后你就可以在 C 代码中使用它来操作组件对象：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/typescript/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_textinput_set_content(_that->refs.input, [color=#0a3069]"hello"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] \$ref [/bgcolor] 属性的值还可以是 [bgcolor=#eee] useRef [/bgcolor] 函数返回的引用对象：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/8">
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
        <p>{`该引用对象只有单个 [bgcolor=#eee] current [/bgcolor] 属性，它绑定了组件对象，允许你在 TypeScript 中声明它的操作方式。不过，目前 [bgcolor=#eee] useRef [/bgcolor] 只实现了 TextInput 组件的 value 属性的读写操作绑定，用法如下：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/9">
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
            <p>{`[bgcolor=#eee] @lcui/react [/bgcolor] 库的 [bgcolor=#eee] useRef [/bgcolor] 和 React 库中的 [bgcolor=#eee] useRef [/bgcolor] 不一样，仅用于引用特定的组件对象。`}</p>
          </Widget>
        </Widget>
        <h3>响应事件</h3>
        <p>{`你可以在 JSX 中通过设置 [bgcolor=#eee] on + 事件名 [/bgcolor] 属性来声明事件处理函数，例如声明按钮的点击事件处理函数：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/10">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/ButtonPage.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Button[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]onClick[/color]=[color=#0a3069]"handleClick"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      点我`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`预处理器将会生成这样的 C 代码来绑定事件处理函数：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/typescript/11">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_widget_on(_that->refs.ref_0, [color=#0a3069]"click"[/color], handleClick, w);`}</Text>
          </Widget>
        </Widget>
        <p>{`你也可以将 JavaScript 的函数与事件绑定：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/12">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/Counter.tsx</Text>
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
        <p>{`预处理器会执行该事件处理函数，根据内部代码执行结果，为其生成 C 语言版的事件处理函数。`}</p>
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
      <Widget className="doc-section">
        <h2>图标库</h2>
        <p>{`[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 是专为 LCUI 适配的图标库，图标都来自 Microsoft 的 fluentui-system-icons 项目。`}</p>
        <p>{`安装方法：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/handbook/typescript/13">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/fluent-icons`}</Text>
          </Widget>
        </Widget>
        <p>{`你可以在 fluentui-system-icons 项目提供的在线图标库页面里选取图标。以放大图标为例，英文名通常是 Zoom In，那么可以这样搜索：`}</p>
        <p>{`从上图可看出 fluentui-system-icons 的图标有 16、20、24 等几种尺寸可选，图标命名方式是"图标名+尺寸+风格"。[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 图标库的命名方式是"图标名+风格"，当风格为 Regular 时可以省略它。具体用法如下：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/14">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ZoomIn[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]ZoomIn[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`fluentui-system-icons 提供的几种尺寸图标是针对该尺寸优化的，看起来都是像素完美的。[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 的图标使用的尺寸默认是 20，如果你的图标尺寸固定且希望图标有更好的渲染效果，可以指定 size 参数：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/15">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]ZoomIn[/color] size={[color=#0550ae]32[/color]} />`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>样式</h2>
        <p>{`预处理器支持多种样式设置方式，包括全局 CSS、CSS Modules、Tailwind CSS、Sass。`}</p>
        <h3>全局样式</h3>
        <p>{`使用 [bgcolor=#eee] import [/bgcolor] 语句从 CSS 文件引入的 CSS 样式是全局生效的。对于那些有传统 CSS 经验的人来说，使用起来很简单，也很熟悉，但随着应用程序的发展，可能会导致样式越来越难以管理。`}</p>
        <h3>CSS Modules</h3>
        <p>{`CSS 的作用域是全局的，如果你担心样式冲突，则可以采用 CSS Modules 方案来创建局部作用域的 CSS 类以避免命名冲突并提高可维护性。`}</p>
        <p>{`首先，添加 [bgcolor=#eee] .module.css [/bgcolor] 后缀的文件，写入 CSS 代码：`}</p>
        <Widget className="code-block" data-language="css" data-source="zh-CN/handbook/typescript/16">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.module.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800].card[/color] {`}</Text>
            <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#eee[/color];`}</Text>
            <Text className="code-line">{`  border-radius: [color=#0550ae]4px[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`然后在 [bgcolor=#eee] .tsx [/bgcolor] 文件中使用 [bgcolor=#eee] import [/bgcolor] 语句导入它：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/handbook/typescript/17">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] styles [color=#cf222e]from[/color] [color=#0a3069]"./MyComponent.module.css"[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`之后将 JSX 表达式中的赋给 className 属性的字符串替换成 styles 的对应属性，例如：`}</p>
        <Widget className="code-block" data-language="diff" data-source="zh-CN/handbook/typescript/18">
          <Widget className="code-block-header">
            <Text className="code-block-title">DIFF</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`- <div className="card" />`}</Text>
            <Text className="code-line">{`+ <div className={styles.card} />`}</Text>
          </Widget>
        </Widget>
        <h3>Tailwind CSS</h3>
        <p>{`Tailwind CSS 是一个功能类优先（Utility-First）的 CSS 框架，它通过提供一系列预定义的 CSS 类，帮助开发者快速、灵活地为组件设置样式。相比传统的 CSS 编写方式，你无需再花费时间去新建 CSS 文件、编写 CSS 规则以及思考 CSS 类名如何命名。在 React 组件中使用它可减少上下文切换，让你专注于组件代码。`}</p>
        <p>{`安装它和相关依赖项之后，复制官方 lcui-quick-start 模板项目中的 [bgcolor=#eee] postcss.config.js [/bgcolor]、[bgcolor=#eee] tailwind.config.js [/bgcolor]、[bgcolor=#eee] app/global.css [/bgcolor] 这几个文件到你的项目源码目录内即可使用。`}</p>
        <p>{`如果你不想将 global.css 放到 app 目录内，请更改 tailwind.config.js 文件内的 content 配置项的路径匹配规则。`}</p>
        <h3>Sass</h3>
        <p>{`一个流行的 CSS 预处理器，它通过变量、嵌套规则和混合元素等功能扩展 CSS。`}</p>
        <p>{`预处理器已经内置了它，在编译 [bgcolor=#eee] .sass [/bgcolor] 和 [bgcolor=#eee] .scss [/bgcolor] 后缀的文件时会自动调用 Sass 预处理器。`}</p>
      </Widget>
    </Widget>
  );
}
