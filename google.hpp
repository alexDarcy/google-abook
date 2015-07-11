#ifndef _GOOGLE_HPP
#define _GOOGLE_HPP

#include "types.hpp"
#include "common.hpp"
#include <boost/algorithm/string/erase.hpp>

namespace google 
{
  int nb = 67;
  std::string fields = "Name,"          
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

  typedef std::vector<contact> addressbook;

  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
  /* standard_wide is used to manage accents. */
  namespace standard_wide = boost::spirit::standard_wide;

  // Skip comments
  template<typename Iterator>
    struct comment_skipper : public qi::grammar<Iterator> {

      qi::rule<Iterator> skip;

      comment_skipper() : comment_skipper::base_type(skip)
      {
        using namespace qi;
        skip = (lit("#") >> *(standard::char_ - eol) >> eol) ;
      }
    };  

  // Our parser (using a custom skipper to skip comments and empty lines )
  template <typename Iterator, typename skipper = comment_skipper<Iterator> >
    struct google_parser : qi::grammar<Iterator, addressbook(), skipper>
  {
    google_parser() : google_parser::base_type(contacts, "contacts")
    {
      using namespace qi;
      using standard_wide::char_;
      using phoenix::at_c;
      using phoenix::push_back;
      using phoenix::val;
      using phoenix::construct;

      header = eps > fields >> eol;
      on_error<fail>
      (
       header
       , std::cout
       << val("Error! Wrong header. Maybe google changed its specs ? ")
       << std::endl
      );

      // Allow to use repeat to cast into a container atomically
      qi::as<std::vector<std::string> > strings;

      /* This avoid an issue with single-member structs */
      content = +(char_ - ',' - eol) | attr("");
      quote = +(char_ - ',' - '"');
      value = ('"' >> quote > '"') | content;
      entry = strings [ repeat(nb-1) [ value >> ',' ] >> value ] >> eol;
      entry.name("entry");
      debug(entry);

      contacts = header 
                 >> *( entry [push_back(_val, _1)]);

      on_error<fail>
        (
         contacts
         , std::cout
         << val("Error! Expecting ")
         << _4                               // what failed?
         << val(" here: \"")
         << construct<std::string>(_3, _2)   // iterators to error-pos, end
         << val("\"")
         << std::endl
        );
    }

    qi::rule<Iterator, std::string()> header;
    qi::rule<Iterator, std::string()> value, content, quote;
    qi::rule<Iterator, contact()> entry;
    qi::rule<Iterator, addressbook(), skipper> contacts;
  };

  // the streaming operator needed for output
  std::ostream&
    operator<< (std::ostream& os, contact const& e)
    {
        os << "name=" << e.data[0] << std::endl;
        os << "mail=" << e.data[28] << "," << e.data[30] << "," << e.data[31] << std::endl;
      return os;
    }

  int parse_google_file(char* fname, addressbook& mybook) {
    typedef std::string::const_iterator iterator_type;

    google_parser<iterator_type> g; // Our grammar
    comment_skipper<iterator_type> skip;

    std::string buffer; 
    if (! read_file_to_buffer(fname, buffer))
      return 0;

    std::string::const_iterator begin = buffer.begin();
    std::string::const_iterator end = buffer.end();

    contact mycontact;
    bool r = phrase_parse(begin, end, g, skip, mybook);

    return r && begin == end;
  }

}

#endif
