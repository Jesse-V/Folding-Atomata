
#ifndef CAMERA
#define CAMERA

#define GLM_SWIZZLE //necessary for certain GLM methods like .xyz()

#include "glm/glm.hpp"
#include <GL/glew.h>
#include <memory>
#include <string>

/**
    Defines the functionality of a fisheye (perspective) camera.
    A Camera is defined by three basic vectors: its position, viewing direction,
    and what is meant by "up". Since this is a perspective camera that resembles
    natural viewing, the Camera is also defined by Field of View and Aspect Ratio
    variables. The frustrum is defined by this two variables, but is bounded
    depth-wise by the near and far field clips. Only fragments of polygons within
    these clip spaces will be rendered. In the mathematics of computer graphics,
    a camera is nothing more than another matrix that is used to project a
    polygon onto the screen. It defines the "camera space".
**/
class Camera
{
    public:
        Camera();
        void reset();
        void sync(GLuint programHandle) const;

        //position and orient the camera
        void setPosition(const glm::vec3& newPosition);
        void lookAt(const glm::vec3& point,
                    const glm::vec3& up = glm::vec3(0, 0, 1)
        );
        void orient(const glm::vec3& position, const glm::vec3& viewingVector,
                    float roll = 0
        );

        //translation irregardless of orientation
        void translateX(float magnitude);
        void translateY(float magnitude);
        void translateZ(float magnitude);
        void translate(const glm::vec3& delta);

        //translation relative to orientation
        void moveForward(float magnitude);
        void moveRight(float magnitude);
        void moveUp(float magnitude);

        //orientation controls
        void pitch(float theta);
        void yaw(float theta, bool aroundUpVector = true);
        void roll(float theta);
        bool constrainedPitch(float theta);
        bool constrainedRoll(float theta);

        //adjusts the the camera's frustrum (its fisheye perspective properties)
        void setFieldOfView(float degrees);
        void setAspectRatio(float ratio);
        void setNearFieldClipDistance(float distance);
        void setFarFieldClipDistance(float distance);
        void setPerspective(float fieldOfViewDegrees, float aspectRatio,
                            float nearClipDistance, float farClipDistance);

        //accessors for camera's position and orientation
        glm::vec3 getPosition() const;
        glm::vec3 getLookingAt() const;
        glm::vec3 calculateLookDirection() const;
        glm::vec3 getUpVector() const;
        glm::mat4 calculateViewMatrix() const;

        //accessors for camera frustrum (perspective matrix properties)
        float getFOV() const;
        float getAspectRatio() const;
        float getNearFieldClip() const;
        float getFarFieldClip() const;
        glm::mat4 getProjectionMatrix() const;
        
        std::string toString() const;

    private:
        void updateProjectionMatrix();

    private:
        glm::vec3 lookingAt_, position_, upVector_;
        float fieldOfView_, aspectRatio_, nearFieldClip_, farFieldClip_;
        glm::mat4 projection_;

    //it'd be cool to have a rotateAround function
};

#endif
