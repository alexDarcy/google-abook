#ifndef _GOOGLE_HPP
#define _GOOGLE_HPP

#include "types.hpp"
#include "common.hpp"
#include <boost/algorithm/string/erase.hpp>

namespace google 
{
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
