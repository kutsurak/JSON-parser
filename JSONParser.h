/* A simple recursive descent JSON parser.
 *
 * Copyright (C) 2011  Panagiotis Koutsourakis
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <exception>

#include "global.h"

namespace json_parser {

class ParseError : public std::exception {
 public:
  ParseError(const std::string &msg): message_(msg) {}
  virtual const char* what() const throw() {
    return message_.c_str();
  }
  virtual ~ParseError() throw() {}
 private:
  std::string message_;
};

class Parser {
 public:
  explicit Parser(std::istream *stream): stream_(stream), line_no_(1) {}
  ~Parser() {}

  /// Parse a JSON value
  Value *jValue();

 private:
  /// The input stream
  std::istream *stream_;
  /// The line number in the stream
  unsigned int line_no_;

  /// Discard whitespace
  void skipWhitespace();
  /// Read one character discarding whitespace
  char readCharacter();

  /// Parse a JSON object
  Object *jObject();
  /// Parse the contents of a JSON object
  Members *jMembers();
  /// Parse a JSON key value pair
  std::pair<std::string, Value*> jPair();
  /// Parse a JSON array
  Array *jArray();
  /// Parse the contents of a JSON array
  Elements *jElements();
  /// Parse a JSON string
  Value *jString();
  /// Parse a JSON number
  Value *jNumber();
  /// Parse an int
  std::string jInt();
  /// Parse a sequence of digits
  std::string jDigits();
  /// Parse an exponent
  std::string jExponent();
  /// Report errors
  void parseError(const std::string&, char);

  // Copy constructors and assignment operators hurt my head...
  DISALLOW_COPY_AND_ASSIGN(Parser);
  
};
}  //namespace json
#endif  //JSON_PARSER_H_
