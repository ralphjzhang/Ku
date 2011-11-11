import os

env = Environment(
    CPPPATH = ['.'],
    LIBS = ['pthread'],
    CCFLAGS = '-Wall --std=c++0x -g -fPIC'
    )

Export('env')

SConscript('ku/fusion/SConscript')
SConscript('ku/log/SConscript')

