import os

env = Environment(
    CPPPATH = ['#.'],
    LIBS = ['pthread'],
    CCFLAGS = '-Wall --std=c++0x -g -fPIC'
    )

Export('env')

SConscript('ku/fusion/SConscript', variant_dir = 'build/fusion', duplicate = 0)
SConscript('ku/log/SConscript', variant_dir = 'build/log', duplicate = 0)
SConscript('ku/lua/SConscript', variant_dir = 'build/lua', duplicate = 0)

