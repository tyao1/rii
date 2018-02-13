cxx_binary(
    name = 'mouse',
    srcs = [
        'examples/mouse.cpp',
    ],
    headers = [
        'mouse.hpp',
        'device.hpp',
    ],
    linker_flags = [
    ],
    compiler_flags = [
        '-std=c++11',
    ],
)


cxx_binary(
    name = 'gamepad',
    srcs = [
        'examples/gamepad.cpp',
    ],
    headers = [
        'device.hpp',
        'gamepad.hpp',
    ],
    linker_flags = [
    ],
    compiler_flags = [
        '-std=c++11',
    ],
)

cxx_binary(
  name = 'asio',
  srcs = ['server.cpp'],
  headers = [
    'device.hpp',
    'gamepad.hpp',
  ],
  deps = [],
  visibility = ['PUBLIC'],
  linker_flags = [
    '-lpthread',
    '-lboost_thread-mt',
    '-lboost_system',
  ],
  compiler_flags = [
    '-std=c++11',
    #'-lboost_thread-mt',

  ]
)
