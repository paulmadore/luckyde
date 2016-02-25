#!/usr/bin/env python3
#
# setup.py: gPodder Setup Script
# Copyright (c) 2005-2015, Thomas Perl <m@thp.io>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#


import glob
import os
import re
import sys

from distutils.core import setup

installing = ('install' in sys.argv and '--help' not in sys.argv)

# Read the metadata from gPodder's __init__ module (doesn't need importing)
main_module = open('src/gpodder/__init__.py', 'rb').read().decode('utf-8')
metadata = dict(re.findall("__([a-z_]+)__\s*=\s*'([^']+)'", main_module))

author, email = re.match(r'^(.*) <(.*)>$', metadata['author']).groups()


class MissingFile(BaseException):
    pass


def info(message, item=None):
    print('=>', message, item if item is not None else '')


def find_data_files(scripts):
    # Support for installing only a subset of translations
    linguas = os.environ.get('LINGUAS', None)
    if linguas is not None:
        linguas = linguas.split()
        info('Selected languages (from $LINGUAS):', linguas)

    for dirpath, dirnames, filenames in os.walk('share'):
        if not filenames:
            continue

        # Skip translations if $LINGUAS is set
        share_locale = os.path.join('share', 'locale')
        if linguas is not None and dirpath.startswith(share_locale):
            _, _, language, _ = dirpath.split(os.sep, 3)
            if language not in linguas:
                info('Skipping translation:', language)
                continue

        # Skip manpages if their scripts are not going to be installed
        share_man = os.path.join('share', 'man')
        if dirpath.startswith(share_man):
            def have_script(filename):
                if not filename.endswith('.1'):
                    return True

                basename, _ = os.path.splitext(filename)
                result = any(os.path.basename(s) == basename for s in scripts)
                if not result:
                    info('Skipping manpage without script:', filename)
                return result
            filenames = list(filter(have_script, filenames))

        def convert_filename(filename):
            filename = os.path.join(dirpath, filename)

            # Skip .in files, but check if their target exist
            if filename.endswith('.in'):
                filename = filename[:-3]
                if installing and not os.path.exists(filename):
                    raise MissingFile(filename)
                return None

            return filename

        filenames = [_f for _f in map(convert_filename, filenames) if _f]
        if filenames:
            # Some distros/ports install manpages into $PREFIX/man instead
            # of $PREFIX/share/man (e.g. FreeBSD). To allow this, we strip
            # the "share/" part if the variable GPODDER_MANPATH_NO_SHARE is
            # set to any value in the environment.
            if dirpath.startswith(share_man):
                if 'GPODDER_MANPATH_NO_SHARE' in os.environ:
                    dirpath = dirpath.replace(share_man, 'man')

            yield (dirpath, filenames)


def find_packages():
    src_gpodder = os.path.join('src', 'gpodder')
    for dirpath, dirnames, filenames in os.walk(src_gpodder):
        if '__init__.py' not in filenames:
            continue

        dirparts = dirpath.split(os.sep)
        dirparts.pop(0)
        package = '.'.join(dirparts)

        yield package


def find_scripts():
    for dirpath, dirnames, filenames in os.walk('bin'):
        for filename in filenames:
            yield os.path.join(dirpath, filename)


try:
    packages = list(sorted(find_packages()))
    scripts = list(sorted(find_scripts()))
    data_files = list(sorted(find_data_files(scripts)))
except MissingFile as mf:
    print("""
    Missing file: %s

    If you want to install, use "make install" instead of using
    setup.py directly. See the README file for more information.
    """ % mf.message, file=sys.stderr)
    sys.exit(1)


setup(
    name='gpodder-core',
    version=metadata['version'],
    description=metadata['tagline'],
    license=metadata['license'],
    url=metadata['url'],

    author=author,
    author_email=email,

    package_dir={'': 'src'},
    packages=packages,
    scripts=scripts,
    data_files=data_files,
)
