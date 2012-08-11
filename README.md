# LCUI 0.12.4 

  The LCUI project is create by a China developer, and his English is 
  not very good, so, some files will appear chinese character, 
  please understand. thanks!

  Please read the file `docs/CHANGES`, it contains IMPORTANT INFORMATION.

  Read the file `INSTALL` for installation instructions.

  See the  file `docs/LICENSE.TXT`  for the available licenses.

# Documentation 

  API reference documentation has not yet been prepared, development to 
  more mature in this project will consider writing it.
  At this stage, you can refer to the header files, source code, and 
  sample programs .

# Building

To bootstrap the build you need to run `./configure` (in the root of the source tree). 

In the simplest case you would run:

	git clone https://github.com/lc-soft/LCUI.git
	cd LCUI
	git submodule update --init
	./configure

Please note that if you downloaded the source code (rather than cloned via git) you likely miss the submodules and the build will therefor fail.

# Bugs 

  Please report bugs  by e-mail to 'lc-soft@live.cn'. Don't forget to send a  detailed explanation of  the problem --  there is nothing worse than receiving a terse message that only says `it doesn't work'.

# Contributing

You can send pull requests via GitHub. Patches should:

1. Follow the style of the existing code.
2. One commit should do exactly one thing.
3. Commit messages should start with a summary line below 80 characters followed by a blank line, and then the reasoning/analysis for why the change was made (if appropriate).
4. Commits that fix a bug in a previous commit (which has already been merged) should start with `fixup!` and then the summary line of the commit it fixes. If you are writing your commit message in TextMate then type `fix⇥` to get the prefix and a menu allowing you to pick the summary line from one of the last 15 commits.
5. Rebase your branch against the upstream’s master. We don’t want to pull redundant merge commits.
6. **Be clear about what license applies to your patch:** The files within this repository are under the [GPL 2][] (or later) but (as the original creator) we are still allowed to create non-free derivatives. However, if patches are given to us under GPL then those cannot make it into any non-free derivatives we may later wish to create. So to make it easier for us (and avoid any legal issues) we prefer if patches are released as public domain.

# Legal

The LCUI Project is released under the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
TextMate is a trademark of Allan Odgaard.

# Thanks to 
  Liu Chao <lc-soft@live.cn>

Special thanks to Liu Chao for his great work on the LCUI's development.


--- end of README.md ---
