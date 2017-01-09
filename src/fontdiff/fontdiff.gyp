# Copyright 2016 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
{
    'targets': [
        {
            'target_name': 'fontdiff',
            'type': 'executable',
            'sources': [
                'main.cc',
            ],
            'dependencies': [
                'fontdiff_lib',
                '../third_party/freetype/freetype.gyp:freetype',
                '../third_party/icu/icu.gyp:icu_uc',
            ],
            'conditions': [
                ['OS == "linux"', {
                    # need C++11 flag for unique_ptr
                    'cflags': ['-std=c++11'],
                    'link_settings': {
                        'libraries': ['-ldl'],
                    },
                }],
            ],
        },
        {
            'target_name': 'fontdiff_lib',
            'type': 'static_library',
            'sources': [
                'argument_parser.cc',
                'cairo_helper.cc',
                'diff_job.cc',
                'font.cc',
                'font_collection.cc',
                'font_loader.cc',
                'icu_helper.cc',
                'language.cc',
                'line.cc',
                'line_differ.cc',
                'page.cc',
                'paragraph.cc',
                'sha1.cc',
                'shaped_text.cc',
                'style.cc',
		'version.cc',
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    '..',
                ],
            },
            'include_dirs': [
                '..',
            ],
            'defines': [
                'FONTDIFF_VERSION=' + '<!(echo $FONTDIFF_VERSION)',
            ],
            'dependencies': [
                '../third_party/cairo/cairo.gyp:cairo',
                '../third_party/dtl/dtl.gyp:dtl',
                '../third_party/expat/expat.gyp:expat',
                '../third_party/freetype/freetype.gyp:freetype',
                '../third_party/harfbuzz/harfbuzz.gyp:harfbuzz',
                '../third_party/icu/icu.gyp:icu_i18n',
                '../third_party/icu/icu.gyp:icu_uc',
                '../third_party/icu/icu.gyp:icu_data',
            ],
            'conditions': [
                ['OS == "linux"', {
                    # need C++11 flag to handle auto keyword
                    'cflags': ['-std=c++11'],
                }],
            ],
        },
    ],
    'target_defaults': {
        'xcode_settings': {
            'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
            'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
            'CLANG_CXX_LIBRARY': 'libc++',
        },
    },
}
