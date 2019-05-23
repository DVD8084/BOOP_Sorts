cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME "Sorts")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

set(JSON_BuildTests OFF CACHE INTERNAL "")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(SFML_DIR "../lib/SFML-2.5.1-release")
endif()

if(NOT SFML_DIR)
    set(SFML_DIR "../lib/SFML-2.5.1-debug")
endif()

set(IMGUI_DIR "../lib/imgui")

set(JSON_DIR "../lib/nlohmann_json")

find_package(SFML REQUIRED audio graphics network window system)

include_directories(${IMGUI_DIR})
include_directories(${JSON_DIR})
include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)

set(IMGUI_SOURCE "${IMGUI_DIR}/imgui.cpp" "${IMGUI_DIR}/imgui_draw.cpp" "${IMGUI_DIR}/imgui_demo.cpp" "${IMGUI_DIR}/imgui_widgets.cpp" "${IMGUI_DIR}/imgui_stdlib.cpp" "${IMGUI_DIR}/imgui-SFML.cpp")
set(SORTS_SOURCE "Vector.cpp" "Vector.h" "SortVector.cpp" "SortVector.h")

add_executable(${PROJECT_NAME} main.cpp ${IMGUI_SOURCE} ${SORTS_SOURCE})
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-network sfml-window sfml-system opengl32)