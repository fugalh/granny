import os
env = Environment(
    ENV=os.environ,
    CPPFLAGS=['-O2'],
    CXXFLAGS=['-std=c++11'],
    CPPPATH=['/usr/local/include', '/opt/local/include'],
    LIBPATH=['/usr/local/lib', '/opt/local/lib'],
    LIBS=['sndfile', 'lo', 'jack', 'zmq'],
    )

srcs = filter(
        lambda f: f.path != "main.cc",
        Glob("*.cc")
        )

env.Program('granny', srcs + ['main.cc'])

env.Append(LIBS=['gtest'])
env.Program('tests', srcs + Glob('test/*.cc'))

# vim: ft=python
