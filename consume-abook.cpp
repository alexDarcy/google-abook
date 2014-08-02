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
  //typedef std::vector<std::string> abook;
  //
  //language review
  //
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
  namespace ascii = boost::spirit::ascii;
  namespace phoenix = boost::phoenix;
  namespace fphoenix = boost::phoenix;
  
  template <typename Iterator>
    //struct abook_parser : qi::grammar<Iterator, person()>
    struct abook_parser : qi::grammar<Iterator, book()>
  {
    abook_parser() : abook_parser::base_type(start)
    {
      using qi::_val;
      using qi::_1;
      using qi::int_;
      using qi::eol;
      using ascii::char_;
      using phoenix::at_c;
      using phoenix::push_back;

      value = +(char_ - eol) [_val += _1] >> eol;

      name %= "name=" >> value;
      email %= "email=" >> value;
      mobile %= "mobile=" >> value;
      nick %= "nick=" >> value;

      pair = 
        "[" >> int_ >> "]" >> eol
        >> name [at_c<0>(_val) = _1]
        >> *email [at_c<1>(_val) = _1 ]
        >> *mobile [at_c<2>(_val) = _1 ]
        >> *nick [at_c<3>(_val) = _1 ]
        // Empty lines
      >> +(*(ascii::blank) >> eol);
      //debug(pair);
      start = 
        *(pair [push_back(_val, _1)] >> *eol);
    }

    qi::rule<Iterator, std::string()> value;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> email;
    qi::rule<Iterator, std::string()> mobile;
    qi::rule<Iterator, std::string()> nick;
    qi::rule<Iterator, abook_entry()> pair;
    qi::rule<Iterator, book()> start;
  };

}

int main() {
  // To print the struct easily
  using namespace boost::fusion;

  typedef std::string::const_iterator iterator_type;
  typedef abook::abook_parser<iterator_type> abook_parser;

  abook_parser g; // Our grammar
  std::string str1, str2;
  abook::book list; // Struct to save data
  std::ifstream file("test.dat", std::ios_base::in);

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

  std::string::const_iterator iter = buffer.begin();
  std::string::const_iterator end = buffer.end();

  bool r = parse(iter, end, g, list);
  if (r && iter == end) {
    std::cout << "ok" <<  std::endl;
    for (std::vector<abook::abook_entry>::iterator it = list.begin() ; it != list.end(); ++it)
      std::cout << (*it) << std::endl;
  }
  else
    std::cout << "failed" << std::endl;

  return 0;
}
