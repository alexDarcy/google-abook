google-abook
=============

![Build status](https://travis-ci.org/alexDarcy/google-abook.svg)

Converts your Google contacts to the abook format.

## Requirements

* Boost (tested with 1.56)
* g++ (tested with 4.9.1) or clang++ (3.5.0)

## Usage
If Boost is not at a standard location, set the `BOOST_DIR` environment variable and build the application:

    export BOOST_DIR=/home/myboost
    make

### Google contacts to abook
Export your contacts to a csv file from the website using the "Google contacts"
format. For an automatic conversion, rename it to `google.csv` and run :

    make abook

The resulting fil is called `addressbook`.

If you want more control, you can run instead :

    make convert IN=google.csv
    ./abook google_utf8.csv out.csv

This will converts the input file into UTF-8. The new file is called google_utf8.csv by default.
You can then feed it to the program and defien the output file (`out.csv`).

### Abook to google contacts

TODO

## Technical details
### Caveat
1. We do not manage duplicate contacts, so you will have to merge them before the import.

2. If you have weird data in some fields, you have to clean up before the
   import.

3. Due to the difference in format, some fields will be lost. At the moment, only
the abook to google contact conversion is available so the loss only occurs
when there are more than 3 emails in the abook file.

4. The abook format was found empirically, so I may have missed some fields. Also,
there is no single phone representation so local numbers (03...) and
international numbers (+337....) can be mixed.

### Issues and improvements
See the tracker.

### Conversion
We use the following format for abook:

    {name, [email], nick, mobile, phone, workphone}

The syntax for google contacts can be found in [google.hpp](google.hpp) but we show here the
relevant correspondances between the two formats :

| Abook     | Google contacts                |
------------|---------------------------------
| name      | name, given_name, family_name  |
| mobile    | phone_1                        |
| phone     | phone_2                        |
| workphone | phone_3                        |
| email[0]  | email_1                        |
| email[1]  | email_2                        |
| email[2]  | email_3                        |


