import sys
import os

colors = {}
colors["cyan"]   = '\033[96m'
colors["purple"] = '\033[95m'
colors["blue"]   = '\033[94m'
colors["green"]  = '\033[92m'
colors["yellow"] = '\033[93m'
colors["red"]    = '\033[91m'
colors["end"]    = '\033[0m'

#If the output is not a terminal, remove the colors
if not sys.stdout.isatty():
   for key, value in colors.iteritems():
      colors[key] = ''

compile_source_message = '%sCompiling %s==> %s$SOURCE%s' % \
   (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

compile_shared_source_message = '%sCompiling shared %s==> %s$SOURCE%s' % \
   (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

link_program_message = '%sLinking Program %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_library_message = '%sLinking Static Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

ranlib_library_message = '%sRanlib Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_shared_library_message = '%sLinking Shared Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

env = Environment(
	CXXCOMSTR = compile_source_message,
	CCCOMSTR = compile_source_message,
	SHCCCOMSTR = compile_shared_source_message,
	SHCXXCOMSTR = compile_shared_source_message,
	ARCOMSTR = link_library_message,
	RANLIBCOMSTR = ranlib_library_message,
	SHLINKCOMSTR = link_shared_library_message,
	LINKCOMSTR = link_program_message,
	
	ENV = {'PATH' : os.environ['PATH'],
		'TERM' : os.environ['TERM'],
		'HOME' : os.environ['HOME']})

Export('env')

def createConfig(target, source, values):
	outFile = file(str(target), "w")
	inFile = file(str(source), "r")
	outFile.write(inFile.read() % values)
	inFile.close()
	outFile.close()

config = file("build/config", "r")
buildConfig = eval(config.read())
config.close()

createConfig("include/common/Config.h", "build/templates/Config.h.in", buildConfig)

SConscript("monitor/SConscript")
SConscript("modules/SConscript")