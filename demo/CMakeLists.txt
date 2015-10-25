include_directories(../include)
include_directories(.)
add_executable(Demo demo.cpp mmio.c)
target_link_libraries (Demo LINK_PUBLIC CARL)