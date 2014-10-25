#CXX=clang
CXX=g++
CPPFLAGS=#-Wall -Wextra
CPPFLAGS+=-I/home/alex/softwares/boost-1.56/      


consume-abook: consume-abook.o 
	${CXX} ${CPPFLAGS} -o $@ $<

test: test.o 
	${CXX} ${CPPFLAGS} -o $@ $<
clean:
	rm -f *.o consume-abook
