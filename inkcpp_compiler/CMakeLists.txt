list(APPEND SOURCES
    compiler.cpp binary_stream.h binary_stream.cpp json.hpp
    json_compiler.h json_compiler.cpp 
    emitter.h emitter.cpp
    reporter.h reporter.cpp
    binary_emitter.h binary_emitter.cpp
	list_data.h list_data.cpp
    command.cpp
    )
add_definitions(-DINK_EXPOSE_JSON)
add_library(inkcpp_compiler_o OBJECT ${SOURCES})
add_library(inkcpp_compiler $<TARGET_OBJECTS:inkcpp_compiler_o>)

target_include_directories(inkcpp_compiler_o PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_include_directories(inkcpp_compiler PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
FILE(GLOB PUBLIC_HEADERS "include/*")
set_target_properties(inkcpp_compiler PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_link_libraries(inkcpp_compiler PRIVATE inkcpp_shared)
target_link_libraries(inkcpp_compiler_o PRIVATE inkcpp_shared)

# Make sure this project and all dependencies use the C++17 standard
target_compile_features(inkcpp_compiler PUBLIC cxx_std_17)
target_compile_features(inkcpp_compiler PUBLIC cxx_std_17)

# Unreal installation
list(REMOVE_ITEM SOURCES "json.hpp")
install(FILES "json.hpp" DESTINATION "inkcpp/Source/ThirdParty/Private/"
	COMPONENT unreal EXCLUDE_FROM_ALL)
install(DIRECTORY "include/" DESTINATION "inkcpp/Source/inkcpp_editor/Private/ink/" COMPONENT unreal EXCLUDE_FROM_ALL)
install(FILES ${SOURCES} DESTINATION "inkcpp/Source/inkcpp_editor/Private/ink/" COMPONENT unreal EXCLUDE_FROM_ALL)
