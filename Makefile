OBJ:=objs
CC:=gcc
CPP:=g++
LD:=ld
AS:=as
NASM:=nasm

SOURCE:=Source
LIBRARY:=include

MAIN:=Main.o
OUTPUT:=RNN_Test.exe

OBJS:= *.o
INCLUDED:=	-I$(LIBRARY) -I$(SOURCE)

CXXFLAGS:= -w -lstdc++ -O2 -std=gnu++11 -Wall -Wextra -g -D_GLIBCXX_DEBUG $(INCLUDED)
LDFLAGS+=-lutil -lboost_iostreams -lboost_system -lboost_filesystem
all: clean build-main clean run

build-main:	$(MAIN)
	$(CXX) -o $(OUTPUT) $^ $(LDFLAGS)
	echo -e "\n\n\n\n\tNeural Networks Built"

run:
	./$(OUTPUT)

clean:
	rm -f build-crt0 *.o */*.o */*/*.o
	rm -rfv objs

%.o: %.c
	$(CC) -c $< -o $@ $(CXXFLAGS)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)
