{
    "targets": [{
        "target_name": "lwip_decoder",
        "sources": [
            # LWIP: 
            #######
            "src/decoder/init.cpp",
            "src/decoder/util.cpp",
            "src/decoder/buffer_worker.cpp",
            "src/decoder/jpeg_decoder.cpp",
            "src/decoder/png_decoder.cpp",
            # LIB JPEG:
            ###########
            "lib/jpeg/jmemnobs.c",
            "lib/jpeg/jcomapi.c",
            "lib/jpeg/jdapimin.c",
            "lib/jpeg/jdapistd.c",
            "lib/jpeg/jdatadst.c",
            "lib/jpeg/jdatasrc.c",
            "lib/jpeg/jdcoefct.c",
            "lib/jpeg/jdcolor.c",
            "lib/jpeg/jddctmgr.c",
            "lib/jpeg/jdhuff.c",
            "lib/jpeg/jdinput.c",
            "lib/jpeg/jdmainct.c",
            "lib/jpeg/jdmarker.c",
            "lib/jpeg/jdmaster.c",
            "lib/jpeg/jdpostct.c",
            "lib/jpeg/jdsample.c",
            "lib/jpeg/jerror.c",
            "lib/jpeg/jfdctflt.c",
            "lib/jpeg/jfdctfst.c",
            "lib/jpeg/jfdctint.c",
            "lib/jpeg/jidctflt.c",
            "lib/jpeg/jidctfst.c",
            "lib/jpeg/jidctint.c",
            "lib/jpeg/jutils.c",
            "lib/jpeg/jmemmgr.c",
            "lib/jpeg/jdarith.c",
            "lib/jpeg/jdmerge.c",
            "lib/jpeg/jaricom.c",
            "lib/jpeg/jquant1.c",
            "lib/jpeg/jquant2.c",
            # LIB PNG:
            ##########
            "lib/png/png.c",
            "lib/png/pngset.c",
            "lib/png/pngget.c",
            "lib/png/pngrutil.c",
            "lib/png/pngtrans.c",
            "lib/png/pngread.c",
            "lib/png/pngrio.c",
            "lib/png/pngrtran.c",
            "lib/png/pngmem.c",
            "lib/png/pngerror.c",
            "lib/png/pngpread.c",
            # ZLIB:
            #######
            "lib/zlib/adler32.c",
            "lib/zlib/crc32.c",
            "lib/zlib/gzlib.c",
            "lib/zlib/gzread.c",
            "lib/zlib/infback.c",
            "lib/zlib/inflate.c",
            "lib/zlib/inftrees.c",
            "lib/zlib/inffast.c",
            "lib/zlib/uncompr.c",
            "lib/zlib/zutil.c",
            "lib/zlib/trees.c"
        ],
        'include_dirs': [
            '<!(node -e "require(\'nan\')")',
            'src/decoder',
            'lib/zlib',
            'lib/jpeg',
            'lib/cimg',
            'lib/png'
        ],
        'conditions': [
            ['OS=="linux"', {
                'cflags!': ['-fno-exceptions'],
                'cflags_cc!': ['-fno-exceptions'],
            }],
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                'include_dirs': ['/usr/include/malloc']
            }],
            ['OS=="win"', {
                'configurations': {
                    'Release': {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'ExceptionHandling': 1
                            }
                        }
                    }
                }
            }]
        ]
    },{
        "target_name": "lwip_encoder",
        "sources": [
            # LWIP: 
            #######
            "src/encoder/init.cpp",
            "src/encoder/jpeg_worker.cpp",
            "src/encoder/png_worker.cpp",
            # LIB JPEG:
            ###########
            "lib/jpeg/jdatadst.c",
            "lib/jpeg/jmemnobs.c",
            "lib/jpeg/jcomapi.c",
            "lib/jpeg/jerror.c",
            "lib/jpeg/jfdctflt.c",
            "lib/jpeg/jfdctfst.c",
            "lib/jpeg/jfdctint.c",
            "lib/jpeg/jidctflt.c",
            "lib/jpeg/jidctfst.c",
            "lib/jpeg/jidctint.c",
            "lib/jpeg/jutils.c",
            "lib/jpeg/jmemmgr.c",
            "lib/jpeg/jaricom.c",
            "lib/jpeg/jquant1.c",
            "lib/jpeg/jquant2.c",
            "lib/jpeg/jcapimin.c",
            "lib/jpeg/jcapistd.c",
            "lib/jpeg/jccoefct.c",
            "lib/jpeg/jccolor.c",
            "lib/jpeg/jcdctmgr.c",
            "lib/jpeg/jchuff.c",
            "lib/jpeg/jcinit.c",
            "lib/jpeg/jcmainct.c",
            "lib/jpeg/jcmarker.c",
            "lib/jpeg/jcmaster.c",
            "lib/jpeg/jcparam.c",
            "lib/jpeg/jcprepct.c",
            "lib/jpeg/jcsample.c",
            "lib/jpeg/jcarith.c",
            # LIB PNG:
            ##########
            "lib/png/png.c",
            "lib/png/pngset.c",
            "lib/png/pngget.c",
            "lib/png/pngtrans.c",
            "lib/png/pngmem.c",
            "lib/png/pngerror.c",
            "lib/png/pngwrite.c",
            "lib/png/pngwutil.c",
            "lib/png/pngwio.c",
            "lib/png/pngwtran.c",
            # ZLIB:
            #######
            "lib/zlib/adler32.c",
            "lib/zlib/crc32.c",
            "lib/zlib/gzlib.c",
            "lib/zlib/zutil.c",
            "lib/zlib/gzwrite.c",
            "lib/zlib/compress.c",
            "lib/zlib/deflate.c",
            "lib/zlib/trees.c"
        ],
        'include_dirs': [
            '<!(node -e "require(\'nan\')")',
            'src/encoder',
            'lib/zlib',
            'lib/jpeg',
            'lib/cimg',
            'lib/png'
        ],
        'conditions': [
            ['OS=="linux"', {
                'cflags!': ['-fno-exceptions'],
                'cflags_cc!': ['-fno-exceptions'],
            }],
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                'include_dirs': ['/usr/include/malloc']
            }],
            ['OS=="win"', {
                'configurations': {
                    'Release': {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'ExceptionHandling': 1
                            }
                        }
                    }
                }
            }]
        ]
    },{
        "target_name": "lwip_image",
        "sources": [
            # LWIP: 
            #######
            "src/image/init.cpp",
            "src/image/image.cpp",
            "src/image/resize_worker.cpp",
            "src/image/rotate_worker.cpp",
            "src/image/blur_worker.cpp",
            "src/image/crop_worker.cpp",
            "src/image/mirror_worker.cpp",
            "src/image/pad_worker.cpp",
            "src/image/sharpen_worker.cpp",
            "src/image/hsl_worker.cpp",
        ],
        'include_dirs': [
            '<!(node -e "require(\'nan\')")',
            'lib/cimg',
        ],
        'conditions': [
            ['OS=="linux"', {
                'cflags!': ['-fno-exceptions'],
                'cflags_cc!': ['-fno-exceptions'],
            }],
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                'include_dirs': ['/usr/include/malloc']
            }],
            ['OS=="win"', {
                'configurations': {
                    'Release': {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'ExceptionHandling': 1
                            }
                        }
                    }
                }
            }]
        ]
    }]
}
