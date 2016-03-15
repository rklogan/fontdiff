# ttfdiff

Finds visual differences between two versions of a TrueType or OpenType font.
Renders an input file to PDF, highlighting any lines where there is a delta.


## Building

```sh
$ sudo apt-get install cmake pkg-config
# sudo apt-get install cairo expat freetype2 \
    harfbuzz harfbuzz-icu expat icu-i18n ninja-build openssl
$ git checkout https://github.com/googlei18n/ttfdiff.git
$ cd ttfdiff; mkdir build; cd build
$ cmake ../src
$ ninja
$ ttfdiff/ttfdiff \
    --before path/to/old/NotoMono-*.ttf \
    --after path/to/new/Noto-*.ttf \
    --specimen ../src/ttfdiff/specimen.html --out a.pdf
```

## Open tasks

* complete the implementation, write docs

* build the tool also for MacOS and Windows

* ship our own, statically linked versions of the various dependencies
  so that users do not have to install them (and to avoid depdendency hell)
