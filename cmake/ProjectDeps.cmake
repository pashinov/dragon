#---------------------------------------------
#
#---------------------------------------------

# find protobuf
find_package(Protobuf 3.7 REQUIRED)
if(PROTOBUF_FOUND)
    message ("protobuf found")
else()
    message (FATAL_ERROR "Cannot find Protobuf")
endif()
