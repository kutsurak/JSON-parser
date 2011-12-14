# A simple recursive descent JSON parser.
#
# Copyright (C) 2011  Panagiotis Koutsourakis
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(json_parser)


if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(BUILD_SHARED_LIBS False)
  set(CMAKE_CXX_FLAGS "-Wall -g")
else ()
  set(BUILD_SHARED_LIBS True)
  set(CMAKE_CXX_FLAGS "-Wall -O2")
endif ()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set (parser_sources
  global.h
  JSON.h
  JSON.cpp
  JSONParser.h
  JSONParser.cpp)

add_library(json_parser ${parser_sources})

set (example_sources
  JSONmain.cpp)

add_executable(json_parser_example ${example_sources})
target_link_libraries(json_parser_example json_parser)
