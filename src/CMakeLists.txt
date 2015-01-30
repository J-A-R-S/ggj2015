cmake_minimum_required(VERSION 2.8)

project(GAME)

include(CPackConfig.cmake)
if(UNIX)
  include(GNUInstallDirs)
endif(UNIX)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Box2D REQUIRED)

if(WIN32)
  find_package(SFML REQUIRED)
else(WIN32)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)
endif(WIN32)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib)
add_subdirectory(games)

if(UNIX)
  install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/include/game"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )

  install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/share/bankrobbery"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
  )
endif(UNIX)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
endif(DOXYGEN_FOUND)
