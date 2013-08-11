
#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <iostream>


Camera::Camera()
{
    reset();
}



// Reset the camera back to its defaults
void Camera::reset()
{
    setPosition(glm::vec3(0.0, 0.0, 0.5));
    lookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));

    fieldOfView_   = 45.0f;         // frustrum viewing aperture
    aspectRatio_   = 4.0f / 3.0f;   // frustrum view angling
    nearFieldClip_ = 0.005f;        // clip anything closer than this
    farFieldClip_  = 200.0f;         // clip anything farther than this
    updateProjectionMatrix();
}



void Camera::sync(GLuint programHandle) const
{
    //assemble view matrix and sync with program handle
    GLint viewMatrixUniform = glGetUniformLocation(programHandle, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE,
                       glm::value_ptr(calculateViewMatrix()));

    //sync projection matrix with program handle
    GLint projMatrixUniform = glGetUniformLocation(programHandle, "projMatrix");
    glUniformMatrix4fv(projMatrixUniform, 1, GL_FALSE,
                       glm::value_ptr(getProjectionMatrix()));

    //assert that they went through to GLSL variables
    if (viewMatrixUniform < 0 || projMatrixUniform < 0)
        throw std::runtime_error("Unable to find Camera uniform variables!");
}



// Set the camera to an arbitrary location without changing orientation
void Camera::setPosition(const glm::vec3& newPosition)
{
    if (newPosition == lookingAt_)
        throw std::runtime_error("Cannot be where we're looking at!");

    position_ = newPosition;
}



// Set the orientation of the camera without changing its position_
void Camera::lookAt(const glm::vec3& gazePoint, const glm::vec3& upVector)
{
    if (gazePoint == position_)
        throw std::runtime_error("Cannot look at same point as position!");

    lookingAt_ = gazePoint;
    upVector_ = upVector;
}



void Camera::orient(const glm::vec3& position, const glm::vec3& viewingVector,
                    float roll
)
{
    throw std::runtime_error("Camera::orient unsupported: upVector issues");
    /*
    position_ = position;
    lookingAt_ = position + viewingVector;

    auto orientation = calculateLookDirection();
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), roll, orientation);
    upVector_ = (rotationMatrix * glm::vec4(upVector_, 1)).xyz();
    */
}



void Camera::translateX(float magnitude)
{
    translate(glm::vec3(magnitude, 0, 0));
}



void Camera::translateY(float magnitude)
{
    translate(glm::vec3(0, magnitude, 0));
}



void Camera::translateZ(float magnitude)
{
    translate(glm::vec3(0, 0, magnitude));
}



//translate the camera along X/Y/Z
void Camera::translate(const glm::vec3& delta)
{
    position_ += delta;
    lookingAt_ += delta;
}



void Camera::moveForward(float magnitude)
{
    translate(calculateLookDirection() * magnitude);
}



void Camera::moveRight(float magnitude)
{
    glm::vec3 orientation = calculateLookDirection();
    glm::vec3 tangental = glm::normalize(glm::cross(orientation, upVector_));
    translate(tangental * magnitude);
}



void Camera::moveUp(float magnitude)
{
    translate(glm::normalize(upVector_) * magnitude);
}



// Pitch the camera along the axis orthogonal to the up and look vectors
void Camera::pitch(float theta)
{
    glm::vec3 lookVector = lookingAt_ - position_;
    glm::vec3 tangental = glm::cross(lookVector, upVector_);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), theta, tangental);

    upVector_  = (rotationMatrix * glm::vec4(upVector_,  1)).xyz();
    lookVector = (rotationMatrix * glm::vec4(lookVector, 1)).xyz();
    lookingAt_ = position_ + lookVector;
}



// Yaw the camera typically around the up vector.
// If aroundUpVector is false, the camera yaws around (0, 0, 1), the global up
void Camera::yaw(float theta, bool aroundUpVector)
{
    glm::vec3 vectorOfRotation = aroundUpVector ? upVector_ : glm::vec3(0, 0, 1);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), theta, vectorOfRotation);

    glm::vec3 lookVector = lookingAt_ - position_;
    lookVector = (rotationMatrix * glm::vec4(lookVector, 1)).xyz();
    upVector_  = (rotationMatrix * glm::vec4(upVector_,  1)).xyz();
    lookingAt_ = position_ + lookVector;
}



//rolls the camera by rotating the up vector around the look direction
void Camera::roll(float theta)
{
    glm::vec3 orientation = calculateLookDirection();
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), theta, orientation);
    upVector_ = (rotationMatrix * glm::vec4(upVector_, 1)).xyz();
}



//returns true if the pitch attempt failed due to constraints
bool Camera::constrainedPitch(float theta)
{
    auto oldUpVector = upVector_;
    auto oldLookingAt = lookingAt_;

    pitch(theta);

    /*
        revert the pitch if any of the conditions are true:
        (orientation.z < 0 && upVector_.z < 0) ==> if looking down upside-down
        (orientation.z > 0 && upVector_.z < 0) ==> if looking up but tilted back
    */

    if (upVector_.z < 0 && fabs(calculateLookDirection().z) > 0.00001f) // != 0
    {
        upVector_ = oldUpVector;
        lookingAt_ = oldLookingAt;
        //std::cout << "Camera pitch constrained. Reverted request." << std::endl;
        return true;
    }

    return false;
}



bool Camera::constrainedRoll(float theta)
{
    auto oldUpVector = upVector_;
    roll(theta);

    if (upVector_.z < 0)
    {
        upVector_ = oldUpVector;
        std::cout << "Camera roll constrained. Reverted request." << std::endl;
        return true;
    }

    return false;
}



void Camera::setFieldOfView(float degrees)
{
    fieldOfView_ = degrees;
    updateProjectionMatrix();
}



void Camera::setAspectRatio(float ratio)
{
    aspectRatio_ = ratio;
    updateProjectionMatrix();
}



void Camera::setNearFieldClipDistance(float distance)
{
    nearFieldClip_ = distance;
    updateProjectionMatrix();
}



void Camera::setFarFieldClipDistance(float distance)
{
    farFieldClip_ = distance;
    updateProjectionMatrix();
}



// Sets the perspective of the camera
void Camera::setPerspective(
    float fieldOfViewDegrees, float aspectRatio,
    float nearClipDistance, float farClipDistance
)
{
    aspectRatio_   = aspectRatio;
    nearFieldClip_ = nearClipDistance;
    farFieldClip_  = farClipDistance;
    setFieldOfView(fieldOfViewDegrees);
}



void Camera::updateProjectionMatrix()
{
    projection_ = glm::perspective(fieldOfView_, aspectRatio_,
                                   nearFieldClip_, farFieldClip_);
}



//accessors:

glm::vec3 Camera::getPosition() const
{
    return position_;
}



glm::vec3 Camera::getLookingAt() const
{
    return lookingAt_;
}



glm::vec3 Camera::calculateLookDirection() const
{
    return glm::normalize(lookingAt_ - position_);
}



glm::vec3 Camera::getUpVector() const
{
    return upVector_;
}



glm::mat4 Camera::calculateViewMatrix() const
{
    return glm::lookAt(getPosition(), getLookingAt(), getUpVector());
}



float Camera::getFOV() const
{
    return fieldOfView_;
}



float Camera::getAspectRatio() const
{
    return aspectRatio_;
}



float Camera::getNearFieldClip() const
{
    return nearFieldClip_;
}



float Camera::getFarFieldClip() const
{
    return farFieldClip_;
}



glm::mat4 Camera::getProjectionMatrix() const
{
    return projection_;
}



std::string Camera::toString() const
{
    std::stringstream ss;

    ss << "Look@: <" << lookingAt_.x << ", " << lookingAt_.y <<
                    ", " << lookingAt_.z << "> ";
    ss << "UpV: <"   << upVector_.x << ", " << upVector_.y <<
                    ", " << upVector_.z << "> ";
    ss << "Pos: <"   << position_.x << ", " << position_.y <<
                    ", " << position_.z << ">";

    return ss.str();
}
