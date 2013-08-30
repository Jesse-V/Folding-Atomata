
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

#ifndef VIEWER
#define VIEWER

#include "Player.hpp"
#include "World/Scene.hpp"
#include <memory>

/**
    Viewer is a high-level class for handling larger game-play tasks.
    It contracts the construction of the current Scene, and passes mouse and
    keyboard inputs to the Player. It also accepts update and render methods
    for a basic gameplay loop. It can use the update function to ask any
    animations or actions to step forward a certain amount of milliseconds
    (typically the last time the update function was called) and then renders
    the current Scene onto the screen.
**/
class Viewer
{
    public:
        void update(int deltaTime);
        void render();
        void handleWindowReshape(int screenWidth, int screenHeight);

        void onKeyPress(unsigned char key, int, int);
        void onSpecialKeyPress(int key, int x , int y);
        void onMouseClick(int button, int state, int x, int y);
        void onMouseMotion(int x, int y);
        void onMouseDrag(int x, int y);

        static Viewer& getInstance();

    private:
        Viewer();
        void reportFPS();
        void addModels();
        void addSkybox();
        void addSlotViewers();
        void addLight();
        std::shared_ptr<Mesh> getSkyboxMesh();
        std::shared_ptr<Camera> createCamera();
        static void sleep(int milliseconds);

    private:
        static Viewer* singleton_;

        std::shared_ptr<Scene> scene_;
        std::shared_ptr<Player> player_;
        float timeSpentRendering_;
        int frameCount_;
};

#endif
