cmake_minimum_required(VERSION 3.15)

include(FetchContent)

project(A_STAR_VISUALIZER)

set (CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp helper.cpp)

target_compile_options(main PRIVATE -Wall -pedantic)

##########
#SDL2

FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.32.8.zip    
)
FetchContent_MakeAvailable(SDL2)

if(TARGET SDL2::SDL2main)
    target_link_libraries(main PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(main PRIVATE SDL2::SDL2)

if(WIN32)
    # Path to SDL2.dll inside the FetchContent folder
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>              # DLL file location
            $<TARGET_FILE_DIR:main>                # Target output dir
    )
endif()

##########z






