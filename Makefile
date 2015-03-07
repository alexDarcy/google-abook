CXX=clang++
#CXX=g++
CPPFLAGS=-I${BOOST_DIR} -Wall -g -std=c++11 -std=c++11

all: google_convert.csv google-abook

google_convert.csv: google.csv
	@echo "Converting from UTF-16 to UTF-8"
	iconv -f UTF-16 -t UTF-8 $< -o $@

google-abook: google-abook.o 
	${CXX} ${CPPFLAGS} -o $@ $<

google-abook.o: google.hpp abook.hpp

clean:
	rm -f *.o google-abook
