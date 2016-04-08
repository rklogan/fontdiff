{
    'targets': [
        {
            'target_name': 'ttfdiff',
            'type': 'executable',
            'sources': [
                'main.cc',
            ],
            'dependencies': [
                'ttfdiff_lib',
                '../third_party/freetype/freetype.gyp:freetype',
                '../third_party/icu/icu.gyp:icu_uc',
            ],
        },
        {
            'target_name': 'ttfdiff_lib',
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
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    '..',
                ],
            },
            'include_dirs': [
                '..',
            ],
            'dependencies': [
                '../third_party/cairo/cairo.gyp:cairo',
                '../third_party/expat/expat.gyp:expat',
                '../third_party/freetype/freetype.gyp:freetype',
                '../third_party/harfbuzz/harfbuzz.gyp:harfbuzz',
                '../third_party/icu/icu.gyp:icu_i18n',
                '../third_party/icu/icu.gyp:icu_uc',
                '../third_party/icu/icu.gyp:icu_data',
            ],
        },
    ],
}
