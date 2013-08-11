
#ifndef LIGHT
#define LIGHT

#include "Modeling/Shading/ShaderUtilizer.hpp"
#include "glm/glm.hpp"
#include <GL/glew.h>

/**
    Defines a point-light, that is, a light defined from a point in space.
    A point-light can be simply defined as it is here: a position, color,
    and power. The power variable defines its intensity and how far it appears
    to spread out before it dissipates into negletable levels.
**/
class Light : public ShaderUtilizer
{
    public:
        Light(
            const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0),
            const glm::vec3& color = glm::vec3(1, 1, 1),
            float power = 0.5f
        );

        void setPosition(const glm::vec3& newPos);
        void setColor(const glm::vec3& newColor);
        void setPower(float power);
        void setEmitting(bool emitting);

        glm::vec3 getPosition() const;
        glm::vec3 getColor() const;
        float getPower() const;
        bool isEmitting() const;

        void sync(GLuint handle, std::size_t lightID);

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        glm::vec3 position_, color_;
        float power_;
        bool emitting_;

        static std::size_t nLights_;
};

typedef std::vector<std::shared_ptr<Light>> LightList;

#endif
