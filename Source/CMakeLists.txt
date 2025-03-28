# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(GLOB SOURCE_FILES "./*.cpp")
add_library(LIEFxRack STATIC ${SOURCE_FILES})
target_include_directories(LIEFxRack PUBLIC "./")
file(GLOB HEADER_FILES "./*.h")
set_property(TARGET LIEFxRack PROPERTY PUBLIC_HEADER ${HEADER_FILES})
target_link_libraries(LIEFxRack PUBLIC IECore)
target_link_libraries(LIEFxRack PUBLIC IEConcurrency)
add_library(Juce INTERFACE)
target_link_libraries(Juce INTERFACE 
  juce::juce_core 
  juce::juce_audio_utils
  juce::juce_osc
  juce::juce_audio_basics
  juce::juce_dsp)
target_link_libraries(LIEFxRack PUBLIC Juce)

set_target_properties(LIEFxRack PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message("Installation prefix: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS LIEFxRack
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(InstallRequiredSystemLibraries)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Resources" DESTINATION ".")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Requirements" DESTINATION ".")

get_target_property(DEPENDENCIES LIEFxRack LINK_LIBRARIES)
foreach(DEPENDENCY ${DEPENDENCIES})
  get_target_property(AliasProperty ${DEPENDENCY} ALIAS)
  if(TARGET ${DEPENDENCY} AND AliasProperty STREQUAL "NOTFOUND")
    message("Setting up dependency: ${DEPENDENCY}")

    set_target_properties(${DEPENDENCY} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    install(TARGETS ${DEPENDENCY}
        EXPORT "LIEFxRackTargets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()
endforeach()
