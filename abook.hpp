#ifndef _ABOOK_HPP
#define _ABOOK_HPP

#include "common.hpp"
#include "types.hpp"

/* Structure to store data */

BOOST_FUSION_ADAPT_STRUCT(
    abook::contact,
    (std::string, name)
    (abook::list, email)
    (std::string, address)
    (std::string, address2)
    (std::string, city)
    (std::string, state)
    (std::string, zip)
    (std::string, country)
    (std::string, phone)
    (std::string, workphone)
    (std::string, fax)
    (std::string, mobile)
    (std::string, nick)
    (std::string, url)
    (std::string, notes)
    (std::string, custom1)
    (std::string, custom2)
    (std::string, custom3)
    (std::string, custom4)


)

namespace abook 
{
  /* Conversion  from google to abook format is really done here */
  contact::contact(google::contact c) {
    name = c.data[0];
    nick = c.data[2];
    notes = c.data[25];
    email.push_back(c.data[28]);
    email.push_back(c.data[30]);
    email.push_back(c.data[32]);
    phone = c.data[39];
    mobile = c.data[41];
    workphone = c.data[43];
  }

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
    struct abook_parser : qi::grammar<Iterator, addressbook(), skipper>
  {
    abook_parser() : abook_parser::base_type(contacts)
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

      header = ("[format]" >> eol)
        > "program=" >> value >> eol
        > "version=" >> value >> eol;

      contacts = header 
        >> *(entry [push_back(_val, _1)] >> *eol);

      // Names for error handling
      contacts.name("contacts");
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
    qi::rule<Iterator, addressbook(), skipper> contacts;
    qi::rule<Iterator> header;
  };

  // the streaming operator needed for output
  std::ostream&
    operator<< (std::ostream& os, contact const& e)
    {
      os << "--------contact---------" << std::endl;
      os << "name=" << e.name << std::endl;
      os << "mail=";
      for (std::string s : e.email) {
        if (!s.empty()) {
          if (s != *e.email.begin())
            os << ",";
          os << s;
        }
      }
      os << std::endl;
      os << "nick=" << e.nick << std::endl;
      os << "mobile=" << e.mobile << std::endl;
      os << "phone=" << e.phone << std::endl;
      os << "workphone=" << e.workphone << std::endl;;
      os << "notes=" << e.notes;
      return os;
    }


}

#endif
