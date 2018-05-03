import os

env = Environment()

env.Append(LIBS = [
    'boost_system',
    'boost_log_setup-mt',
    'boost_log-mt',
    'boost_thread-mt',
	'boost_filesystem',
	'boost_serialization'
])

source 	= env.Glob('./src/*.cpp')
app 	= env.Program(target = './bin/server', source = source)