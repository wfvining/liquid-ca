cmake_minimum_required(VERSION 3.1)
project(motion-ca)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(model SHARED
  src/Point.cpp
  src/Heading.cpp
  src/Agent.cpp
  src/Model.cpp
  src/Network.cpp
  src/Rule.cpp
  src/MovementRule.cpp)

find_package(Threads REQUIRED)
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

add_executable(one_d_lattice
  src/OneDLattice.cpp
  src/one_dimensional_lattice.cpp)

target_link_libraries(one_d_lattice model pthread)

add_executable(levy_walk
  src/levy_walk.cpp)
target_link_libraries(levy_walk model pthread)

add_executable(velocity_experiment
  src/velocity_experiment.cpp)
target_link_libraries(velocity_experiment model pthread)

add_executable(velocity_experiment_time
  src/velocity_experiment_time.cpp)
target_link_libraries(velocity_experiment_time model pthread)

add_executable(levy_viz
  src/levy_viz.cpp)
target_link_libraries(levy_viz model)

add_executable(network_statistics
  src/network_statistics.cpp)
target_link_libraries(network_statistics model)

add_executable(aggregate_to_dot
  src/aggregate_to_dot.cpp)
target_link_libraries(aggregate_to_dot model)

add_executable(random_spatial
  src/random_spatial.cpp)
target_link_libraries(random_spatial model pthread)

add_executable(random_regular
  src/random_regular_networks.cpp
  src/OneDLattice.cpp)
target_link_libraries(random_regular model pthread)

add_executable(density_sweep
  src/density_sweep.cpp)
target_link_libraries(density_sweep model pthread)

add_executable(velocity_sweep
  src/velocity_sweep.cpp)
target_link_libraries(velocity_sweep model pthread)

add_executable(density_history
  src/aggregate_density_history.cpp)
target_link_libraries(density_history model)

add_executable(synchronization
  src/synchronization.cpp)
target_link_libraries(synchronization model pthread)

add_executable(sim_viz
  src/sim_viz.cpp)
target_link_libraries(sim_viz model sfml-graphics sfml-window sfml-system)

configure_file(
  scripts/levy_ca_experiment.sh.in
  scripts/levy_ca_experiment.sh @ONLY)

configure_file(
  scripts/ca_performance.gp.in
  scripts/ca_performance.gp @ONLY)

## Build googletest and testcases
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# configure googletest
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
# build googletest
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

enable_testing()

add_executable(model_tests
  test/point_test.cpp
  test/heading_test.cpp
  test/agent_test.cpp
  test/model_test.cpp
  test/network_test.cpp
  test/model_stats_test.cpp
  test/rule_test.cpp)

target_link_libraries(model_tests model gmock_main)
add_test(ModelTests model_tests)
