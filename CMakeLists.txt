cmake_minimum_required(VERSION 3.0)
project(Shaderprogramming)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src")

##-------------------------------add your source files here-------------------------------------------------------------
##        new source paths must be added to the include list, if contained headers are used elsewhere!
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderable.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderable.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/SpotLight.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/SpotLight.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/PointLight.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/PointLight.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Texture2D.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Texture2D.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/DroneController.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/DroneController.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/DroneAnimator.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/DroneAnimator.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/CollectibleManager.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/CollectibleManager.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Bounds.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Bounds.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/TextRenderer.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/TextRenderer.cpp")

list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/fragment.glsl")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/vertex.glsl")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/textFrag.glsl")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/textVert.glsl")

## Framework/Assets
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Assets/AssetManager.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Assets/AssetManager.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Assets/ShaderProgram.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Assets/ShaderProgram.cpp")
# add that directory to include list:
list(APPEND INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Assets")

## Framework/Rendering
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Rendering/Primitives.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Rendering/Primitives.cpp")
# add that directory to include list:
list(APPEND INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/Rendering")
## Framework/SceneElements
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/SceneElements/Transform.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/SceneElements/Transform.cpp")
# add that directory to include list:
list(APPEND INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/Framework/SceneElements")
## Game
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Game.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Game.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scene.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Scene.cpp")
# add that directory to include list:
list(APPEND INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/Game")

##---------------------------------framework files----------------------------------------------------------------------
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/CommonTypes.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/fw_config.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/GameWindow.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/GameWindow.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/glerror.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/glerror.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/Input.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/Input.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/libheaders.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/OBJLoader.h")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/framework/OBJLoader.cpp")
# add that directory to include list:
list(APPEND INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/framework")

##--------------------------------external dependencies-----------------------------------------------------------------
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs")

##--------------------------------executable target---------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)

add_executable(Shaderprogramming ${SOURCES})
target_include_directories(
        Shaderprogramming
        PRIVATE ${INCLUDES}
)

target_link_libraries(Shaderprogramming PUBLIC cga2fw_external_dependencies)

##-------------------------------copy assets to output------------------------------------------------------------------

file(COPY "assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")