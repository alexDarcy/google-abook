#CXX=clang
CXX=g++

consume-abook: consume-abook.o
	${CXX} -o $@ $<
