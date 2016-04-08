# ttfdiff

Finds visual differences between two versions of a TrueType or OpenType font.
Renders a specimen file to PDF, highlighting any lines where there is a delta.

The specimen is HTML with embedded CSS styling:
```html
<html>
   <p lang="de-CH" style="font-size:12pt">Hoi!</p>
</html>
```


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

This seems to work on MacOS X and at least some Linux distros.
Feel free to improve; pull requests welcome.
