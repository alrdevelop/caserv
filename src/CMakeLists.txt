﻿# CMakeList.txt : CMake project for caserver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("caserver")

# Deps
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)

# Add source to this project's executable.
add_executable (caserver 
	"main.cpp" 
  "common/logger.h"
  "common/string.h"
  "openssl/defines.h"
)

target_link_libraries(caserver PRIVATE OpenSSL::SSL OpenSSL::Crypto spdlog::spdlog spdlog::spdlog_header_only)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET caserver PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
