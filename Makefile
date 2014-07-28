#CXX=clang
CXX=g++

consume-abook: consume-abook.o
	${CXX} -o $@ $<

clean:
	rm -f *.o consume-abook
