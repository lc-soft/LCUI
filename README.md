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

## Introduction

LCUI is a freely available software library for building user interfaces. It is written in C and supports the use of XML and CSS to describe the graphical interface of simple desktop apps.

[中文版说明文档](README.zh-cn.md)

### Features

- **Written in C:** Suitable for small applications written primarily in C.
- **Cross platform:** Support for Windows and Linux, you can write Windows Desktop apps and Universal Windows Platform apps, as well as Linux Desktop apps.
- **XML parsing:** Support for using XML markup language to describe the structure of the graphical user interface.
- **CSS parsing:** Support for using a simplified version of CSS to describe the presentation of the graphical user interface. The CSS version used does not support all the features of CSS, like `transition`, `transform`, `animation`, `@media`, `@key-frames` etc, you can read the file [css_parser.c](https://github.com/lc-soft/LCUI/blob/53e268251a53bf371ca7aaa7862ec69fb4d0015a/src/gui/css_parser.c#L550) for more details.
- **HTML-Like layout:** Support the **block**, **inline-block** and simple **flex** layout, If you have web page development experience, then LCUI will be familiar.
- **Flexible:** Support for adapting the interface to screens of different pixel densities by setting global scaling. Support for using screen density related sp and dp units to describe position and size of the elements.
- **Text rendering:** Support for the text to set the global font, line height, horizontal alignment, and also support to set the color, background color, bold, italic for some text blocks separately.
- **Font management:** Support for loading multiple font files and using fonts of different families, weights and styles.
- **Image processing:** Provide image read interfaces: support for reading jpg, png and bmp format images.
- **Touch:** Supports multi-touch, but currently only on Windows platforms.

### Missing features

LCUI is a personal project, its main purpose is to allow the author to easily develop simple GUI applications. 

What is "simple"? It means fewer features, for example:

- No hardware acceleration, graphics rendering is inefficient.
- Cannot select text and copy it on user interface.
- Cannot use CTRL+C to copy content, and cannot use CTRL+V to paste content to input boxes.
- Cannot use the Input Method Engine to input non-ASCII characters, like Chinese, Japanese, etc.
- Rounded corner borders are not supported in this version.
- Layout system is simple. Grid and table layouts are not supported.
- No English documentation, it is hard to use.

If you can solve any of the above problems, you can provide technical support to the author by submitting an issue or pull request.

### Related Projects

Want to know what LCUI can do? You can view the following projects:

- [LCUI.css](https://github.com/lc-ui/lcui.css) - A UI component framework for building LCUI application.
- [LCFinder](https://github.com/lc-soft/LC-Finder) - An image manager, it uses most of the features of LCUI, and you can use it as a reference to evaluate whether LCUI's performance and development complexity meets your needs.

## Documentation

- Tutorial: [https://docs.lcui.lc-soft.io/](https://docs.lcui.lc-soft.io/) 
- Changelog: [docs/CHANGES.md](docs/CHANGES.md)

The English version of the documentation is not available, please check the files in the [test](test) directory and the source code of the related projects above.

## Building

### Prerequisites

If you want to build full-featured LCUI, we suggest you install the following
 dependent libraries:

 * [libpng](http://www.libpng.org/pub/png/libpng.html) — PNG image compression library
 * [libjpeg](http://www.ijg.org/) — JPEG image compression library
 * [libxml2](http://xmlsoft.org/) — The XML C parser and toolkit
 * [libx11](https://www.x.org/) — X11 client-side library
 * [freetype](https://www.freetype.org/) — Font engine
 * [fontconfig](https://www.freedesktop.org/wiki/Software/fontconfig/) — Font configuration & location

If your system is Ubuntu, you can run following command to install dependencies:

    apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev libfontconfig1-dev

### Building On Windows

LCUI is mainly developed in the Windows environment, currently only Visual Studio solution file are available, you can use [Visual Studio](https://visualstudio.microsoft.com/) to open the file `build/windows/LCUI.sln` and build LCUI.

Installing a dependency library in windows is troublesome, but you can choose to use [vcpkg](https://github.com/Microsoft/vcpkg) to solve it:

    ./vcpkg install freetype libxml2 libjpeg-turbo libpng

### Bootstrap

To bootstrap the build you need to run `./configure` (in the root of the source tree).

In the simplest case you would run:

    git clone https://github.com/lc-soft/LCUI.git
    cd LCUI
    ./autogen.sh
    ./configure
    make
    make install

If you want to experience the demo, please run:

    cd test
    make
    ./hellowrold

If you want to use custom building options, please read the [INSTALL](INSTALL) file for more details.

## Contribution

Please read through our [contributing guidelines](/.github/CONTRIBUTING.md). Included are directions for opening issues, coding standards, and notes on development.

## License

The LCUI Project is released under [the MIT License]((http://opensource.org/licenses/MIT)).
