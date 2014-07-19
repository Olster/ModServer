{
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
        'base/logger.cpp',
        'net/socket/tcp_listener.cpp',
        'net/socket/tcp_socket.cpp',
        'net/ip_endpoint.cpp',
        'net/server.cpp',
        'net/session.cpp',
        'net/tcp_session.cpp',
        'server_plugin/plugin_loader.cpp',
        'server_plugin/server_plugin.cpp',
        'main.cpp'
      ],
      'xcode_settings': {
        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
        'OTHER_CPLUSPLUSFLAGS': [ '-stdlib=libc++' ]
      },
      'conditions': [
        ['OS == "linux"', {
          'cflags': [
            #'-Wall',
            #'-Wextra',
            #'-Weffc++',
            # Use C++11
            '-std=c++11',
            '-g'
          ],
          'link_settings': {
            'libraries': [
              # Provides .so functions (dlopen, etc).
              '-ldl'            
            ]
          },
          'defines': [
            'UNIX'
          ],
          'sources': [
            'base/dynamic_lib_unix.cpp',  
            'net/socket/socket_unix.cpp'       
          ],
        }],
        ['OS == "win"', {
          'defines': [
            'WIN32'
          ],
          'sources': [
            # gyp didn't parse OS properly.
            'base/dynamic_lib_win.cpp',
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
      'conditions': [
        ['OS == "linux"', {
          'defines': [
            'UNIX'
          ],
          'xcode_settings': {
            'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
            'OTHER_CPLUSPLUSFLAGS': [ '-stdlib=libc++' ]
          },
          'cflags': [
            #'-Wall',
            #'-Wextra',
            #'-Weffc++',
            # Use C++11
            '-std=c++11'
          ],
        }],
        ['OS == "win"', {
          'defines': [
            'WIN32'
          ]
        }]
      ],
    },
  ],
}
