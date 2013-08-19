
#ifndef COLOR_BUFFER
#define COLOR_BUFFER

#include "OptionalDataBuffer.hpp"
#include <glm/glm.hpp>
#include <memory>

class ColorBuffer : public OptionalDataBuffer
{
    public:
        ColorBuffer(const glm::vec3& color, std::size_t count);
        ColorBuffer(const std::vector<glm::vec3>& colors);
        std::vector<glm::vec3> getColors();

        virtual void initialize(GLuint programHandle);
        virtual void enable();
        virtual void disable();
        virtual void store();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        std::vector<glm::vec3> colors_;
        GLuint colorBuffer_;
        GLint colorAttrib_;
};

typedef std::shared_ptr<ColorBuffer> ColorPtr;

#endif
