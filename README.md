# The LCUI Project

![LCUI Logo](https://lcui.lc-soft.io/static/images/lcui-logo-lg.png)

[![Join the chat at https://gitter.im/lc-soft/LCUI](https://badges.gitter.im/lc-soft/LCUI.svg)](https://gitter.im/lc-soft/LCUI?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/lc-soft/LCUI.png?branch=master)](https://travis-ci.org/lc-soft/LCUI)
[![Coverage Status](https://coveralls.io/repos/github/lc-soft/LCUI/badge.svg?branch=develop)](https://coveralls.io/github/lc-soft/LCUI?branch=develop)
[![Github All Releases](https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg)](https://github.com/lc-soft/LCUI/releases)
[![License](https://img.shields.io/badge/license-GPLv2-blue.svg)](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
[![OpenCollective](https://opencollective.com/lcui/backers/badge.svg)](#backers) 
[![OpenCollective](https://opencollective.com/lcui/sponsors/badge.svg)](#sponsors)

## Description

[中文版说明文档](README.zh-cn.md)

LCUI is a freely available software library to create GUI application.
  
It is written in C, support the use XML and CSS describe the graphical 
interface, you can use it to make some simple effects, like in this example:

![example screenshot](https://lcui.lc-soft.io/static/images/feature-screenshot-02.png)

Author is from China, but his English is not very good, so, some 
files will appear chinese character, please understand. thanks.

Please read the file `docs/CHANGES.md`, it contains IMPORTANT INFORMATION.

Read the file `INSTALL` for installation instructions.

See the  file `docs/LICENSE.TXT`  for the available licenses.

## Documentation 

Tutorial: https://lcui.lc-soft.io/guide/

API reference documentation has not yet been prepared, you can refer to the 
header files, source code, and sample programs.

## Building

### Bootstrap

To bootstrap the build you need to run `./configure` (in the root of the 
source tree). 

In the simplest case you would run:

	git clone https://github.com/lc-soft/LCUI.git
	cd LCUI
	./autogen.sh
	./configure

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

LCUI is mainly develop in the Windows environment, you can use VisualStudio 
to open file `build/VS2012/LCUI.sln` and compile LCUI.

## Bugs 

Please report bugs  by e-mail to `lc-soft@live.cn`. Don't forget to send a 
detailed explanation of  the problem --  there is nothing worse than receiving
a terse message that only says `it doesn't work`.

## Contributing

**You can send pull requests via GitHub.**

Patches should:

1. Follow the style of the existing code.
2. One commit should do exactly one thing.
3. Commit messages should start with a summary line below 80 characters 
   followed by a blank line, and then the reasoning/analysis for why the
    change was made (if appropriate).
4. Commits that fix a bug in a previous commit (which has already been 
   merged) should start with `fixup!` and then the summary line of the 
   commit it fixes. If you are writing your commit message in LCUI
   then type `fix⇥` to get the prefix and a menu allowing you to pick 
   the summary line from one of the last 15 commits.
5. Rebase your branch against the upstream’s master. We don’t want to 
   pull redundant merge commits.
6. **Be clear about what license applies to your patch:** The files with
   in this repository are under the [GPL 2][] (or later) but (as the 
   original creator) we are still allowed to create non-free derivatives.
   However, if patches are given to us under GPL then those cannot make 
   it into any non-free derivatives we may later wish to create. So to 
   make it easier for us (and avoid any legal issues) we prefer if 
   patches are released as public domain.

**You can donate support the development of LCUI.**

I'm currently an independent developer and your contributions are useful. 
I have setup an [LCUI Patreon page](https://www.patreon.com/lcsoft) if you 
want to donate and enable me to spend more time improving the library. 
One-off donations are also greatly appreciated. Thanks!

### GitHub Workflow

Developing patches should follow this workflow:

#### Initial Setup

1.	Fork on GitHub (click Fork button)
2.	Clone to computer: `git clone git@github.com:«github account»/LCUI.git`
3.	cd into your repo: `cd LCUI`
4.	Set up remote upstream: `git remote add -f upstream git://github.com/lc-soft/LCUI.git`

#### Adding a Feature

1.	Create a branch for the new feature: `git checkout -b my_new_feature`
2.	Work on your feature, add and commit as usual

Creating a branch is not strictly necessary, but it makes it easy to 
delete your branch when the feature has been merged into upstream, diff
 your branch with the version that actually ended in upstream, and to 
 submit pull requests for multiple features (branches).

#### Pushing to GitHub

8.	Push branch to GitHub: `git push origin my_new_feature`
9.	Issue pull request: Click Pull Request button on GitHub

#### Useful Commands

If a lot of changes has happened upstream you can replay your local changes
 on top of these, this is done with `rebase`, e.g.:

	git fetch upstream
	git rebase upstream/master

This will fetch changes and re-apply your commits on top of these.

This is generally better than merge, as it will give a clear picture of which 
commits are local to your branch. It will also “prune” any of your local 
commits if the same changes have been applied upstream.

You can use `-i` with `rebase` for an “interactive” rebase. This allows
 you to drop, re-arrange, merge, and reword commits, e.g.:

	git rebase -i upstream/master


## Legal

The LCUI Project is released under the GNU General Public License as published
 by the Free Software Foundation, either version 2 of the License, or (at your 
option) any later version.

## Thanks to 

Liu Chao <lc-soft@live.cn>

Special thanks to Liu Chao for his great work on the LCUI's development.

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

--- end of README.md ---
