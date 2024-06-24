cmake_minimum_required(VERSION 3.5...3.16)

project(lzf
	VERSION 3.6
	DESCRIPTION "LZF"
	LANGUAGES C
)

add_library(lzf STATIC src/lzf_c.c src/lzf_d.c)

target_include_directories(lzf PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include
)