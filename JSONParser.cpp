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
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#include "JSON.h"
#include "JSONParser.h"

using std::string;
using std::stringstream;

namespace json_parser {

void Parser::skipWhitespace() {
  char c;
  while(isspace(c = stream_->get())) {
    if (c == '\n') {
      line_no_++;
    }
  }

  // Put the first non whitespace char back in the stream
  stream_->unget();
}

char Parser::readCharacter() {
  skipWhitespace();
  char c = stream_->get();

  return c;
}

/**
 * A JSON object is delimited by curly braces, and contains a
 * (possibly empty) list of members.
 */
Object *Parser::jObject() {
  skipWhitespace();
  char c;
  if ((c  = readCharacter()) != '{') {
    parseError("character '{'", c);
  }
  
  Object *ret = NULL;
  // The JSON object can either be empty...
  if ((c = readCharacter()) == '}') {
    ret = new Object();
  } else { // ...or contain a list of pairs
    stream_->unget();
    ret = new Object(jMembers());
    if ((c = readCharacter()) != '}') {
      parseError("character '}'", c);
    }
  }

  return ret;
}

/**
 * The JSON members are a list of pairs.
 */
Members *Parser::jMembers() {
  skipWhitespace();
  // Read a pair from the stream
  std::pair<std::string, Value*> pair = jPair();
  Members *ret;
  char c = readCharacter();
  // If the next char is ',' we have more pairs to read
  if (c == ',') {
    ret = jMembers();
  } else {
    stream_->unget();
    ret = new Members();
  }
  // Because we are working recursivelly we need to put the pair we
  // read in the front of the member list in order to maintain the
  // order.
  ret->insert(pair);

  return ret;
}

/**
 * A pair is a string followed by the character ':' followed by a JSON
 * value.
 */
std::pair<string, Value*> Parser::jPair() {
  skipWhitespace();
  Value *str = jString();
  char c = readCharacter();
  if (c != ':') {
    parseError("character ':'", c);
  }
  Value *val = jValue();

  std::string key = str->toJSON(0, false);
  // Strip the first and last double quote from the key
  // for easier searching in the map.
  key = key.substr(key.find_first_of("\"") + 1, key.find_last_of("\"") - 1);
  std::pair<std::string, Value*> ret(key, val);
  delete str;
  return ret;
}

/**
 * A JSON array is a (possibly empty) list of elements delimited by
 * braces.
 */
Array *Parser::jArray() {
  skipWhitespace();

  char c;
  if ((c = readCharacter()) != '[') {
    parseError("character '['", c);
  }
  Array *ret = NULL;
  if ((c = readCharacter()) == ']') {
    ret = new Array();
  } else {
    stream_->unget();
    ret = new Array(jElements());
    if ((c = readCharacter()) != ']') {
      parseError("character ']'", c);
    }
  }

  return ret;
}

/**
 * JSON elements is a list of JSON values.
 */
Elements *Parser::jElements() {
  skipWhitespace();
  Value *val = jValue();
  Elements *ret;
  char c;
  if ((c = readCharacter()) == ',') {
    ret = jElements();
  } else {
    stream_->unget();
    ret = new Elements();
  }

  ret->insert(val);
  return ret;
}

/**
 * A JSON value is either a JSON Object, a JSON array, a JSON string,
 * a JSON number or one of the literals true, false and null.
 */
Value *Parser::jValue() {
  skipWhitespace();
  Value *ret = NULL;

  char c = readCharacter();
  char true_buf[] = "rue";
  char false_buf[] = "alse";
  char null_buf[] = "ull";
  switch(c) {
    case '"':
      stream_->unget();
      ret = jString();
      break;
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '0':
    case '-':
      stream_->unget();
      ret = jNumber();
      break;
    case '{':
      stream_->unget();
      ret = jObject();
      break;
    case '[':
      stream_->unget();
      ret = jArray();
      break;
    case 't':
      for(int i = 0; i < 3; i++) {
        if ((c = readCharacter()) != true_buf[i]) {
          stringstream exp; 
          exp << "character '" << true_buf[i] << "'";
          parseError(exp.str(), c);
        }
      }
      ret = new Value();
      ret->SetValue("true");
      break;
    case 'f':
      for(int i = 0; i < 4; i++) {
        if ((c = readCharacter()) != false_buf[i]) {
          stringstream exp; 
          exp << "character '" << false_buf[i] << "'";
          parseError(exp.str(), c);
        }
      }
      ret = new Value();
      ret->SetValue("false");
      break;
    case 'n':
      for(int i = 0; i < 3; i++) {
        if ((c = readCharacter()) != null_buf[i]) {
          stringstream exp; 
          exp << "character '" << null_buf[i] << "'";
          parseError(exp.str(), c);
        }
      }
      ret = new Value();
      ret->SetValue("null");
      break;
    default:
      parseError("JSON Value", c);
      break;
  };

  return ret;
}

/**
 * A JSON string is the characters between two double quotation marks.
 *
 * @todo This method does not currently support Unicode or control
 * characters as it should.
 */
Value *Parser::jString() {
  skipWhitespace();
  string str;

  char c = stream_->get();
  if (c != '"') {
    parseError("character '\"'", c);
  }

  while((c = stream_->get()) != '"') {
    str.push_back(c);
  }

  Value *ret = new Value();
  ret->SetValue("\"" + str + "\"");

  return ret;
}

/**
 * A JSON number is either an int, or an int followed by a fractional
 * part, or by an exponent, or both.
 */
Value *Parser::jNumber() {
  skipWhitespace();
  Value *ret = new Value();
  std::string str = jInt();

  char c = stream_->get();
  switch(c) {
    case '.': // Fractional part
      str += "." + jDigits();
      c = stream_->get();
      if (c == 'e' || c == 'E') { // Maybe there is still an exponent.
        stream_->unget();
        str += jExponent();
      } else {
        stream_->unget();
      }
      break;
    case 'e': case 'E': // Exponent
      stream_->unget();
      str += jExponent();
      break;
    default:
      stream_->unget();
      break;
  };
  
  ret->SetValue(str);

  return ret;
}

/**
 * An int is either a single digit, or a non zero digit, followed by
 * more digits. Possibly there is a minus at the start.
 */
std::string Parser::jInt() {
  skipWhitespace();
  char c = stream_->get();

  std::string ret;
  switch(c) {
    case '-':
      ret = "-" + jDigits();
      break;
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': 
      stream_->unget();
      ret = jDigits();
      break;
    case '0':
      ret = "0";
      break;
    default:
      parseError("digit or '-'", c);
      break;
  }

  return ret;
}

/**
 * Reads a sequence of digits.
 */
std::string Parser::jDigits() {
  string str;

  char c = stream_->get();
  if (!isdigit(c)) {
    parseError("digit", c);
  }

  str.push_back(c);
  while(isdigit(c = stream_->get())) {
    str.push_back(c);
  }

  stream_->unget();
  return str;
}


/**
 * An exponent is the character 'e' or 'E' followed optionally by a
 * sign character, followed by a sequence of digits.
 */
std::string Parser::jExponent() {
  std::string ret;
  
  char c = stream_->get();
  if (c != 'e' && c != 'E') {
    parseError("'e' or 'E'", c);
  }

  ret += c;
  c = stream_->get();
  if (c == '+' || c == '-') {
    ret += c;
    ret += jDigits();
  } else if (isdigit(c)) {
    stream_->unget();
    ret += jDigits();
  } else {
    parseError("'+', '-' or digit", c);
  }

  return ret;  
}

void Parser::parseError(const std::string &expected, char found) {
  
  if (found == -1) {
    throw ParseError("Unexpected EOF!");
  }
  
  stringstream msg;
  msg << "Expected " << expected << ". Found: '"
      << found << "' (int=" << static_cast<int>(found)
      << ") at line " << line_no_;
  throw ParseError(msg.str());
}

}  // namespace json
