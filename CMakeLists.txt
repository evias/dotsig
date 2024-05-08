# info
cmake_minimum_required(VERSION 3.15)
project(dotsig)

# system
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
set(BOTAN_DEPS_VERSION 3.4.0)
find_package(Botan ${BOTAN_DEPS_VERSION} REQUIRED)

# sources
add_subdirectory(src/)
add_executable(dotsig ${DOTSIG_SOURCES} src/main.cpp)

# link main
target_link_libraries(dotsig Botan::Botan)