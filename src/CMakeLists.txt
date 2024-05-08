file(GLOB dotsig_api_SRC
  "*.h"
  "[!main]*.cpp"
)

set(DOTSIG_SOURCES ${dotsig_api_SRC} PARENT_SCOPE)