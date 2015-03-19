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
    std::string nick;
    std::string mobile;
    std::string phone;
    std::string workphone;

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

  std::string header = "Name,"          
    "Given Name,"
    "Additional Name,"
    "Family Name,"
    "Yomi Name,"
    "Given Name Yomi,"
    "Additional Name Yomi,"
    "Family Name Yomi,"
    "Name Prefix,"
    "Name Suffix,"
    "Initials,"
    "Nickname,"
    "Short Name,"
    "Maiden Name,"
    "Birthday,"
    "Gender,"
    "Location,"
    "Billing Information,"
    "Directory Server,"
    "Mileage,"
    "Occupation,"
    "Hobby,"
    "Sensitivity,"
    "Priority,"
    "Subject,"
    "Notes,"
    "Group Membership,"
    "E-mail 1 - Type,"
    "E-mail 1 - Value,"
    "E-mail 2 - Type,"
    "E-mail 2 - Value,"
    "E-mail 3 - Type,"
    "E-mail 3 - Value,"
    "E-mail 4 - Type,"
    "E-mail 4 - Value,"
    "IM 1 - Type,"
    "IM 1 - Service,"
    "IM 1 - Value,"
    "Phone 1 - Type,"
    "Phone 1 - Value,"
    "Phone 2 - Type,"
    "Phone 2 - Value,"
    "Phone 3 - Type,"
    "Phone 3 - Value,"
    "Address 1 - Type,"
    "Address 1 - Formatted,"
    "Address 1 - Street,"
    "Address 1 - City,"
    "Address 1 - PO Box,"
    "Address 1 - Region,"
    "Address 1 - Postal Code,"
    "Address 1 - Country,"
    "Address 1 - Extended Address,"
    "Organization 1 - Type,"
    "Organization 1 - Name,"
    "Organization 1 - Yomi Name,"
    "Organization 1 - Title,"
    "Organization 1 - Department,"
    "Organization 1 - Symbol,"
    "Organization 1 - Location,"
    "Organization 1 - Job Description,"
    "Website 1 - Type,"
    "Website 1 - Value,"
    "Website 2 - Type,"
    "Website 2 - Value,"
    "Website 3 - Type,"
    "Website 3 - Value,"
    "Custom Field 1 - Type,"
    "Custom Field 1 - Value";
}

namespace abook 
{
  /* Conversion  from google to abook format is really done here */
  contact::contact(google::contact c) {
    name = c.data[0];
    email.push_back(c.data[29]);
    email.push_back(c.data[31]);
    email.push_back(c.data[33]);
  }
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


#endif

