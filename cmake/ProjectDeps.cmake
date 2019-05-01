#---------------------------------------------
#
#---------------------------------------------

# find protobuf
find_package(Protobuf 3.5.0 REQUIRED)
if(PROTOBUF_FOUND)
    message ("protobuf found")
else()
    message (FATAL_ERROR "Cannot find Protobuf")
endif()
