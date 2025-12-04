cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)

project(Atomix_cpp VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# === CHEMIN VERS LES LIBS LOCALES ===
set(CPP_LIBS_DIR "${CMAKE_SOURCE_DIR}/../cpp_libs")

# === MACRO POUR FETCH LOCAL OU DISTANT ===
macro(FetchContent_DeclareLocalOrRemote name local_path git_repo git_tag)
    if(EXISTS "${local_path}/CMakeLists.txt")
        message(STATUS "${name}: Utilisation de la version locale dans ${local_path}")
        FetchContent_Declare(${name}
            SOURCE_DIR ${local_path}
        )
    else()
        message(STATUS "${name}: Téléchargement depuis ${git_repo}")
        FetchContent_Declare(${name}
            GIT_REPOSITORY ${git_repo}
            GIT_TAG ${git_tag}
            SOURCE_DIR ${local_path}
        )
    endif()
endmacro()

# === RAYLIB ===
FetchContent_DeclareLocalOrRemote(raylib
    "${CPP_LIBS_DIR}/raylib"
    "https://github.com/raysan5/raylib.git"
    "5.5"
)

# === FLECS ===
FetchContent_DeclareLocalOrRemote(flecs
    "${CPP_LIBS_DIR}/flecs"
    "https://github.com/SanderMertens/flecs.git"
    "v4.1.2"
)

# === IMGUI (pas de CMakeLists.txt natif) ===
set(IMGUI_DIR "${CPP_LIBS_DIR}/imgui")
if(NOT EXISTS "${IMGUI_DIR}/imgui.cpp")
    message(STATUS "imgui: Téléchargement depuis GitHub")
    FetchContent_Declare(imgui_fetch
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.5
        SOURCE_DIR ${IMGUI_DIR}
    )
    FetchContent_MakeAvailable(imgui_fetch)
else()
    message(STATUS "imgui: Utilisation de la version locale dans ${IMGUI_DIR}")
endif()

# === RLIMGUI (pas de CMakeLists.txt natif) ===
set(RLIMGUI_DIR "${CPP_LIBS_DIR}/rlImGui")
if(NOT EXISTS "${RLIMGUI_DIR}/rlImGui.cpp")
    message(STATUS "rlImGui: Téléchargement depuis GitHub")
    FetchContent_Declare(rlimgui_fetch
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG main
        SOURCE_DIR ${RLIMGUI_DIR}
    )
    FetchContent_MakeAvailable(rlimgui_fetch)
else()
    message(STATUS "rlImGui: Utilisation de la version locale dans ${RLIMGUI_DIR}")
endif()

# === MAKE AVAILABLE RAYLIB & FLECS ===
FetchContent_MakeAvailable(raylib flecs)

# === IMGUI (bibliothèque statique) ===
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})

# === RLIMGUI (binding raylib + imgui) ===
add_library(rlimgui STATIC
    ${RLIMGUI_DIR}/rlImGui.cpp
)
target_include_directories(rlimgui PUBLIC ${RLIMGUI_DIR})
target_link_libraries(rlimgui PUBLIC imgui raylib)

# === EXECUTABLE ===
add_executable(${PROJECT_NAME} 
    src/atom.cpp
    src/partie.cpp 
    src/atomixRaylib.cpp
    src/atomixRaylib/partieObserver.cpp
    src/main.cpp
    )
target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib 
   # flecs::flecs_static 
   # imgui 
   # rlimgui
)

# === GESTION ASSETS ===
# 1. Copie assets/ vers build/assets/ (dev)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# 2. Macro ASSETS_PATH pour le code (absolue en dev, relative en release)
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    ASSETS_PATH="${CMAKE_BINARY_DIR}/assets/"  # Dev: copie dans build
    $<$<CONFIG:Release>:ASSETS_PATH="./assets/">  # Release: relatif
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copie des assets..."
)