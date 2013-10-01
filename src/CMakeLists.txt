cmake_minimum_required(VERSION 2.6)

project(folding-atomata)

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

#find png
#http://fedetft.wordpress.com/2010/03/07/cmake-part-3-finding-libraries/
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
if(NOT PNG_FOUND)
    message(ERROR " png++ not found!")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -Wno-c++98-compat-pedantic -pedantic -Weverything -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-non-virtual-dtor -Wno-weak-vtables -Wno-padded -Wno-cast-align -Wno-gnu -Wno-nested-anon-types -Wno-documentation-unknown-command -Wno-unknown-pragmas")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-g -O3 --std=c++11 -Wall -Wextra -Wdouble-promotion -Wfloat-equal -Wunsafe-loop-optimizations -Wno-unused-parameter")
endif()

include_directories(. libs)

#organized by importance
add_executable(FoldingAtomata
    main.cpp
    Options.cpp

    Viewer/Viewer.cpp
    Viewer/SlotViewer.cpp
    Viewer/User.cpp
    Viewer/FAHClientIO.cpp

    World/Scene.cpp
    World/Camera.cpp
    World/Light.cpp

    PyON/TrajectoryParser.cpp
    PyON/StringManip.cpp

    Modeling/BaseModel.cpp
    Modeling/Mesh/Mesh.cpp

    Modeling/DataBuffers/VertexBuffer.cpp
    Modeling/DataBuffers/IndexBuffer.cpp

    Modeling/Shading/ShaderManager.cpp
    Modeling/Shading/ShaderSnippet.cpp
    Modeling/Shading/Program.cpp
    Modeling/Shading/Shader.cpp

    Trajectory/ProteinAnalysis.cpp
    Trajectory/Trajectory.cpp
    Trajectory/Topology.cpp
    Trajectory/Atom.cpp

    Sockets/ClientSocket.cpp
    Sockets/Socket.cpp
)

target_link_libraries(FoldingAtomata glut GLEW GL ${GLEW_LIBRARIES} png)

#for a "make install" installation
set(DEB_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/debian/extra_includes")
set(SKYBOX_FILES "${DEB_FOLDER}/skybox")
set(SKYBOX_DESTINATION "/usr/share/FoldingAtomata/images/")
set(MENU_DESKTOP_FILE "${DEB_FOLDER}/Folding Atomata.desktop")

install(TARGETS FoldingAtomata     DESTINATION /usr/bin)
install(FILES ${MENU_DESKTOP_FILE} DESTINATION /usr/share/applications)
install(FILES ${DEB_FOLDER}/FoldingAtomata-logo.png DESTINATION /usr/share/pixmaps)
install(FILES ${DEB_FOLDER}/FoldingAtomata.1.gz     DESTINATION /usr/share/man/man1)
install(FILES ${DEB_FOLDER}/demoProtein       DESTINATION /usr/share/FoldingAtomata)
install(FILES ${SKYBOX_FILES}/MSM.png         DESTINATION ${SKYBOX_DESTINATION})
install(FILES ${SKYBOX_FILES}/Primase.png     DESTINATION ${SKYBOX_DESTINATION})
install(FILES ${SKYBOX_FILES}/Ribosome.png    DESTINATION ${SKYBOX_DESTINATION})
