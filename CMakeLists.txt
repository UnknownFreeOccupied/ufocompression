cmake_minimum_required(VERSION 3.5...3.16)

project(ufocompression
	VERSION 2.0.0
	DESCRIPTION "The UFO compression library"
	LANGUAGES CXX
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_subdirectory(1stparty)
add_subdirectory(3rdparty)

add_library(ufocompression SHARED
	src/ufo/compression/lz4.cpp
	src/ufo/compression/lzf.cpp
	src/ufo/compression/none.cpp
	src/ufo/compression/zlib.cpp
	src/ufo/compression/zstd.cpp
)
add_library(UFO::Compression ALIAS ufocompression)

set_target_properties(ufocompression PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
)

target_link_libraries(ufocompression PUBLIC UFO::Utility PRIVATE lz4 lzf zlib libzstd_static)

target_include_directories(ufocompression PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	# ${zstd_SOURCE_DIR}/lib
)

target_compile_features(ufocompression PUBLIC cxx_std_17)

include(GNUInstallDirs)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Only enable tests when we're the root project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  add_subdirectory(tests)
endif()

install(TARGETS ufocompression
	EXPORT ufocompressionTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT ufocompressionTargets
	FILE "ufocompressionTargets.cmake"
	NAMESPACE UFO::
	DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${PROJECT_SOURCE_DIR}/cmake/ufocompressionConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ufocompressionConfig.cmake"
	INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ufocompressionConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

INSTALL(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ufocompressionConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ufocompressionConfigVersion.cmake"
	DESTINATION lib/cmake/${PROJECT_NAME}
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include
)