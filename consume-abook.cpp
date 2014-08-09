//#define BOOST_SPIRIT_DEBUG
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/fusion/include/io.hpp>
#include <iostream>
#include <fstream>

/* Parse a file in abook format 
 * standard_wide is used to manage accents. */

/* Structure to store data */
namespace abook 
{
  struct abook_entry
  {
    abook_entry()  {}
    abook_entry(std::string n, std::string e): name(n), email(e) {}

    std::string name;
    std::string email;
    std::string mobile;
    std::string nick;
  };

  // the streaming operator needed for output
  std::ostream&
  operator<< (std::ostream& os, abook_entry const& e)
  {
    os << "--------Entry---------" << std::endl;
    os << "name=" << e.name << std::endl;
    os << "mail=" << e.email << std::endl;
    os << "mobile=" << e.mobile << std::endl;
    os << "nick=" << e.nick;
    return os;
  }

  typedef std::vector<abook_entry> book;
}

BOOST_FUSION_ADAPT_STRUCT(
    abook::abook_entry,
    (std::string, name)
    (std::string, email)
    (std::string, mobile)
    (std::string, nick)
    )

  namespace abook
{
  namespace qi = boost::spirit::qi;
  namespace standard_wide = boost::spirit::standard_wide;
  namespace phoenix = boost::phoenix;

  // Skip comments and empty lines
  template<typename Iterator>
    struct comment_skipper : public qi::grammar<Iterator> {

      qi::rule<Iterator> skip;

      comment_skipper() : comment_skipper::base_type(skip)
      {
        using namespace qi;
        skip = (lit("#") >> *(standard::char_ - eol) >> eol) 
          | +(*(standard_wide::blank) >> eol);
        //debug(skip);
      }
    };  

  // Our parser (using a custom skipper to skip comments and empty lines )
  template <typename Iterator, typename skipper = comment_skipper<Iterator> >
    struct abook_parser : qi::grammar<Iterator, book(), skipper>
  {
    abook_parser() : abook_parser::base_type(book_g)
    {
      using qi::_val;
      using qi::_1;
      using qi::_2;
      using qi::_3;
      using qi::_4;
      using qi::int_;
      using qi::eol;
      using qi::fail;
      using qi::on_error;
      using standard_wide::char_;
      using phoenix::at_c;
      using phoenix::push_back;
      using phoenix::construct;
      using phoenix::val;

      // We read any charactor different from eol
      value = +(char_ - eol) [_val += _1] >> eol;

      name %= "name=" >> value;
      email %= "email=" >> value;
      mobile %= "mobile=" >> value;
      nick %= "nick=" >> value;

      // Only the name and the number are mandatory 
      entry = 
        "[" >> int_ >> "]" >> eol
        > name [at_c<0>(_val) = _1]
        >> *email [at_c<1>(_val) = _1 ]
        >> *mobile [at_c<2>(_val) = _1 ]
        >> *nick [at_c<3>(_val) = _1 ];
      //debug(entry);

      header = "[format]" >> eol
        >> "program=" >> value
        >> "version=" >> value;

      book_g = *header 
        >> *(entry [push_back(_val, _1)] >> *eol);

      // Names for error handling
      book_g.name("book_g");
      header.name("header");
      name.name("name");

      on_error<fail>
        (
         book_g
         , std::cout
         << val("Error! Expecting ")
         << _4                               // what failed?
         << val(" here: \"")
         << construct<std::string>(_3, _2)   // iterators to error-pos, end
         << val("\"")
         << std::endl
        );

    }

    qi::rule<Iterator, std::string()> value;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> email, mobile, nick;
    qi::rule<Iterator, abook_entry()> entry;
    qi::rule<Iterator, book(), skipper> book_g;
    qi::rule<Iterator> header;
  };

  // Generator
  template <typename OutputIterator>
    bool generate_book(OutputIterator& sink, book const& b)
    {
      using boost::spirit::karma::stream;
      using boost::spirit::karma::generate;
      using boost::spirit::eol;

      bool r = generate(sink,
          stream % eol,
          b
          );
      return r;
    }

  void write_to_file(book &list) {
    //std::ofstream file("test.out", std::ios_base::out);
    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);

    if (!abook::generate_book(sink, list))
      std::cout << "Generating failed\n";
    else
      std::cout << "Generated: " << generated << "\n";
  }
}

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

int parse_abook_file(char* fname, abook::book& list) {
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

  bool r = phrase_parse(begin, end, g, skip, list);

  return r && begin == end;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "Need a filename as argument" << std::endl;
    return 1;
  }

  abook::book list; // Struct to save data
  int res = parse_abook_file(argv[1], list);

  if (res) {
    std::cout << "full match" <<  std::endl;
    write_to_file(list);
  }
  else
    std::cout << "parsing failed" << std::endl;

  return 0;
}
