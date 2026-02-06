cmake_minimum_required (VERSION 3.22)

project(stef_game1)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    1
)
set(BUILD_EXAMPLES ON)
FetchContent_MakeAvailable(raylib)


file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "stef_game1/*.cpp")
add_executable(stef_game1 ${SRC})
target_compile_features(stef_game1 PUBLIC cxx_std_23)
target_link_libraries(stef_game1 PRIVATE raylib)
