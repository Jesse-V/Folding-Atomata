
#ifndef MODEL
#define MODEL

#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/DataBuffers/OptionalDataBuffer.hpp"
#include "Modeling/Shading/Program.hpp"
#include "Renderable.hpp"
#include <vector>
#include <memory>

typedef std::vector<std::shared_ptr<OptionalDataBuffer>> BufferList;

/**
    A Model is a polygon-based representation of a real-world object.
    Models are essentially defined by a Mesh, which specifies their shape
    as a set of small simply polygons (often triangles, sometimes quads).
    The Mesh is positioned and contoured in world space via a model matrix.
    Alone, this Mesh is completely colorless and is only visible by the Scene's
    ambient light. To include other properties, OptionalDataBuffers can be added
    to the Model. These can define per-vertex properties such as normals or colors,
    or even per-fragment properties such as textures or bump maps.
    All of these are saved under a cs5400::Program, a wrapper for the OpenGL
    program on the GPU. Whether or not a Model (and all its corresponding data)
    is rendered can be toggled with the setVisible method.
**/
class Model : Renderable
{
    public:
        Model(const std::shared_ptr<Mesh>& mesh);
        Model(const std::shared_ptr<Mesh>& mesh, const BufferList& optionalDBs);

        virtual void saveAs(const ProgramPtr& program);
        void setVisible(bool visible);
        void setModelMatrix(const glm::mat4& matrix); // model -> world coords
        void setRenderMode(GLenum newMode);
        virtual void render(GLint modelMatrixID);

        BufferList getOptionalDataBuffers();

    private:
        void enableDataBuffers();
        void disableDataBuffers();

    protected:
        std::shared_ptr<Mesh> mesh_;
        BufferList optionalDBs_;
        glm::mat4 modelMatrix_;
        bool isVisible_;
};

#endif
