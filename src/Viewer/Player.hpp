
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

#ifndef PLAYER
#define PLAYER

/**
    The Player class handles functions relating to user interaction and movement.
    Its main goal is to represent the user interacting with the Scene.
    For example, one of its basic jobs is to accept and handle mouse and keyboard
    actions and use them to move the Scene's Camera accordingly.
    The mouse controls the orientation of the camera, and the WASDQE keys
    control the location. The camera moves with linear acceleration and
    geometric (non-linear) deceleration, which creates smooth and fluid movement.
**/

#include "World/Scene.hpp"
#include <memory>
#include <unordered_set>

class Player
{
    public:
        const float ACCELERATION = 0.0011f;
        const float GEOMETRIC_SPEED_DECAY = 0.96f;
        const float MAX_SPEED = 1.5f;

        const float PITCH_COEFFICIENT = 0.05f;
        const float YAW_COEFFICIENT = 0.05f;
        const float ROLL_SPEED = 0.05f;

    public:
        Player(std::shared_ptr<Scene> scene);
        void update(int deltaTime);
        void applyAcceleration(int deltaTime);
        void setWindowOffset(int x, int y);
        void grabPointer();
        void releasePointer();
        void recenterCursor();

        void onKeyPress(unsigned char key);
        void onKeyRelease(unsigned char key);
        void onSpecialKeyPress(int key);
        void onSpecialKeyRelease(int key);
        void onMouseClick(int button, int state, int x, int y);
        void onMouseMotion(int x, int y);
        void onMouseDrag(int x, int y);
        
    private:
        enum class KeyAction : short
        {
            FORWARD, BACKWARD,
            RIGHT, LEFT,
            UP, DOWN, 
            POSITIVE_ROLL, NEGATIVE_ROLL
        };

        struct KeyHash
        {
            std::size_t operator()(const KeyAction& myEnum) const
            {
                return static_cast<std::size_t>(myEnum);
            }
        };

        std::shared_ptr<Scene> scene_;
        bool mouseControlsCamera_;
        int windowCenterX_, windowCenterY_;
        
        glm::vec3 movementDelta_;
        std::unordered_set<KeyAction, KeyHash> downKeys_;
        //std::hash_map<char, CameraKey> keyBindings;
};

#endif
