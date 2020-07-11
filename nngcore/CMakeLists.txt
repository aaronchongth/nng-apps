cmake_minimum_required(VERSION 3.5.0)

project(nngcore VERSION 0.0.1 LANGUAGES C CXX)

find_package(ament_cmake REQUIRED)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_BUILD_TYPE)
  # Use the Release build type by default if the user has not specified one
  set(CMAKE_BUILD_TYPE Release)
endif()

include(GNUInstallDirs)

find_package(ament_cmake REQUIRED)
find_package(nng CONFIG REQUIRED)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
PROTOBUF_GENERATE_CPP(
  PROTO_SRC 
  PROTO_HEADER 
  msgs/free_fleet_msgs.proto
)
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
  CACHE INTERNAL "Path to generated protobuf files.")

add_library(nngcore
  src/Client.cpp
  ${PROTO_SRC}
  ${PROTO_HEADER}
)
target_link_libraries(nngcore
  ${Protobuf_LIBRARIES}
  nng::nng
)
target_include_directories(nngcore
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${ProtobufIncludePath}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${Protobuf_INCLUDE_DIRS}   
)

ament_export_interfaces(nngcore HAS_LIBRARY_TARGET)
ament_export_dependencies(Protobuf nng)

install(
  DIRECTORY include/nngcore
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  TARGETS nngcore
  EXPORT nngcore
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

ament_package()
