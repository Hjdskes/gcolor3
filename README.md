Gcolor3
=========

**A simple color chooser dialog written in GTK3, much alike Gcolor2.**

Installation
------------

The only dependency required to build and run Gcolor3, is GTK3.

Once the ridiculous amount of dependencies are installed, just run these commands to build and install Gcolor3:

	$ ./autogen
    $ make
    # make clean install

Translation
-----------

You can help translate Gcolor3 in your language!
To do so, simply follow these steps:

	$ cd po
	$ intltool-update --pot
	$ cp gcolor3.pot <language code>.po

Where <language code> is, obviously, the code of your language (e.g. nl for Dutch, fr for French, en for English...)
Edit the `LINGUAS` file and add your language code. Please keep the list alphabetically.
Lastly, open the .po file you just generated and translate all the string. Don't forget to fill in the information in the header!

Bugs
----

For any bug or request [fill an issue][bug] on [GitHub][ghp].

  [bug]: https://github.com/Unia/gcolor3/issues
  [ghp]: https://github.com/Unia/gcolor3

* `~/.rgb.txt` does not get initialized at startup, resulting in emptry treeview. saving a color resolves this.

License
-------
**Gcolor3** is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

**Gcolor3** is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

**Copyright © 2013** Jente Hidskes <jthidskes@outlook.com>
