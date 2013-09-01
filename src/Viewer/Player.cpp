
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
#include <iostream>


Player::Player(std::shared_ptr<Scene> scene) : 
    scene_(scene), mouseControlsCamera_(true),
    windowCenterX_(glutGet(GLUT_WINDOW_WIDTH)  / 2),
    windowCenterY_(glutGet(GLUT_WINDOW_HEIGHT) / 2)
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
            downKeys_.insert(KeyAction::FORWARDS);
            break;

        case 's':
            downKeys_.insert(KeyAction::BACKWARD);
            break;

        case 'd':
            downKeys_.insert(KeyAction::RIGHT);
            break;

        case 'a':
            downKeys_.insert(KeyAction::LEFT);
            break;

        case 'e':
            downKeys_.insert(KeyAction::UP);
            break;

        case 'q':
            downKeys_.insert(KeyAction::DOWN);
            break;

        case ESCAPE:
            releasePointer();
            break;
    }
}



void Player::onKeyRelease(unsigned char key)
{
    switch(key)
    {
        case 'w':
            downKeys_.erase(KeyAction::FORWARDS);
            break;

        case 's':
            downKeys_.erase(KeyAction::BACKWARD);
            break;

        case 'd':
            downKeys_.erase(KeyAction::RIGHT);
            break;

        case 'a':
            downKeys_.erase(KeyAction::LEFT);
            break;

        case 'e':
            downKeys_.erase(KeyAction::UP);
            break;

        case 'q':
            downKeys_.erase(KeyAction::DOWN);
            break;
    }
}



void Player::onSpecialKeyPress(int key)
{
    switch(key)
    {
        case GLUT_KEY_PAGE_UP:
            downKeys_.insert(KeyAction::NEGATIVE_ROLL);
            break;

        case GLUT_KEY_PAGE_DOWN:
            downKeys_.insert(KeyAction::POSITIVE_ROLL);
            break;
    }
}



void Player::onSpecialKeyRelease(int key)
{
    switch(key)
    {
        case GLUT_KEY_PAGE_UP:
            downKeys_.erase(KeyAction::NEGATIVE_ROLL);
            break;

        case GLUT_KEY_PAGE_DOWN:
            downKeys_.erase(KeyAction::POSITIVE_ROLL);
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
    if (downKeys_.count(KeyAction::FORWARDS))
        movementDelta_.x += ACCELERATION * deltaTime;
    if (downKeys_.count(KeyAction::BACKWARD))
        movementDelta_.x -= ACCELERATION * deltaTime;

    if (downKeys_.count(KeyAction::RIGHT))
        movementDelta_.y += ACCELERATION * deltaTime;
    if (downKeys_.count(KeyAction::LEFT))
        movementDelta_.y -= ACCELERATION * deltaTime;

    if (downKeys_.count(KeyAction::UP))
        movementDelta_.z += ACCELERATION * deltaTime;
    if (downKeys_.count(KeyAction::DOWN))
        movementDelta_.z -= ACCELERATION * deltaTime;

    movementDelta_ = glm::clamp(movementDelta_, -MAX_SPEED, MAX_SPEED);

    auto camera = scene_->getCamera();
    camera->moveForward(movementDelta_.x);
    camera->moveRight(movementDelta_.y);
    camera->moveUp(movementDelta_.z);

    if (downKeys_.empty())
        movementDelta_ *= GEOMETRIC_SPEED_DECAY;
}



void Player::setWindowOffset(int x, int y)
{
    //there's something upstream in Linux or the drivers going on here: 
    //this code alone shouldn't fix #30, yet it does for some reason

    windowCenterX_ = glutGet(GLUT_WINDOW_WIDTH)  / 2;
    windowCenterY_ = glutGet(GLUT_WINDOW_HEIGHT) / 2;
}
