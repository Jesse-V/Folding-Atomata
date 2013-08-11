
#ifndef NORMAL_BUFFER
#define NORMAL_BUFFER

#include "OptionalDataBuffer.hpp"
#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/Mesh/Triangle.struct"
#include <vector>
#include <string>

/**
    A NormalBuffer is an OptionalDataBuffer that can be added to a Model to
    give each vertex a normal vector. Ideally, the normal should point outward
    from the bounds of the polygon defined by that point, but this is often
    difficult to calculate, especially for very complex models where a vertex
    is shared between multiple triangles or quads. Therefore, three different
    normal calculation algorithms are included. The NormalBuffer is constructed
    with the results of one of these algorithms, and a per-vertex variable
    is defined in GLSL GPU-side.
**/
class NormalBuffer : public OptionalDataBuffer
{
    public:
        NormalBuffer(const std::vector<glm::vec3>& normals);
        virtual void initialize(GLuint programHandle);
        virtual void store();
        virtual void enable();
        virtual void disable();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        static std::vector<glm::vec3> calcNormalsMWA(
            const std::shared_ptr<Mesh>& mesh
        );
        static std::vector<glm::vec3> calcNormalsMWASER(
            const std::shared_ptr<Mesh>& mesh
        );
        static std::vector<glm::vec3> calcNormalsMWE(
            const std::shared_ptr<Mesh>& mesh
        );

    private:
        static std::vector<glm::vec3> calculateTriangleNormals(
            const std::shared_ptr<Mesh>& mesh,
            std::vector<std::vector<size_t>>& vtmap
        );

    private:
        std::vector<glm::vec3> normals_;

        GLuint normalBuffer_;
        GLint normalAttrib_;
};

#endif
