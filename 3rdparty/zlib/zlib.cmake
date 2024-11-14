include(FetchContent)

FetchContent_Declare(
	zlib
	GIT_REPOSITORY	https://github.com/zlib-ng/zlib-ng.git
	GIT_TAG 	      c939498c7f2a1d645833a98251365e2c814fd736 # 2.2.2
	GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(zlib)