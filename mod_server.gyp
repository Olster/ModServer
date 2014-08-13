{
  'target_defaults': {
    'include_dirs': ['.'],
    'conditions': [
      ['OS == "win"', {
        'defines': [
          'WIN32'
        ]
      }],
      ['OS == "linux"', {
        'defines': [
          'UNIX'
        ],
        'cflags': [
          '-Wall',
          '-Wextra',
          '-Weffc++',
          '-std=c++11'
        ],
      }],
    ],
    'msvs_settings': {
      'VCCLCompilerTool': {
        'WarningLevel': '4',
        'WarnAsError': 'true',
        'ExceptionHandling': '1'
      },
    },
    'msvs_disabled_warnings': [
      # Unsafe function snprintf().
      4996,

      # Conditional expression is constant.
      4127
    ],
  },
  'targets': [
    {
      'target_name': 'mod_server',
      'type': 'executable',
      'defines': [
        # Setting fd_set size higher to support more client.
        # TODO(Olster): Move to Win configuration when Unix uses epoll().
        'FD_SETSIZE=128',
        
        # Make targets for debug and release.
        'DEBUG'
      ],
      'include_dirs': [
        '.'
      ],
      'sources': [
        # Adding headers here as well make them visiable in Visual Studio.
	'base/build_required.h',
        'base/dynamic_lib.h',
        'base/logger.cpp',
        'base/logger.h',
        'net/socket/socket.cpp',
        'net/socket/socket.h',
        'net/socket/tcp_listener.cpp',
        'net/socket/tcp_listener.h',
        'net/socket/tcp_socket.cpp',
        'net/socket/tcp_socket.h',
        'net/ip_endpoint.cpp',
        'net/ip_endpoint.h',
        'net/server.cpp',
        'net/server.h',
        'net/session.cpp',
        'net/session.h',
        'net/tcp_session.cpp',
        'net/tcp_session.h',
        'server_plugin/plugin_loader.cpp',
        'server_plugin/plugin_loader.h',
        'server_plugin/server_plugin.cpp',
        'server_plugin/server_plugin.h',
        'main.cpp'
      ],
      'xcode_settings': {
        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
        'OTHER_CPLUSPLUSFLAGS': [ '-stdlib=libc++' ]
      },
      'conditions': [
        ['OS == "linux"', {
          'sources': [
            'base/dynamic_lib_unix.cpp',
            'base/os_info_unix.cpp'
          ],
        }],
        ['OS == "win"', {
          'sources': [
            # gyp didn't parse OS properly.
            'base/dynamic_lib_win.cpp',
            'base/path_win.cpp',
            'base/os_info_win.cpp',
            'net/socket/socket_win.cpp'
          ],
        }],
      ],
    },
    {
      'target_name': 'http',
      # Can set to |<(library)|
      'type': 'shared_library',
      'defines': [
        'PLUGIN_IMPLEMENTATION'
      ],
      'include_dirs': [
        '.'
      ],
      'sources': [
        'server_plugin/http/http_handler.cpp',
        'server_plugin/http/http_plugin.cpp'
      ],
      'xcode_settings': {
        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
        'OTHER_CPLUSPLUSFLAGS': [ '-stdlib=libc++' ]
      },
    },
    {
      'target_name': 'run_tests',
      'type': 'executable',
      'dependencies': [
        'gtest'
      ],
      'sources': [
        'base/dynamic_lib_unittest.cpp',
        'net/socket/socket.cpp',
        'net/socket/socket_unittest.cpp',
        'net/ip_endpoint_unittest.cpp',
        'net/ip_endpoint.cpp',
        'run_tests.cpp'
      ],
      'include_dirs': [
        '.'
      ],
      'xcode_settings': {
        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
        'OTHER_CPLUSPLUSFLAGS': [ '-stdlib=libc++' ]
      },
      'conditions': [
        ['OS == "linux"', {
          'defines': [
            'UNIX'
          ],
          'cflags': [
            '-Wall',
            '-Wextra',
            '-Weffc++',
            # Use C++11
            '-std=c++11',
            '-g'
          ],
          'link_settings': {
            'libraries': [
              # Provides .so functions (dlopen, etc).
              '-ldl',
              '-pthread'
            ]
          },
          'sources': [
            'base/dynamic_lib_unix.cpp',
          ],
        }],
        ['OS == "win"', {
          'defines': [
            'WIN32'
          ],
          'sources': [
            'base/dynamic_lib_win.cpp',
            'net/socket/socket_win.cpp'
          ],
        }],
      ],
    },
    {
      'target_name': 'gtest',
      'type': 'static_library',
      'sources': [
        'gtest/src/gtest-death-test.cc',
        'gtest/src/gtest-internal-inl.h',
        'gtest/src/gtest-filepath.cc',
        'gtest/src/gtest_main.cc',
        'gtest/src/gtest-port.cc',
        'gtest/src/gtest-printers.cc',
        'gtest/src/gtest-test-part.cc',
        'gtest/src/gtest-typed-test.cc',
        'gtest/src/gtest.cc'
      ],
      'include_dirs': [
        '.',
        'gtest',
        'gtest/include'
      ],
      'direct_dependent_settings': {
        'defines': [
          'UNIT_TEST'
        ],
        'include_dirs': [
          'gtest/include'
        ],
      },
    },
  ],
}
