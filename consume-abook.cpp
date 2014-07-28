#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
//#include <boost/lambda/lambda.hpp>
//#include <boost/bind.hpp>

#include <iostream>
#include <fstream>

namespace client
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  template <typename Iterator>
    bool parse_numbers(Iterator first, Iterator last, std::vector<double>& v)
    {
      using qi::double_;
      using qi::phrase_parse;
      using ascii::space;
      using ascii::char_;

      bool r = phrase_parse(
          first,                          
          last,                           
          + (char_ - "=") >> "=" >> +(char_),   
          space
          );
      if (first != last) // fail if we did not get a full match
        return false;
      return r;
    }
}

int main() {
  std::string str;
  std::ifstream file("test.dat");

  if (file.is_open()){
    while (getline(file, str)){
      std::cout << "Reading " << str << "...";
      std::vector<double> results;
      if (client::parse_numbers(str.begin(), str.end(), results)){
        std::cout << "ok" <<  std::endl;
        for (int i = 0; i < results.size(); ++i) 
          std::cout << results[i] << " "; 
        std::cout << std::endl;

      }

      else
        std::cout << "failed" << std::endl;
      }
      file.close();
    }
    else
      std::cout << "Failed to open file" << std::endl;

    return 0;
  }
