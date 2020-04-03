{
    "targets": [{
        "target_name": "testaddon",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "cppsrc/main.cpp",
			"cppsrc/player.cpp",
			"cppsrc/world.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'cflags!': [ '-fno-exceptions' ],
		'cflags_cc!': [ '-fno-exceptions' ],
		'xcode_settings': {
			'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
			'CLANG_CXX_LIBRARY': 'libc++',
			'MACOSX_DEPLOYMENT_TARGET': '10.7',
		},
		'msvs_settings': {
			'VCCLCompilerTool': { 'ExceptionHandling': 1 },
		},
    }]
}