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

all: cipher.out file-kbc.out file-kh.out

file-kbc.o: file-kbc.c
	gcc ${CXXFLAGS} -c file-kbc.c

helpers.o: helpers.c helpers.h
	gcc ${CXXFLAGS} -c helpers.c

kbc.o: kbc.c kbc.h helpers.h
	gcc ${CXXFLAGS} -c kbc.c helpers.h

tester.o: tester.c helpers.h
	gcc ${CXXFLAGS} -c tester.c helpers.h

kh.o: kh.c kh.h
	gcc ${CXXFLAGS} -c kh.c

file-kh.o: file-kh.c
	gcc ${CXXFLAGS} -c file-kh.c

file-kh.out: kh.o file-kh.o helpers.o
	gcc ${CXXFLAGS} -o file-kh.out kh.o file-kh.o helpers.o

file-kbc.out: kbc.o file-kbc.o helpers.o
	gcc ${CXXFLAGS} -o file-kbc.out kbc.o helpers.o file-kbc.o

cipher.out: kbc.o tester.o helpers.o kh.o
	gcc ${CXXFLAGS} -o cipher.out kbc.o tester.o helpers.o kh.o

clean:
	rm -rf *.out *.o *.h.gch