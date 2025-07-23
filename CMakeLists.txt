cmake_minimum_required(VERSION 3.15)
project(A_STAR_VISUALIZER)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp helper.cpp)
target_compile_options(main PRIVATE -Wall -pedantic)

# Add SDL2 subdirectory (assumes it builds the shared lib)
add_subdirectory(external/SDL2)

# Link against the dynamic SDL2 library and SDL2main
if(WIN32)

  target_link_libraries(main SDL2main)
  
  add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:SDL2>
      $<TARGET_FILE_DIR:main>
  )
endif()

target_link_libraries(main SDL2)


