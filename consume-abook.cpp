#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/lambda/lambda.hpp>
//#include <boost/bind.hpp>

#include <iostream>
#include <fstream>

namespace client
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  template <typename Iterator>
    struct entry_parser : qi::grammar<Iterator>
  {
    entry_parser() : entry_parser::base_type(start)
    {
      using qi::eps;
      using qi::lit;
      using qi::_val;
      using qi::_1;
      using ascii::char_;

      start = + (char_ - "=") >> "=" >> +(char_);
    }

    qi::rule<Iterator> start;
  };

}

int main() {
  typedef std::string::const_iterator iterator_type;
  typedef client::entry_parser<iterator_type> entry_parser;
  entry_parser g; // Our grammar
  std::string str;
  unsigned result;
  std::ifstream file("test.dat");

  if (file.is_open()){
    while (getline(file, str)){
      std::cout << "Reading " << str << "...";
      std::string::const_iterator iter = str.begin();
      std::string::const_iterator end = str.end();

      bool r = parse(iter, end, g, result);
      if (r && iter == end) 
        std::cout << "ok" <<  std::endl;
      else
        std::cout << "failed" << std::endl;
      }
      file.close();
    }
    else
      std::cout << "Failed to open file" << std::endl;

    return 0;
  }
