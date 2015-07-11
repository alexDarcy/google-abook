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
    using namespace google;

    name = c.data[pos["name"]];
    nick = c.data[pos["nick"]];
    email.push_back(c.data[pos["email1"]]);
    email.push_back(c.data[pos["email2"]]);
    email.push_back(c.data[pos["email2"]]);
    email.push_back(c.data[pos["email3"]]);
    phone = c.data[pos["phone1"]];
    mobile = c.data[pos["phone2"]];
    workphone = c.data[pos["phone3"]];

    country = c.data[pos["country"]];
    address = c.data[pos["address"]];
    city = c.data[pos["city"]];
    zip = c.data[pos["zip"]];
    country = c.data[pos["country"]];
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

  void print_val(std::ostream& os, const std::string& key, const std::string& val) 
  {
    if (!val.empty())
      os << key << val << std::endl;
  }

  // Custom function instead of using Karma as we need to print the index
  void print_contact(std::ostream& os, contact const& e, int i)
  {
    os << "[" << i << "]" << std::endl;
    os << "name=" << e.name << std::endl;
    os << "email=";
    for (std::string s : e.email) {
      if (!s.empty()) {
        if (s != *e.email.begin())
          os << ",";
        os << s;
      }
    }
    os << std::endl;
    print_val(os, "address=", e.address);
    print_val(os, "address2=", e.address2);
    print_val(os, "city=", e.city);
    print_val(os, "state=", e.state);
    print_val(os, "zip=", e.zip);
    print_val(os, "country=", e.country);
    print_val(os, "phone=", e.phone);
    print_val(os, "workphone=", e.workphone);
    print_val(os, "fax=", e.fax);
    print_val(os, "mobile=", e.mobile);
    print_val(os, "nick=", e.nick);
    print_val(os, "url=", e.url);
    print_val(os, "notes=", e.notes);
    print_val(os, "custom1=", e.custom1);
    print_val(os, "custom2=", e.custom2);
    print_val(os, "custom3=", e.custom3);
    print_val(os, "custom4=", e.custom4);
    os << std::endl;
  }

  // Generator for outputing the data
  void generate_addressbook(std::ostream& os, addressbook const& b)
  {
    for (unsigned int i = 0; i < b.size(); ++i) {
      print_contact(os, b[i], i);
    }
  }

}

#endif
