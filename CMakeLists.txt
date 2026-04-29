cmake_minimum_required(VERSION 3.5)
project(FireShader)
enable_language(RC)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Including GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

# Including GLAD
include_directories(glad/include)

# Including SLD and SLD_MIXER

include_directories(sdl/SDL2-2.32.0/include)
include_directories(sdl/SDL2_mixer-2.8.1/include)
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/sdl/SDL2-2.32.0/lib/x64")
set(SDL2_MIXER_LIB_DIR "${CMAKE_SOURCE_DIR}/sdl/SDL2_mixer-2.8.1/lib/x64")

add_library(SDL2main STATIC IMPORTED)
set_target_properties(SDL2main PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIB_DIR}/SDL2main.lib"
)
add_library(SDL2Lib STATIC IMPORTED)
set_target_properties(SDL2Lib PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIB_DIR}/SDL2.lib"
)
add_library(SDL2MixerLib STATIC IMPORTED)
set_target_properties(SDL2MixerLib PROPERTIES
IMPORTED_LOCATION "${SDL2_MIXER_LIB_DIR}/SDL2_mixer.lib"
)

# Including freetype
include_directories(freetype/include)
set(FREETYPE_LIB_PATH "${CMAKE_SOURCE_DIR}/freetype/lib/win64/freetype.lib")
add_library(freetype STATIC IMPORTED)
set_target_properties(freetype PROPERTIES IMPORTED_LOCATION ${FREETYPE_LIB_PATH})

set(SOURCE_FILES
        src/main.cpp
        src/Camera3D.h
        src/Camera3D.cpp
        glad/src/glad.c
        src/Rendering/Shader.cpp
        src/Rendering/Shader.h
        src/Rendering/Texture.cpp
        src/Rendering/Texture.h
        src/ResourceManager.cpp
        src/ResourceManager.h
        src/Rendering/stb_image.h
        src/Rendering/SpriteRenderer.cpp
        src/Rendering/SpriteRenderer.h
        src/Rendering/PostProcessor.cpp
        src/Rendering/PostProcessor.h
        src/AudioEngine.cpp
        src/AudioEngine.h
        src/Rendering/TextRenderer.cpp
        src/Rendering/TextRenderer.h
        src/CubesTestingScene.h
        src/CubesTestingScene.cpp
        src/Object3D.h
        src/Object3D.cpp
        src/Rendering/LightEmissor.h
        src/Rendering/LightEmissor.cpp
        src/Mesh.cpp
        src/Mesh.h
#        src/LightTestScene.h
#        src/LightTestScene.cpp
        src/FireScene.cpp
        src/FireScene.h
        src/Material.cpp
        src/Material.h
#        src/ParticleManager.cpp
#        src/ParticleManager.h
        src/Spark.cpp
        src/Spark.h

)


add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES}
        #src/GameLogic/Game.cpp
        #src/GameLogic/Game.h
        #src/main.cpp
)
if(MINGW)
    # This allows us to execute from outside CLion
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
            "-static-libgcc"
            "-static-libstdc++"
            "-static"
    )
endif()
# Linking GLFW, OGL, SDL y SDL_Mixer

target_link_libraries(${CMAKE_PROJECT_NAME}
        glfw
        ${GLFW_LIBRARIES}
        ${OPENGL_LIBRARY}

        # Imported targets (SDL)
        SDL2main
        SDL2Lib
        SDL2MixerLib

        # FreeType
        freetype

        # Gemini says this is necessary but idc
#        ws2_32
#        winmm
#        version
)


# glm included here
include_directories(glm/glm)
include_directories(glm/glm/gtx) # useful for to_string
include_directories(src)
include_directories(src/Rendering)
# also source files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Copiar la carpeta de assets a la build
return()

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(BUILD_ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
file(MAKE_DIRECTORY ${BUILD_ASSETS_DIR})
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${BUILD_ASSETS_DIR}
        COMMENT "Copying assets folder to the build directory..."
)
