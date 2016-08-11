Gcolor3
=======

**Choose colors from the picker or the screen**

This README is only relevant for development resources and instructions. For a
description of Gcolor3, screenshots and installation instructions for end-users,
please see the [website](https://unia.github.io/projects/gcolor3/).

Compile from source
------------

To compile Gcolor3, you need the GTK+ 3 development packages and your
distribution's package containing the tools to compile packages.

When the build- and runtime dependencies of Gcolor3 have been installed, run the
following commands to build and install Gcolor3:

	$ ./autogen.sh
	$ make
	# make clean install

Translations
------------

You can help translating Gcolor3 to your own language! Currently, Gcolor3 is
available in Dutch, American and British English, French, Galician, German,
Greek, Serbian, Swedish and Ukrainian.

New translations are always welcome! To do so, simply follow these steps:

	$ cd po
	$ intltool-update --pot
	$ mv Gcolor3.pot xx.po

Where `xx` is the code of your language (e.g. `nl` for Dutch or `en_GB` for
British English). Edit the
[LINGUAS](https://github.com/Unia/gcolor3/blob/master/po/LINGUAS) file and add
your language code. Please keep the list sorted alphabetically. Lastly, open
the `.po` file you just generated and translate all the strings. Don't forget to
fill in the information in the header!

When a translation needs updating, execute the following commands:

	$ cd po
	$ intltool-update --pot
	$ intltool-update --dist --gettext-package=Gcolor3 --output-file=xx.po xx

When you are done translating, either make a pull request on [GitHub][github] or send me
the file via [email](mailto:hjdskes@gmail.com).

Bugs
----

For any bug or request, please [create an
issue](https://github.com/Unia/gcolor3/issues/new) on [GitHub][github].

License
-------

Please see [LICENSE](https://github.com/Unia/gcolor3/blob/master/LICENSE) on [GitHub][github].

**Copyright © 2013 - 2016** Jente Hidskes &lt;hjdskes@gmail.com&gt;

  [github]: https://github.com/Unia/gcolor3

