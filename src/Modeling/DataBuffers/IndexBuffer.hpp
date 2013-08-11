
#ifndef INDEX_BUFFER
#define INDEX_BUFFER

#include "DataBuffer.hpp"
#include "Modeling/Mesh/Triangle.struct"
#include "Modeling/Mesh/Quad.struct"
#include <vector>

/**
    A IndexBuffer defines a set of Triangles by referencing triplets of points
    in space. This helps to efficiently define a Mesh when vertexes are being
    shared by multiple triangles or quads, so it is recommended to use an
    IndexBuffer in conjunction with a VertexBuffer whenever possible.
    IndexBuffer has no GLSL data GPU-side; it is just used by OpenGL.
**/
class IndexBuffer : public DataBuffer
{
    public:
        IndexBuffer(std::size_t length, GLenum type = GL_TRIANGLES);
        IndexBuffer(const std::vector<GLuint>& indices, GLenum type = GL_TRIANGLES);

        void draw(GLenum mode);
        bool canInterpretAs(GLenum type);
        std::vector<Triangle> reinterpretAsTriangles();
        std::vector<Quad> reinterpretAsQuads();
        std::vector<Triangle> castToTriangles();

        virtual void initialize(GLuint programHandle);
        virtual void store();
        virtual void enable();
        virtual void disable();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        std::vector<GLuint> indices_;
        GLuint indexBuffer_;
        GLenum acceptedAsType_;
};

#endif
