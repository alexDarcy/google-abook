CXX=clang++
#CXX=g++
ifdef BOOST_DIR
CPPFLAGS=-I${BOOST_DIR}
endif
CPPFLAGS+=-Wall -g -std=c++11 

IN=google.csv
CSV=google_utf8.csv

all: google-abook ${CSV} addressbook

abook: addressbook ${CSV}

addressbook: ${CSV} google-abook
	./google-abook $< $@


convert:
ifneq ("$(wildcard ${IN})","")
	@echo "Converting from UTF-16 to UTF-8"
	iconv -f UTF-16 -t UTF-8 ${IN} -o ${CSV}
else
	@echo "Missing file ${IN}"
endif

google-abook: main.o 
	${CXX} ${CPPFLAGS} -o $@ $<

main.o: google.hpp abook.hpp types.hpp common.hpp

clean:
	rm -f *.o google-abook
	rm google_utf8.csv
