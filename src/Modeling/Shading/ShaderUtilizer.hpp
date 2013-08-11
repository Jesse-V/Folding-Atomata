#ifndef SHADER_UTILIZER
#define SHADER_UTILIZER

#include "ShaderSnippet.hpp"
#include <memory>
#include <vector>

typedef std::shared_ptr<ShaderSnippet> SnippetPtr;
typedef std::vector<SnippetPtr> SnippetList;

/**
    A ShaderUntilizer is a purely abstract class that declares two methods
    for fetching GLSL code. Many classes in /Modeling as well as in /World
    use OpenGL calls to hook into GLSL variables, so they require corresponding
    GLSL code to declare and handle those variables accordingly GPU-side.
    Although more modern versions of OpenGL/GLSL use geometry shaders,
    the ShaderUtilizer declares methods for returning only GLSL for 
    vertex and fragment shaders. Subclasses define the implementation of what
    code is returned.
**/
class ShaderUtilizer
{
    public:
        virtual SnippetPtr getVertexShaderGLSL() = 0;
        virtual SnippetPtr getFragmentShaderGLSL() = 0;
};

#endif
