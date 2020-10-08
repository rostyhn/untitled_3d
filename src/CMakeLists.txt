set(PROJECT_NAME OpenGL3DGameTutorial-ThinMatrix)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "freetype.dll"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "Engine/GameManager.h"
    "Entities/Camera.h"
    "Entities/Entity.h"
    "Entities/FPSCamera.h"
    "Entities/Light.h"
    "GameState.h"
    "MenuItem.h"
    "MenuState.h"
    "Models/RawModel.h"
    "Models/TexturedModel.h"
    "RenderEngine/DisplayManager.h"
    "RenderEngine/EntityRenderer.h"
    "RenderEngine/Loader.h"
    "RenderEngine/MasterRenderer.h"
    "RenderEngine/OBJLoader.h"
    "RenderEngine/TerrainRenderer.h"
    "RenderEngine/TextRenderer.h"
    "RunState.h"
    "Shaders/BasicShader.h"
    "Shaders/ShaderProgram.h"
    "Shaders/TerrainShader.h"
    "Shaders/TextShader.h"
    "Terrain/Terrain.h"
    "Textures/ModelTexture.h"
    "Toolbox/Maths.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Engine/GameManager.cpp"
    "Entities/Camera.cpp"
    "Entities/FPSCamera.cpp"
    "main.cpp"
    "MenuItem.cpp"
    "MenuState.cpp"
    "RenderEngine/DisplayManager.cpp"
    "RenderEngine/EntityRenderer.cpp"
    "RenderEngine/Loader.cpp"
    "RenderEngine/MasterRenderer.cpp"
    "RenderEngine/OBJLoader.cpp"
    "RenderEngine/TerrainRenderer.cpp"
    "RenderEngine/TextRenderer.cpp"
    "RunState.cpp"
    "Shaders/BasicShader.cpp"
    "Shaders/ShaderProgram.cpp"
    "Shaders/TerrainShader.cpp"
    "Shaders/TextShader.cpp"
    "Terrain/Terrain.cpp"
    "Toolbox/Maths.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE OpenGL3DGameTutorialThinMatrix)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "glew32;"
    "glfw3;"
    "glfw3dll;"
    "OpenGL32;"
    "freetype;"
    "legacy_stdio_definitions"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}//lib"
)

