{
    'targets': [
        {
            'target_name': 'fontdiff_lib_coverage',
            'type': 'static_library',
            'sources': [
                '../fontdiff/argument_parser.cc',
                '../fontdiff/cairo_helper.cc',
                '../fontdiff/diff_job.cc',
                '../fontdiff/font.cc',
                '../fontdiff/font_collection.cc',
                '../fontdiff/font_loader.cc',
                '../fontdiff/icu_helper.cc',
                '../fontdiff/language.cc',
                '../fontdiff/line.cc',
                '../fontdiff/line_differ.cc',
                '../fontdiff/page.cc',
                '../fontdiff/paragraph.cc',
                '../fontdiff/sha1.cc',
                '../fontdiff/shaped_text.cc',
                '../fontdiff/style.cc',
                '../fontdiff/version.cc',
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
                    'cflags': [
                        '-std=c++11',
                        '-Wall',
                        '--coverage',
                        '-fprofile-arcs',
                        '-ftest-coverage',
                    ],
                    'ldflags': [
                        '-lgov',
                        '--coverage',
                    ],
                    'link_settings': {
                        'libraries': [ '-ldl' ],
                    },
                }],
            ],
        },
        {
            'target_name': 'test_coverage',
            'type': 'executable',
            'sources': [
                't_main.cc',
            ],'direct_dependent_settings': {
                'include_dirs': [
                    '..',
                ],
            },
            'include_dirs': [
                '..',
            ],
            'dependencies': [
                'fontdiff_lib_coverage',
                '../third_party/freetype/freetype.gyp:freetype',
                '../third_party/icu/icu.gyp:icu_uc',
            ],
            'conditions': [
                ['OS == "linux"', {
                    # need C++11 flag for unique_ptr
                    'cflags': [
                        '-std=c++11',
                        '-Wall',
                        '--coverage',
                        '-fprofile-arcs',
                        '-ftest-coverage',
                    ],
                    'ldflags': [
                        '-lgcov',
                        '--coverage',
                    ],
                    'link_settings': {
                        'libraries': [ '-ldl' ],
                    },
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