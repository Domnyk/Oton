import os

env = Environment()

env.Append(LIBS = [
	'boost_system',
	'boost_filesystem',
	'boost_serialization',
])

source 	= env.Glob('./src/*.cpp')
app 	= env.Program(target = './bin/server', source = source)