import os

env = Environment()
Export('env')

env.Append(LIBS = [
    'boost_system',
    'boost_log_setup-mt',
    'boost_log-mt',
    'boost_thread-mt',
	'boost_filesystem',
	'boost_serialization',
	'opencv_highgui',
	'opencv_core',
	'opencv_videoio',
	'opencv_imgproc',
	'opencv_imgcodecs'
])

# Source related instruction
sourceRootPath = './src/'

rootSource 		= SConscript(os.path.join(sourceRootPath, 'SConscript'))
movieSource 	= SConscript(os.path.join(sourceRootPath, 'movie/', 'SConscript'))
networkSource 	= SConscript(os.path.join(sourceRootPath, 'network/', 'SConscript'))

source 	= rootSource + movieSource + networkSource
app 	= env.Program(target = './bin/server', source = source)