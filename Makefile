DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS = -g -Wall -Wextra -O0 
    MODE_MSG = "Debug Mode"
else
    CXXFLAGS = -O3 -Wall -Wextra
    MODE_MSG = "Release Mode"
endif
SOURCES  = file.c helpers.c kes.c tester.c
OBJECTS  = $(SOURCES:.cpp=.o)

all: cipher.out file.out

file.o: file.c
	gcc ${CXXFLAGS} -c file.c

helpers.o: helpers.c helpers.h
	gcc ${CXXFLAGS} -c helpers.c

kbc.o: kbc.c kbc.h
	gcc ${CXXFLAGS} -c kbc.c

tester.o: tester.c
	gcc ${CXXFLAGS} -c tester.c

file.out: kbc.o file.o helpers.o
	gcc ${CXXFLAGS} -o file.out kes.o helpers.o file.o

cipher.out: kbc.o tester.o helpers.o
	gcc ${CXXFLAGS} -o cipher.out kes.o tester.o helpers.o

clean:
	rm -rf *.out *.o