#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import string

config = {
	"project":		"Aesalon",
	"desc":			"a tool to visualize program behaviour in real time",
	"authors":		"Aesalon development team",
	"license":		"GNU GPLv3",
	"devyears":		"2009-2011",
	"website":		"http://aesalon.berlios.de/",
	"srcBase":		"src",
	"incBase":		"include",
	"guardBase":	"Aesalon",
	"fileHeader":
"""/** %(project)s, %(desc)s.
	Copyright (C) %(devyears)s, %(authors)s.
	
	%(project)s is distributed under the terms of the %(license)s. See
	the included file LICENSE for more information.
	
	@file %(path)s
*/
""",
	"cHeader":
"""
#ifndef %(includeGuard)s
#define %(includeGuard)s



#endif
""",

	"cSource":
"""
#include "%(incPath)s"


""",
	"cppHeader":
"""
#ifndef %(includeGuard)s
#define %(includeGuard)s

""",
	"openNamespace":
"""
namespace %(namespace)s {
""",
	
	"cppClassDef":
"""
class %(className)s {
public:
	%(className)s();
	~%(className)s();
};
""",
	"closeNamespace":
"""
} // namespace %(namespace)s
""",
	"cppHeaderEnd":
"""
#endif

"""
}

class Generator:
	def __init__(self):
		self.__name = "Unknown name"
		pass

	def name(self):
		return self.__name
	def setName(self, name):
		self.__name = name

	def generate(self, name):
		pass

class CHeader(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("cheader")
	
	def generate(self, name):
		path = name
		path += ".h"
		
		fp = file(os.path.join(config["incBase"], path), "w")
		
		includeGuard = config["guardBase"] + " " + name.replace("/", " ")
		while True:
			index = includeGuard.find(" ")
			if index == -1: break
			
			includeGuard = includeGuard[:index] + includeGuard[index+1].upper() + includeGuard[index+2:]
		
		includeGuard += "_H"
		
		fileConfig = config
		fileConfig["path"] = path
		fileConfig["includeGuard"] = includeGuard
		
		fp.write(config["fileHeader"] % fileConfig)
		fp.write(config["cHeader"] % fileConfig)
		
		fp.close()
		
		return path

class CSource(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("csource")
	
	def generate(self, name, incPath=""):
		path = os.path.join(config["srcBase"], name)
		path += ".c"
		
		fp = file(path, "w")
		
		fileConfig = config
		fileConfig["path"] = path
		fileConfig["incPath"] = incPath
		
		fp.write(fileConfig["fileHeader"] % fileConfig)
		if incPath != "":
			fp.write(fileConfig["cSource"] % fileConfig)
		
		fp.close()

class CGenerator(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("c")
	
	def generate(self, name):
		CHeader().generate(name)
		CSource().generate(name, True)



class CppHeader(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("cppheader")
	
	def generate(self, name):
		path = name
		path = path.replace("::", "/")
		last = path.rfind("/")
		if last != -1:
			path = path[:last].lower() + path[last:]
		path += ".h"
		filepath = os.path.join(config["incBase"], path)
		
		fp = file(filepath, "w")
		
		includeGuard = config["guardBase"] + " " + name.replace("/", "_")
		includeGuard = includeGuard.replace("::", "_")
		while True:
			index = includeGuard.find(" ")
			if index == -1: break
			
			includeGuard = includeGuard[:index] + includeGuard[index+1].upper() + includeGuard[index+2:]
		
		includeGuard += "_H"
		
		fileConfig = config
		fileConfig["path"] = filepath
		fileConfig["includeGuard"] = includeGuard
		
		fp.write(config["fileHeader"] % fileConfig)
		fp.write(config["cppHeader"] % fileConfig)
		
		namespaceList = name.split("::")
		if len(namespaceList) != 1:
			className = namespaceList[-1]
			namespaceList = namespaceList[:-1]
			for namespace in namespaceList:
				fileConfig["namespace"] = namespace
				fp.write(fileConfig["openNamespace"] % fileConfig)
			
			fileConfig["className"] = className
			fp.write(fileConfig["cppClassDef"] % fileConfig)
			
			for namespace in namespaceList[::-1]:
				fileConfig["namespace"] = namespace
				fp.write(fileConfig["closeNamespace"] % fileConfig)
		
		fp.write(config["cppHeaderEnd"] % fileConfig)
		
		fp.close()
		
		return path

class CppSource(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("cppsource")
	
	def generate(self, name, incPath=""):
		path = os.path.join(config["srcBase"], name)
		path = path.replace("::", "/")
		last = path.rfind("/")
		if last != -1:
			path = path[:last].lower() + path[last:]
		path += ".cpp"
		
		fp = file(path, "w")
		
		fileConfig = config
		fileConfig["path"] = path
		fileConfig["incPath"] = incPath
		
		fp.write(fileConfig["fileHeader"] % fileConfig)
		if incPath != "":
			fp.write(fileConfig["cSource"] % fileConfig)
		
		namespaceList = name.split("::")
		if len(namespaceList) != 1:
			className = namespaceList[-1]
			namespaceList = namespaceList[:-1]
			for namespace in namespaceList:
				fileConfig["namespace"] = namespace
				fp.write(fileConfig["openNamespace"] % fileConfig)
			
			fp.write("\n\n")
			
			for namespace in namespaceList[::-1]:
				fileConfig["namespace"] = namespace
				fp.write(fileConfig["closeNamespace"] % fileConfig)
		
		fp.close()

class CppGenerator(Generator):
	def __init__(self):
		Generator.__init__(self)
		self.setName("cpp")
	
	def generate(self, name):
		incPath = CppHeader().generate(name)
		CppSource().generate(name, incPath)



def main():
	generatorMap = dict()
	def addGenerator(generator):
		generatorMap[generator.name()] = generator
	
	addGenerator(CHeader())
	addGenerator(CSource())
	addGenerator(CGenerator())
	
	addGenerator(CppHeader())
	addGenerator(CppSource())
	addGenerator(CppGenerator())
	
	if len(sys.argv) < 3:
		print "Usage:", sys.argv[0], "generator name1 [name2 name3 . . .]"
		print "Where generator is one of:"
		for g in generatorMap.values():
			print "\t", g.name()
		return
	generator = generatorMap[sys.argv[1]]
	
	for name in sys.argv[2:]:
		generator.generate(name)

main()
