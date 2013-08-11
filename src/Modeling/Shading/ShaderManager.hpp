
#ifndef SHADER_MANAGER
#define SHADER_MANAGER

#include "Modeling/Model.hpp"
#include "Modeling/DataBuffers/DataBuffer.hpp"
#include "World/Light.hpp"
#include "Program.hpp"
#include <memory>
#include <vector>
#include <GL/glut.h>

/**
    A ShaderManager's primary job is to assemble GLSL code from the ShaderManager
    subclasses, and then create a cs5400:Program from that shader code.
    As GLSL is C-based, preprocessor directives go first, followed by fields,
    followed by methods, followed by code in main(). The ShaderManager
    assembles the GLSL code in this order, and then uses classes within the
    cs5400 namespace to register a program on the GPU from this code. The
    CPU-side code is then free to hook into GLSL variables under this Program.
    The ShaderManager classes makes heavy use of typedefs to reduce the length
    of templatized types. Refer to Program.hpp, ShaderUtilizer.hpp, and
    /World/Light.hpp for full typedef declarations if they are not obvious.
**/
class ShaderManager
{
    public:
        static ProgramPtr createProgram(const std::shared_ptr<Model>& obj,
            const SnippetPtr& sceneVertexShader,
            const SnippetPtr& sceneFragmentShader, const LightList& lights
        );

    private:
        static std::vector<SnippetPtr> assembleVertexSnippets(
            const SnippetPtr& sceneVertexShader,
            const BufferList& buffers, const LightList& lights
        );
        static std::string assembleVertexShaderStr(
            const BufferList& buffers, const SnippetPtr& sceneVertexShader,
            const LightList& lights
        );

        static std::vector<SnippetPtr> assembleFragmentSnippets(
            const SnippetPtr& sceneFragmentShader,
            const BufferList& buffers, const LightList& lights
        );
        static std::string assembleFragmentShaderStr(
            const BufferList& buffers, const SnippetPtr& sceneFragmentShader,
            const LightList& lights
        );

        static std::string assembleFields(const SnippetList& snippets);
        static std::string assembleMethods(const SnippetList& snippets);
        static std::string assembleMainBodyCode(const SnippetList& snippets);
        static std::string buildShader(const std::string& fields,
            const std::string& methods, const std::string& mainBodyCode
        );
};

#endif
