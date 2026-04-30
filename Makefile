DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS = -g -Wall -Wextra -O0 
    MODE_MSG = Debug Mode
else
    CXXFLAGS = -O3 -Wall -Wextra
    MODE_MSG = Release Mode
endif
SOURCES  = file.c helpers.c kes.c tester.c
OBJECTS  = $(SOURCES:.cpp=.o)

all: cipher.out file-kbc.out file-kh.out

build: file-kbc.out file-kh.out file-kbc.exe file-kh.exe

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

file-kh.exe: helpers.c helpers.h kh.c file-kh.c
	x86_64-w64-mingw32-gcc ${CXXFLAGS} -o file-kh.exe helpers.c helpers.h kh.c file-kh.c

file-kbc.out: kbc.o file-kbc.o helpers.o kh.o
	gcc ${CXXFLAGS} -o file-kbc.out kbc.o helpers.o file-kbc.o kh.o

file-kbc.exe: helpers.c helpers.h kbc.c kh.c file-kbc.c
	x86_64-w64-mingw32-gcc ${CXXFLAGS} -o file-kbc.exe helpers.c helpers.h kbc.c kh.c file-kbc.c

cipher.out: kbc.o tester.o helpers.o kh.o
	gcc ${CXXFLAGS} -o cipher.out kbc.o tester.o helpers.o kh.o

clean:
	rm -rf *.out *.o *.h.gch *.exe

cleanall: clean
	rm -rf vgcore.* *.kbc
