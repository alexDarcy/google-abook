//#define BOOST_SPIRIT_DEBUG
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
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
    std::string name;
    std::string email;
    std::string mobile;
    std::string nick;
  };
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
  namespace fphoenix = boost::phoenix;

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

  // Our grammar (using a custom skipper to skip comments and empty lines )
  template <typename Iterator, typename skipper = comment_skipper<Iterator> >
    struct abook_parser : qi::grammar<Iterator, book(), skipper>
  {
    abook_parser() : abook_parser::base_type(start)
    {
      using qi::_val;
      using qi::_1;
      using qi::int_;
      using qi::eol;
      using standard_wide::char_;
      using phoenix::at_c;
      using phoenix::push_back;

      value = +(char_ - eol) [_val += _1] >> eol;

      name %= "name=" >> value;
      email %= "email=" >> value;
      mobile %= "mobile=" >> value;
      nick %= "nick=" >> value;

      entry = 
        "[" >> int_ >> "]" >> eol
        >> name [at_c<0>(_val) = _1]
        >> *email [at_c<1>(_val) = _1 ]
        >> *mobile [at_c<2>(_val) = _1 ]
        >> *nick [at_c<3>(_val) = _1 ];
      debug(entry);

      header = "[format]" >> eol
        >> "program=" >> value
        >> "version=" >> value;

      start = *header 
        >> *(entry [push_back(_val, _1)] >> *eol);
    }

    qi::rule<Iterator, std::string()> value;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> email;
    qi::rule<Iterator, std::string()> mobile;
    qi::rule<Iterator, std::string()> nick;
    qi::rule<Iterator, abook_entry()> entry;
    qi::rule<Iterator, book(), skipper> start;
    qi::rule<Iterator> header;
  };

}

int main(int argc, char *argv[]) {
  // To print the struct easily
  using namespace boost::fusion;

  typedef std::string::const_iterator iterator_type;
  typedef abook::abook_parser<iterator_type> abook_parser;
  typedef abook::comment_skipper<iterator_type> comment_skipper;

  abook_parser g; // Our grammar
  comment_skipper skip;
  std::string str1, str2;
  abook::book list; // Struct to save data
  if (argc != 2) {
    std::cout << "Need a filename as argument" << std::endl;
    return 1;
  }
  std::ifstream file(argv[1], std::ios_base::in);

  if (!file) {
    std::cout << "Failed to open file" << std::endl;
    return 1;
  }


  // Read file into buffer
  std::string buffer; 
  file.unsetf(std::ios::skipws); // No white space skipping!
  std::copy(
      std::istream_iterator<char>(file),
      std::istream_iterator<char>(),
      std::back_inserter(buffer));

  std::string::const_iterator begin = buffer.begin();
  std::string::const_iterator end = buffer.end();

  bool r = phrase_parse(begin, end, g, skip, list);
  //bool r = phrase_parse(begin, end, g, skip, list);
  if (r && begin == end) {
    std::cout << "ok" <<  std::endl;
    for (std::vector<abook::abook_entry>::iterator it = list.begin() ; it != list.end(); ++it)
      std::cout << (*it) << std::endl;
  }
  else
    std::cout << "failed" << std::endl;

  return 0;
}
