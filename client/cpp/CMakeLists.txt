# SOPMQ - Scalable optionally persistent message queue
# Copyright 2014 InWorldz, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

PROJECT( sopmq-client )
cmake_minimum_required(VERSION 2.8)

set(INCLUDES ${THIRDPARTY_DIR})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 1)

set(DEFS ${DEFS} -DBOOST_ALL_DYN_LINK)

find_package(Boost COMPONENTS system program_options log thread chrono REQUIRED)
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
set(LIBS ${LIBS} ${OPENSSL_LIBRARIES})

set(INCLUDES ${INCLUDES} ${CRYPTOPP_INCLUDE_DIR})
set(LIBS ${LIBS} ${CRYPTOPP_LIBRARIES})

find_package(Protobuf REQUIRED)
set(INCLUDES ${INCLUDES} ${PROTOBUF_INCLUDE_DIR})
set(LIBS ${LIBS} ${PROTOBUF_LIBRARIES})

add_definitions(${DEFS})
file(GLOB LIB_SRCS src/*.h src/*.cpp)

#file(GLOB MESSAGE_SRCS ../../messages/cpp/*.h ../../messages/cpp/*.cc ../../messages/cpp/*.cpp)

set(INCLUDES ${INCLUDES} ${SHARED_DIR}/src)
set(LIBS ${LIBS} sopmq-shared)

set(INCLUDES ${INCLUDES} ../../messages/cpp)
include_directories(${INCLUDES})

add_library(sopmq-client ${LIB_SRCS} ${MESSAGE_SRCS})
target_link_libraries(sopmq-client ${LIBS})
