cmake_minimum_required(VERSION 3.20)

project(E_COLI
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================
# GLFW
# ============================================================

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(
    externals/glfw-3.4
)


# ============================================================
# GLAD
# ============================================================

add_library(glad STATIC

    externals/glad/src/glad.c
)

target_include_directories(glad PUBLIC

    externals/glad/include
)


# ============================================================
# GLM
# ============================================================

add_library(glm INTERFACE)

target_include_directories(glm INTERFACE

    externals/glm
)


# ============================================================
# FREETYPE
# ============================================================

set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

add_subdirectory(

    externals/ft2143/freetype-2.14.3

    EXCLUDE_FROM_ALL
)


# ============================================================
# SOURCE FILES
# ============================================================

file(GLOB_RECURSE PROJECT_SOURCES

    CONFIGURE_DEPENDS

    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)


# ============================================================
# E_COLI
# ============================================================

add_executable(E_COLI

    ${PROJECT_SOURCES}
)


# ============================================================
# INCLUDE DIRECTORIES
# ============================================================

file(GLOB_RECURSE PROJECT_INCLUDE_DIRS

    LIST_DIRECTORIES true

    "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
)


target_include_directories(E_COLI PRIVATE

    src

    ${PROJECT_INCLUDE_DIRS}

    externals/glad/include

    externals/glm

    externals/ft2143/freetype-2.14.3/include
)


# ============================================================
# LIBRARIES
# ============================================================

target_link_libraries(E_COLI PRIVATE

    glad

    glfw

    glm

    freetype
)


# ============================================================
# SHADERS
# ============================================================

add_custom_command(

    TARGET E_COLI

    POST_BUILD

    COMMAND
        ${CMAKE_COMMAND}
        -E copy_directory

        "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"

        "${CMAKE_BINARY_DIR}/shaders"

    COMMENT
        "Copiando shaders..."
)