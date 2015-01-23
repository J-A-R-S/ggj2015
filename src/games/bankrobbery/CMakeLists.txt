add_executable(bankrobbery
  main.cc
)

target_link_libraries(bankrobbery
  gamebase0
  gamegraphics0
  gamemodel0
)

install(
  TARGETS bankrobbery
  RUNTIME DESTINATION games
)
