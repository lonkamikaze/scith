CXX=       clang++12
CXXFLAGS+= -std=c++20 -Wall -Werror -ferror-limit=1
CXXFLAGS+= -march=native -O2 -pipe

.SUFFIXES: .o .pcm .cppm

.cppm.pcm:
	${CXX} ${CXXFLAGS} --precompile ${>:C/^/-fmodule-file=/:M*.pcm} $<

all: traits.o itraits.o iaccess.o parse.o ivec.o

depend.mk: src/*.[hc]pp
	${CXX} ${CXXFLAGS} -M ${>:M*.cpp} > $@

depend: depend.mk .PHONY

clean:
	rm -f *.o *.pcm

*.o *.pcm: Makefile
