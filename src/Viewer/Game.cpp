
#include "Game.hpp"
#include "Voxels/Landscape.hpp"
#include <memory>
#include <thread>
#include <iostream>


Game::Game(int screenWidth, int screenHeight):
    scene_(std::make_shared<Scene>(getCamera(screenWidth, screenHeight))),
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



void Game::reportFPS()
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



void Game::addModels()
{
    addCubes();
}



void Game::addCubes()
{
    auto landscape = std::make_shared<Landscape>();
    landscape->generateChunk(scene_);
}



void Game::addLight()
{
    scene_->setAmbientLight(glm::vec3(1));

    /*auto light1 = std::make_shared<Light>(
        glm::vec3(0),       //position
        glm::vec3(0, 0, 1), //green
        2.0f                //power
    );

    scene_->addLight(light1);*/

    auto light2 = std::make_shared<Light>(
        scene_->getCamera()->getPosition(), //position
        glm::vec3(1),                       //white
        5.0f                                //power
    );

    scene_->addLight(light2);

    checkGlError();
}



std::shared_ptr<Camera> Game::getCamera(int screenWidth, int screenHeight)
{
    auto camera = std::make_shared<Camera>();
    camera->setAspectRatio(screenWidth / (float)screenHeight);
    camera->setPosition(glm::vec3(0, -2.98502f, 2.09858f));

    camera->lookAt(
        glm::vec3(0, 11.7493f, -0.712136f),
        glm::vec3(0, 0.187381f, 0.982287f)
    );

    return camera;
}



void Game::update(int deltaTime)
{

}



void Game::render()
{
    glClearColor(.39f, 0.58f, 0.93f, 0.0f); //nice blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    timeSpentRendering_ += scene_->render();
    frameCount_++;

    glutSwapBuffers();
}



/**
 * Causes the current thread to sleep
 * for the specified number of milliseconds
 */
void Game::sleep(int milliseconds)
{
    std::chrono::milliseconds duration(milliseconds);
    std::this_thread::sleep_for(duration); //C++11 way to sleep!
}



void Game::onKeyPress(unsigned char key, int, int)
{
    player_->onKeyPress(key);
}



void Game::onSpecialKeyPress(int key, int, int)
{
    player_->onSpecialKeyPress(key);
}



void Game::onMouseClick(int button, int state, int x, int y)
{
    player_->onMouseClick(button, state, x, y);
}



void Game::onMouseMotion(int x, int y)
{
    player_->onMouseMotion(x, y);
}



void Game::onMouseDrag(int x, int y)
{
    player_->onMouseDrag(x, y);
}



Game* Game::singleton_ = 0;

Game& Game::getInstance()
{
    try
    {
        if (singleton_)
            return *singleton_;

        std::cout << "Creating Game..." << std::endl;
        singleton_ = new Game(
            glutGet(GLUT_SCREEN_WIDTH),
            glutGet(GLUT_SCREEN_HEIGHT)
        );
        std::cout << "... finished creating Game." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << std::endl;
        std::cerr << "Caught " << typeid(e).name() <<
            " during Game initiation: " << e.what();
        std::cerr << std::endl;

        std::cerr << "\"And on that bombshell it is time to end. Good night!\"";
        std::cerr << std::endl;
        glutDestroyWindow(glutGetWindow());
    }

    return *singleton_;
}
