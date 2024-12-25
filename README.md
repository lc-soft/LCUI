<p align="center">
  <a href="http://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    The C library for build user interfaces
  </p>
  <p align="center">
    <a href="https://github.com/lc-soft/LCUI/actions"><img alt="GitHub Actions" src="https://github.com/lc-soft/LCUI/workflows/C%2FC%2B%2B%20CI/badge.svg"></a>
    <a href="https://codecov.io/gh/lc-soft/LCUI"><img src="https://codecov.io/gh/lc-soft/LCUI/branch/develop/graph/badge.svg" /></a>
    <img src="https://img.shields.io/github/repo-size/lc-soft/LCUI.svg" alt="Repo size">
    <img src="https://img.shields.io/github/languages/code-size/lc-soft/LCUI.svg" alt="Code size">
  </p>
</p>

## Table of contents

<!-- TOC -->

- [Table of contents](#table-of-contents)
- [Introduction](#introduction)
  - [Main Features](#main-features)
  - [Overview of Features](#overview-of-features)
  - [Architecture](#architecture)
- [Quick Start](#quick-start)
- [Documentation](#documentation)
  - [Tutorials](#tutorials)
  - [References](#references)
- [Roadmap](#roadmap)
- [Contribution](#contribution)
- [FAQ](#faq)
- [License](#license)

<!-- /TOC -->

## Introduction

[中文版说明文档](README.zh-cn.md)

LCUI is a library written in C for building graphical user interfaces. Its goal is to explore and practice new ways of developing user interfaces, characterized by its small size, ease of use, and provision of convenient development tools to help developers quickly create desktop applications with graphical user interfaces.

**LC** originates from the initials of the author's name. The library was initially designed to help the author develop small projects and gain development experience. However, the author did not gain a competitive advantage in the job market, which is crowded with C/C++ experts, and thus had to work in web front-end development. As a result, LCUI now leans towards integrating technologies from the web front-end domain.

### Key Features  

- **Cross-platform:** Supports Windows and Linux.  
- **Fully custom-drawn components:** Ensures consistent appearance and behavior of components across platforms.  
- **DPI awareness:** Automatically scales the UI on high-resolution screens to maintain a clear display.  
- **Built-in CSS engine:** Enables the use of CSS for defining UI styles and layouts, making it easier for developers with web development experience to get started.  
- **Modern development tools:** Includes tools that allow you to build user interfaces using [TypeScript](https://lcui-dev.github.io/docs/guide/typescript) with JSX syntax, the React library, and other web frontend technologies.  

### Development Experience Preview

You can get an overview of the development experience for LCUI applications from the following image:

![preview](preview.png)

- **Component Development in React-like Style:** Writing UI configuration files using TypeScript. With the combined advantages of TypeScript, JSX syntax, and the LCUI React library, you can succinctly describe interface structure, resource dependencies, component states, data binding, and event binding.
- **Various Stylesheet Writing Methods:** [Tailwind CSS](https://tailwindcss.com/), [CSS Modules](https://github.com/css-modules/css-modules), [Sass](https://sass-lang.com/), and global CSS.
- **File-system based router:** Organize application pages in directory form, with each page corresponding to a directory. The path of the directory serves as the route for that page. With the built-in application router, you can easily implement page switching and navigation without manual route configuration.
- **User-friendly and Modern Icon Library:** Icons are sourced from the [fluentui-system-icons](https://github.com/microsoft/fluentui-system-icons) library, with partial customization to fit LCUI's characteristics, offering similar usage patterns.
- **Command-line Development Tool:** Run the `lcui build` command to preprocess configuration files within the app directory, then generate corresponding C source code and resource files.

### Architecture

Over time LCUI has been built up to be based on various libraries:

- [lib/yutil](./lib/yutil): Utility library providing common data structures and functions.  
- [lib/pandagl](./lib/pandagl): PandaGL (Panda Graphics Library), a graphics library offering font management, text layout, image processing, and rendering capabilities.  
- [lib/css](./lib/css): CSS parser and selector engine, enabling CSS parsing and selection functionalities.  
- [lib/ptk](./lib/ptk): Platform Toolkit library offering cross-platform unified system-related APIs, including message loops, window management, timers, worker threads, and input methods.  
- [lib/thread](./lib/thread): Thread library offering cross-platform multithreading capabilities.  
- [lib/ui](./lib/ui): Core UI library that manages UI components, event queues, style computation, drawing, and other essential UI functionalities.  
- [lib/ui-xml](./lib/ui-xml): XML parsing library enabling the creation of UI components from XML file content.  
- [lib/ui-cursor](./lib/ui-cursor): Cursor library providing cursor rendering capabilities.  
- [lib/ui-server](./lib/ui-server): UI server that maps UI components to system windows.  
- [lib/ui-router](./lib/ui-router): Router manager offering routing and navigation functionalities.  
- [lib/ui-widgets](./lib/ui/widgets): Predefined basic component library offering essential UI components such as text, buttons, and scrollbars.  
- [lib/worker](./lib/worker): Worker thread library providing simple communication and management for worker threads.  

## Quick Start

Before you begin, you need to install the following software on your computer:

- [Git](https://git-scm.com/download/): Version control tool used to download the source code of the example project.
- [XMake](https://xmake.io/#/zh-cn/?id=%e5%ae%89%e8%a3%85): Build tool used to build the project.
- [Node.js](https://nodejs.org/): JavaScript runtime environment used to run the LCUI command-line development tool.

Then, run the following commands in a command-line window:

```shell
# Install the LCUI command-line development tool
npm install -g @lcui/cli

# Create an LCUI application project
lcui create my-lcui-app
```

Follow the prompts provided by the commands afterward.

## Documentation

- [Online Documentation](https://lcui-dev.github.io/docs/next/guides/base/)
- [Request for Comments (RFC)](https://lcui-dev.github.io/docs/next/rfcs/)
- [Changelog](CHANGELOG.md)
- [Contribution Guidelines](.github/CONTRIBUTING.md)

### Tutorials

- [Todo List](https://lcui-dev.github.io/docs/next/tutorials/todolist): Learn the basic concepts and usage of LCUI, as well as how to use it to build interfaces and implement state management, interface updates, and interactions.
- [Rendering Fabric Animation](https://lcui-dev.github.io/docs/next/tutorials/render-fabric): Rewrite the existing fabric simulation program's JavaScript source code in C language, and use the cairo graphics library for fabric rendering. Then, apply LCUI to implement fabric animation playback and interaction.
- [Browser](https://lcui-dev.github.io/docs/next/tutorials/browser): Referencing a web browser, use LCUI to implement similar interface structure, layout, style, and multi-tab management features. Utilize LCUI's routing management functionality to implement multi-tab page state management and navigation, as well as a simple file browsing page. **(This tutorial is outdated, contributions to update it are welcome)**

### References

Some features of LCUI and related projects are inspired by other open-source projects. You can refer to their documentation to understand the basic concepts and usage.

- [DirectXTK](https://github.com/Microsoft/DirectXTK/wiki/StepTimer): Source code reference for the step timer.
- [Vue Router](https://router.vuejs.org/zh/guide/): Reference for the router manager. [Some functionalities](https://github.com/search?q=repo%3Alc-soft%2FLCUI+vuejs%2Fvue-router&type=code) also reference the source code of Vue Router.
- [Next.js](https://nextjs.org/docs/app/building-your-application/routing): Reference for route definition methods.

## Roadmap

Below are the items for potential future development:

- **LCUI**  
  - Improve API design.  
  - Enhance the CSS engine to support `inherit` and `!important`.  
  - Add an [SDL](https://www.libsdl.org/) backend to replace the lib/ptk library.  
  - Adapt other open-source graphics libraries to achieve better rendering performance.  
  - Optimize memory usage.  
  - Improve performance.  

- **Command-Line Tools**  
  - `lcui build --watch`: Continuously monitor file changes and automatically rebuild.  
  - `lcui dev-server`: Similar to webpack-dev-server, build LCUI applications as websites to allow developers to preview interfaces in the browser.  
  - Add build caching to rebuild only the modified files.  

- **React Component Library**  
  Develop a component library for LCUI applications using TypeScript + React, inspired by some web frontend component libraries (e.g., [radix](https://www.radix-ui.com/), [shadcn/ui](https://ui.shadcn.com/)), and reuse components from the [LC Design](https://github.com/lcui-dev/lc-design) library.  

- **Documentation**  
  - Tutorials  
  - Request for Comments (RFC)  

## Contribution

Think LCUI is slow to update? there are many ways to [contribute](.github/CONTRIBUTING.md) to LCUI.

- [Submit bugs](https://github.com/lc-soft/LCUI/issues) and help us verify fixes as they are checked in.
- Share some interesting ideas related to GUI development in the [issues page](https://github.com/lc-soft/LCUI/issues).
- [Search for FIXME comments](https://github.com/lc-soft/LCUI/search?l=C&q=FIXME) in the source code and try to fix them.
- Fund the issues that interest you on [IssueHunt](https://issuehunt.io/r/lc-soft/LCUI) to attract other developers to contribute.
- Review the [source code changes](https://github.com/lc-soft/LCUI/pulls).
- [Contribute bug fixes](CONTRIBUTING.md).

LCUI has adopted the code of conduct defined by the Contributor Covenant. This document is used across many open source communities, and we think it articulates our values well. For more, see the [Code of Conduct](CODE_OF_CONDUCT.md).

## FAQ
**Is this a browser engine? Or a development library like Electron that integrates a browser environment?**  

No, you can think of it as a traditional GUI development library that incorporates some web technologies.  

**Why should I choose LCUI over other GUI libraries/frameworks?**  

It's recommended to prioritize other GUI libraries/frameworks, as LCUI currently lacks:  

- **A rich set of built-in components:** You will need to build almost all the components in your interface from scratch, leading to higher development costs.  
- **Efficient graphics rendering performance:** Interfaces with extensive and complex content may experience lag.  
- **An animation system:** The lack of visual feedback affects the user interaction experience.  

Given its current state, LCUI is best suited for fulfilling the author's personal needs or developing simple tools with minimal interface content and interactions.  

**Support development with TypeScript? If I know how to use it, why wouldn’t I just go with Electron?**

Currently, TypeScript is primarily used to describe UI and resource dependencies, with only a few TypeScript features in use. You don’t need to study it in depth; in most cases, you can simply refer to example code for implementation.

If you have extensive web development experience and are willing to learn Electron, then Electron is clearly the better choice.

**How about CSS support?**

The following is a list of supported CSS features. Checked is supported (But does not mean full support). Unlisted properties are not supported by default.

<details>
  <summary>CSS feature coverage</summary>

- at rules
  - [x] `@font-face`
  - [ ] `@keyframes`
  - [ ] `@media`
- keywords
  - [ ] `!important`
- selectors
  - [x] `*`
  - [x] `type`
  - [x] `#id`
  - [x] `.class`
  - [x] `:hover`
  - [x] `:focus`
  - [x] `:active`
  - [x] `:first-child`
  - [x] `:last-child`
  - [ ] `[attr="value"]`
  - [ ] `:not()`
  - [ ] `:nth-child()`
  - [ ] `parent > child`
  - [ ] `a ~ b`
  - [ ] `::after`
  - [ ] `::before`
  - [ ] ...
- units
  - [x] px
  - [x] dp
  - [x] sp
  - [x] pt
  - [x] %
  - [ ] rem
  - [ ] vh
  - [ ] vw
- properties
  - [x] top, right, bottom, left
  - [x] width, height
  - [x] visibility
  - [x] display
    - [x] none
    - [x] inline-block
    - [x] block
    - [x] flex
    - [ ] inline-flex
    - [ ] inline
    - [ ] grid
    - [ ] table
    - [ ] table-cell
    - [ ] table-row
    - [ ] table-column
    - [ ] ...
  - [x] position
    - [x] static
    - [x] relative
    - [x] absolute
    - [ ] fixed
  - [x] box-sizing
    - [x] border-box
    - [x] content-box
  - [x] border
  - [x] border-radius
  - [x] background-color
  - [x] background-image
  - [x] background-position
  - [x] background-cover
  - [ ] background
  - [x] pointer-events
  - [x] font-face
  - [x] font-family
  - [x] font-size
  - [x] font-style
  - [x] flex
  - [x] flex-shrink
  - [x] flex-grow
  - [x] flex-basis
  - [x] flex-wrap
  - [x] flex-direction
  - [x] justify-content
    - [x] flex-start
    - [x] center
    - [x] flex-end
  - [x] align-items
    - [x] flex-start
    - [x] center
    - [x] flex-end
    - [x] stretch
  - [ ] float
  - [ ] transition
  - [ ] transform
  - [ ] ...

</details>

## License

The LCUI Project is released under [the MIT License]((https://opensource.org/licenses/MIT)).
