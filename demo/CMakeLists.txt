cmake_minimum_required(VERSION 3.2.2)

find_package(Boost COMPONENTS system timer REQUIRED)
find_package(Boost)
include_directories(../include)
include_directories(.)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Demo demo.cpp)
#target_link_libraries (Demo LINK_PUBLIC CARL boost_graph)
target_link_libraries(Demo 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY}
  CARL
  boost_graph
)
set_property (TARGET Demo PROPERTY CXX_STANDARD 11)