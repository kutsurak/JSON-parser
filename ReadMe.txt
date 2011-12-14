This program is a simple recursive descent parser for the JSON data
format.

Currently it does not support unicode characters (as it should) and
some of the control characters for strings are not yet
implemented. Other than that it is fairly complete.

There are two main modules the parser itself defined in JSONParser.h
and JSONParser.cpp as well as a representation for JSON objects
defined in JSON.h and JSON.cpp. Currently the JSON objects are in the
simplest possible form I could make them work. For instance the JSON
members are not implemented as a map, although it makes a lot of sense
to do so, but as a linked list. Probably the next thing that will
happen will be this change.

For more information about JSON take a look at: http://www.json.org

COMPILATION

The source directory includes a CMakeLists.txt. I'm not using any
fancy CMake commands so any recent version of CMake should be able to
process it. In order to compile do the following:

$ cd <source directory>
$ mkdir build
$ cd build
$ cmake ../
$ make

This sequence of commands will produce a static library
(libjson_parser.a) as well as an example program (json_parser_example)
that takes one JSON file as argument and pretty prints the JSON data
in it.

In order to compile it you just need a recent C++ compiler. The code
does not use any fancy C++ features, so any compiler conforming to the
1998 ISO C++ standard should be ok.

Have fun.

For any comments/bug reports send an email at
panagiotis.koutsourakis(at)gmail[dOt]com.
