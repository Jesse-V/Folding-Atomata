
#include "main.hpp"
#include "Viewer/Viewer.hpp"
#include <thread>
#include <sstream>

/*
glm::unproject:
    http://stackoverflow.com/questions/15709045/glmunproject-issues-world-to-screen-space-alignment-error
    http://stackoverflow.com/questions/15060583/glmunproject-to-convert-mouse-position-to-3-coordinates
    http://glm.g-truc.net/api/a00151.html#ga540d5f6bb3f41e5dfa38d6ebd8771765
    http://stackoverflow.com/questions/9901453/using-glms-unproject

Useful socket links:
    http://www.codeproject.com/Questions/368114/telnet-within-a-C-Cplusplus-program
    http://www.cpp-home.com/forum/viewtopic.php?t=8936
    http://codebase.eu/tutorial/linux-socket-programming-c/
    http://stackoverflow.com/questions/2952733/using-sys-socket-h-functions-on-windows

    http://stackoverflow.com/questions/670891/is-there-a-way-for-multiple-processes-to-share-a-listening-socket
    https://forums.oracle.com/thread/2416799?tstart=210
    http://msdn.microsoft.com/en-us/library/5w7b7x5f.aspx
    http://www.cplusplus.com/forum/general/12836/
*/

static bool readyToUpdate = false;

void updateCallback()
{
    try
    {
        if (readyToUpdate)
        {
            static int oldTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);

            int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
            int deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;

            Viewer::getInstance().update(deltaTime);
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //sleep for a bit
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during update: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void renderCallback()
{
    try
    {
        Viewer::getInstance().render();
        glutPostRedisplay(); //make a call to render again at screen's FPS

        readyToUpdate = true;
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during render: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}


void windowReshapeCallback(int width, int height)
{
    try
    {
        Viewer::getInstance().handleWindowReshape(width, height);
        glViewport(0, 0, width, height); //this is a subtle but critical call!
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " in window callback: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void keyPressCallback(unsigned char key, int x, int y)
{
    try
    {
        Viewer::getInstance().onKeyPress(key, x, y);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during key press: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void specialKeyPressCallback(int key, int x, int y)
{
    try
    {
        Viewer::getInstance().onSpecialKeyPress(key, x, y);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name()
            << " during special key press: " << e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void mouseClickCallback(int button, int state, int x, int y)
{
    try
    {
        Viewer::getInstance().onMouseClick(button, state, x, y);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during mouse click: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void mouseMotionCallback(int x, int y)
{
    try
    {
        Viewer::getInstance().onMouseMotion(x, y);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during mouse motion: "
            << e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void mouseDragCallback(int x, int y)
{
    try
    {
        Viewer::getInstance().onMouseDrag(x, y);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during mouse drag: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



/* Initializes glut. Sets the window size and title to the specified values */
void initializeGlutWindow(int width, int height, const std::string& windowTitle)
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(windowTitle.c_str());

    std::cout << "window is " << width << " by " << height << ", a ratio of " << (width / (float)height) << std::endl;
}



void createGlContext()
{
    GLenum glew_status = glewInit();
    if (glew_status == GLEW_OK)
        std::cout << "Successfully created GL context." << std::endl;
    else
    {
        std::stringstream errorStream("");
        errorStream << "glewInit() error. Failed to create GL context.";
        errorStream << std::endl;
        errorStream << glewGetErrorString(glew_status);
        throw std::runtime_error(errorStream.str());
    }

    std::cout << "<GL context>" << std::endl;
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "</GL context>" << std::endl;
}



void assertSystemRequirements()
{
    std::stringstream stream("");
    stream << glGetString(GL_SHADING_LANGUAGE_VERSION);

    float version;
    stream >> version;

    const float MIN_GLSL = 1.20f;

    if (version < MIN_GLSL)
        throw std::runtime_error("Your driver/GPU does not support OpenGL 2.1");
    else
        std::cout << "GLSL v" << MIN_GLSL << " required, have " << version <<
            ", so passed system requirements." << std::endl;
}



void assignCallbacks()
{
    glutIdleFunc(updateCallback);
    glutDisplayFunc(renderCallback);
    glutReshapeFunc(windowReshapeCallback);

    glutKeyboardFunc(keyPressCallback);
    glutSpecialFunc(specialKeyPressCallback);

    glutMouseFunc(mouseClickCallback);
    glutMotionFunc(mouseDragCallback);
    glutPassiveMotionFunc(mouseMotionCallback);
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    initializeGlutWindow(glutGet(GLUT_SCREEN_WIDTH),
                         glutGet(GLUT_SCREEN_HEIGHT),
                         "Folding Atomata - third-party F@h simulation viewer"
    );

    try
    {
        createGlContext();
        assertSystemRequirements();
        assignCallbacks();

        std::cout << "Finished Glut and window initialization." << std::endl;

        Viewer::getInstance(); //calls Viewer's constructor, sets up everything...
        glutMainLoop();
    }
    catch (std::exception& e)
    {
        std::cerr << std::endl << "Caught " << typeid(e).name() <<
            " during glut setup: " << e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
