cmake_minimum_required(VERSION 2.6)

project(atomata)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS  "-g -O3 --std=c++11 -Wno-c++98-compat-pedantic -pedantic -Weverything -Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-non-virtual-dtor -Wno-weak-vtables -Wno-padded -Wno-cast-align")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS  "-g -O3 --std=c++11 -pedantic -Wall -Wextra -Wdouble-promotion -Wfloat-equal -Wunsafe-loop-optimizations -Wno-unused-parameter")
endif()

include_directories(.)
#link_directories()

add_executable(atomata
    main.cpp
    Trajectory/Trajectory.cpp
    Trajectory/Topology.cpp
    Trajectory/Snapshot.cpp
    Trajectory/Bond.cpp
    Trajectory/Atom.cpp
    PyON/TrajectoryParser.cpp
    Sockets/ClientSocket.cpp
    Sockets/Socket.cpp
)

IF(WIN32)
    target_link_libraries(atomata glut glew32 GL)
ELSE()
    target_link_libraries(atomata glut GLEW GL)
ENDIF()
