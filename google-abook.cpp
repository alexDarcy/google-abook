//#define BOOST_SPIRIT_DEBUG
#include "google.hpp"

/* Convert a file in abook format to google contacts format using the Spirit and
 * Karma Boost libraries */

int parse_abook_file(char* fname, abook::addressbook& mybook) {
  typedef std::string::const_iterator iterator_type;
  typedef abook::abook_parser<iterator_type> abook_parser;
  typedef abook::comment_skipper<iterator_type> comment_skipper;

  abook_parser g; // Our grammar
  comment_skipper skip;

  std::string buffer; 
  if (! read_file_to_buffer(fname, buffer))
    return 0;

  std::string::const_iterator begin = buffer.begin();
  std::string::const_iterator end = buffer.end();

  bool r = phrase_parse(begin, end, g, skip, mybook);

  return r && begin == end;
}

void abook_to_google(char* in, char* out) {
  abook::addressbook mybook; // Struct to save data
  int res = parse_abook_file(in, mybook);

  if (res) {
    std::cout << "full match" <<  std::endl;
    //write_to_file(mybook);
    google::write_contacts(out, mybook);
  }
  else
    std::cout << "parsing failed" << std::endl;
}

void google_to_abook(char* in, char* out) {
  google::addressbook mybook; // Struct to save data
  int res = parse_google_file(in, mybook);

  if (res) {
    std::cout << "full match" <<  std::endl;
  }
  else
    std::cout << "parsing failed" << std::endl;
}


int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Usage:" << std::endl;
    std::cout << "consume-abook input output" << std::endl;
    return 1;
  }

  //abook_to_google(argv[1], argv[2]);
  google_to_abook(argv[1], argv[2]);

  return 0;
}
