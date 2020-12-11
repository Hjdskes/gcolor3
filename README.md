Color Picker
============

**Choose colors from the picker or the screen**

This README is only relevant for development resources and instructions. For a
description of Color Picker, screenshots and installation instructions for end-users,
please see the [website](https://www.hjdskes.nl/projects/gcolor3/).

Compile from source
------------

### GNOME Builder (Recommended)

GNOME Builder is the environment used for the development of this 
application. It can use Flatpak manifests to create a consistent building
and running environment cross-distro. Thus, it is highly recommended you 
use it.

1. Download [GNOME Builder](https://flathub.org/apps/details/org.gnome.Builder).
2. In Builder, click the "Clone Repository" button at the bottom, using `git@gitlab.gnome.org/World/gcolor3.git`
or `https://gitlab.gnome.org/World/gcolor3.git` as the URL.
3. Click the build button at the top once the project is loaded.

### Manual

If you feel at home in the terminal or would like to build outside of Builder,
these are the commands to use:

```bash
git clone https://gitlab.gnome.org/World/gcolor3.git
cd gcolor3
meson _build
ninja -C _build
```

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

