# fontdiff

`fontdiff` is a utility for testing fonts. Given a collection of
TrueType or [OpenType](http://opentypecookbook.com/index.html) fonts
before and after some change, it typesets a PDF for human inspection.

For every line in the specimen text, the tool renders two raster
images in high resolution. One image is typeset using the old font
version, another with the new version. If there is any difference
between these two high-resulution images (even if it just whitespace,
for example when kerning has changed), the output PDF will highlight
the difference in color.

![Screenshot](doc/fontdiff-1.png)

Since fontdiff is using
[HarfBuzz](https://www.freedesktop.org/wiki/Software/HarfBuzz/) for
OpenType shaping, text gets rendered in a similar way to Firefox,
Chrome, Android, LibreOffice, XeTeX, GNOME, KDE, ChromeOS, and other
products.


## Specimen

The specimen format is HTML with embedded CSS styling:
```html
<html lang="la">
  <p>Lorem <span style="font-weight:700">ipsum</span> dolor sit amet.</p>
</html>
```

The `lang` tag can make a difference for rendering. For example, when
diffing a font with [Polish letter
forms](https://glyphsapp.com/tutorials/localize-your-font-polish-kreska),
you can write `<span lang="pl">ćńóśź</span> <span lang="und">ćńóśź</span>`
to see ćńóśź both in their Polish and in their default variant. fontdiff
recognizes the same [language
tags](https://www.w3.org/International/articles/language-tags/) as modern
web browsers; “und” is the “undefined” language.


## Limitations

This is not an official Google product. We just needed some small tool
for testing our font production pipeline, and perhaps you will find it
useful, too.  However, please understand the limitations:

* **Not a full renderer:** HTML and CSS are much richer than what this
  little testing tool supports. If you miss something, please do not
  hesitate to make the change yourself and send a pull request.

* **Lousy codebase:** The current state of the codebase is pretty
  embarrassing: No tests, too many dependencies, and so on. This is
  not a real product, and definitely not a showcase for writing
  production code.

* **Not secure:** Do not run this tool on untrusted input. It parses
  complicated input formats (TrueType, OpenType, HTML) but is not
  hardened in any way.  Use it at your own risk.

* **Not a specimen generator:** This tool compares how an existing
  specimen text gets rendered with two versions of the same
  font. However, it will not automatically find text to be rendered.
  Some font designers may want to manually craft specimens. Others,
  say designers working on a Cherokee font, may want to render the
  entire Cherokee Wikipedia with fontdiff to find visual changes
  between two font revisions. Yet others may want to write a Python
  script that generates an HTML specimen with every kerning pair or
  OpenType contextual substitution sequence for an existing
  font. Those who make font editors, font compressors, or font
  production pipelines may want to use fontdiff for checking that a new
  versions of their software makes no visual changes to the output of
  their respective tools. Because these use cases are wildly
  different, we felt it would be best to limit the scope of ttdfiff to
  the actual diffing operation.


## Building

```sh
$ git clone --recursive https://github.com/googlei18n/fontdiff.git
$ cd fontdiff
$ ./src/third_party/gyp/gyp -f make --depth . --generator-output build src/fontdiff/fontdiff.gyp
$ make --directory build
$ build/out/Default/fontdiff \
    --before path/to/old/FooFont*.ttf \
    --after path/to/new/FooFont*.ttf \
    --specimen src/fontdiff/specimen.html \
    --out out.pdf
```

This incantation seems to work on reasonably recent versions of MacOS X.
If you want to support other platforms, please feel free to make the change;
pull requests are very welcome.


## Contributing

Here’s a few ideas what to do if you are bored and want to get your
hands dirty:

* **Port to other platforms.** This will be a very easy starter task
  for people familiar with the respective platforms. It should
  not take more than a few hours, since the project already uses
  cross-platform libraries and a cross-platform build system.

* **Highlight deltas within a line.** Currently, the tool just
  highlights lines where there is any difference between the old and
  the new version of a font. However, the output PDF does not show
  where exactly this delta is located. Highlighting the delta
  locations inside each line would be quite useful for font designers,
  since it is much easier to spot the interesting place.  To implement
  this, one could probably run the classical diff algorithm to find
  additions/removals/changes, but comparing vertical 1-pixel columns
  instead of characters.  There are a couple free libraries that
  implement diffing on arbitrary objects, for example
  [DTL](https://github.com/cubicdaiya/dtl/blob/master/test/Objdifftest.cpp).
  This task would probably take a week to implement.

* **Testing.** Currently, the codebase is entirely untested, which makes
  it hard to maintain the code.

* **More constructs from CSS and HTML.** For finding changes in
  high-quality fonts,
  [font-feature-settings](https://www.w3.org/TR/css-fonts-3/#font-rend-desc)
  would be particularly useful.

* **MathML.** With (even limited) support for MathML rendering,
  font designers could use fontdiff to test their mathematical fonts.

* **Sandboxing.** To protect users against attacks that use malicious
  font or specimen files, set up a
  [sandbox](https://en.wikipedia.org/wiki/Sandbox_%28computer_security%29)
  early in the tool’s execution and then do all the rendering inside
  the sandbox.

* **CSS box model.** It might be nice to properly support the layout of
  CSS and HTML, where boxes can be inside boxes. Not sure if this is
  terribly important for testing/diffing fonts, but if this tickles
  your interest, go ahead.

* **Paragraph layout.** The current paragraph layout is rather
  ad-hoc. On the positive side, fontdiff uses
  [ICU](http://site.icu-project.org/) for finding potential line
  breaks, so fontdiff can be used for developing fonts for Thai and
  other languages that do not mark word boundaries. However, there is
  currently no hyphenation or justification, and the current support
  for bi-directional text is rather iffy. One option for doing this
  properly could be [Raqm](https://github.com/HOST-Oman/libraqm);
  another might be
  [Minikin](https://android.googlesource.com/platform/frameworks/minikin/+/master). If
  you want to look into this, make sure to think about the special
  needs for diffing fonts. In particular, a changed glyph width should
  not require a re-flow in the entire paragaph; otherwise, the
  resulting diffs would become meaningless. When fontdiff breaks
  paragraphs into lines, it currently measures the width of text runs
  using _both_ the old and the new font version. The decision about
  line breaking is then made based on the maximum of both these
  values, and therefore fontdiff does not run into this reflowing
  problem.  It will probably be quite difficult to replicate this
  logic when using an existing paragraph layout library. However,
  being able to precisely locate the difference between two fonts is
  important for font designers, and this was the reason for writing
  the tool in the first place.

* **Reduce binary size.** To avoid dependency hell and have
  reproducible versioning, fontdiff is statically linked to all its
  dependencies.  This is a very intentional choice, but ICU’s data
  files are _huge_.  By building a static ICU data library with only
  the needed data files (the break iterators), the size of the
  compiled fontdiff binary would shrink by about 20MB. If you are
  interested in this task, check out how
  [NodeJS](https://github.com/nodejs/node/blob/master/tools/icu/README.md)
  does it. This task would not be very difficult, but it’s probably also
  not the biggest problem to solve.

Have fun!
