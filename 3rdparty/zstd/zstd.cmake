include(FetchContent)

set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

FetchContent_Declare(
	zstd
	GIT_REPOSITORY	https://github.com/facebook/zstd.git
	GIT_TAG 	      794ea1b0afca0f020f4e57b6732332231fb23c70 # v1.5.6
	GIT_PROGRESS    TRUE
	SOURCE_SUBDIR		build/cmake
)

FetchContent_MakeAvailable(zstd)