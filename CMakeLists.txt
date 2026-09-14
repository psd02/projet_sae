cmake_minimum_required(VERSION 4.3)
project(projet_sae)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.0
)

FetchContent_MakeAvailable(SDL3)


add_executable(projet_sae main.cpp
        src/Affichage.cpp
        src/Affichage.h
)


target_link_libraries(projet_sae PRIVATE SDL3::SDL3)

# Copy SDL3.dll next to the executable
add_custom_command(TARGET projet_sae POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:projet_sae>
)