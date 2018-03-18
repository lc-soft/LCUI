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
- [License](#license)
- [Documentation](#documentation)
- [Building](#building)
- [Contribution](#contribution)
- [Backers](#backers)
- [Sponsors](#sponsors)

## Introduction

LCUI is a freely available software library for building user interfaces. It is written in C, support the use XML and CSS to describe the graphical interface, you can use it to build simple desktop apps.

[中文版说明文档](README.zh-cn.md)

### Features

- **written in C:** Suitable for applications that are smaller and are primarily written in C.
- **Cross platform:** Support for Windows and Linux, you can write Windows Desktop apps and Universal Windows Platform apps, as well as Linux Desktop apps.
- **XML parsing:** Support for using XML markup language to describe the structure of the graphical user interface.
- **CSS parsing:** Support for using CSS language to describing the presentation of the graphical user interface, But the range of CSS support and browser is different, it is a simplified version, cannot support all the features of CSS, like `transition`, `transform`, `animation`, `@media`, `@key-frames` etc, you can read the file [css_parser.c](https://github.com/lc-soft/LCUI/blob/53e268251a53bf371ca7aaa7862ec69fb4d0015a/src/gui/css_parser.c#L550) for more details.
- **HTML-Like layout:** Support the **block**, **inline-block** and simple **flex** layout, If you have web page development experience, then the use of LCUI will be easier to get started.
- **Flexible:** Support for adapting the interface to screens of different pixel densities by setting global scaling, And also support for using screen density related sp and dp units to describe position and size of the elements.
- **Text rendering:** Support for the text to set the global font, line height, horizontal alignment, and also support to set the color, background color, bold, italic for some text blocks separately.
- **Font management:** Support for loading multiple font files, and support to use font of different family, weight and style.
- **Image processing:** Provide image read interfaces, support for reading jpg, png and bmp format images.
- **Touch:** Support multi-touch, but currently only support Windows system.

### Missing features

LCUI is a personal project, its main purpose is to allow author to easily develop simple GUI applications, what is "simple"? it means fewer features, for example:

- No hardware acceleration, graphics rendering inefficient.
- Cannot select text and copy it on user interface.
- Cannot use CTRL+C to copy content, and cannot use CTRL+V to paste content to input box.
- Cannot use the Input Method Engine to input non-ASCII characters, like Chinese, Japanese, etc.
- rounded corners border is not supported on this version.
- Layout system is simple, grid and table layout is not supported.
- No English documents, it is hard to use.

If you have the above problem solving experience, you can provide technical support to author by submitting issue or pull request.

### Screenshots

![Hello, World!](https://lcui.lc-soft.io/static/images/screenshot-lcui-hello.png)

[![LCUI.css](https://lcui.lc-soft.io/static/images/screenshot-lcui-css.gif)](https://github.com/lc-ui/LCUI.css)

[![LCFinder](https://lcui.lc-soft.io/static/images/screenshot-lcfinder.png)](https://github.com/lc-soft/LC-Finder)

## Documentation

- Tutorial: [https://lcui.lc-soft.io/guide/](https://lcui.lc-soft.io/guide/)
- Changelog: [docs/CHANGES.md](docs/CHANGES.md)

API reference documentation has not yet been prepared, you can refer to the header files, source code, and tests.

## Building

### Bootstrap

To bootstrap the build you need to run `./configure` (in the root of the source tree).

In the simplest case you would run:

    git clone https://github.com/lc-soft/LCUI.git
    cd LCUI
    ./autogen.sh
    ./configure
    make
    make install

If you want to experience the effect of demo, please run:

    cd test
    make
    ./hellowrold

If you want to custom building options, please read the [INSTALL](INSTALL) file for more details.

### Prerequisites

If you want to build full-featured LCUI, we suggest you install the following
 dependent libraries:

 * [libpng](http://www.libpng.org/pub/png/libpng.html) — PNG image compression library
 * [libjpeg](http://www.ijg.org/) — JPEG image compression library
 * [libxml2](http://xmlsoft.org/) — The XML C parser and toolkit
 * [libx11](https://www.x.org/) — X11 client-side library
 * [freetype](https://www.freetype.org/) — Font engine

If you system is Ubuntu, you can run following command to install dependencies:

    apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev

### Building On Windows

LCUI is mainly develop in the Windows environment, you can use VisualStudio to open file `build/windows/LCUI.sln` and compile LCUI.

## Contribution

Please read through our [contributing guidelines](/.github/CONTRIBUTING.md). Included are directions for opening issues, coding standards, and notes on development.

## License

The LCUI Project is released under [the MIT License]((http://opensource.org/licenses/MIT)).

## Backers

Support us with a monthly donation and help us continue our activities. [[Become a backer](https://opencollective.com/lcui#backer)]

<a href="https://opencollective.com/lcui/backer/0/website" target="_blank"><img src="https://opencollective.com/lcui/backer/0/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/1/website" target="_blank"><img src="https://opencollective.com/lcui/backer/1/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/2/website" target="_blank"><img src="https://opencollective.com/lcui/backer/2/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/3/website" target="_blank"><img src="https://opencollective.com/lcui/backer/3/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/4/website" target="_blank"><img src="https://opencollective.com/lcui/backer/4/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/5/website" target="_blank"><img src="https://opencollective.com/lcui/backer/5/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/6/website" target="_blank"><img src="https://opencollective.com/lcui/backer/6/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/7/website" target="_blank"><img src="https://opencollective.com/lcui/backer/7/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/8/website" target="_blank"><img src="https://opencollective.com/lcui/backer/8/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/9/website" target="_blank"><img src="https://opencollective.com/lcui/backer/9/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/10/website" target="_blank"><img src="https://opencollective.com/lcui/backer/10/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/11/website" target="_blank"><img src="https://opencollective.com/lcui/backer/11/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/12/website" target="_blank"><img src="https://opencollective.com/lcui/backer/12/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/13/website" target="_blank"><img src="https://opencollective.com/lcui/backer/13/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/14/website" target="_blank"><img src="https://opencollective.com/lcui/backer/14/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/15/website" target="_blank"><img src="https://opencollective.com/lcui/backer/15/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/16/website" target="_blank"><img src="https://opencollective.com/lcui/backer/16/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/17/website" target="_blank"><img src="https://opencollective.com/lcui/backer/17/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/18/website" target="_blank"><img src="https://opencollective.com/lcui/backer/18/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/19/website" target="_blank"><img src="https://opencollective.com/lcui/backer/19/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/20/website" target="_blank"><img src="https://opencollective.com/lcui/backer/20/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/21/website" target="_blank"><img src="https://opencollective.com/lcui/backer/21/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/22/website" target="_blank"><img src="https://opencollective.com/lcui/backer/22/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/23/website" target="_blank"><img src="https://opencollective.com/lcui/backer/23/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/24/website" target="_blank"><img src="https://opencollective.com/lcui/backer/24/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/25/website" target="_blank"><img src="https://opencollective.com/lcui/backer/25/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/26/website" target="_blank"><img src="https://opencollective.com/lcui/backer/26/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/27/website" target="_blank"><img src="https://opencollective.com/lcui/backer/27/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/28/website" target="_blank"><img src="https://opencollective.com/lcui/backer/28/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/29/website" target="_blank"><img src="https://opencollective.com/lcui/backer/29/avatar.svg"></a>

## Sponsors

Become a sponsor and get your logo on our README on Github with a link to your site. [[Become a sponsor](https://opencollective.com/lcui#sponsor)]

<a href="https://opencollective.com/lcui/sponsor/0/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/0/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/1/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/1/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/2/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/2/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/3/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/3/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/4/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/4/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/5/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/5/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/6/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/6/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/7/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/7/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/8/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/8/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/9/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/9/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/10/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/10/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/11/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/11/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/12/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/12/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/13/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/13/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/14/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/14/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/15/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/15/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/16/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/16/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/17/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/17/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/18/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/18/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/19/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/19/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/20/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/20/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/21/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/21/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/22/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/22/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/23/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/23/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/24/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/24/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/25/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/25/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/26/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/26/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/27/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/27/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/28/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/28/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/29/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/29/avatar.svg"></a>
