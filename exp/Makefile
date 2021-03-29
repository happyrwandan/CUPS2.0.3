OBJS = exp.o http.o ipp.o ipp_parser.o loader.o network.o membuf.o util.o
CC=gcc
CXX=clang++
CFLAGS=-Wno-pointer-sign
CXXFLAGS=-Wno-write-strings -std=c++0x -stdlib=libc++
LIBS=-lcrypto -lc++
all: exp 

exp: $(OBJS)
	$(CC) -o exp $(OBJS) $(LIBS)

clean:
	rm -f exp *.o

exp.o: exp.h http.h ipp.h ipp_parser.h ipp_constants.h network.h membuf.h util.h
http.o: http.h ipp.h ipp_constants.h network.h membuf.h util.h 
ipp.o: ipp.h ipp_parser.h ipp_constants.h util.h http.h network.h membuf.h
ipp_parser.o: ipp_parser.h ipp_constants.h
loader.o: loader.h membuf.h
network.o: network.h membuf.h
util.o: util.h
membuf.o: membuf.h
