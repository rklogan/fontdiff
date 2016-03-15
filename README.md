# ttfdiff

Finds visual differences between two versions of a TrueType or OpenType font.
Renders an input file to PDF, highlighting any lines where there is a delta.


## Building

```sh
$ sudo apt-get install pkg-config cairo expat freetype2 \
    harfbuzz harfbuzz-icu expat icu-i18n ninja-build openssl
$ git checkout https://github.com/googlei18n/ttfdiff.git
$ cd ttfdiff; mkdir build; cd build
$ cmake ../src .
$ ninja
```

## Open tasks

* build the tool also for MacOS and Windows
* ship our own, statically linked version of the various libraries
