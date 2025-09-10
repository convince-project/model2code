# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier: APACHE-2.0

cmake_minimum_required(VERSION 3.16)
project(model2code)

# Option to request a portable CPU-compatible build. When ON the build adds
# conservative CPU flags (for example -march=x86-64) to avoid generating
# microarchitecture-specific instructions that could cause SIGILL on some
# systems when running prebuilt binaries.
option(PORTABLE_BUILD "Enable portable CPU flags for builds (adds -march=x86-64)" ON)

if(PORTABLE_BUILD)
  set(PORTABLE_CXX_FLAGS "-march=x86-64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PORTABLE_CXX_FLAGS}")
  message(STATUS "PORTABLE_BUILD is ON: adding '${PORTABLE_CXX_FLAGS}' to CMAKE_CXX_FLAGS")
else()
  message(STATUS "PORTABLE_BUILD is OFF")
endif()

find_package(tinyxml2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${TinyXML2_INCLUDE_DIRS}  # Include TinyXML2 directories
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
)


target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)

install(
  TARGETS ${PROJECT_NAME}
  COMPONENT utilities
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_sources( ${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Data.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/strManipulation.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Replacer.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Translator.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ExtractFromXML.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ExtractFromElement.cpp 
  )

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Command Line Tools")





