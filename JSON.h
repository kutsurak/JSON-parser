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
#ifndef JSON_H_
#define JSON_H_

#include <list>
#include <map>
#include <string>

#include "global.h"

namespace json_parser {

/// Represents a simple JSON value (number, string, literal) using a
/// string.
class Value {
 public:
  Value(): value_() {}
  void SetValue(const std::string &val) {
    value_ = val;
  }

  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  virtual std::string toJSON(int spc, bool skip_initial) const;
  virtual ~Value() {}
 private:
  std::string value_;
  DISALLOW_COPY_AND_ASSIGN(Value);
};

/// A linked list of JSON values for use in a JSON Array.
class Elements {
 public:
  Elements(): values_() {}
  void insert(Value *val) {
    values_.push_front(val);
  }
  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  std::string toJSON(int spc, bool skip_initial) const;
  
  ~Elements() {
    for(std::list<Value *>::iterator it = values_.begin();
        it != values_.end(); it++) {
      delete *it;
    }
  }
 private:
  std::list<Value *> values_;
  DISALLOW_COPY_AND_ASSIGN(Elements);
};

/// A JSON array (inherits from Value). It just holds a pointer to the
/// elements.
class Array : public Value{
 public:
  Array(): elems_(NULL) {}
  explicit Array(Elements *elems): elems_(elems) {}
  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  std::string toJSON(int spc, bool skip_initial) const;
  
  ~Array() {
    delete elems_;
  }
 private:
  Elements *elems_;
  DISALLOW_COPY_AND_ASSIGN(Array);
};

/// A JSON pair. Holds two JSON values (key and value).
class Pair {
 public:
  Pair(Value *str, Value *val): key_(str), value_(val) {}
  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  std::string toJSON(int spc, bool skip_initial) const;
  
  ~Pair() {
    delete key_;
    delete value_;
  }
 private:
  Value *key_;
  Value *value_;
  DISALLOW_COPY_AND_ASSIGN(Pair);
};

/// A linked list of elements for use in JSON object.
class Members {
 public:
  Members(): pairs_() {}
  void insert(std::pair<std::string, Value *> p) {
    pairs_.insert(p);
  }
  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  std::string toJSON(int spc, bool skip_initial) const;
  
  ~Members() {
    typedef std::map<std::string, Value*>::iterator map_iterator;
    for (map_iterator it = pairs_.begin();
         it != pairs_.end(); it++) {
      delete it->second;
    }
  }
 private:
  std::map<std::string, Value*> pairs_;
  DISALLOW_COPY_AND_ASSIGN(Members);
};

/// A JSON object (inherits from Value) that just holds a pointer to
/// its members.
class Object : public Value {
 public:
  Object(): members_() {}
  explicit Object(Members *mem): members_(mem) {}
  /**
   * Performs pretty printing.
   * @param spc How many spaces to put before the data.
   * @param skip_initial If true, it does not print the initial spaces
   * before the data.
   */
  std::string toJSON(int spc, bool skip_initial) const;
  ~Object() {
    delete members_;
  }
 private:
  Members *members_;
  DISALLOW_COPY_AND_ASSIGN(Object);
};
}  // namespace json
#endif  // JSON_H_
