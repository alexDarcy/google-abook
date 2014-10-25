#ifndef _ABOOK_HPP
#define _ABOOK_HPP

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

namespace abook 
{
  typedef std::vector<std::string> list;

  struct contact
  {
    std::string name;
    list email;
    std::string nick;
    std::string mobile;
    std::string phone;
    std::string workphone;

    contact()  {}
    contact(std::string n, std::string e): name(n){
      email.push_back(e);
    }
  };

  // the streaming operator needed for output
  std::ostream&
    operator<< (std::ostream& os, contact const& e)
    {
      os << "--------contact---------" << std::endl;
      os << "name=" << e.name << std::endl;
      os << "mail=";
      //for (list::iterator it = e.email.begin(); it != e.email.end(); ++it) 
      BOOST_FOREACH(std::string s, e.email) {
        os << s << "|";
      }
      os << std::endl;
      os << "nick=" << e.nick << std::endl;
      os << "mobile=" << e.mobile << std::endl;
      os << "phone=" << e.phone << std::endl;
      os << "workphone=" << e.workphone;
      return os;
    }

  typedef std::vector<contact> book;

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
      using qi::lit;
      using qi::on_error;
      using standard_wide::char_;
      using phoenix::at_c;
      using phoenix::push_back;
      using phoenix::construct;
      using phoenix::val;

      // We read any charactor different from eol
      value = +(char_ - eol) [_val += _1] ;
      email = +(char_ - ',' - eol) [_val += _1];

      name %= "name=" > value > eol;
      nick %= "nick=" > value > eol;
      mobile %= "mobile=" > value > eol;
      phone %= "phone=" > value > eol;
      workphone %= "workphone=" > value > eol;

      emails = "email=" 
          // A list of characters separated by ',' 
          >> email [push_back(_val, _1)] % ',' 
          // The last character may be a comma
          >>  *lit(',') >> eol;

      // Only the name is mandatory and should be the first entry
      // Other values do not have a definite order
      entry = 
        "[" > int_ > "]" > eol
        > name [at_c<0>(_val) = _1]
        >> * (emails [at_c<1>(_val) = _1]
            | nick [at_c<2>(_val) = _1 ]
            | phone [at_c<4>(_val) = _1 ] 
            | mobile [at_c<3>(_val) = _1 ]
            | workphone [at_c<5>(_val) = _1 ]);

      //debug(entry);

      header = "[format]" >> eol
        > "program=" >> value >> eol
        > "version=" >> value >> eol;

      book_g = header 
        >> *(entry [push_back(_val, _1)] >> *eol);

      // Names for error handling
      book_g.name("book_g");
      header.name("header");
      entry.name("entry");

      on_error<fail>
        (
         header
         , std::cout
         << val("Error! Expecting ") << _4 << val(" here: \"")
         << construct<std::string>(_3, _2)   // iterators to error-pos, end
         << val("\"")
         << std::endl
        );

    }

    qi::rule<Iterator, std::string()> value, email;
    qi::rule<Iterator, list()> emails;
    qi::rule<Iterator, std::string()> name, nick;
    qi::rule<Iterator, std::string()> mobile, phone, workphone;
    qi::rule<Iterator, contact()> entry;
    qi::rule<Iterator, book(), skipper> book_g;
    qi::rule<Iterator> header;
  };

  // Generator for outputing the data
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

  void write_to_file(book &mybook) {
    //std::ofstream file("test.out", std::ios_base::out);
    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);

    if (!abook::generate_book(sink, mybook))
      std::cout << "Generating failed\n";
    else
      std::cout << "Generated: " << generated << "\n";
  }

}

#endif
