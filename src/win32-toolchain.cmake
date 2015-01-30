# OS name
set(CMAKE_SYSTEM_NAME Windows)

# Set sysroot
#set(CMAKE_SYSROOT /usr/i686-w64-mingw32)

# Set compiler
set(CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++)

# Set directory path
set(CMAKE_LIBRARY_PATH /usr/i686-w64-mingw32/bin)
set(CMAKE_INCLUDE_PATH /usr/i686-w64-mingw32/include)
set(CMAKE_MODULE_PATH /usr/i686-w64-mingw32/cmake/Modules)

# Force CMake to search path
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
