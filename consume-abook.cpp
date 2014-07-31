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
namespace entity 
{
  struct person
  {
    std::string name;
    std::string email;
  };
  typedef std::vector<person> list_person;
  //typedef std::vector<std::string> list_person;
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
  namespace phoenix = boost::phoenix;
  template <typename Iterator>
    //struct entry_parser : qi::grammar<Iterator, person()>
    struct entry_parser : qi::grammar<Iterator, list_person()>
  {
    entry_parser() : entry_parser::base_type(start)
    {
      using qi::_val;
      using qi::_1;
      using qi::eol;
      using ascii::char_;
      using phoenix::at_c;
      using phoenix::push_back;

      value = +(char_ - eol) [_val += _1];
      value_last = +(char_) [_val += _1];

      pair = 
        "name=" >> value [at_c<0>(_val) = _1] >> eol 
        >> "email=" >> value_last [at_c<1>(_val) = _1];
      start = pair [push_back(_val, _1)];
    }

    qi::rule<Iterator, std::string()> value;
    qi::rule<Iterator, std::string()> value_last;
    qi::rule<Iterator, person()> pair;
    qi::rule<Iterator, list_person()> start;
  };

}

int main() {
  typedef std::string::const_iterator iterator_type;
  typedef entity::entry_parser<iterator_type> entry_parser;

  entry_parser g; // Our grammar
  std::string str1, str2;
  entity::list_person list; // Struct to save data
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
    std::cout << "got: " ;
    //for (std::vector<std::string>::iterator it = list.begin() ; it != list.end(); ++it)
    //  std::cout << *it;
    for (std::vector<entity::person>::iterator it = list.begin() ; it != list.end(); ++it)
      std::cout << boost::fusion::as_vector(*it);
    std::cout << std::endl;
  }
  else
    std::cout << "failed" << std::endl;

  return 0;
}
