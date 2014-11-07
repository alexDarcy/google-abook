consume-abook
=============

Converts a list of contacts from the abook format to Google contacts format
using Boost. Only this conversion is available at the moment.

## Requirements

It requires Boost and has been tested successfully with Boost 1.56.
The code compiles with gcc but fails to link with clang.

## Usage

Edit the makefile if Boost is installed in a non-standard location. Then compile
and run:
    
    make
    ./consume-abook input output

## Technical details
### Caveat
Due to the difference in format, some fields will be lost. At the moment, only
the abook to google contact conversion is available so the loss only occurs
when there are more than 3 emails in the abook file.

The abook format was found empirically, so I may have missed some fields. Also,
there is no single phone representation so local numbers (03...) and
international numbers (+337....) can be mixed.

### Issues and improvements
See the tracker.

### Conversion
We use the following format for abook:

    {name, [email], nick, mobile, phone, workphone}

The syntax for google contacts can be found in google.hpp but we show here the
relevant correspondances between the two formats :

   | Abook     | Google contacts                |
   ------------|---------------------------------
   | name      | name, given_name, family_name  |
   ------------|---------------------------------
   | mobile    | phone_1                        |
   | phone     | phone_2                        |
   | workphone | phone_3                        |
   ------------|---------------------------------
   | email[0]  | email_1                        |
   | email[1]  | email_2                        |
   | email[2]  | email_3                        |


