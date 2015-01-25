include_directories(${BOX2D_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

add_executable(bankrobbery
  main.cc
  local/Car.cc
  local/Map.cc
  local/Scenario.cc
)

target_link_libraries(bankrobbery
  gamebase0
  gamegraphics0
  ${BOX2D_LIBRARIES}
)

install(
  TARGETS bankrobbery
  RUNTIME DESTINATION games
)
