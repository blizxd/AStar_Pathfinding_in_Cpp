cmake_minimum_required(VERSION 3.15)
project(A_STAR_VISUALIZER)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp helper.cpp)
target_compile_options(main PRIVATE -Wall -pedantic)

add_subdirectory(external/SDL2)

target_link_libraries(main SDL2::SDL2)
if(TARGET SDL2::SDL2main)
    target_link_libraries(main SDL2::SDL2main)
endif()

if(WIN32)

add_custom_command(
  TARGET Sandbox POST_BUILD COMMAND
  cmake -E copy_if_different
    $<TARGET_FILE:SDL2>
    $<TARGET_FILE_DIR:main>
)

endif()
