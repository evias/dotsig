# info
cmake_minimum_required(VERSION 3.15)
project(dotsig VERSION 1.0.0)

# system
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
set(BOTAN_DEPS_VERSION 3.4.0)

if (WIN32)
  set(CMAKE_PREFIX_PATH "C:/Botan/lib/cmake/Botan-3.4.0")
  set(BOTAN_INCLUDE_PATH "C:/Botan/include/botan-3")
  set(BOTAN_HEADER_FILES "${BOTAN_INCLUDE_PATH}/botan")
  set(BOTAN_LIB "C:/Botan/lib/botan-3.lib")
  set(BOTAN_DLL "C:/Botan/bin/botan-3.dll")
else()
  set(BOTAN_INCLUDE_PATH /usr/local/include/botan-3)
  set(BOTAN_HEADER_FILES "${BOTAN_INCLUDE_PATH}/botan")
  set(BOTAN_LIB /usr/local/lib/libbotan-3.a)
  set(OTHER_LIBS "-lpthread")
  find_file(
    libstdcpp-static
    libstdc++.a
    HINTS /usr/local/gcc-13.2.0
    HINTS /usr/local/lib/gcc/current
  )
endif()

find_package(Botan ${BOTAN_DEPS_VERSION} REQUIRED)

# licensing
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/LICENSE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt)
file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/README.md ${CMAKE_CURRENT_BINARY_DIR}/README.txt)

# sources
add_subdirectory(src/)
add_executable(dotsig ${DOTSIG_SOURCES} src/main.cpp)
add_library(Botan-static INTERFACE ${BOTAN_HEADER_FILES})

# link main
target_include_directories(dotsig PUBLIC ${BOTAN_INCLUDE_PATH})
target_link_libraries(
  dotsig
  Botan-static
  ${BOTAN_LIB}
  ${libstdcpp-static}
  ${OTHER_LIBS}
)

# installation
install(TARGETS dotsig)
if (WIN32)
  install(FILES ${BOTAN_DLL})
endif()

# package managers
set(CPACK_PACKAGE_NAME "dotsig")
set(CPACK_PACKAGE_VENDOR "evi.as")
string(
  CONCAT dotsig_description
  "dotsig is a powerful tool to create and verify "
  "digital signatures using RSA cryptography (PKCS1 v1.5), "
  "PGP and elliptic-curve cryptography (ECDSA)."
)
set(CPACK_PACKAGE_DESCRIPTION ${dotsig_description})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${dotsig_description})
set(CPACK_PACKAGE_EXECUTABLES "dotsig;dotsig")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
set(CPACK_NSIS_PACKAGE_NAME "dotsig")

include(CPack)