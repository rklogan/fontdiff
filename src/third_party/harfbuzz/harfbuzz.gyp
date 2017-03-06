{
    'targets': [
        {
            'target_name': 'harfbuzz',
            'type': 'static_library',
	    'defines': [
	        'HAVE_FREETYPE',
                'HAVE_FT_GET_VAR_BLEND_COORDINATES',
                'HAVE_ICU',
                'HAVE_ICU_BUILTIN',
		'HAVE_OT',
	    ],
            'sources': [
                'files/src/hb-blob.cc',
                'files/src/hb-buffer-serialize.cc',
                'files/src/hb-buffer.cc',
                'files/src/hb-common.cc',
                #'files/src/hb-coretext.cc',
                #'files/src/hb-directwrite.cc',
                'files/src/hb-face.cc',
                #'files/src/hb-fallback-shape.cc',
                'files/src/hb-font.cc',
                'files/src/hb-ft.cc',
                #'files/src/hb-glib.cc',
                #'files/src/hb-gobject-structs.cc',
                #'files/src/hb-graphite2.cc',
                'files/src/hb-icu.cc',
                'files/src/hb-ot-font.cc',
                'files/src/hb-ot-layout.cc',
                'files/src/hb-ot-map.cc',
                'files/src/hb-ot-var.cc',
                'files/src/hb-ot-shape-complex-arabic.cc',
                'files/src/hb-ot-shape-complex-default.cc',
                'files/src/hb-ot-shape-complex-hangul.cc',
                'files/src/hb-ot-shape-complex-hebrew.cc',
                'files/src/hb-ot-shape-complex-indic-table.cc',
                'files/src/hb-ot-shape-complex-indic.cc',
                'files/src/hb-ot-shape-complex-myanmar.cc',
                'files/src/hb-ot-shape-complex-thai.cc',
                'files/src/hb-ot-shape-complex-tibetan.cc',
                'files/src/hb-ot-shape-complex-use-table.cc',
                'files/src/hb-ot-shape-complex-use.cc',
                'files/src/hb-ot-shape-fallback.cc',
                'files/src/hb-ot-shape-normalize.cc',
                'files/src/hb-ot-shape.cc',
                'files/src/hb-ot-tag.cc',
                'files/src/hb-set.cc',
                'files/src/hb-shape-plan.cc',
                'files/src/hb-shape.cc',
                'files/src/hb-shaper.cc',
                #'files/src/hb-ucdn.cc',
                'files/src/hb-unicode.cc',
                #'files/src/hb-uniscribe.cc',
                'files/src/hb-warning.cc',

	        'files/src/hb-buffer-deserialize-json.rl',
	        'files/src/hb-buffer-deserialize-text.rl',
	        'files/src/hb-ot-shape-complex-indic-machine.rl',
	        'files/src/hb-ot-shape-complex-myanmar-machine.rl',
	        'files/src/hb-ot-shape-complex-use-machine.rl',
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    'autoconf_generated',
                    'files/src',
                ],
            },
            'include_dirs': [
                'autoconf_generated',
		'files/src',
		'<(INTERMEDIATE_DIR)',
	    ],
      	    'rules': [
                {
		    'rule_name': 'ragel',
		    'extension': 'rl',
	      	    'outputs': [
 		        '<(INTERMEDIATE_DIR)/<(RULE_INPUT_ROOT).hh'
		    ],
                    'action': [
		         '<(PRODUCT_DIR)/ragel', '-e', '-F1',
		         '-o', '<@(_outputs)',
			 '<(RULE_INPUT_PATH)'
		    ],
        	}
      	    ],
	    'dependencies': [
                '../freetype/freetype.gyp:freetype',
                '../icu/icu.gyp:icu_uc',
                '../ragel/ragel.gyp:ragel',
	    ],
            'conditions': [
                ['OS == "linux"', {

                    # This assumes that we're running on Intel.
                    # The alternative is to run the configuration script, which
                    # detects architecture. But doing so can cause GLib to be
                    # chosen over ICU, which causes problems later in linking.
                    'defines': [
                        'HAVE_INTEL_ATOMIC_PRIMITIVES',
                    ],

                    #'actions': [
                    #    {
                    #        'action_name': 'gen_config_h',
                    #        'inputs': ['files/autogen.sh'],
                    #        'outputs': ['files/config.h'],
                    #        'action': ['eval', 'cd files && ./autogen.sh'],
                    #    }
                    #],
                    #'defines': [
                    #    'HAVE_CONFIG_H',
                    #],
                    #'include_dirs': [
                    #    'files',
                    #],

                }],
            ],
        },
    ]
}
