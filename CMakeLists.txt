cmake_minimum_required(VERSION 3.0.1)
project(motion-ca)

include_directories(include)

add_library(model SHARED
  src/Point.cpp
  src/Heading.cpp
  )

find_package(Threads REQUIRED)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(model_tests
  test/point_test.cpp
  test/heading_test.cpp)

# Google test must link pthreads, apparently this is not pulled in
# automatically by the find_package(GTest) command.
target_link_libraries(model_tests model ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(ModelTests model_tests)
