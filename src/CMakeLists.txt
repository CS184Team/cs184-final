cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DUSE_CLANG)
set(PC2M_PATH ../../pc2m2)
set(BP_PATH ../../BallPivoting)

#file(GLOB PC2M_SOURCES ${PC2M_PATH}/src/*.cpp ${BP_PATH}/src/*.cpp)
#file(GLOB PC2M_HEADERS ${PC2M_PATH}/src/*.h ${BP_PATH}/src/*.h)

file(GLOB PC2M_SOURCES ${BP_PATH}/src/*.cpp)
file(GLOB PC2M_HEADERS ${BP_PATH}/src/*.h)

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
add_library(PC2M ${PC2M_SOURCES} ${PC2M_HEADERS})

file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/targets targets/*.c targets/*.cpp)
foreach(target_file ${APP_SOURCES})
    # I used a simple string replace, to cut off .cpp.
    string(REPLACE ".cpp" "" target_name ${target_file})
    string(REPLACE ".c" "" target_name ${target_name})
    add_executable(${target_name} targets/${target_file})
    if (APPLE)
        set_property(TARGET ${target_name} APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
    endif (APPLE)
    target_link_libraries(${target_name}
            MyLib
            PC2M
            CGL ${CGL_LIBRARIES}
            glew ${GLEW_LIBRARIES}
            glfw ${GLFW_LIBRARIES}
            ${OPENGL_LIBRARIES}
            ${FREETYPE_LIBRARIES}
            )
    install(TARGETS ${target_name} DESTINATION ${ColladaViewer_SOURCE_DIR})
endforeach(target_file ${APP_SOURCES})

set(EXECUTABLE_OUTPUT_PATH ..)