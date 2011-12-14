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
#include <list>
#include <string>

#include "JSON.h"

using std::string;

namespace json_parser {
std::string Object::toJSON(int spc, bool skip_initial) const {
  std::string str;
  std::string spcs;

  if (members_ != NULL) {
    for (int i = 0; i < spc; i++) {
      spcs += " ";
    }
    if (skip_initial) {
      str += "{\n";
    } else {
      str += spcs + "{\n";
    }
    str += members_->toJSON(spc + 1, false);
    str += spcs + "}";
  } else {
    str = "{}";
  }
  return str;
}

std::string Members::toJSON(int spc, bool skip_initial) const {
  std::string str;
  std::string spcs;

  for (int i = 0; i < spc; i++) {
    spcs += " ";
  }

  unsigned int sz = pairs_.size();
  unsigned int i = 0;
  typedef std::map<std::string, Value*>::const_iterator map_iterator;
  for (map_iterator it = pairs_.begin();
       it != pairs_.end(); it++) {
    str += spcs + it->first + " : " + it->second->toJSON(spc, true);
    if (i < sz - 1) {
      str += ",\n";
      i++;
    } else {
      str += "\n";
      }
  }

  return str;
}

std::string Pair::toJSON(int spc, bool skip_initial) const {
  std::string str = key_->toJSON(spc, false) + " : " + value_->toJSON(spc, true);
  return str;
}

std::string Array::toJSON(int spc, bool skip_initial) const {
  std::string spcs;
  for (int i = 0; i < spc; i++) {
    spcs += " ";
  }
  std::string str;
  if (skip_initial) {
    str += "[\n";
  } else {
    str += spcs + "[\n";
  }
  str += elems_->toJSON(spc + 1, false);
  str += spcs + "]";
  return str;
}

std::string Elements::toJSON(int spc, bool skip_initial) const {
  std::string str;

  unsigned int sz = values_.size();
  unsigned int i = 0;
  for (std::list<Value *>::const_iterator it = values_.begin();
       it != values_.end(); it++) {
    str += (*it)->toJSON(spc, false);
    if (i < sz - 1) {
      str += ",\n";
      i++;
    } else {
      str += "\n";
    }
  }

  return str;
}

std::string Value::toJSON(int spc, bool skip_initial) const {
  string spcs;
  for (int i = 0; i < spc; i++) {
    spcs += " ";
  }

  if (skip_initial)
    return value_;
  else
    return spcs + value_;
}


} // namespace json
