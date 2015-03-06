CXX=clang++
#CXX=g++
CPPFLAGS=-I${BOOST_DIR} -Wall -g


google-abook: google-abook.o 
	${CXX} ${CPPFLAGS} -o $@ $<

google-abook.o: google.hpp abook.hpp

clean:
	rm -f *.o google-abook
