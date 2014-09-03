{
    "targets": [{
        "target_name": "lwip",
        "sources": [
            # LWIP: 
            #######
            "src/lwip.cpp",
            "src/LwipImage.cpp",
            # LIB JPEG:
            ###########
            "lib/jpeg/jmemnobs.c",
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
            "lib/jpeg/jcomapi.c",
            "lib/jpeg/jcparam.c",
            "lib/jpeg/jcprepct.c",
            "lib/jpeg/jcsample.c",
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
            "lib/jpeg/jcarith.c",
            "lib/jpeg/jdarith.c",
            "lib/jpeg/jquant1.c",
            "lib/jpeg/jquant2.c",
            "lib/jpeg/jdmerge.c",
            "lib/jpeg/jaricom.c",
            # LIB PNG:
            ##########
            "lib/png/png.c",
            "lib/png/pngset.c",
            "lib/png/pngget.c",
            "lib/png/pngrutil.c",
            "lib/png/pngtrans.c",
            "lib/png/pngwutil.c",
            "lib/png/pngread.c",
            "lib/png/pngrio.c",
            "lib/png/pngwio.c",
            "lib/png/pngwrite.c",
            "lib/png/pngrtran.c",
            "lib/png/pngwtran.c",
            "lib/png/pngmem.c",
            "lib/png/pngerror.c",
            "lib/png/pngpread.c",
            # ZLIB:
            #######
            "lib/zlib/adler32.c",
            "lib/zlib/compress.c",
            "lib/zlib/crc32.c",
            "lib/zlib/deflate.c",
            "lib/zlib/gzclose.c",
            "lib/zlib/gzlib.c",
            "lib/zlib/gzread.c",
            "lib/zlib/gzwrite.c",
            "lib/zlib/infback.c",
            "lib/zlib/inflate.c",
            "lib/zlib/inftrees.c",
            "lib/zlib/inffast.c",
            "lib/zlib/trees.c",
            "lib/zlib/uncompr.c",
            "lib/zlib/zutil.c"
        ],
        'include_dirs': ['lib/zlib', 'lib/jpeg', 'lib/cimg', 'lib/png'],
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
