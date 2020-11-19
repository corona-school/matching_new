{
  'variables': {
    # Putting a variables dict inside another variables dict looks kind of
    # weird.  This is done so that 'external_root', etc are defined as
    # variables within the outer variables dict here.  This is necessary
    # to get these variables defined for the conditions within this variables
    # dict that operate on these variables.
    'variables': {
      'variables': {
        'addon_root%': './src/algo/addon'
      },
      'addon_root%': '<(addon_root)',
      'code_root%': '<(addon_root)/code',
      'external_root%': '<(addon_root)/code/external',
    },
    'addon_root%': '<(addon_root)',
    'code_src%': '<(code_root)/src',
    'boost_include%': '<(external_root)/boost/include',
    'json_include%': '<(external_root)/json/include',
  },
  'targets': [
    {
      'target_name': 'corona_school_matching',
      'defines': [
        'DEFINE_FOO',
        'DEFINE_A_VALUE=value',
        'NAPI_DISABLE_CPP_EXCEPTIONS',
      ],
      'include_dirs': [
        '<!@(node -p "require(\'node-addon-api\').include")',
        '<@(boost_include)',
        '<@(json_include)',
        '<@(code_src)',
      ],
      'sources': [
        '<(addon_root)/index.cpp',
        '<(code_src)/graph_creator.cpp',
        '<(code_src)/matching.cpp'
      ],
      'cflags!': [ '-fno-exceptions',],
      'cflags_cc!': [ '-fno-exceptions',  ],
      'cflags_cc': [
        '-std=c++17' , # use c++17
      ],
      'linkflags': [
      '-lstdc++fs -lc++fs -ld',
      ],
      'conditions': [
        ['OS=="linux"', {
          'defines': [
            'LINUX_DEFINE',
          ],
          'include_dirs': [
            'include/linux',
          ],
        }],
        ['OS=="mac"', {
          'defines': [
            'MAC_DEFINE',
          ],
          'xcode_settings': { # define special things for macOS (in particular, without that, c++17 wouldn't work on macOS)
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.15',
            'CLANG_CXX_LANGUAGE_STANDARD': 'c++17',
            'CLANG_CXX_LIBRARY': 'libc++',
          },
        }],
        ['OS=="win"', {
          'defines': [
            'WINDOWS_SPECIFIC_DEFINE',
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [ '-std:c++17', '-IC:../<(boost_include)' ],
            },
          },
        }, { # OS != "win",
          'defines': [
            'NON_WINDOWS_DEFINE',
          ],
        }]
      ],
    },
  ],
}