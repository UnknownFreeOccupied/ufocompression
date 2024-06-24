include(FetchContent)

FetchContent_Declare(
	zlib
	GIT_REPOSITORY	https://github.com/zlib-ng/zlib-ng.git
	GIT_TAG 	      80514c17b384df68fbe83cca69ece0521b85f708 # 2.2.0
	GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(zlib)