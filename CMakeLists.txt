# SPDX-License-Identifier: GPL-2.0-only
# Copyright © 2024 Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.29)
project(IEFxRack VERSION 1.0.0 LANGUAGES C CXX)

# BEGIN misc function definitions

function(begin_section_message section_title)
  message("\n------------------------------------------------------------")
  message("${section_title}\n")
endfunction()

# END misc function definitions

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

begin_section_message("Compiler Setup")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(APPLE) # LLVM Bug https://github.com/llvm/llvm-project/issues/86077
  set(CMAKE_OSX_DEPLOYMENT_TARGET 13.7)
  execute_process(COMMAND brew --prefix llvm OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE LLVM_PREFIX)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L${LLVM_PREFIX}/lib/c++")
  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -L${LLVM_PREFIX}/lib/c++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${LLVM_PREFIX}/lib/c++")
endif()
message("Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("version: ${CMAKE_CXX_COMPILER_VERSION}")
message("CXX standard: ${CMAKE_CXX_STANDARD}")
begin_section_message("Working environment")
message("System name: ${CMAKE_SYSTEM_NAME}")
message("System version: ${CMAKE_SYSTEM_VERSION}")
message("System processor: ${CMAKE_SYSTEM_PROCESSOR}")

message("\n------------------------------------------------------------")
message("Configuring ThirdParty Libraries")
message("\n------------------------------------------------------------")
add_subdirectory(ThirdParty/IECore)
message("Configuring JUCE")
set(JUCE_MODULES_ONLY TRUE)
add_subdirectory(ThirdParty/JUCE)
message("\n------------------------------------------------------------")
add_subdirectory(Source)
add_subdirectory(Application)