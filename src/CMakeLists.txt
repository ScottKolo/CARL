find_package(Boost)
include_directories(../include)
include_directories(${Boost_INCLUDE_DIRS})

set(Boost_USE_MULTITHREADED ON)
set(CMAKE_CXX_FLAGS "-O3")

add_library(CARL SHARED heavy_edge_matching.cpp)
set_property(TARGET CARL PROPERTY CXX_STANDARD 11)
#target_include_directories (CARL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic")
endif()