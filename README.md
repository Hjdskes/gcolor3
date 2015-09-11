Gcolor3
=======

**A simple color chooser dialog written in GTK3, much alike Gcolor2.**

![Gcolor3-screenshot](https://github.com/Unia/gcolor3/raw/master/gcolor3.png "gcolor3")
![Gcolor2-screenshot](https://github.com/Unia/gcolor3/raw/master/gcolor2.png "gcolor2")

Installation
------------

### Distribution packages

Gcolor3 is packaged for some distributions. This should be your preferred method of installation, if
your distribution is listed below:

* **Arch Linux:** There are AUR packages for the [latest stable release](https://aur.archlinux.org/packages/gcolor3/) and for the [git version](https://aur.archlinux.org/packages/gcolor3-git/).
* **Fedora:** There is a [copr repository](https://copr.fedoraproject.org/coprs/fnux/GColor3/) to
  install Gcolor3. Please see the instructions there.

### Compile from source

The only dependency required to build and run Gcolor3, is GTK3.

Once the ridiculous amount of dependencies is installed, just run these commands to build and install Gcolor3:

	$ ./autogen.sh
	$ make
	# make clean install

Translations
------------

You can help translate Gcolor3 to your language! To do so, simply follow these steps:

	$ cd po
	$ intltool-update --pot
	$ mv Gcolor3.pot <language code>.po

Where `<language code>` is the code of your language (e.g. `nl` for Dutch, `fr` for French, `en_GB` for British English...).
Edit the [LINGUAS](https://github.com/Unia/gcolor3/blob/master/po/LINGUAS) file and add your language code. Please keep the list sorted alphabetically.
Lastly, open the `.po` file you just generated and translate all the strings. Don't forget to fill in the information in the header!

When a translation needs updating, execute the following commands:

	$ cd po
	$ intltool-update --pot
	$ intltool-update --dist --gettext-package=Gcolor3 --output-file=<language code>_new.po <language code>

Then make the necessary changes and overwrite the old `.po` file:

	$ mv <language code>_new.po <language code>.po

Bugs
----

For any bug or request [fill an issue](https://github.com/Unia/gcolor3/issues) on [GitHub][github].

License
-------

Please see [LICENSE](https://github.com/Unia/gcolor3/blob/master/LICENSE) on [GitHub][github].

**Copyright © 2013 - 2015** Jente Hidskes &lt;hjdskes@gmail.com&gt;

  [github]: https://github.com/Unia/gcolor3

