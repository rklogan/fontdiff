{
    'targets': [{
        'target_name': 'expat',
        'type': 'static_library',
        'defines': [ 'HAVE_MEMMOVE' ],
        'sources': [
            'files/expat/lib/xmlparse.c',
            'files/expat/lib/xmlrole.c',
            'files/expat/lib/xmltok.c',
        ],
        'direct_dependent_settings': {
            'include_dirs': [ 'expat/files/expat/lib' ],
        },
    }]
}
