cmake_minimum_required(VERSION 3.5.0)

project(core VERSION 0.0.1 LANGUAGES C CXX)

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

add_library(core
  src/Client.cpp
  ${PROTO_SRC}
  ${PROTO_HEADER}
)
target_link_libraries(core
  ${Protobuf_LIBRARIES}
  nng::nng
)
target_include_directories(core
  PUBLIC
    ${Protobuf_INCLUDE_DIR}
)

ament_export_interfaces(core HAS_LIBRARY_TARGET)
ament_export_dependencies(Protobuf)

# add_executable(client
#   src/client.cpp
# )
# target_link_libraries(client
#   nng::nng
#   ${Protobuf_LIBRARIES}
# )
# target_include_directories(client
#   PRIVATE
#     ${Protobuf_INCLUDE_DIR}
# )

# add_executable(server
#   src/server.cpp
#   src/stringmsg.pb.cc
#   src/addressbook.pb.cc
# )
# target_link_libraries(server
#   nng::nng
#   ${Protobuf_LIBRARIES}
# )
# target_include_directories(server
#   PRIVATE
#     ${Protobuf_INCLUDE_DIR}
# )

# install(
#   TARGETS 
#     client
#     server
#   RUNTIME DESTINATION lib/nng-apps
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib
# )

install(
  TARGETS core
  EXPORT core
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

ament_package()
