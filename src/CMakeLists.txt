cmake_minimum_required(VERSION 2.6)

project(FoldingAtomata)

#find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OpenGL not found!")
endif()

#find Glut
#http://stackoverflow.com/questions/9460242/how-to-compile-glut-opengl-project-with-cmake-and-kdevelop-in-linux
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " Glut not found!")
endif()

#find Glew
#https://github.com/progschj/OpenGL-Examples/blob/master/cmake_modules/FindGLEW.cmake

find_path(GLEW_INCLUDE_PATH GL/glew.h
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    ${GLEW_ROOT_DIR}/include
    DOC "The directory where GL/glew.h resides")

# Prefer the static library.
find_library(GLEW_LIBRARY
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

if(GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
    SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
else()
    message(ERROR " Glew not found!")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -Wno-c++98-compat-pedantic -pedantic -Weverything -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-non-virtual-dtor -Wno-weak-vtables -Wno-padded -Wno-cast-align")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -pedantic -Wall -Wextra -Wdouble-promotion -Wfloat-equal -Wunsafe-loop-optimizations -Wno-unused-parameter")
endif()

include_directories(.)

#organized by importance
add_executable(FoldingAtomata
    main.cpp

    Viewer/Viewer.cpp
    Viewer/SlotViewer.cpp
    Viewer/Player.cpp

    World/Scene.cpp
    World/Camera.cpp
    World/Light.cpp

    PyON/TrajectoryParser.cpp

    Modeling/Model.cpp
    Modeling/Mesh/Mesh.cpp
    Modeling/Mesh/PlyParser.cpp

    Modeling/DataBuffers/VertexBuffer.cpp
    Modeling/DataBuffers/IndexBuffer.cpp
    Modeling/DataBuffers/NormalBuffer.cpp
    Modeling/DataBuffers/SampledBuffers/SampledBuffer.cpp
    Modeling/DataBuffers/SampledBuffers/TextureBuffer.cpp
    Modeling/DataBuffers/SampledBuffers/CoordinateMapReader.cpp

    Modeling/Shading/ShaderManager.cpp
    Modeling/Shading/ShaderSnippet.cpp
    Modeling/Shading/Program.cpp
    Modeling/Shading/Shader.cpp

    Trajectory/Trajectory.cpp
    Trajectory/Topology.cpp
    Trajectory/Snapshot.cpp
    Trajectory/Bond.cpp
    Trajectory/Atom.cpp

    Sockets/Connection.cpp
    Sockets/ClientSocket.cpp
    Sockets/Socket.cpp
)

#set(CMAKE_INSTALL_PREFIX /usr/bin)

#for a "make install" installation
set(DEB_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../deb)
install(TARGETS FoldingAtomata DESTINATION /usr/bin)
install(FILES ${DEB_FOLDER}/usr/share/applications/FoldingAtomata.desktop DESTINATION /usr/share/applications)

target_link_libraries(FoldingAtomata glut GLEW GL ${GLEW_LIBRARIES})
