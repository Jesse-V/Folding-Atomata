
#ifndef MESH
#define MESH

#include "Modeling/DataBuffers/IndexBuffer.hpp"
#include "Modeling/DataBuffers/VertexBuffer.hpp"
#include "Triangle.struct"

/**
    A Mesh contains the set of simple polygons that comprise a representation
    of a real-world object. A Mesh must contain a VertexBuffer, which defines
    the set of points in space the make up the Mesh. A Mesh can optionally have
    an IndexBuffer, which construct a set of simple polygons from these points.
    For example, if GL_TRIANGLES is the rendering mode passed to Mesh::draw,
    a Mesh could define a square by just a VertexBuffer of 
    {(0, 0), (1, 1), (0, 1), (0, 0), (1, 0), (1, 1)} which would tell OpenGL
    to render the two triangles defined by the three sets of points.
    Alternatively, a Mesh initialized with both a VertexBuffer and an IndexBuffer
    would be {(0, 0), (1, 1), (0, 1), (1, 0)} and {(0, 1, 2), (0, 3, 1)}.
    The memory benefits get more significant with more complex models where
    a vertex is used to help define more than one triangle. A Mesh can be
    rendered as a cloud of points or a wireframe (depending on the mode passed
    to Mesh::draw) but is most often drawn with GL_TRIANGLES.
    Note that "glEnable(GL_CULL_FACE); glCullFace(GL_BACK);" prevents OpenGL
    from drawing triangles that are on the backside of objects, (not facing
    the viewer) a significant rendering optimization.
**/
class Mesh : public DataBuffer
{
    public:
        Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer,
             GLenum mode = GL_TRIANGLES
        );
        Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer,
             const std::shared_ptr<IndexBuffer>& indexBuffer,
             GLenum mode = GL_TRIANGLES
        );

        virtual void initialize(GLuint programHandle);
        virtual void store();
        virtual void enable();
        virtual void disable();
        virtual void draw();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        std::vector<glm::vec3> getVertices();
        std::vector<Triangle> getTriangles();

        std::shared_ptr<VertexBuffer> getVertexBuffer();
        std::shared_ptr<IndexBuffer> getIndexBuffer();

    private:
        std::shared_ptr<VertexBuffer> vertexBuffer_;
        std::shared_ptr<IndexBuffer> indexBuffer_;
        GLenum renderingMode_;
};

#endif
