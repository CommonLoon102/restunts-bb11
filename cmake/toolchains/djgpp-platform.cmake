# Override CMake's default Watcom conventions after Platform/DOS.cmake.
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_DL_LIBS "")
