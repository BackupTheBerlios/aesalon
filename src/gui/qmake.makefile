QMAKE=/usr/bin/qmake

all: generate_project generate_makefile

generate_project:
	$(QMAKE) -project && echo "Generated project"

generate_makefile: generate_project
	$(QMAKE)

