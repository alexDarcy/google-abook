#ifndef _GOOGLE_HPP
#define _GOOGLE_HPP

#include "abook.hpp"
#include <boost/algorithm/string/erase.hpp>

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

    void set_names(abook::contact cur) {
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

    // Abook has 3 phone numbers and Google too.
    // We remove white spaces for coherence 
    // FIXME does not remove all whitespaces
    void set_phones(abook::contact cur) {
      if (! cur.mobile.empty()) {
        phone_1_type = "Mobile";
        phone_1_value = cur.mobile;
        boost::algorithm::erase_all(phone_1_value, " "); 
      }
      if (! cur.phone.empty()) {
        phone_2_type = "Home";
        phone_2_value = cur.phone;
        boost::algorithm::erase_all(phone_2_value, " "); 
      }
      if (! cur.workphone.empty()) {
        phone_2_type = "Work";
        phone_2_value = cur.workphone;
        boost::algorithm::erase_all(phone_3_value, " "); 
      }
    }

    void set_emails(abook::contact cur) {
      int n;

      if (cur.email.empty()) return;
      n = cur.email.size();

      email_1_type = "Email 1";
      email_1_value = cur.email[0];
      if (n > 1) {
        email_2_type = "Email 2";
        email_2_value = cur.email[1];
      }
      if (n > 2) {
        email_3_type = "Email 3";
        email_3_value = cur.email[2];
      }
      if (n > 3)
        std::cout << "Warning: more than 3 emails, skipping others" << std::endl;
    }

    // The conversion from abook to google is really done here
    contact(abook::contact cur){
      set_names(cur);
      set_emails(cur);
      set_phones(cur);
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

  void write_csv_header(std::ofstream& file) {
    file << "Name,Given Name,Additional Name,Family Name,";
    file << "Yomi Name,Given Name Yomi,Additional Name Yomi,Family Name Yomi,";
    file << "Name Prefix,Name Suffix,Initials,Nickname,Short Name,Maiden Name,";
    file << "Birthday,Gender,Location,Billing Information,";
    file << "Directory Server,Mileage,Occupation,Hobby,Sensitivity,";
    file << "Priority,Subject,Notes,Group Membership,";
    file << "E-mail 1 - Type,E-mail 1 - Value,";
    file << "E-mail 2 - Type,E-mail 2 - Value,";
    file << "E-mail 3 - Type,E-mail 3 - Value,";
    file << "IM 1 - Type,IM 1 - Service,IM 1 - Value,";
    file << "Phone 1 - Type,Phone 1 - Value,";
    file << "Phone 2 - Type,Phone 2 - Value,";
    file << "Phone 3 - Type,Phone 3 - Value,";
    file << "Address 1 - Type,Address 1 - Formatted,";
    file << "Address 1 - Street,Address 1 - City,Address 1 - PO Box,";
    file << "Address 1 - Region,Address 1 - Postal Code,Address 1 - Country,";
    file << "Address 1 - Extended Address,Organization 1 - Type,";
    file << "Organization 1 - Name,Organization 1 - Yomi Name,";
    file << "Organization 1 - Title,Organization 1 - Department,";
    file << "Organization 1 - Symbol,Organization 1 - Location,";
    file << "Organization 1 - Job Description,";
    file << "Website 1 - Type,Website 1 - Value,";
    file << "Website 2 - Type,Website 2 - Value,";
    file << "Website 3 - Type,Website 3 - Value" << std::endl;
  }

  // Convert from abook to google format and output it
  void write_contacts(char* fname, abook::book& old_contacts) {
    std::ofstream file(fname, std::ios_base::out);

    // Convert from abook to google format
    book new_contacts(old_contacts.begin(), old_contacts.end());

    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);

    if (!generate_book(sink, new_contacts))
      std::cout << "Generating failed\n";
    else
    {
      write_csv_header(file);
      file << generated << std::endl;
    }
  }
}
#endif
