# info
cmake_minimum_required(VERSION 3.15)
project(dotsig)

# system
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
set(BOTAN_DEPS_VERSION 3.4.0)
find_package(Botan ${BOTAN_DEPS_VERSION} REQUIRED)

# find libstdc++ on build host
# todo: missing HINTS/PATHS for Windows
find_file(
  libstdcpp-static
  libstdc++.a
  HINTS /usr/local/gcc-13.2.0
  HINTS /usr/local/lib/gcc/current
)

# sources
add_subdirectory(src/)
add_executable(dotsig ${DOTSIG_SOURCES} src/main.cpp)
add_library(Botan-static INTERFACE /usr/local/include/botan-3/botan)

# link main
target_include_directories(dotsig PUBLIC /usr/local/include/botan-3)
target_link_libraries(
  dotsig
  Botan-static
  /usr/local/lib/libbotan-3.a
  ${libstdcpp-static}
  -lpthread
)