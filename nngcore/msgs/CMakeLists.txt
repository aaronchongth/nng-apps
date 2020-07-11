PROTOBUF_GENERATE_CPP(
  PROTO_SRC 
  PROTO_HEADER 
  free_fleet_msgs.proto
)
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
  CACHE INTERNAL "Path to generated protobuf files.")
