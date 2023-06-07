<p align="center">
  <a href="https://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    A small C library for building user interfaces with C, XML and CSS.
  </p>
  <p align="center">
    <a href="https://github.com/lc-soft/LCUI/actions"><img alt="GitHub Actions" src="https://github.com/lc-soft/LCUI/workflows/C%2FC%2B%2B%20CI/badge.svg"></a>
    <a href="https://codecov.io/gh/lc-soft/LCUI"><img src="https://codecov.io/gh/lc-soft/LCUI/branch/develop/graph/badge.svg" /></a>
    <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/lc-soft/LCUI.svg" alt="License"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/release/lc-soft/LCUI/all.svg" alt="Github Release"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg" alt="Github All Releases"></a>
    <img src="https://img.shields.io/github/repo-size/lc-soft/LCUI.svg" alt="Repo size">
    <img src="https://img.shields.io/github/languages/code-size/lc-soft/LCUI.svg" alt="Code size">
  </p>
</p>

## Table of contents

<!-- TOC -->

- [Table of contents](#table-of-contents)
- [Introduction](#introduction)
    - [Features](#features)
    - [Screenshots](#screenshots)
    - [Architecture](#architecture)
    - [Design references](#design-references)
- [Compilation](#compilation)
- [Installation](#installation)
- [Contribution](#contribution)
- [Documentation](#documentation)
- [FAQ](#faq)
- [License](#license)

<!-- /TOC -->

## Introduction

LCUI (LC's UI Library) is a small C library for building user interfaces.

[中文版说明文档](README.zh-cn.md)

### Features

- **Written in C:** Suitable for small applications written primarily in C.language designed for LCUI, it makes it easier to create interactive UIs.
- **Cross platform:** Support for Windows and Linux, you can write Windows Desktop apps and Universal Windows Platform apps, as well as Linux Desktop apps.
- **XML + CSS:** Includes XML and CSS parsers, you can use XML and CSS to describe interface structure and style.
- **Web-like development experience:** Since the design and implementation of LCUI's functions, including layout, CSS, and renderer, are mostly referenced from MDN documents and some popular web front-end development libraries, its development experience and interface effects will have some similarities to web pages. If you already have experience writing web pages with HTML and CSS, it will be easier to get started.
- **Flexible:** Support for adapting the interface to screens of different pixel densities by setting global scaling. Support for using screen density related sp and dp units to describe position and size of the elements.

### Screenshots

<table>
  <tbody>
    <tr>
      <td>
        <a class="thumbnail" href="https://github.com/lc-soft/LCUI/blob/develop/test/helloworld.c">
          <img src="https://lcui.org/static/images/showcase/screenshot-lcui-hello.png" alt="Hello App"/>
        </a>
      </td>
      <td>
        <a class="thumbnail" href="https://github.com/lc-soft/LC-Finder">
          <img src="https://gitee.com/lc-soft/LC-Finder/raw/develop/screenshots/2.jpg" alt="LC Finder"/>
        </a>
      </td>
    </tr>
    <tr>
      <td>
        <a class="thumbnail" href="https://github.com/lc-ui/lcui-router-app">
          <img src="https://gitee.com/lc-ui/lcui-router-app/raw/master/screenshot.gif" alt="LCUI Router App"/>
        </a>
      </td>
      <td>
        <a class="thumbnail" href="https://github.com/lc-ui/lc-design">
          <img src="https://lcui.lc-soft.io/static/images/showcase/lc-design-example-preview.png" alt="LC Design"/>
        </a>
      </td>
    </tr>
  </tbody>
</table>

### Architecture

Over time LCUI has been built up to be based on various libraries, as shown below:

```text
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                               ┃
┃                        LCUI Application                       ┃
┃                                                               ┃
┃        ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓        ┃
┃        ┃                    LCUI 3                   ┃        ┃
┃      ┏━┻━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━┻━┓      ┃
┃      ┃ ui-server ┃ ui-widgets ┃ ui-builder ┃ ui-anchor ┃      ┃
┃    ┏━┻━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━┻━┓    ┃
┃    ┃ platform ┃    ui    ┃  worker  ┃  timer  ┃  cursor  ┃    ┃
┣━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┻━━━━┫
┃ thread ┃ yutil  ┃ image  ┃   css   ┃ paint  ┃  font  ┃  text  ┃
┗━━━━━━━━┻━━━━━━━━┻━━━━━━━━┻━━━━━━━━━┻━━━━━━━━┻━━━━━━━━┻━━━━━━━━┛
```

- [lib/css](./lib/css): CSS (Cascading Style Sheet) parser and selection engine, providing CSS parsing and selection capabilities.
- [lib/image](./lib/image): Image file manipulation library, provides BMP, JPG, PNG image file reading ability and PNG write ability.
- [lib/paint](./lib/paint): 2D graphics library.
- [lib/platform](./lib/platform): Platform library, provides cross-platform unified system related API, including message loop, window management, input method, etc.
- [lib/text](./lib/text): Text typesetting library, provides text typesetting capabilities.
- [lib/thread](./lib/thread): Thread library, providing cross-platform multithreading capabilities.
- [lib/timer](./ui/timer): Timer library, provides the ability to perform operations on a scheduled basis.
- [lib/ui](./lib/ui): Graphical interface core library, providing UI component management, event queue, style calculation, drawing and other necessary UI capabilities.
- [lib/ui-anchor](./lib/anchor): Anchor widget, provide capabilities similar to hyperlinks.
- [lib/ui-builder](./lib/anchor): UI Builder, provides the ability to create a UI from the content of an XML file.
- [lib/ui-cursor](./lib/ui-cursor): Cursor, provides cursor drawing capability.
- [lib/ui-server](./lib/ui-server): UI Server, provides the ability to map UI widgets to system Windows.
- [lib/ui-widgets](./lib//ui/widgets): UI widget library, provides basic UI components like text, buttons, scrollbars, and so on.
- [lib/worker](./lib/worker): Worker thread library, provides simple worker thread communication and management capabilities.
- [lib/yutil](./lib/yutil): Utility library, provides linked list, hash table, red black tree, string and other related common functions.

### Design references

- [SDL](https://github.com/SDL-mirror/SDL/tree/master/src/video/x11) — X11 driver code reference
- [FreeType](https://www.freetype.org/freetype2/docs/design/design-3.html#section-1) — Data structure naming style reference
- [LevelDB](https://github.com/google/leveldb/blob/master/include/leveldb/c.h) — Function naming style reference
- [jQuery](https://jquery.com/) — Widget operation method naming style reference
- [MDN](https://developer.mozilla.org/zh-CN/docs/Web/CSS) — CSS standard reference

## Compilation

Install [XMake](https://xmake.io/#/zh-cn/) and run the following command:

```bash
# Clone repository
git clone https://github.com/lc-soft/LCUI.git

# Go into the repository
cd LCUI

# Build
xmake

```

If you want to experience the results of the test program:

```bash
# Package the built files for building the test program
xmake package

# Go into test directory
cd test

# Build
xmake -P .

# Run
xmake run -P . -w . helloworld
```

## Installation

```bash
xmake install

# Or install it in your custom directory
xmake install -o /path/to/your/custom/installdir
```

## Contribution

Think LCUI is slow to update? there are many ways to [contribute](.github/CONTRIBUTING.md) to LCUI.

- [Submit bugs](https://github.com/lc-soft/LCUI/issues) and help us verify fixes as they are checked in.
- Share some interesting ideas related to GUI development in the [issues page](https://github.com/lc-soft/LCUI/issues).
- [Search for FIXME comments](https://github.com/lc-soft/LCUI/search?l=C&q=FIXME) in the source code and try to fix them.
- Fund the issues that interest you on [IssueHunt](https://issuehunt.io/r/lc-soft/LCUI) to attract other developers to contribute.
- Review the [source code changes](https://github.com/lc-soft/LCUI/pulls).
- [Contribute bug fixes](CONTRIBUTING.md).

LCUI has adopted the code of conduct defined by the Contributor Covenant. This document is used across many open source communities, and we think it articulates our values well. For more, see the [Code of Conduct](CODE_OF_CONDUCT.md).

## Documentation

- Tutorial: [https://docs.lcui.lc-soft.io/](https://docs.lcui.lc-soft.io/) (Chinese version only)
- Changelog: [docs/CHANGES.md](docs/CHANGES.md)

The English version of the documentation is not available, because the author does not have time to write the English version, please read the files in the [test](test) directory and related projects to learn how to use LCUI.

## FAQ

1. **Why develop it?**

   - Explore and experiment new GUI development way
   - Let other developers share about how to write better code
   - Make it easier for me to find better jobs to make more money
   - Pass the time

1. **Is this a browser kernel? Or a development library like Electron that integrates the browser environment?**

    No, you can think of it as a traditional GUI development library that applied some Web technologies.

1. **What do I need to pay attention to if I'm going to use it?**

    We recommend that your project meet the following requirements:

    - The user interface is simple and does not require advanced features such as tables, animations, and transformations.
    - The code design is reasonable, and the core function code and UI logic code are independent of each other. Even if you find that the requirements are not met after using LCUI, you can easily migrate to other GUI libraries.

1. **Why do I choose the LCUI instead of Electron?**

    In contrast to fully functional Electron, these features of LCUI, such as small binary file size and low memory usage, are not worth mentioning, except for technical research and share, you have no reason to use LCUI.

1. **Is it the same as writing a web page?**

    Not exactly, there are the following differences need to be noted:

    - The interface description file format is XML, slightly different from HTML.
    - You need to implement all the features of your application in C code, which is much less development efficiency than JavaScript.
    - No `<script>` tag, you can't embed JavaScript or C code like HTML.
    - The widget is the basic layout element, not the text, and there is no `inline` display type.
    - The scrollbar is a widget, and the `overflow: scroll;` style doesn't show scrollbars automatically, you need to create it and specify the container and scroll layer.
    - All text is rendered by the TextView widget, which has a display type of `block` instead of `inline`.
    - The widget does not overflow the bounding box of the parent widget, and the effect is similar to the applied style: `overflow: hidden;`.
    - An absolutely positioned widget is always relative to its parent widget, not the first non-statically positioned widget of the parent.
    - There are no tools like [Chrome Devtools](https://developers.google.com/web/tools/chrome-devtools) to debug graphical interfaces, you need to rely on your own imagination and development experience to verify that the bug is from your code or LCUI.。

1. **How about CSS support?**

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
