
#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include "DataBuffer.hpp"
#include "glm/glm.hpp"
#include <vector>

/**
    A VertexBuffer contains a set of points in 3D space. These help define
    the shape of a Mesh. A VertexBuffer can be drawn by itself when using an
    IndexBuffer is not appropriate, but it is often a better idea to use
    an IndexBuffer in conjunction with a VertexBuffer to define a Mesh.
    A VertexBuffer has GLSL data GPU-side: each vertex is given a 3D vector that
    defines its position in model-space.
**/
class VertexBuffer : public DataBuffer
{
    public:
        VertexBuffer(const std::vector<glm::vec3>& vertices);
        virtual void initialize(GLuint programHandle);
        virtual void store();
        virtual void enable();
        virtual void disable();
        void draw(GLenum mode);

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        std::vector<glm::vec3> getVertices();

    private:
        void storePoints();
        void enableVertices();

    private:
        std::vector<glm::vec3> vertices_;
        GLuint vertexBuffer_;
        GLint vertexAttrib_;
};

#endif
