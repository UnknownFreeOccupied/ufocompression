include(FetchContent)

FetchContent_Declare(
	lz4
	GIT_REPOSITORY	https://github.com/lz4/lz4.git
	GIT_TAG 	      ebb370ca83af193212df4dcbadcc5d87bc0de2f0 # v1.10.0
	GIT_PROGRESS    TRUE
	SOURCE_SUBDIR		build/cmake
)

FetchContent_MakeAvailable(lz4)