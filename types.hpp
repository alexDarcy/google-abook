#ifndef _TYPES_HPP
#define _TYPES_HPP

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
#include <vector>
#include <string>


namespace google { struct contact;}

namespace abook {
  typedef std::vector<std::string> list;

  struct contact
  {
    std::string name;
    list email;
    std::string address;
    std::string address2;
    std::string city;
    std::string state;
    std::string zip;
    std::string country;
    std::string phone;
    std::string workphone;
    std::string fax;
    std::string mobile;
    std::string nick;
    std::string url;
    std::string notes;
    std::string custom1;
    std::string custom2;
    std::string custom3;
    std::string custom4;

    contact()  {};
    contact(std::string n, std::string e): name(n){ email.push_back(e); }
    contact(google::contact c);
  };

  typedef std::vector<contact> addressbook;
}


namespace google
{
  struct contact
  {
    std::vector<std::string> data;

    contact(){};
    /* We avoid using a boost fusion sequence as it is troublesome with a
     * single attribute. This means we have to supply a custom constructor */
    contact(std::vector<std::string> list) : data(list) {};

    contact(abook::contact) {};
  };

  typedef std::vector<contact> addressbook;

}

// Generator for outputing the data
template <typename OutputIterator, typename T>
bool generate_addressbook(OutputIterator& sink, T const& b)
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

template<typename T> void write_contacts(char* out, T const& book) {
  //std::ofstream file("test.out", std::ios_base::out);
  typedef std::back_insert_iterator<std::string> iterator ;
  std::string generated;
  iterator sink(generated);

  if (!generate_addressbook<iterator, T>(sink, book))
    std::cout << "Generating failed\n";
  else
    std::cout << "Generated: " << generated << "\n";
}


#endif

