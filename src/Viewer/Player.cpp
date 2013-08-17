
#include "Player.hpp"
#include "World/Camera.hpp"
//#include <iostream>


Player::Player(std::shared_ptr<Scene> scene) : 
    CENTER_X(glutGet(GLUT_SCREEN_WIDTH) / 2),
    CENTER_Y(glutGet(GLUT_SCREEN_HEIGHT) / 2),
    scene_(scene), mouseControlsCamera_(true)
{}



void Player::grabPointer()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    mouseControlsCamera_ = true;
    recenterCursor();
}



void Player::releasePointer()
{
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    mouseControlsCamera_ = false;
}



void Player::recenterCursor()
{
    glutWarpPointer(CENTER_X, CENTER_Y); //moves mouse cursor
}



void Player::onKeyPress(unsigned char key)
{
    static const auto ESCAPE = (unsigned char)27;

    std::shared_ptr<Camera> camera = scene_->getCamera();
    
    switch(key)
    {
        case 'a':
            camera->moveRight(-TRANSLATION_SPEED);
            break;

        case 'd':
            camera->moveRight(TRANSLATION_SPEED);
            break;

        case 's':
            camera->moveForward(-TRANSLATION_SPEED);
            break;

        case 'w':
            camera->moveForward(TRANSLATION_SPEED);
            break;

        case 'q':
            camera->moveUp(-TRANSLATION_SPEED);
            break;

        case 'e':
            camera->moveUp(TRANSLATION_SPEED);
            break;

        case ESCAPE:
            releasePointer();
            break;
    }

    //scene_->getLights()[0]->setPosition(scene_->getCamera()->getPosition());
    //std::cout << scene_->getCamera()->toString() << std::endl;
}



void Player::onSpecialKeyPress(int key)
{
    std::shared_ptr<Camera> camera = scene_->getCamera();

    switch(key)
    {
        case GLUT_KEY_PAGE_UP:
            camera->constrainedRoll(-ROLL_SPEED);
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera->constrainedRoll(ROLL_SPEED);
            break;
    }
}



void Player::onMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mouseControlsCamera_)
            releasePointer();
        else
            grabPointer();
    }
}



void Player::onMouseMotion(int x, int y)
{
    static int lastX, lastY;
    static bool determinedLastPosition;

    if (!mouseControlsCamera_)
        return;

    if (!determinedLastPosition)
    {
        lastX = x;
        lastY = y;
        determinedLastPosition = true;
        return;
    }

    if (x != CENTER_X || y != CENTER_Y)
    {
        scene_->getCamera()->constrainedPitch((lastY - y) * PITCH_COEFFICIENT);
        scene_->getCamera()->yaw((lastX - x) * YAW_COEFFICIENT);
        recenterCursor();
    }

    lastX = x;
    lastY = y;
}



void Player::onMouseDrag(int x, int y)
{

}
