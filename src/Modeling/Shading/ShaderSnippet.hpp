#ifndef SHADER_SNIPPET
#define SHADER_SNIPPET

#include <string>

/**
    GLSL is a C-based language, and follows a very similar format. Fields follow
    preprocessor directives, methods follow fields, and main() follows fields.
    The ShaderSnippet class is a container for these three sections of code.
    The ShaderManager then assembles then in the correct order.
**/
class ShaderSnippet
{
    public:
        ShaderSnippet();
        ShaderSnippet(const std::string& fields,
            const std::string& methods, const std::string& mainBodyCode
        );

        std::string getFields();
        std::string getMethods();
        std::string getMainBodyCode();

    private:
        std::string fields_, methods_, mainBodyCode_;
};

#endif
