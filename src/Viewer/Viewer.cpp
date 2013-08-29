
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

#include "Viewer.hpp"
#include "SlotViewer.hpp"
#include "FAHClientIO.hpp"
#include "../Sockets/Connection.hpp"
#include "../Sockets/SocketException.hpp"
#include <thread>
#include <iostream>

/*
    1) return a vector of all trajectories for all slots
    2) display only slot 0
    3) display only snapshot 0
        draw atoms
        draw bonds
    4) then draw multiple snapshots (0, 1, 2, 3, ..., 0, 1, 2, 3, ...)

*/


Viewer::Viewer() :
    scene_(std::make_shared<Scene>(getCamera())),
    player_(std::make_shared<Player>(scene_)),
    timeSpentRendering_(0), frameCount_(0)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    addLight();
    addModels();
    
    player_->grabPointer();
    reportFPS();
}



void Viewer::reportFPS()
{
    std::thread fpsReporter([&]() {
        while (true)
        {
            sleep(1000);

            float msPerFrame = timeSpentRendering_ / frameCount_;
            std::cout << frameCount_ << " FPS, spent " <<
                timeSpentRendering_ << " ms rendering, avg " << 
                msPerFrame << " ms/frame, avg " <<
                (int)(scene_->getModelCount() * 1000 / msPerFrame) << 
                " Models/sec" << std::endl;

            frameCount_ = 0;
            timeSpentRendering_ = 0;
        }
    });

    fpsReporter.detach();
}



void Viewer::addModels()
{
    try
    {
        FAHClientIO io(Connection("localhost", 36330).createClientSocket());

        std::vector<TrajectoryPtr> trajectories = io.getTrajectories();

        if (trajectories.size() == 0)
            throw std::runtime_error("Not enough slots to work with.");

        SlotViewer slotViewer(trajectories[0], scene_);
    }
    catch (SocketException&)
    {
        throw std::runtime_error("Error connecting to FAHClient!");
    }
}



void Viewer::addLight()
{
    scene_->setAmbientLight(glm::vec3(1));
/*
    auto light1 = std::make_shared<Light>(
        glm::vec3(0),       //position
        glm::vec3(0, 0, 1), //blue
        10.0f                //power
    );

    scene_->addLight(light1);*/

    checkGlError();
}



std::shared_ptr<Camera> Viewer::getCamera()
{
    auto camera = std::make_shared<Camera>();
    camera->setPosition(glm::vec3(0, -50, 0));

    camera->lookAt(
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1)
    );

    return camera;
}



void Viewer::update(int deltaTime)
{

}



void Viewer::render()
{
    //glClearColor(.39f, 0.58f, 0.93f, 0.0f); //nice blue background
    //glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    timeSpentRendering_ += scene_->render();
    frameCount_++;

    glutSwapBuffers();
}



void Viewer::handleWindowReshape(int newWidth, int newHeight)
{
    scene_->getCamera()->setAspectRatio(newWidth / (float)newHeight);

    std::cout << "Windows updated to " << newWidth << " by " << newHeight << 
        ", a ratio of " << (newWidth / (float)newHeight) << std::endl;
}



/**
 * Causes the current thread to sleep
 * for the specified number of milliseconds
 */
void Viewer::sleep(int milliseconds)
{
    std::chrono::milliseconds duration(milliseconds);
    std::this_thread::sleep_for(duration); //C++11 way to sleep!
}



void Viewer::onKeyPress(unsigned char key, int, int)
{
    player_->onKeyPress(key);
}



void Viewer::onSpecialKeyPress(int key, int, int)
{
    player_->onSpecialKeyPress(key);
}



void Viewer::onMouseClick(int button, int state, int x, int y)
{
    player_->onMouseClick(button, state, x, y);
}



void Viewer::onMouseMotion(int x, int y)
{
    player_->onMouseMotion(x, y);
}



void Viewer::onMouseDrag(int x, int y)
{
    player_->onMouseDrag(x, y);
}



Viewer* Viewer::singleton_ = 0;

Viewer& Viewer::getInstance()
{
    try
    {
        if (singleton_)
            return *singleton_;

        std::cout << "Creating Viewer..." << std::endl;
        singleton_ = new Viewer();
        std::cout << "... finished creating Viewer." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << std::endl;
        std::cerr << "Caught " << typeid(e).name() <<
            " during Viewer initiation: " << e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }

    return *singleton_;
}




/* num-slots
std::vector<glm::vec2> getOffsetVectors(int numberOfSlots)
{
    switch (numberOfSlots)
    {
        case 1 :
            return {
                glm::vec2(0, 0)
            };

        case 2 :
            return {
                glm::vec2(0, 1),
                glm::vec2(0, -1)
            }

        case 3 :
            return {
                glm::vec2(0, 1),
                glm::vec2(1, -0.7),
                glm::vec2(-1, -0.7)
            }

        case 4 :
            return {
                glm::vec2(-1, 1),
                glm::vec2(1, -1),
                glm::vec2(1, 1),
                glm::vec2(-1, -1)
            }

        case 5 :
            return {
                glm::vec2(-1, 1),
                glm::vec2(1, -1),
                glm::vec2(1, 1),
                glm::vec2(-1, -1),
                glm::vec2(0, 0)
            }

        default :
            throw std::runtime_error("Too many slots");
    }
}

static std::vector<SlotViewer> viewAllSlots(const std::string& host, int port);
*/


/*
    http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c

    protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }
    */

//slot-info, in between "\"id\": \"" and "\",\n"
    //1) check for heartbeat, throw error if no connection
    //2) get topology. This data should be accessible as soon it's available.
    //3) get checkpoints. Can view more and more of them as they come in.
    /*
    try
    {
        SlotViewer viewer(Connection("localhost", 36330), 0);

        while (true)
        {
            std::chrono::milliseconds duration(2000);
            std::this_thread::sleep_for(duration);
            std::cout << "Main looping..." << std::endl;
        }

    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught: " << e.description() << "\n";
    }
*/
