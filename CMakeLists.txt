cmake_minimum_required(VERSION 3.5.0)

project(nng-apps VERSION 0.0.1 LANGUAGES C CXX)

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

add_executable(client
  src/client.cpp
)
target_link_libraries(client
  nng::nng
)

add_executable(server
  src/server.cpp
)
target_link_libraries(server
  nng::nng
)

install(
  TARGETS 
    client
    server
  RUNTIME DESTINATION lib/nng-apps
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

ament_package()
