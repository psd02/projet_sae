cmake_minimum_required(VERSION 4.3)
project(projet_sae)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.0
)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.2.0
)

FetchContent_MakeAvailable(SDL3 SDL3_image)


add_executable(projet_sae main.cpp
        src/Affichage.cpp
        src/Affichage.h
        src/background_sprite.h
        src/background_sprite_path.h
        src/Engine.cpp
        src/Engine.h
        src/json.hpp
)


target_link_libraries(projet_sae PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)


# Copy SDL3.dll next to the executable
add_custom_command(TARGET projet_sae POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:projet_sae>
)

# Same for SDL3_image
add_custom_command(TARGET projet_sae POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE_DIR:projet_sae>
)

# Add the assets folder
add_custom_command(TARGET projet_sae POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:projet_sae>/assets
)
