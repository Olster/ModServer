# scons "Make" file for Unix.

env = Environment()

env.Program(target = 'stHTTPs',
            source = ['main.cpp',
                      'base/logger.cpp',
                      'net/socket/socket_unix.cpp',
                      'net/socket/server_socket.cpp',
                      'net/socket/tcp_socket.cpp',
                      'net/http_connection.cpp',
                      'net/http_request.cpp',
                      'net/http_response.cpp',
                      'net/http_server.cpp',
                      'resource/resource.cpp'])
                      
env.Append(CCFLAGS = ['-Wall', '-std=c++11', '-ggdb'])
env.Append(CPPPATH = '.')
env.Append(CPPDEFINES = ['UNIX'])
