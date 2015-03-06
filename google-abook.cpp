//#define BOOST_SPIRIT_DEBUG
#include "google.hpp"

/* Convert a file in abook format to google contacts format using the Spirit and
 * Karma Boost libraries */


int read_file_to_buffer(char *fname, std::string &buffer) {
  std::ifstream file(fname, std::ios_base::in);

  if (!file) {
    std::cout << "Failed to open file" << std::endl;
    return 0;
  }

  // Read file into buffer
  file.unsetf(std::ios::skipws); // No white space skipping!
  std::copy(
      std::istream_iterator<char>(file),
      std::istream_iterator<char>(),
      std::back_inserter(buffer));
  return 1;
}

int parse_abook_file(char* fname, abook::book& mybook) {
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

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Usage:" << std::endl;
    std::cout << "consume-abook input output" << std::endl;
    return 1;
  }

  abook::book mybook; // Struct to save data
  int res = parse_abook_file(argv[1], mybook);

  if (res) {
    std::cout << "full match" <<  std::endl;
    //write_to_file(mybook);
    google::write_contacts(argv[2], mybook);
  }
  else
    std::cout << "parsing failed" << std::endl;

  return 0;
}