
#ifndef GAME
#define GAME

#include "Player.hpp"
#include "World/Scene.hpp"
#include <memory>

/**
    Game is a high-level class for handling larger game-play tasks.
    It contracts the construction of the current Scene, and passes mouse and
    keyboard inputs to the Player. It also accepts update and render methods
    for a basic gameplay loop. It can use the update function to ask any
    animations or actions to step forward a certain amount of milliseconds
    (typically the last time the update function was called) and then renders
    the current Scene onto the screen.
**/
class Game
{
    public:
        void update(int deltaTime);
        void render();

        void onKeyPress(unsigned char key, int, int);
        void onSpecialKeyPress(int key, int x , int y);
        void onMouseClick(int button, int state, int x, int y);
        void onMouseMotion(int x, int y);
        void onMouseDrag(int x, int y);

        static Game& getInstance();

    private:
        Game(int screenWidth, int screenHeight);
        void reportFPS();
        void addModels();
        void addCubes();
        void addLight();
        std::shared_ptr<Camera> getCamera(int screenWidth, int screenHeight);
        static void sleep(int milliseconds);

    private:
        static Game* singleton_;

        std::shared_ptr<Scene> scene_;
        std::shared_ptr<Player> player_;
        float timeSpentRendering_;
        int frameCount_;
};

#endif
