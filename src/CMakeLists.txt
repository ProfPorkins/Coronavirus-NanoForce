cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "CoronavirusNF")
project(${PROJECT_NAME})

# 
# Great git submodules Reference: https://www.vogella.com/tutorials/GitSubmodules/article.html
#

#
# ------------------------ Add SFML ------------------------
# This causes the SFML .dll files to be copied into the Multiplayer-Step0-ECS executable folder.
# Only need this for development time, for a production release statically link
# everything together: set BUILD_SHARED_LIBS to FALSE
# Reference: https://en.sfml-dev.org/forums/index.php?topic=24070.0
# Reference: https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake
#
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(ThirdParty/SFML)
include_directories(ThirdParty/SFML/include)

#
# ------------------------ Add CTTI ------------------------
# Reference: https://github.com/Manu343726/ctti
#
add_subdirectory(ThirdParty/ctti)
include_directories(ThirdParty/ctti/include)

#
# ------------------------ Add RapidJSON ------------------------
# Reference: https://github.com/Tencent/rapidjson
#
add_subdirectory(ThirdParty/rapidjson)
include_directories(ThirdParty/rapidjson/include)

#
# ------------------------ Add the Client Project ------------------------
# Manually specifying all the source files.
#
set(CLIENT_HEADER_FILES
    GameModel.hpp
    )
set(CLIENT_SOURCE_FILES
    main.cpp
    GameModel.cpp
    )

set(CLIENT_COMPONENTS_HEADERS
    components/Age.hpp
    components/AnimatedSprite.hpp
    components/Audio.hpp
    components/Birth.hpp
    components/Bomb.hpp
    components/Bullets.hpp
    components/Collidable.hpp
    components/Component.hpp
    components/Control.hpp
    components/Damage.hpp
    components/Drag.hpp
    components/Health.hpp
    components/Lifetime.hpp
    components/Momentum.hpp
    components/Orientation.hpp
    components/Position.hpp
    components/Powerup.hpp
    components/Size.hpp
    components/Sprite.hpp
    )
set(CLIENT_COMPONENTS_SOURCES
    components/Size.cpp
    )

set(CLIENT_ENTITIES_HEADERS
    entities/Bomb.hpp
    entities/Bullet.hpp
    entities/Entity.hpp
    entities/Player.hpp
    entities/Powerup.hpp
    entities/PowerupBomb.hpp
    entities/PowerupRapidFire.hpp
    entities/PowerupSpreadFire.hpp
    entities/Virus.hpp
    entities/Weapon.hpp
    entities/WeaponBomb.hpp
    entities/WeaponEmpty.hpp
    entities/WeaponGun.hpp
    entities/WeaponRapidFire.hpp
    entities/WeaponSpreadFire.hpp
    )
set(CLIENT_ENTITIES_SOURCES
    entities/Bomb.cpp
    entities/Bullet.cpp
    entities/Entity.cpp
    entities/Player.cpp
    entities/Powerup.cpp
    entities/Virus.cpp
    entities/Weapon.cpp
    entities/WeaponBomb.cpp
    entities/WeaponGun.cpp
    entities/WeaponRapidFire.cpp
    entities/WeaponSpreadFire.cpp
    )

set(CLIENT_LEVEL_HEADERS
    levels/Level.hpp
    levels/PetriDish.hpp
    levels/LevelName.hpp
    )
set(CLIENT_LEVEL_SOURCES
    levels/Level.cpp
    levels/PetriDish.cpp
    )

set(CLIENT_MISC_HEADERS
    misc/ConcurrentQueue.hpp
    misc/math.hpp
    misc/misc.hpp
    )
set(CLIENT_MISC_SOURCES
    misc/math.cpp
    misc/misc.cpp
    )

set(CLIENT_RENDERERS_HEADERS
    renderers/Background.hpp
    renderers/GameStatus.hpp
    renderers/HUD.hpp
    )
set(CLIENT_RENDERERS_SOURCES
    renderers/Background.cpp
    renderers/GameStatus.cpp
    renderers/HUD.cpp
    )

set(CLIENT_SERVICES_HEADERS
    services/Configuration.hpp
    services/ConfigurationPath.hpp
    services/Content.hpp
    services/ContentKey.hpp
    services/KeyboardInput.hpp
    services/MouseInput.hpp
    services/SoundPlayer.hpp
    )
set(CLIENT_SERVICES_SOURCES
    services/Configuration.cpp
    services/Content.cpp
    services/KeyboardInput.cpp
    services/MouseInput.cpp
    services/SoundPlayer.cpp
    )

set(CLIENT_SYSTEMS_HEADERS
    systems/Age.hpp
    systems/AnimatedSprite.hpp
    systems/Birth.hpp
    systems/Collision.hpp
    systems/Health.hpp
    systems/Lifetime.hpp
    systems/Movement.hpp
    systems/Particle.hpp
    systems/ParticleSystem.hpp
    systems/Powerup.hpp
    systems/RendererAnimatedSprite.hpp
    systems/RendererParticleSystem.hpp
    systems/RendererSprite.hpp
    systems/RendererVirus.hpp
    systems/System.hpp
    )
set(CLIENT_SYSTEMS_SOURCES
    systems/Age.cpp
    systems/AnimatedSprite.cpp
    systems/Birth.cpp
    systems/Collision.cpp
    systems/Health.cpp
    systems/Lifetime.cpp
    systems/Movement.cpp
    systems/ParticleSystem.cpp
    systems/Powerup.cpp
    systems/RendererAnimatedSprite.cpp
    systems/RendererParticleSystem.cpp
    systems/RendererSprite.cpp
    systems/RendererVirus.cpp
    systems/System.cpp
    )

set(CLIENT_PARTICLE_EFFECTS_HEADERS
    systems/effects/CircleExpansionEffect.hpp
    systems/effects/ParticleEffect.hpp
    systems/effects/PlayerStartEffect.hpp
    )
set(CLIENT_PARTICLE_EFFECTS_SOURCES
    systems/effects/CircleExpansionEffect.cpp
    systems/effects/ParticleEffect.cpp
    systems/effects/PlayerStartEffect.cpp
    )

set(CLIENT_UIFRAMEWORK_HEADERS
    UIFramework/Activate.hpp
    UIFramework/Element.hpp
    UIFramework/KeyboardOption.hpp
    UIFramework/MenuItem.hpp
    UIFramework/Selection.hpp
    UIFramework/Text.hpp
    )
set(CLIENT_UIFRAMEWORK_SOURCES
    UIFramework/Activate.cpp
    UIFramework/KeyboardOption.cpp
    UIFramework/MenuItem.cpp
    UIFramework/Selection.cpp
    UIFramework/Text.cpp
    )

set(CLIENT_VIEWS_HEADERS
    views/About.hpp
    views/Credits.hpp
    views/Gameplay.hpp
    views/LevelSelect.hpp
    views/MainMenu.hpp
    views/MenuView.hpp
    views/Settings.hpp
    views/View.hpp
    views/ViewState.hpp
    )
set(CLIENT_VIEWS_SOURCES
    views/About.cpp
    views/Credits.cpp
    views/Gameplay.cpp
    views/LevelSelect.cpp
    views/MainMenu.cpp
    views/MenuView.cpp
    views/Settings.cpp
    )

#
# Organize the files into some logical groups
#
source_group("Main\\Header Files" FILES ${CLIENT_HEADER_FILES})
source_group("Main\\Source Files" FILES ${CLIENT_SOURCE_FILES})

source_group("ECS\\Components\\Header Files" FILES ${CLIENT_COMPONENTS_HEADERS})
source_group("ECS\\Components\\Source Files" FILES ${CLIENT_COMPONENTS_SOURCES})

source_group("ECS\\Entities\\Header Files" FILES ${CLIENT_ENTITIES_HEADERS})
source_group("ECS\\Entities\\Source Files" FILES ${CLIENT_ENTITIES_SOURCES})

source_group("ECS\\Systems\\Header Files" FILES ${CLIENT_SYSTEMS_HEADERS})
source_group("ECS\\Systems\\Source Files" FILES ${CLIENT_SYSTEMS_SOURCES})

source_group("ECS\\Systems\\Effects\\Header Files" FILES ${CLIENT_PARTICLE_EFFECTS_HEADERS})
source_group("ECS\\Systems\\Effects\\Source Files" FILES ${CLIENT_PARTICLE_EFFECTS_SOURCES})

source_group("Levels\\Header Files" FILES ${CLIENT_LEVEL_HEADERS})
source_group("Levels\\Source Files" FILES ${CLIENT_LEVEL_SOURCES})

source_group("Misc\\Header Files" FILES ${CLIENT_MISC_HEADERS})
source_group("Misc\\Source Files" FILES ${CLIENT_MISC_SOURCES})

source_group("Renderers\\Header Files" FILES ${CLIENT_RENDERERS_HEADERS})
source_group("Renderers\\Source Files" FILES ${CLIENT_RENDERERS_SOURCES})

source_group("Services\\Header Files" FILES ${CLIENT_SERVICES_HEADERS})
source_group("Services\\Source Files" FILES ${CLIENT_SERVICES_SOURCES})

source_group("UI\\Framework\\Header Files" FILES ${CLIENT_UIFRAMEWORK_HEADERS})
source_group("UI\\Framework\\Source Files" FILES ${CLIENT_UIFRAMEWORK_SOURCES})

source_group("UI\\Views\\Header Files" FILES ${CLIENT_VIEWS_HEADERS})
source_group("UI\\Views\\Source Files" FILES ${CLIENT_VIEWS_SOURCES})

#
# Need a list of all code files for convenience
#
set(CLIENT_CODE_FILES
    ${CLIENT_SOURCE_FILES}
    ${CLIENT_HEADER_FILES}
    ${CLIENT_COMPONENTS_HEADERS}
    ${CLIENT_COMPONENTS_SOURCES}
    ${CLIENT_ENTITIES_HEADERS}
    ${CLIENT_ENTITIES_SOURCES}
    ${CLIENT_LEVEL_HEADERS}
    ${CLIENT_LEVEL_SOURCES}
    ${CLIENT_MISC_HEADERS}
    ${CLIENT_MISC_SOURCES}
    ${CLIENT_PARTICLE_EFFECTS_HEADERS}
    ${CLIENT_PARTICLE_EFFECTS_SOURCES}
    ${CLIENT_RENDERERS_HEADERS}
    ${CLIENT_RENDERERS_SOURCES}
    ${CLIENT_SERVICES_HEADERS}
    ${CLIENT_SERVICES_SOURCES}
    ${CLIENT_SYSTEMS_HEADERS}
    ${CLIENT_SYSTEMS_SOURCES}
    ${CLIENT_UIFRAMEWORK_HEADERS}
    ${CLIENT_UIFRAMEWORK_SOURCES}
    ${CLIENT_VIEWS_HEADERS}
    ${CLIENT_VIEWS_SOURCES}
    )

#
# This is the program executable target
#
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # For Windows, linking with sfml-main so SFML's main is used, allow subsystem type to
    # be Windows instead of Console, to prevent the display of a console window on startup.
    # Reference: https://en.sfml-dev.org/forums/index.php?topic=6278.0
    add_executable(${PROJECT_NAME} WIN32 ${CLIENT_CODE_FILES})
    target_link_libraries(${PROJECT_NAME} sfml-main sfml-graphics sfml-audio sfml-system sfml-window)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(${PROJECT_NAME} ${CLIENT_CODE_FILES})
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sfml-window)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
    add_executable(${PROJECT_NAME} ${CLIENT_CODE_FILES})
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sfml-window)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# Want the C++ 17 standard for our project
#
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#
# Enable a lot of warnings, forcing better code to be written
#
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra -pedantic) # -Wconversion -Wsign-conversion
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-R,\$ORIGIN")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra -pedantic)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

#
# Enable static multithreaded library linking for MSVC
# Reference: https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html#prop_tgt:MSVC_RUNTIME_LIBRARY
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME}
 PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()

#
# Copy the default configuration file into the build folder so it is available at runtime
# 
set(DEFAULT_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/client.settings.json)
set(BUILD_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/client.settings.json)
if (NOT EXISTS ${BUILD_CONFIG_FILE} OR ${DEFAULT_CONFIG_FILE} IS_NEWER_THAN ${BUILD_CONFIG_FILE})
   configure_file(${DEFAULT_CONFIG_FILE} ${BUILD_CONFIG_FILE} COPYONLY)
endif()

add_dependencies(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sfml-window)

#
# Move the assets into the build folder so they load at runtime
#
set(ASSETS_DIR assets)
set(ASSETS_FONT_DIR ${ASSETS_DIR}/fonts)
set(ASSETS_IMAGE_DIR ${ASSETS_DIR}/images)
set(ASSETS_AUDIO_DIR ${ASSETS_DIR}/audio)
set(ASSETS_MUSIC_DIR ${ASSETS_DIR}/music)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_FONT_DIR}/Shojumaru-Regular.ttf
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_FONT_DIR}/Shojumaru-Regular.ttf
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/menu-background-2.jpg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/menu-background-2.jpg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-1.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-1.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-2.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-2.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-3.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-3.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-4.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-4.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-5.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-5.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/petri-6.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/petri-6.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/playerShip1_blue.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/playerShip1_blue.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/sars-cov-2.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/sars-cov-2.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/antibody.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/antibody.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/bomb.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/bomb.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/player-start-particle.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/player-start-particle.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/virus-particle.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/virus-particle.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/powerup-rapid-fire.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/powerup-rapid-fire.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/powerup-spread-fire.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/powerup-spread-fire.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_IMAGE_DIR}/powerup-bomb.png
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_IMAGE_DIR}/powerup-bomb.png
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/menu-activate.wav
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/menu-activate.wav
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/menu-accept.wav
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/menu-accept.wav
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/powerup.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/powerup.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/fire.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/fire.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/fire-bomb.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/fire-bomb.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/explode-bomb.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/explode-bomb.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/player-start.wav
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/player-start.wav
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/player-death.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/player-death.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/virus-death.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/virus-death.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_AUDIO_DIR}/thruster-level3.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_AUDIO_DIR}/thruster-level3.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_MUSIC_DIR}/background-music-1.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_MUSIC_DIR}/background-music-1.ogg
)

add_custom_command(
    TARGET ${PROJECT_NAME}
 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_MUSIC_DIR}/background-music-2.ogg
            ${CMAKE_CURRENT_BINARY_DIR}/${ASSETS_MUSIC_DIR}/background-music-2.ogg
)

#
# ------------------------ Clang Format ------------------------
#
#
# Prepare a pre-build step to run clang-format over all the [ch]pp source files.
# Start by finding the location of the clang-format executable.
#
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    #
    # Need to take the simple source file locations used for the project and get their full
    # file system locations for use in putting together the clang-format command line
    #
    unset(CODE_FILES_PATHS)
    foreach(CODE_FILE ${CLIENT_CODE_FILES})
        get_source_file_property(WHERE "${CODE_FILE}" LOCATION)
        set(CODE_FILES_PATHS ${CODE_FILES_PATHS} ${WHERE})
    endforeach()

    #
    # This creates the clang-format target/command
    #
    add_custom_target(
        ClangFormat
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${CODE_FILES_PATHS}
    )
else()
    message("Unable to find clang-format")
endif()
