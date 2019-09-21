<p align="center">
  <a href="http://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    A small C library for building user interfaces with C, XML and CSS.
  </p>
  <p align="center">
    <a href="https://travis-ci.org/lc-soft/LCUI"><img src="https://travis-ci.org/lc-soft/LCUI.png?branch=master" alt="Build Status"></a>
    <a href="https://coveralls.io/github/lc-soft/LCUI?branch=develop"><img src="https://coveralls.io/repos/github/lc-soft/LCUI/badge.svg?branch=develop" alt="Coverage Status"></a>
    <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/lc-soft/LCUI.svg" alt="License"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/release/lc-soft/LCUI/all.svg" alt="Github Release"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg" alt="Github All Releases"></a>
    <img src="https://img.shields.io/github/repo-size/lc-soft/LCUI.svg" alt="Repo size">
    <img src="https://img.shields.io/github/languages/code-size/lc-soft/LCUI.svg" alt="Code size">
  </p>
</p>

## Table of contents

- [Introduction](#introduction)
    - [Features](#features)
    - [Missing features](#missing-features)
    - [Related projects](#related-projects)
    - [Design reference](#design-reference)
- [Quick start](#quick-start)
    - [Windows](#windows)
    - [Ubuntu](#ubuntu)
- [Contribution](#contribution)
- [Documentation](#documentation)
- [FAQ](#faq)
- [License](#license)

## Introduction

LCUI is a freely available software library for building user interfaces. It is written in C and supports the use of XML and CSS to describe the graphical interface of simple desktop apps.

[中文版说明文档](README.zh-cn.md)

### Features

- **Written in C:** Suitable for small applications written primarily in C.language designed for LCUI, it makes it easier to create interactive UIs.
- **Cross platform:** Support for Windows and Linux, you can write Windows Desktop apps and Universal Windows Platform apps, as well as Linux Desktop apps.
- **XML + CSS:** Support the use of XML and CSS to describe the GUI layout and style, saving interface development time and maintenance costs.
- **Flexible:** Support for adapting the interface to screens of different pixel densities by setting global scaling. Support for using screen density related sp and dp units to describe position and size of the elements.
- **Simple image reading:** Provide jpg, png and bmp image read APIs.
- **Touch:** Supports multi-touch, but currently only on Windows platforms.

### Missing features

LCUI is a personal project, its main purpose is to allow the author to easily develop simple GUI applications.

What is "simple"? It means fewer features, for example:

- No hardware acceleration, graphics rendering is inefficient.
- Cannot select text and copy it on user interface.
- Cannot use CTRL+C to copy content, and cannot use CTRL+V to paste content to input boxes.
- Cannot use the Input Method Engine to input non-ASCII characters, like Chinese, Japanese, etc.
- Layout system is simple. Grid and table layouts are not supported.
- No English documentation, it is hard to use.

There are many similar open source projects(E.g: [SDL](https://github.com/SDL-mirror/SDL), [imgui](https://github.com/ocornut/imgui)) that can be referenced today, most of the missing features in LCUI can find relevant implementations from these projects, so the development cost of LCUI is also reduced a lot. One of the reasons for open source this project is to conduct technical exchanges. If you have research experience in similar projects, please consider providing improvements to this project.

### Related projects

Want to know what LCUI can do? You can view the following projects:

- [LC Design](https://github.com/lc-ui/lc-design) - A UI component framework for building LCUI application.
- [LC Finder](https://github.com/lc-soft/LC-Finder) - An image manager, it uses most of the features of LCUI, and you can use it as a reference to evaluate whether LCUI's performance and development complexity meets your needs.
- [Trad](https://github.com/lc-soft/trad) - A language based on JavaScript syntax that can be compiled into C, it pre-built LCUI bindings, with its language features and JSX syntax support to easily create interactive graphical interfaces based on LCUI.

### Design reference

- [SDL](https://github.com/SDL-mirror/SDL/tree/master/src/video/x11) — X11 driver code reference
- [FreeType](https://www.freetype.org/freetype2/docs/design/design-3.html#section-1) — Data structure naming style reference
- [LevelDB](https://github.com/google/leveldb/blob/master/include/leveldb/c.h) — Function naming style reference
- [jQuery](https://jquery.com/) — Widget operation method naming style reference
- [MDN](https://developer.mozilla.org/zh-CN/docs/Web/CSS) — CSS standard reference

## Quick start

### Windows

clone and run the [lc-ui/lcui-quick-start](https://github.com/lc-ui/lcui-quick-start) repository to see a minimal LCUI app in action:

```shell
# Clone repository
git clone https://github.com/lc-ui/lcui-quick-start

# Go into the repository
cd lcui-quick-start

# Install NodeJS dependencies
npm install

# Install C/C++ dependencies for x64 CPU architecture
lcpkg install --arch x64

# Run the app with debug mode
lcpkg run start --mode debug
```

Want to write an LCUI application from scratch? you can use [lcpkg](https://github.com/lc-soft/lcpkg) to quick install LCUI in your project directory:

```shell
# Initialize the lcpkg configuration file to tell lcpkg about your project
lcpkg init

# Download and install the compiled LCUI library from GitHub
lcpkg install github.com/lc-soft/LCUI
```

After successful installation, follow the help documentation output by lcpkg to configure your project's build configuration.

If you want to manually compile LCUI from source code, use lcpkg in the LCUI source directory to install the dependency libraries:

```shell
lcpkg install
```

After that, [Visual Studio](https://visualstudio.microsoft.com/) to open the `build/windows/LCUI.sln` file, and then build LCUI.

### Ubuntu

```shell
# Install the dependencies
sudo apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev

# Clone this repository
git clone https://github.com/lc-soft/LCUI.git

# Go into the repository
cd LCUI

# Generate automake configure script
./autogen.sh

# Check the build environment and configure automake scripts
./configure

# Build
make

# If you wnat install LCUI
sudo make install

# Go into test directory
cd test

# Run helloworld application
./helloworld
```

> **Note:** If you want to customize the compiler, compile flags, install location, and other configuration items, read the [INSTALL](INSTALL) file.

## Contribution

There are many ways to [contribute](CONTRIBUTING.md) to LCUI.

- [Submit bugs](https://github.com/lc-soft/LCUI/issues) and help us verify fixes as they are checked in.
- [Search for FIXME comments](https://github.com/lc-soft/LCUI/search?l=C&q=FIXME) in the source code and try to fix them.
- Fund to you interested issue on [IssueHunt](https://issuehunt.io/r/lc-soft/LCUI) to attract other developers to contribute.
- Support this project on [OpenCollective](https://opencollective.com/LCUI).
- Review the [source code changes](https://github.com/lc-soft/LCUI/pulls).
- [Contribute bug fixes](CONTRIBUTING.md).

LCUI has adopted the code of conduct defined by the Contributor Covenant. This document is used across many open source communities, and we think it articulates our values well. For more, see the [Code of Conduct](CODE_OF_CONDUCT.md).

## Documentation

- Tutorial: [https://docs.lcui.lc-soft.io/](https://docs.lcui.lc-soft.io/) (Chinese version only)
- Changelog: [docs/CHANGES.md](docs/CHANGES.md)

The English version of the documentation is not available, because the author does not have time to write the English version, please read the files in the [test](test) directory and related projects to learn how to use LCUI.

## FAQ

1. **Why develop it?**

   - To explore and experiment new GUI development way
   - Get more good job and make more money
   - Leave a mark on the world
   - Pass the time

1. **Is this a browser kernel? Or a development library like Electron that integrates the browser environment?**

    No, you can think of it as a traditional GUI development library that applied some Web technologies.

1. **Why do I choose the LCUI instead of Electron?**

    In contrast to fully functional Electron, small file size and low memory usage doesn't matter, except for technical research and communication, there's no reason to use LCUI.

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
        - [x] visiblility
        - [x] display
          - [x] none
          - [x] inline-block
          - [x] block
          - [x] flex
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
        - [x] pointer-evnets
        - [x] font-face
        - [x] font-family
        - [x] font-size
        - [x] font-style
        - [x] justify-content
          - [x] flex-start
          - [x] center
          - [x] flex-end
        - [ ] float
        - [ ] transition
        - [ ] transform
        - [ ] flex
        - [ ] ...
    </details>

## License

The LCUI Project is released under [the MIT License]((http://opensource.org/licenses/MIT)).
