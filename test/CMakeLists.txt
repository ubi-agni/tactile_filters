enable_testing()
find_package(GTest)

if(GTEST_FOUND)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

# unittest sources
file(GLOB TEST_SOURCES test_*.cpp)
add_executable(unittests ${TEST_SOURCES})
target_link_libraries(unittests ${GTEST_BOTH_LIBRARIES} pthread ${PROJECT_NAME})

add_test(
  NAME unittests
  COMMAND unittests
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif(GTEST_FOUND)
