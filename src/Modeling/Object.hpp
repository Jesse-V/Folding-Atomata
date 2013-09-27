#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/DataBuffers/OptionalDataBuffer.hpp"
#include <vector>
#include <memory>

//https://github.com/progschj/OpenGL-Examples/blob/master/06instancing1.cpp

class Object
{
    public:
        Object(const std::shared_ptr<Mesh>& mesh);
        Object(const std::shared_ptr<Mesh>& mesh, const BufferList& optionalDBs);

        void setRenderMode(GLenum newMode);
        void render(GLuint programHandle);

        BufferList getOptionalDataBuffers();

    private:
        void enableDataBuffers();
        void disableDataBuffers();

    protected:

};
