set(SOURCES cbor.c cbor_internal.c cbor_stack.c cbor_unicode.c cbor_encoders.c cbor_serializers.c)

set(CMAKE_SKIP_BUILD_RPATH FALSE)

if (NOT DEFINED CMAKE_MACOSX_RPATH)
	set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(cbor ${SOURCES})
add_library(cbor_shared SHARED ${SOURCES})

set_target_properties(cbor_shared PROPERTIES OUTPUT_NAME cbor)

install(TARGETS cbor cbor_shared
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
