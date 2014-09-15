#CXX=clang
CXX=g++
CPPFLAGS=#-Wall -Wextra


consume-abook: consume-abook.o
	${CXX} ${CPPFLAGS} -o $@ $<

clean:
	rm -f *.o consume-abook
