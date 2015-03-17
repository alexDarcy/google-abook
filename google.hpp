#ifndef _GOOGLE_HPP
#define _GOOGLE_HPP

#include "abook.hpp"
#include <boost/algorithm/string/erase.hpp>

namespace google
{

  std::string header = "Name,Given Name,Additional Name,Family Name,Yomi Name,Given Name Yomi,Additional Name Yomi,Family Name Yomi,Name Prefix,Name Suffix,Initials,Nickname,Short Name,Maiden Name,Birthday,Gender,Location,Billing Information,Directory Server,Mileage,Occupation,Hobby,Sensitivity,Priority,Subject,Notes,Group Membership,E-mail 1 - Type,E-mail 1 - Value,E-mail 2 - Type,E-mail 2 - Value,E-mail 3 - Type,E-mail 3 - Value,E-mail 4 - Type,E-mail 4 - Value,IM 1 - Type,IM 1 - Service,IM 1 - Value,Phone 1 - Type,Phone 1 - Value,Phone 2 - Type,Phone 2 - Value,Phone 3 - Type,Phone 3 - Value,Address 1 - Type,Address 1 - Formatted,Address 1 - Street,Address 1 - City,Address 1 - PO Box,Address 1 - Region,Address 1 - Postal Code,Address 1 - Country,Address 1 - Extended Address,Organization 1 - Type,Organization 1 - Name,Organization 1 - Yomi Name,Organization 1 - Title,Organization 1 - Department,Organization 1 - Symbol,Organization 1 - Location,Organization 1 - Job Description,Website 1 - Type,Website 1 - Value,Website 2 - Type,Website 2 - Value,Website 3 - Type,Website 3 - Value,Custom Field 1 - Type,Custom Field 1 - Value";

  struct contact
  {
    std::vector<std::string> data;
    
    contact(){};
    /* We avoid using a boost fusion sequence as it is troublesome with a
     * single attribute. This means we have to supply a custom constructor */
    contact(std::vector<std::string> list) : data(list) {};
  };
}

namespace google 
{
  typedef std::vector<contact> addressbook;

  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
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

      // Allow to use repeat to cast into a container atomically
      qi::as<std::vector<std::string> > strings;

      /* This avoid an issue with single-member structs */
      content = +(char_ - ',' - eol) | attr("(unspecified)");
      quote = +(char_ - ',' - '"');
      value = ('"' >> quote > '"') | content;
      entry = strings [ repeat(68) [ value >> ',' ] >> value ] >> eol;
      entry.name("entry");
      //debug(entry);
      contacts = header >> eol
        >> *( entry [push_back(_val, _1)]);

      contacts.name("contacts");
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

    qi::rule<Iterator, std::string()> value, content, quote;
    qi::rule<Iterator, contact()> entry;
    qi::rule<Iterator, addressbook(), skipper> contacts;
  };

  // the streaming operator needed for output
  std::ostream&
    operator<< (std::ostream& os, contact const& e)
    {
        os << "name=" << e.data[0] << std::endl;
      return os;
    }

  // Generator for outputing the data
  template <typename OutputIterator>
    bool generate_book(OutputIterator& sink, addressbook const& b)
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

  // Convert from abook to google format and output it
  void write_contacts(char* fname, const addressbook& old_contacts) {
//    std::ofstream file(fname, std::ios_base::out);
//    addressbook new_contacts(old_contacts.begin(), old_contacts.end());

    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);

    std::cout << "size data" << old_contacts.size() << std::endl;
    if (!generate_book(sink, old_contacts))
      std::cout << "Generating failed\n";
    else
    {
      std::cout << "Generating done\n";
      std::cout << generated << std::endl;
    }
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
  return 0;
  }

}

#endif
