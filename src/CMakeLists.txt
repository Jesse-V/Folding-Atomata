cmake_minimum_required(VERSION 2.6)

project(FoldingAtomata)

#find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR " OpenGL not found!")
endif (NOT OPENGL_FOUND)

#find Glut
#http://stackoverflow.com/questions/9460242/how-to-compile-glut-opengl-project-with-cmake-and-kdevelop-in-linux
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if (NOT GLUT_FOUND)
    message(ERROR " Glut not found!")
endif (NOT GLUT_FOUND)

#find Glew
#https://github.com/progschj/OpenGL-Examples/blob/master/cmake_modules/FindGLEW.cmake
if (WIN32)
    find_path(GLEW_INCLUDE_PATH GL/glew.h
        $ENV{PROGRAMFILES}/GLEW/include
        ${GLEW_ROOT_DIR}/include
        DOC "The directory where GL/glew.h resides")

    find_library( GLEW_LIBRARY
        NAMES glew GLEW glew32 glew32s
        PATHS
        $ENV{PROGRAMFILES}/GLEW/lib
        ${GLEW_ROOT_DIR}/lib
        ${PROJECT_SOURCE_DIR}/src/nvgl/glew/bin
        ${PROJECT_SOURCE_DIR}/src/nvgl/glew/lib
        DOC "The GLEW library")
else (WIN32)
    find_path( GLEW_INCLUDE_PATH GL/glew.h
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        ${GLEW_ROOT_DIR}/include
        DOC "The directory where GL/glew.h resides")

    # Prefer the static library.
    find_library( GLEW_LIBRARY
        NAMES libGLEW.a GLEW
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        ${GLEW_ROOT_DIR}/lib
        DOC "The GLEW library")
endif (WIN32)

if (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
    SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
else ()
    message(ERROR " Glew not found!")
endif ()

if (NOT WIN32)
    set(CMAKE_INSTALL_PREFIX /usr/bin)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -Wno-c++98-compat-pedantic -pedantic -Weverything -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-non-virtual-dtor -Wno-weak-vtables -Wno-padded -Wno-cast-align")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -pedantic -Wall -Wextra -Wdouble-promotion -Wfloat-equal -Wunsafe-loop-optimizations -Wno-unused-parameter")
endif ()

include_directories(.)

add_executable(FoldingAtomata
    main.cpp
    Viewer/SlotViewer.cpp
    PyON/TrajectoryParser.cpp
    Trajectory/Trajectory.cpp
    Trajectory/Topology.cpp
    Trajectory/Snapshot.cpp
    Trajectory/Bond.cpp
    Trajectory/Atom.cpp
    Sockets/Connection.cpp
    Sockets/ClientSocket.cpp
    Sockets/Socket.cpp
)

#for a "make install" installation
install(TARGETS FoldingAtomata DESTINATION bin)

if (WIN32)
    target_link_libraries(FoldingAtomata glut glew32 GL)
else ()
    target_link_libraries(FoldingAtomata glut GLEW GL ${GLEW_LIBRARIES}) #${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
endif ()
