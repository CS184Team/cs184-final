cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${CGL_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
)

link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
        ${CGL_LIBRARY_DIRS}
        ${FREETYPE_LIBRARY_DIRS}
)

file(GLOB LIB_SOURCES *.cpp *.c)
file(GLOB LIB_HEADERS *.h)
add_library(MyLib ${LIB_SOURCES} ${LIB_HEADERS})

file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/targets targets/*.c targets/*.cpp)
foreach(target_file ${APP_SOURCES})
    # I used a simple string replace, to cut off .cpp.
    string(REPLACE ".cpp" "" target_name ${target_file})
    string(REPLACE ".c" "" target_name ${target_name})
    add_executable(${target_name} targets/${target_file})
    target_link_libraries(${target_name}
            MyLib
            CGL ${CGL_LIBRARIES}
            glew ${GLEW_LIBRARIES}
            glfw ${GLFW_LIBRARIES}
            ${OPENGL_LIBRARIES}
            ${FREETYPE_LIBRARIES}
            )
    install(TARGETS ${target_name} DESTINATION ${ColladaViewer_SOURCE_DIR})
    if (APPLE)
        set_property(TARGET ${target_name} APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
    endif (APPLE)
endforeach(target_file ${APP_SOURCES})

set(EXECUTABLE_OUTPUT_PATH ..)