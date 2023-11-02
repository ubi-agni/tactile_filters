cmake_minimum_required(VERSION 3.1.3)
project(tactile_filters VERSION 0.0.2)

find_package(PkgConfig REQUIRED)
pkg_search_module(YAML yaml-cpp)

## Specify additional locations of header files
include_directories(SYSTEM ${YAML_INCLUDE_DIRS})

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
target_link_libraries(${PROJECT_NAME} PRIVATE ${YAML_LIBRARIES})

## testing
enable_testing()
add_subdirectory(test)

## install
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  # specify where libs and binaries should go to
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  # specify where to install our headers
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
  # specify where includes were installed (for package export)
  INCLUDES DESTINATION include
)

## cmake package config
include(CMakePackageConfigHelpers)
configure_file("${PROJECT_NAME}Config.cmake.in"
               "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
               @ONLY)

write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${VERSION}"
  COMPATIBILITY AnyNewerVersion
  )

# install the export set for use in the install tree
install(EXPORT ${PROJECT_NAME}Targets
  DESTINATION cmake)

# install the *Config.cmake and *ConfigVersion.cmake
install(FILES
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION cmake)

## pkgconfig file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION lib/pkgconfig)

# install package.xml
install(FILES package.xml
        DESTINATION share/${PROJECT_NAME})
