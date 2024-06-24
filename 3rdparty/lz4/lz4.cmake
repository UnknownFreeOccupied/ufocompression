include(FetchContent)

FetchContent_Declare(
	lz4
	GIT_REPOSITORY	https://github.com/lz4/lz4.git
	GIT_TAG 	      5ff839680134437dbf4678f3d0c7b371d84f4964 # v1.9.4
	GIT_PROGRESS    TRUE
	SOURCE_SUBDIR		build/cmake
)

FetchContent_MakeAvailable(lz4)