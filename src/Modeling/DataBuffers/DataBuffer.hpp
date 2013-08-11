
#ifndef DATA_BUFFER
#define DATA_BUFFER

#include "Modeling/Shading/ShaderUtilizer.hpp"
#include "Modeling/Shading/ShaderSnippet.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

/**
    A DataBuffer is a collection of OpenGL data that is attached to a Program.
    DataBuffers often have variables or arrays of information,
    which must be initialized/registered on the GPU and then stored in VRAM.
    Before rendering, the DataBuffer should be enabled, and then disabled after
    it is used.
**/
class DataBuffer : public ShaderUtilizer
{
    public:
        virtual void initialize(GLuint programHandle) = 0;
        virtual void store() = 0;
        virtual void enable() = 0;
        virtual void disable() = 0;
};

#endif
