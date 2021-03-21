<p align="center">
  <a href="https://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    A small C library for building user interfaces with C, XML and CSS.
  </p>
  <p align="center">
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
    - [Status](#status)
    - [Screenshots](#screenshots)
    - [Related projects](#related-projects)
    - [Design references](#design-references)
- [Quick start](#quick-start)
    - [Windows](#windows)
        - [Create new LCUI project](#create-new-lcui-project)
        - [Install LCUI for existing project](#install-lcui-for-existing-project)
        - [Manual compilation and installation](#manual-compilation-and-installation)
    - [Ubuntu](#ubuntu)
- [Roadmap](#roadmap)
    - [Primary Roadmap](#primary-roadmap)
    - [Secondary Roadmap](#secondary-roadmap)
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

### Status

Platform | CI Status
---------|:---------
Windows  | [![Windows Build status](https://ci.appveyor.com/api/projects/status/ps6mq5sy8jkbks0y?svg=true)](https://ci.appveyor.com/project/lc-soft/lcui)
Linux    | [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=linux&label=build&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=arm64&label=build%20for%20ARM64&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=wasm&label=build%20for%20WebAssembly&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [!["GitHub Actions](https://github.com/lc-soft/LCUI/workflows/C%2FC%2B%2B%20CI/badge.svg)](https://github.com/lc-soft/LCUI/actions)
OSX      | [![OSX Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=osx&label=build&branch=develop)](https://travis-ci.org/lc-soft/LCUI)

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

### Related projects

Want to know what LCUI can do? You can view the following projects:

- [LCUI CLI](https://github.com/lc-ui/lcui-cli) - Command line interface for rapid LCUI development.
- [LCUI Router](https://github.com/lc-soft/lcui-router) - The official router for LCUI. It provides a similar development experience to the [Vue Router](https://github.com/vuejs/vue-router) and make building multiple views applications with LCUI a breeze.
- [LC Design](https://github.com/lc-ui/lc-design) - A UI component framework for building LCUI application.
- [LC Finder](https://github.com/lc-soft/LC-Finder) - An image manager, it uses most of the features of LCUI, and you can use it as a reference to evaluate whether LCUI's performance and development complexity meets your needs.
- [Trad](https://github.com/lc-soft/trad) - A language based on JavaScript syntax that can be compiled into C, it preset LCUI bindings, provides [React](https://reactjs.org/) like development experience, makes it painless to create interactive UIs based on LCUI.
- [LCUI Quick Start](https://github.com/lc-ui/lcui-quick-start) - A minimal LCUI application.
- [LCUI Router App](https://github.com/lc-ui/lcui-router-app) - A very simple browser-like application to show the LCUI and LCUI Router features.
- [LC Design App](https://github.com/lc-ui/lc-design-app) - A minimal LCUI application for preview LC Design component library.

### Design references

- [SDL](https://github.com/SDL-mirror/SDL/tree/master/src/video/x11) — X11 driver code reference
- [FreeType](https://www.freetype.org/freetype2/docs/design/design-3.html#section-1) — Data structure naming style reference
- [LevelDB](https://github.com/google/leveldb/blob/master/include/leveldb/c.h) — Function naming style reference
- [jQuery](https://jquery.com/) — Widget operation method naming style reference
- [MDN](https://developer.mozilla.org/zh-CN/docs/Web/CSS) — CSS standard reference

## Quick start

### Windows

#### Create new LCUI project

Quickly create an LCUI project using [lcui-cli](https://github.com/lc-ui/lcui-cli):

```bash
# Install lcui-cli and lcpkg
npm install -g @lcui/cli lcpkg

# Create an LCUI project named myapp
lcui create myapp

# Go into project directory
cd myapp

# Install dependencies
lcpkg install

# Run it
lcpkg run start
```

#### Install LCUI for existing project

Use [lcpkg](https://github.com/lc-soft/lcpkg) to quick install LCUI in your project directory:

```bash
# Initialize the lcpkg configuration file to tell lcpkg about your project
lcpkg init

# Download and install the compiled LCUI library from GitHub
lcpkg install github.com/lc-soft/LCUI
```

After successful installation, follow the help documentation output by lcpkg to configure your project's build configuration.

#### Manual compilation and installation

If you want to manually compile LCUI from source code:

1. Open CMD window, and run following command in the LCUI source directory to install the dependency libraries:
    ```bash
    lcpkg install
    # If you want compile for x64 CPU architecture
    lcpkg install --arch x64
    # If you want compile for Universal Windows Platform (UWP)
    lcpkg install --platform uwp
    lcpkg install --arch x64 --platform uwp
    ```
1. Rename `config.win.h.in` in the include directory to `config.h`.
1. Open the `build/windows/LCUI.sln` file with [Visual Studio](https://visualstudio.microsoft.com/), and then build LCUI.

### Ubuntu

```bash
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

## Roadmap

The roadmap for this project is divided into primary and secondary roadmaps, with the primary roadmap developed by the project maintainer and the secondary roadmap developed by the open source community contributors and developers like you.

### Primary Roadmap

- Make project code more standardized, readable, and maintainable
- Improve relevant development tools and sample applications to improve the development efficiency and make it easier to use

### Secondary Roadmap

- Explore new GUI development methods to make LCUI unique
- [Accessbility](https://developer.mozilla.org/en-US/docs/Learn/Accessibility/What_is_accessibility) support
- Research [SDL](https://github.com/SDL-mirror/SDL), [imgui](https://github.com/ocornut/imgui) and other similar open source projects, and try to fill in what LCUI is missing
- Refactor the existing graphics processing interface to make it easy to integrate with mainstream 2D graphics libraries and bring better graphics rendering performance to LCUI
- Add driver support for Mac OS, Android and iOS
- Add more mouse cursor styles
- Add clipboard support

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
