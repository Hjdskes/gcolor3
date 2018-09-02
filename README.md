Gcolor3
=======

**Choose colors from the picker or the screen**

This README is only relevant for development resources and instructions. For a
description of Gcolor3, screenshots and installation instructions for end-users,
please see the [website](https://hjdskes.nl/projects/gcolor3/).

Compile from source
------------

To compile Gcolor3, you need the GTK+ 3 development packages and your
distribution's package containing the tools to compile packages.
You also need the [meson](http://mesonbuild.com) build system.

When the build- and runtime dependencies of Gcolor3 have been installed, run the
following commands to build and install Gcolor3:

	$ meson build
	$ ninja -C build
	# ninja -C build install

Translations
------------

You can help translating Gcolor3 to your own language! Currently, Gcolor3 is
available in Dutch, American and British English, French, Galician, German,
Greek, Norwegian Bokmål, Serbian, Spanish, Swedish and Ukrainian.

New translations are always welcome! To do so, simply follow these steps:

	$ meson build
	$ ninja -C build gcolor3-pot
	$ mv po/Gcolor3.pot po/xx.po

Where `xx` is the code of your language (e.g. `nl` for Dutch or `en_GB` for
British English). Edit the
[LINGUAS](https://gitlab.gnome.org/World/gcolor3/blob/master/po/LINGUAS) file
and add your language code. Please keep the list sorted alphabetically. Lastly,
open the `.po` file you just generated and translate all the strings. Don't
forget to fill in the information in the header!

When a translation needs updating, execute the following commands:

	$ meson build
	$ ninja -C build gcolor3-update-po

When you are done translating, either make a pull request on [GitLab][gitlab]
or send me the file via [email](mailto:hjdskes@gmail.com).

Bugs
----

For any bug or request, please [create an
issue](https://gitlab.gnome.org/World/gcolor3/issues/new?issue%5Bassignee_id%5D=&issue%5Bmilestone_id%5D=)
on [GitLab][gitlab].

License
-------

Please see [LICENSE](https://gitlab.gnome.org/World/gcolor3/blob/master/LICENSE) on [GitLab][gitlab].

**Copyright © 2013 - 2018** Jente Hidskes &lt;hjdskes@gmail.com&gt;

  [gitlab]: https://gitlab.gnome.org/World/gcolor3

