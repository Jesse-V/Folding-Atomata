
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
    windowCenterX_(glutGet(GLUT_WINDOW_WIDTH)  / 2),
    windowCenterY_(glutGet(GLUT_WINDOW_HEIGHT) / 2),
    scene_(scene), mouseControlsCamera_(true),
    lastKeyPressed_(std::chrono::steady_clock::now())
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
    glutWarpPointer(windowCenterX_, windowCenterY_); //moves mouse cursor
}



void Player::onKeyPress(unsigned char key)
{
    static const auto ESCAPE = (unsigned char)27;
    
    switch(key)
    {
        case 'w':
            movementDelta_.x += ACCELERATION; //move forwards
            break;

        case 's':
            movementDelta_.x -= ACCELERATION; //move backwards
            break;

        case 'd':
            movementDelta_.y += ACCELERATION; //move right
            break;

        case 'a':
            movementDelta_.y -= ACCELERATION; //move left
            break;

        case 'e':
            movementDelta_.z += ACCELERATION; //move up
            break;

        case 'q':
            movementDelta_.z -= ACCELERATION; //move down
            break;

        case ESCAPE:
            releasePointer();
            break;
    }

    movementDelta_ = glm::clamp(movementDelta_, -MAX_SPEED, MAX_SPEED);
    lastKeyPressed_ = std::chrono::steady_clock::now();
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

    if (x != windowCenterX_ || y != windowCenterY_)
    {
        scene_->getCamera()->pitch((lastY - y) * PITCH_COEFFICIENT);
        scene_->getCamera()->yaw((lastX - x) * YAW_COEFFICIENT);
        recenterCursor();
    }

    lastX = x;
    lastY = y;
}



void Player::onMouseDrag(int x, int y)
{}



void Player::update(int deltaTime)
{
    auto camera = scene_->getCamera();

    camera->moveForward(movementDelta_.x);
    camera->moveRight(movementDelta_.y);
    camera->moveUp(movementDelta_.z);

    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;
    auto timeDiff = steady_clock::now() - lastKeyPressed_;
    long ms = duration_cast<milliseconds>(timeDiff).count();
    
    if (ms >= KEY_PRESSED_TIMEOUT) //if the key is no longer considered down, brake
        movementDelta_ *= GEOMETRIC_SPEED_DECAY;
}



void Player::setWindowOffset(int x, int y)
{
    //there's something upstream in Linux going on here: 
    //this code alone shouldn't fix #30, yet it does for some reason
    
    windowCenterX_ = glutGet(GLUT_WINDOW_WIDTH)  / 2;
    windowCenterY_ = glutGet(GLUT_WINDOW_HEIGHT) / 2;
}
