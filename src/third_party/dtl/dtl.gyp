{
    'targets': [
        {
            'target_name': 'dtl',
            'type': 'none',
            'sources': [
                'dtl/dtl/dtl.hpp',
                'dtl/dtl/Diff.hpp',
                'dtl/dtl/Diff3.hpp',
            ],
            'include_dirs' : ['dtl'],
            'direct_dependent_settings': {
                'include_dirs': [ 'dtl' ],
            },
            'cflags_cc+': ['-O2', '-g', '-Wall'],
        }
    ]
}
