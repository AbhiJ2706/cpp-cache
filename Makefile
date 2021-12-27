CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Og
EXEC = main
OBJECTS = main.o cache.o entry.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}