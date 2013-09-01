
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

#include "Player.hpp"
#include "World/Camera.hpp"
#include <algorithm>


Player::Player(std::shared_ptr<Scene> scene) : 
    CENTER_X(glutGet(GLUT_SCREEN_WIDTH) / 2),
    CENTER_Y(glutGet(GLUT_SCREEN_HEIGHT) / 2),
    scene_(scene), mouseControlsCamera_(true),
    speed_(0)
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
            actions_.push(MOVE_LEFT);
            last_ = MOVE_LEFT;
            break;

        case 'd':
            actions_.push(MOVE_RIGHT);
            last_ = MOVE_RIGHT;
            break;

        case 's':
            actions_.push(MOVE_BACKWARD);
            last_ = MOVE_BACKWARD;
            break;

        case 'w':
            actions_.push(MOVE_FORWARD);
            last_ = MOVE_FORWARD;
            break;

        case 'q':
            actions_.push(MOVE_UP);
            last_ = MOVE_UP;
            break;

        case 'e':
            actions_.push(MOVE_DOWN);
            last_ = MOVE_DOWN;
            break;

        case ESCAPE:
            releasePointer();
            break;
    }

    speed_ += ACCELERATION;

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
        scene_->getCamera()->pitch((lastY - y) * PITCH_COEFFICIENT);
        scene_->getCamera()->yaw((lastX - x) * YAW_COEFFICIENT);
        recenterCursor();
    }

    lastX = x;
    lastY = y;
}



void Player::onMouseDrag(int x, int y)
{

}



void Player::update(int deltaTime)
{
    if (speed_ > 0)
        applyCameraAction(last_);
    
    while (!actions_.empty())
    {
        Action action = actions_.front();
        actions_.pop();

        applyCameraAction(action);
    }

    speed_ = std::max(0.0f, std::min(speed_ - deltaTime * BRAKE_SPEED, MAX_SPEED));
}



void Player::applyCameraAction(const Action& action)
{
    auto camera = scene_->getCamera();

    switch(action)
    {
        case MOVE_LEFT:
            camera->moveRight(-speed_);
            break;

        case MOVE_RIGHT:
            camera->moveRight(speed_);
            break;

        case MOVE_BACKWARD:
            camera->moveForward(-speed_);
            break;

        case MOVE_FORWARD:
            camera->moveForward(speed_);
            break;

        case MOVE_DOWN:
            camera->moveUp(-speed_);
            break;

        case MOVE_UP:
            camera->moveUp(speed_);
            break;
    }
}
