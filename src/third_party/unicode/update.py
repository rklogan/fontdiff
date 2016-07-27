import os, os.path
import urllib


EMOJI_VERSION = '3.0'
IVD_VERSION = '2014-05-16'
UCD_VERSION = '9.0.0'


def update_emoji():
    urlpath = 'http://www.unicode.org/Public/emoji/%s/' % EMOJI_VERSION
    for filename in ('emoji-data.txt', 'emoji-sequences.txt',
                     'emoji-zwj-sequences.txt'):
        _update_file(urlpath + filename, os.path.join('emoji', filename))


def update_ivd():
    urlpath = 'http://www.unicode.org/ivd/data/%s/' % IVD_VERSION
    for filename in ('IVD_Collections.txt', 'IVD_Sequences.txt',
                     'IVD_Stats.txt'):
        _update_file(urlpath + filename, os.path.join('ivd', filename))


def update_ucd():
    urlpath = 'http://www.unicode.org/Public/%s/ucd/' % UCD_VERSION
    for filename in ('DerivedAge.txt', 'Scripts.txt', 'ScriptExtensions.txt'):
        _update_file(urlpath + filename, os.path.join('ucd', filename))


def _update_file(urlpath, filename):
    print("Updating " + filename)
    url = urllib.urlopen(urlpath)
    with open(filename, 'w') as out:
        out.write(url.read())
    url.close()


if __name__ == '__main__':
    update_emoji()
    update_ivd()
    update_ucd()
