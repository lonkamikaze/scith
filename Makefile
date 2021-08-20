CXXFLAGS+=      -std=c++20 -Wall -Werror
CXXFLAGS+=      -march=native -O2 -pipe
CXXFLAGS_clang= -ferror-limit=1
CXXFLAGS_gcc=   -fmax-errors=1

CXXVER!=        ${CXX} --version
CXXFLAGS+=      ${CXXFLAGS_${CXXVER:Mg++*:U:C/.+/gcc/}${CXXVER:Mclang:U}}

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
