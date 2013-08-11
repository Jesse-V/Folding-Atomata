
#ifndef CS5400_SHADER
#define CS5400_SHADER

#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

namespace cs5400
{
    namespace detail
    {
        struct VertexShaderTag{};
        struct FragmentShaderTag{};

        template<typename T> struct ShaderType;

        template<>
        struct ShaderType<VertexShaderTag>
        {
            static const GLenum value = GL_VERTEX_SHADER;
        };

        template<>
        struct ShaderType<FragmentShaderTag>
        {
            static const GLenum value = GL_FRAGMENT_SHADER;
        };
    }


    template<typename T>
    class Shader
    {
        public:
            Shader():
                handle(glCreateShader(detail::ShaderType<T>::value))
            {}

            ~Shader()
            {
                glDeleteShader(handle);
            }

            GLuint getHandle()
            {
                return handle;
            }

        private:
            GLuint handle;
    };

    typedef Shader<detail::VertexShaderTag> VertexShader;
    typedef Shader<detail::FragmentShaderTag> FragmentShader;

    std::shared_ptr<VertexShader> makeVertexShader(const std::string& path);
    std::shared_ptr<VertexShader> makeVertexShaderStr(const std::string& shaderCode);

    std::shared_ptr<FragmentShader> makeFragmentShader(const std::string& path);
    std::shared_ptr<FragmentShader> makeFragmentShaderStr(const std::string& shaderCode);
}

#endif
