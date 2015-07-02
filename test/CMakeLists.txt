enable_testing()

######################################################
# build gtest
include(ExternalProject)
set(GTEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest)

# try to use /usr/src/gtest
find_path(GTEST_SOURCE CMakeLists.txt PATHS /usr/src/gtest DOC "local gtest source dir" NO_DEFAULT_PATH)
find_path(GTEST_INCLUDE_DIRS gtest/gtest.h DOC "gtest include dir")
if(GTEST_SOURCE AND GTEST_INCLUDE_DIRS)
  ExternalProject_Add(GTest
    SOURCE_DIR /usr/src/gtest
    PREFIX ${GTEST_DIR}
    BINARY_DIR ${GTEST_DIR}
    INSTALL_COMMAND "")
  set(GTEST_FOUND TRUE)
else()
  ExternalProject_Add(GTest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    URL_HASH SHA1=f85f6d2481e2c6c4a18539e391aa4ea8ab0394af
    PREFIX ${GTEST_DIR}
    BINARY_DIR ${GTEST_DIR}
    INSTALL_COMMAND "")
  set(GTEST_INCLUDE_DIRS ${GTEST_DIR}/src/GTest/include)
  set(GTEST_FOUND TRUE)
endif()

if(GTEST_FOUND)
  add_library(gtest IMPORTED STATIC GLOBAL)
  set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION ${GTEST_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})

  add_library(gtest_main IMPORTED STATIC GLOBAL)
  set_target_properties(gtest_main PROPERTIES
    IMPORTED_LOCATION ${GTEST_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
  message(WARNING "Google Test not available, skipping unittests")
endif()
#####################################################

if(GTEST_FOUND)
include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

# unittest sources
file(GLOB TEST_SOURCES test_*.cpp)
add_executable(unittests ${TEST_SOURCES})
add_dependencies(unittests ${PROJECT_NAME} GTest)
target_link_libraries(unittests gtest gtest_main pthread ${PROJECT_NAME})

add_test(
  NAME unittests
  COMMAND unittests
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif(GTEST_FOUND)
