
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

#define _GLIBCXX_USE_NANOSLEEP

#include "Viewer.hpp"
#include "FAHClientIO.hpp"
#include "Sockets/SocketException.hpp"
#include "Modeling/Shading/ShaderManager.hpp"
#include "PyON/TrajectoryParser.hpp"
#include "Options.hpp"
#include <thread>
#include <algorithm>
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
    scene_(std::make_shared<Scene>(createCamera())),
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
    //addSkybox();
    //addSlotViewers();

    auto model = std::make_shared<Model>(getSkyboxMesh());
    auto program = ShaderManager::createProgram(model,
        scene_->getVertexShaderGLSL(),
        scene_->getFragmentShaderGLSL(), scene_->getLights()
    );
    scene_->addModel(model, program, true);
}



void Viewer::addSkybox()
{
    /*std::cout << "Creating skybox..." << std::endl;

    const std::string IMAGES_DIR("/usr/share/FoldingAtomata/images/");
    auto msmImage      = std::make_shared<Image>(IMAGES_DIR + "MSM.png");
    auto primaseImage  = std::make_shared<Image>(IMAGES_DIR + "Primase.png");
    auto ribosomeImage = std::make_shared<Image>(IMAGES_DIR + "Ribosome.png");

    auto cubeTextureMap = std::make_shared<CubeTextureMap>(msmImage, msmImage,
        primaseImage, primaseImage, ribosomeImage, ribosomeImage);
    BufferList list = { cubeTextureMap };

    auto model = std::make_shared<Model>(getSkyboxMesh(), list);
    auto matrix = glm::scale(glm::mat4(), glm::vec3(100));
    model->setModelMatrix(matrix);

    std::cout << "Generating program for skybox..." << std::endl;
    auto program = ShaderManager::createProgram(model,
        scene_->getVertexShaderGLSL(),
        scene_->getFragmentShaderGLSL(), scene_->getLights()
    );

    std::cout << "... done generating skybox program." << std::endl;

    scene_->addModel(model, program, true); //add to Scene and save

    std::cout << "... done creating skybox." << std::endl;*/
}



std::shared_ptr<Mesh> Viewer::getSkyboxMesh()
{
    static std::shared_ptr<Mesh> mesh = nullptr;

    if (mesh)
        return mesh;

    const std::vector<glm::vec3> VERTICES = {
        glm::vec3(-1, -1, -1),
        glm::vec3(-1, -1,  1),
        glm::vec3(-1,  1, -1),
        glm::vec3(-1,  1,  1),
        glm::vec3( 1, -1, -1),
        glm::vec3( 1, -1,  1),
        glm::vec3( 1,  1, -1),
        glm::vec3( 1,  1,  1)
    };

    //visible from the inside only, so faces in
    const std::vector<GLuint> INDICES = {
        0, 1, 5, 4, //front
        6, 7, 3, 2, //back
        2, 0, 4, 6,  //top
        7, 5, 1, 3, //bottom
        2, 3, 1, 0, //left
        4, 5, 7, 6  //right
    };

    auto vBuffer = std::make_shared<VertexBuffer>(VERTICES);
    auto iBuffer = std::make_shared<IndexBuffer>(INDICES, GL_QUADS);
    mesh = std::make_shared<Mesh>(vBuffer, iBuffer, GL_QUADS);
    return mesh;
}



void Viewer::addSlotViewers()
{
    std::vector<TrajectoryPtr> trajectories;

    try
    {
        auto socket = std::make_shared<ClientSocket>(
            Options::getInstance().getHost(),
            Options::getInstance().getPort()
        );

        FAHClientIO io(socket);
        trajectories = io.getTrajectories();

        if (trajectories.empty())
            std::cerr << "Not enough slots to work with. " <<
                "Using demo protein." << std::endl;
    }
    catch (SocketException&)
    {
        std::cerr << "Error connection to FAHClient (SocketException). " <<
                "Using demo protein." << std::endl;
    }

    if (trajectories.empty())
    {
        const std::string FILENAME = "/usr/share/FoldingAtomata/demoProtein";
        std::ifstream fin(FILENAME, std::ios::in | std::ios::binary);
        if (!fin.is_open())
            throw std::runtime_error("Unable to demo protein!");

        std::string proteinStr;
        fin.seekg(0, std::ios::end);
        proteinStr.resize((unsigned long)fin.tellg()); //allocate enough space
        fin.seekg(0, std::ios::beg);
        fin.read(&proteinStr[0], (long)proteinStr.size()); //read entire file
        fin.close();

        TrajectoryParser parser(proteinStr, false);
        trajectories.push_back(parser.parse());
    }

    auto slot0Viewer = std::make_shared<SlotViewer>(trajectories[0], scene_);
    slotViewers_.push_back(slot0Viewer);
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



std::shared_ptr<Camera> Viewer::createCamera()
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
    player_->update(deltaTime);
    for (auto viewer : slotViewers_)
        viewer->update(deltaTime);
}



void Viewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    timeSpentRendering_ += scene_->render();
    frameCount_++;

    glutSwapBuffers();
}



void Viewer::handleWindowReshape(int newWidth, int newHeight)
{
    scene_->getCamera()->setAspectRatio(newWidth / (float)newHeight);
    player_->setWindowOffset(glutGet(GLUT_WINDOW_X), glutGet(GLUT_WINDOW_Y));

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



std::shared_ptr<Player> Viewer::getPlayer()
{
    return player_;
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
        std::cout << "... done creating Viewer." << std::endl;
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
