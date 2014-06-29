{
    "targets": [{
        "target_name": "lwip",
        "sources": [
            "lwip.cpp",
            "LwipImage.cpp",
            "lib/jpeg-compressor/jpgd.cpp"
        ],
        'cflags!': ['-fno-exceptions'],
        'cflags_cc!': ['-fno-exceptions'],
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                }
            }]
        ]
    }]
}
