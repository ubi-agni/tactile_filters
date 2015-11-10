cmake_minimum_required(VERSION 2.8.3)
project(tactile_filters)
set(LIB_VERSION 0.0.1)

include(FindPkgConfig)
pkg_search_module(YAML yaml-cpp)

## Specify additional locations of header files
include_directories(${YAML_INCLUDE_DIRS})

## enable C++11 standard
add_definitions(-std=c++11)
if(YAML_FOUND)
   add_definitions(-DHAVE_YAML)
endif(YAML_FOUND)

set(HEADERS Range.h TactileValue.h TactileValueArray.h
    Calibration.h PieceWiseLinearCalib.h)
set(SOURCES Range.cpp TactileValue.cpp TactileValueArray.cpp
    PieceWiseLinearCalib.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME} ${YAML_LIBRARIES})

## testing
enable_testing()
add_subdirectory(test)

## install
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

## cmake package config
# Add all targets to the build-tree export set
export(TARGETS ${PROJECT_NAME}
  FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

configure_file(${PROJECT_NAME}Config.cmake.in
               ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
               @ONLY)
configure_file(${PROJECT_NAME}ConfigVersion.cmake.in
               ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
               @ONLY)
# install the *Config.cmake and *ConfigVersion.cmake
install(FILES
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION share)

# install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}Targets
  DESTINATION share)

## pkgconfig file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION lib/pkgconfig)
