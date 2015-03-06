CXX=clang++
#CXX=g++
BOOST_DIR=/home/softwares/boost-1.56
CPPFLAGS=-I${BOOST_DIR} -Wall -g


google-abook: google-abook.o 
	${CXX} ${CPPFLAGS} -o $@ $<

google-abook.o: google.hpp abook.hpp

clean:
	rm -f *.o google-abook
