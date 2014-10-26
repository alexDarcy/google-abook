//#define BOOST_SPIRIT_DEBUG
#include "abook.hpp"
/* Parse a file in abook format 
 * standard_wide is used to manage accents. */


/* Structure to store data */

BOOST_FUSION_ADAPT_STRUCT(
    abook::contact,
    (std::string, name)
    (abook::list, email)
    (std::string, nick)
    (std::string, mobile)
    (std::string, phone)
    (std::string, workphone)
)

namespace google
{
  struct contact
  {
    std::string name;
    std::string given_name;
    std::string additional_name;
    std::string family_name;
    std::string yomi_name;
    std::string given_name_yomi;
    std::string additional_name_yomi;
    std::string family_name_yomi;
    std::string name_prefix;
    std::string name_suffix;
    std::string initials;
    std::string nickname;
    std::string short_name;
    std::string maiden_name;
    std::string birthday;
    std::string gender;
    std::string location;
    std::string billing_information;
    std::string directory_server;
    std::string mileage;
    std::string occupation;
    std::string hobby;
    std::string sensitivity;
    std::string priority;
    std::string subject;
    std::string notes;
    std::string group_membership;
    std::string email_1_type;
    std::string email_1_value;
    std::string email_2_type;
    std::string email_2_value;
    std::string email_3_type;
    std::string email_3_value;
    std::string im_type;
    std::string im_service;
    std::string im_value;
    std::string phone_1_type;
    std::string phone_1_value;
    std::string phone_2_type;
    std::string phone_2_value;
    std::string phone_3_type;
    std::string phone_3_value;
    std::string address_1_type;
    std::string address_1_formatted;
    std::string address_1_street;
    std::string address_1_city;
    std::string address_1_po_box;
    std::string address_1_region;
    std::string address_1_postal_code;
    std::string address_1_country;
    std::string address_1_extended_address;
    std::string organization_type;
    std::string organization_name;
    std::string organization_yomi_name;
    std::string organization_title;
    std::string organization_department;
    std::string organization_symbol;
    std::string organization_location;
    std::string organization_job_description;
    std::string website_1_type;
    std::string website_1_value;
    std::string website_2_type;
    std::string website_2_value;
    std::string website_3_type;
    std::string website_3_value;
    
    // The conversion from abook to google is really done here
    contact(abook::contact cur){
      std::string::size_type n;
      name = cur.name;

      // Extract the other names
      try {
        n = name.find(" ");
        given_name = name.substr(0, n);
        family_name = name.substr(n+1, name.size());
      }
      catch (const std::out_of_range& oor)  {
        std::cout << "Warning: no family name for " << name << std::endl;
      }
    }
  };
}

  /* Easier output */
  BOOST_FUSION_ADAPT_STRUCT(
      google::contact,
      (std::string, name)
      (std::string, given_name)
      (std::string, additional_name)
      (std::string, family_name)
      (std::string, name_prefix)
      (std::string, name_suffix)
      (std::string, initials)
      (std::string, nickname)
      (std::string, short_name)
      (std::string, maiden_name)
      (std::string, birthday)
      (std::string, gender)
      (std::string, location)
      (std::string, billing_information)
      (std::string, directory_server)
      (std::string, mileage)
      (std::string, occupation)
      (std::string, hobby)
      (std::string, sensitivity)
      (std::string, priority)
      (std::string, subject)
      (std::string, notes)
      (std::string, group_membership)
      (std::string, email_1_type)
      (std::string, email_1_value)
      (std::string, email_2_type)
      (std::string, email_2_value)
      (std::string, email_3_type)
      (std::string, email_3_value)
      (std::string, im_type)
      (std::string, im_service)
      (std::string, im_value)
      (std::string, phone_1_type)
      (std::string, phone_1_value)
      (std::string, phone_2_type)
      (std::string, phone_2_value)
      (std::string, phone_3_type)
      (std::string, phone_3_value)
      (std::string, address_1_type)
      (std::string, address_1_formatted)
      (std::string, address_1_street)
      (std::string, address_1_city)
      (std::string, address_1_po_box)
      (std::string, address_1_region)
      (std::string, address_1_postal_code)
      (std::string, address_1_country)
      (std::string, address_1_extended_address)
      (std::string, organization_type)
      (std::string, organization_name)
      (std::string, organization_yomi_name)
      (std::string, organization_title)
      (std::string, organization_department)
      (std::string, organization_symbol)
      (std::string, organization_location)
      (std::string, organization_job_description)
      (std::string, website_1_type)
      (std::string, website_1_value)
      (std::string, website_2_type)
      (std::string, website_2_value)
      (std::string, website_3_type)
      (std::string, website_3_value)
)

namespace google
{
  using boost::phoenix::arg_names::arg1;
  // the streaming operator needed for output
  std::ostream&
    operator<< (std::ostream& os, contact const& e)
    {
      boost::fusion::for_each(e, os << arg1 << ",");
      return os;
    }

  typedef std::vector<contact> book;

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

  void write_csv_header() {
    std::cout << "Name,Given Name,Additional Name,Family Name,";
    std::cout << "Yomi Name,Given Name Yomi,Additional Name Yomi,Family Name Yomi,";
    std::cout << "Name Prefix,Name Suffix,Initials,Nickname,Short Name,Maiden Name,";
    std::cout << "Birthday,Gender,Location,Billing Information,";
    std::cout << "Directory Server,Mileage,Occupation,Hobby,Sensitivity,";
    std::cout << "Priority,Subject,Notes,Group Membership,";
    std::cout << "E-mail 1 - Type,E-mail 1 - Value,";
    std::cout << "E-mail 2 - Type,E-mail 2 - Value,";
    std::cout << "E-mail 3 - Type,E-mail 3 - Value,";
    std::cout << "IM 1 - Type,IM 1 - Service,IM 1 - Value,";
    std::cout << "Phone 1 - Type,Phone 1 - Value,";
    std::cout << "Phone 2 - Type,Phone 2 - Value,";
    std::cout << "Phone 3 - Type,Phone 3 - Value,";
    std::cout << "Address 1 - Type,Address 1 - Formatted,";
    std::cout << "Address 1 - Street,Address 1 - City,Address 1 - PO Box,";
    std::cout << "Address 1 - Region,Address 1 - Postal Code,Address 1 - Country,";
    std::cout << "Address 1 - Extended Address,Organization 1 - Type,";
    std::cout << "Organization 1 - Name,Organization 1 - Yomi Name,";
    std::cout << "Organization 1 - Title,Organization 1 - Department,";
    std::cout << "Organization 1 - Symbol,Organization 1 - Location,";
    std::cout << "Organization 1 - Job Description,";
    std::cout << "Website 1 - Type,Website 1 - Value,";
    std::cout << "Website 2 - Type,Website 2 - Value,";
    std::cout << "Website 3 - Type,Website 3 - Value" << std::endl;
  }

  // Convert from abook to google format and output it
  void write_contacts(abook::book& old_contacts) {
    // Convert from abook to google format
    book new_contacts(old_contacts.begin(), old_contacts.end());

    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);

    if (!generate_book(sink, new_contacts))
      std::cout << "Generating failed\n";
    else
    {
      write_csv_header();
      std::cout << generated << std::endl;
    }
  }

}


int read_file_to_buffer(char *fname, std::string &buffer) {
  std::ifstream file(fname, std::ios_base::in);

  if (!file) {
    std::cout << "Failed to open file" << std::endl;
    return 0;
  }

  // Read file into buffer
  file.unsetf(std::ios::skipws); // No white space skipping!
  std::copy(
      std::istream_iterator<char>(file),
      std::istream_iterator<char>(),
      std::back_inserter(buffer));
  return 1;
}

int parse_abook_file(char* fname, abook::book& mybook) {
  typedef std::string::const_iterator iterator_type;
  typedef abook::abook_parser<iterator_type> abook_parser;
  typedef abook::comment_skipper<iterator_type> comment_skipper;

  abook_parser g; // Our grammar
  comment_skipper skip;

  std::string buffer; 
  if (! read_file_to_buffer(fname, buffer))
    return 0;

  std::string::const_iterator begin = buffer.begin();
  std::string::const_iterator end = buffer.end();

  bool r = phrase_parse(begin, end, g, skip, mybook);

  return r && begin == end;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "Need a filename as argument" << std::endl;
    return 1;
  }

  abook::book mybook; // Struct to save data
  int res = parse_abook_file(argv[1], mybook);

  if (res) {
    std::cout << "full match" <<  std::endl;
    //write_to_file(mybook);
    google::write_contacts(mybook);
  }
  else
    std::cout << "parsing failed" << std::endl;

  return 0;
}
