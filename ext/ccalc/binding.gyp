{
    'targets':[
        {
            'target_name':'ccalc',
            'include_dirs':[
                "<!(node -e \"require('nan')\")"
            ],
            'sources':[
                'src/init.cc'
            ]
        }
    ]
}