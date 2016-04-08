# ttfdiff

`ttfdiff` is a utility for testing fonts. Given a collection
of TrueType or OpenType fonts before and after some change,
 it typesets a PDF for human inspection.

For every line in the specimen, the tool renders a high-resolution
bitmap. If the font change makes any visible difference (even if it is
just whitespace, for example when kerning tables are different),
the output PDF will contain the old and new version next to each
other.

![Screenshot](doc/ttfdiff-1.png)


## Specimen

The specimen is HTML with embedded CSS styling, such as:
```html
<html lang="la">
   <p>Lorem <span style="font-weight:700">ipsum</span> dolor sit amet.</p>
</html>
```

Currently, only a _very_ small subset of HTML and CSS is supported:
`lang`, `font-weight`, and `font-size`. If you need more, please feel free
to extend the tool.


## Limitations

We quickly kludged up this tool for testing our font production.  Perhaps
you find it useful, too.  However, please understand the limitations:

* *Not a full renderer:* HTML and CSS are much richer than what
  this little testing tool supports. However, there seemed
  little point in inventing yet another ad-hoc format. If you
  miss some HTML or CSS construct, please do not hesitate to
  make the change yourself; pull requests are very welcome.

* *Lousy codebase:* The current state of the codebase is pretty
  embarrassing: Hardly any tests, massive dependencies, and so on.
  This is not a real product, it just helps to quickly compare
  two revisions of a TrueType or OpenType font.


## Building

```sh
$ git clone --recursive https://github.com/googlei18n/ttfdiff.git
$ cd ttfdiff
$ ./src/third_party/gyp/gyp -f make --depth . --generator-output build src/ttfdiff/ttfdiff.gyp
$ make --directory build
$ build/out/Default/ttfdiff \
    --before path/to/old/FooFont*.ttf \
    --after path/to/new/FooFont*.ttf \
    --specimen src/ttfdiff/specimen.html \
    --out out.pdf
```

This seems to work on MacOS X. If you want to support other platforms,
please feel free to make the change. Pull requests are very welcome.
