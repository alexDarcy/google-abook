//#define BOOST_SPIRIT_DEBUG
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <fstream>

/* Structure to store data */
namespace entity 
{
  struct person
  {
    std::string name;
    std::string email;
  };
}

BOOST_FUSION_ADAPT_STRUCT(
    entity::person,
    (std::string, name)
    (std::string, email)
)

namespace entity
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  template <typename Iterator>
    struct entry_parser : qi::grammar<Iterator, person()>
  {
    entry_parser() : entry_parser::base_type(start)
    {
      using qi::eps;
      using qi::lit;
      using qi::_val;
      using qi::_1;
      using qi::eol;
      using ascii::char_;

      start %= 
        "name=" >> + (char_ - eol) >> eol >> "email=" >> + (char_);
    }

    qi::rule<Iterator, person()> start;
  };

}

int main() {
  typedef std::string::const_iterator iterator_type;
  typedef entity::entry_parser<iterator_type> entry_parser;
  entry_parser g; // Our grammar
  std::string str1, str2;
  entity::person pers; // Struct to save data
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

  bool r = parse(iter, end, g, pers);
  if (r && iter == end) {
    std::cout << "ok" <<  std::endl;
    std::cout << "got: " << boost::fusion::as_vector(pers) << std::endl;
  }
  else
    std::cout << "failed" << std::endl;

  return 0;
}
