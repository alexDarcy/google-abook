#CXX=clang
CXX=g++
CPPFLAGS=#-Wall -Wextra
CPPFLAGS+=-I/home/alex/softwares/boost-1.56/      


consume-abook: consume-abook.o 
	${CXX} ${CPPFLAGS} -o $@ $<

consume-abook.o: google.hpp abook.hpp

clean:
	rm -f *.o consume-abook
