cmake_minimum_required(VERSION 3.10)
project(prism)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/spdlog)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    endif()
    add_compile_definitions(PRISM_DEBUG)
else()
    # temporary, to test
    add_compile_definitions(PRISM_DEBUG)
    #add_compile_definitions(PRISM_NODEBUG)
endif()

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

include_directories(
                    vendor/glad/include/
                    vendor/GLFW/include/
                    vendor/glm/glm/
                    vendor/imgui/
                    vendor/spdlog/include/
                    vendor/stb/
                    vendor/json/include/
                    vendor/pmap/parallel_hashmap/
                    src/)

file(GLOB STB_SRC vendor/stb/*.c)
file(GLOB GLAD_SOURCE vendor/glad/src/glad.c)
file(GLOB IMGUI_SOURCES vendor/imgui/*.cpp)
file(GLOB IMGUI_HEADERS vendor/imgui/*.h)
file(GLOB IMGUI_GLFW_HEADERS vendor/imgui/backends/imgui_impl_glfw.h)
file(GLOB IMGUI_GLFW_SOURCES
                    vendor/imgui/backends/imgui_impl_opengl3.cpp
                    vendor/imgui/backends/imgui_impl_glfw.cpp)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

file(GLOB CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME}
                    ${STB_SRC}
                    ${IMGUI_SOURCES}
                    ${IMGUI_GLFW_SOURCES}
                    ${GLAD_SOURCE} 
                    ${SOURCES} 
                    ${IMGUI_HEADERS}
                    ${HEADERS} 
                    ${IMGUI_GLFW_HEADERS}
                    ${PROJECT_CONFIGS} )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        _USE_MATH_DEFINES
)

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
              
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
    DEPENDS ${RESOURCES})