#CXX=clang++
#CXX=g++
ifdef BOOST_DIR
CPPFLAGS=-I${BOOST_DIR}
endif
CPPFLAGS+=-Wall -g -std=c++11 

all: google-abook

abook: addressbook google_convert.csv 

addressbook: google_convert.csv google-abook
	./google-abook $< $@

CSV=google.csv

google_convert.csv: 
ifneq ("$(wildcard ${CSV})","")
	@echo "Converting from UTF-16 to UTF-8"
	iconv -f UTF-16 -t UTF-8 ${CSV} -o $@
else
	@echo "Missing file ${CSV}"
endif

google-abook: main.o 
	${CXX} ${CPPFLAGS} -o $@ $<

main.o: google.hpp abook.hpp types.hpp common.hpp

clean:
	rm -f *.o google-abook
	rm google_convert.csv
