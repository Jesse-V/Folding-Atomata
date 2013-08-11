
#ifndef COORDINATE_MAP_READER
#define COORDINATE_MAP_READER

#include <vector>
#include <string>
#include <GL/glew.h>

/**
    A CoordinateMapReader reads a set of UV coordinates for use by a
    SampledBuffer. This helps OpenGL figure out how the image should be
    conformed and laid down on the Mesh. These coordinates can specify a
    very complex mapping if it's not trivial to map the texture onto
    the underlying Mesh.
**/
class CoordinateMapReader
{
    public:
       static std::vector<GLfloat> getMap(const std::string& fileName);
};

#endif
