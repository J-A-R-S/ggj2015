include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

set(LIBGAMEGRAPHICS0_SRC
  Action.cc
  Animation.cc
  Control.cc
  Entity.cc
  Group.cc
  Resource.cc
)

add_library(gamegraphics0 SHARED
  ${LIBGAMEGRAPHICS0_SRC}
)

if(WIN32)
  target_link_libraries(gamegraphics0
    gamebase0
    ${Boost_LIBRARIES}
    sfml-audio
    sfml-system
    sfml-network
    sfml-graphics
    sfml-window
  )
else(WIN32)
  target_link_libraries(gamegraphics0
    ${Boost_LIBRARIES}
    ${SFML2_LIBRARIES}
  )
endif(WIN32)

set_target_properties(gamegraphics0
  PROPERTIES
  VERSION ${CPACK_PACKAGE_VERSION}
  SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
)

if(UNIX)
  install(
    TARGETS gamegraphics0
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
endif(UNIX)

