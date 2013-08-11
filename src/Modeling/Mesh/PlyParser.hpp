
#ifndef PLY_PARSER
#define PLY_PARSER

#include "Mesh.hpp"
#include "Triangle.struct"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glut.h>
#include <vector>
#include <memory>
#include <string>

typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::pair<IndexBufferPtr, GLenum> IndexCollection;

/**
    A PlyParser implements a reader for .ply files. PLY files are essentially
    defined as a list of points in space, followed by a list of sets of indices
    corresponding to the bounds of the plane defined by the referenced points.
    Some PLY files include additional per-vertex data, such as colors or normals.
    Here though, this PlyParser only has the ability to read a simple mesh of
    triangles from ASCII .ply files. Other data can be included in the file,
    though X,Y,Z data must come first in the "element" section, and triangle
    indices must be first in the "face" section. Any other data will be ignored.
    If a .ply mesh is defined by GL_QUADS instead of GL_TRIANGLES, an exception
    will be thrown during parsing. 
**/
class PlyParser
{
    public:
        static MeshPtr getMesh(const std::string& fileName);
        static std::string readFile(const std::string& fileContents);

    private:
        static std::vector<std::string> seperatePly(const std::string& fileContents);
        static VertexBufferPtr parseVertices(const std::string& verticesStr);
        static IndexCollection parseIndices(const std::string& facesStr);
        static std::pair<int, int> getSizes(const std::string& header);
        static int getLastWordAsInt(const std::string& str);
};

#endif
