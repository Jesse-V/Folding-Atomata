
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

#include "main.hpp"
#include "Viewer/Viewer.hpp"
#include <thread>
#include <sstream>

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



void keyPressCallback(unsigned char key, int, int)
{
    try
    {
        Viewer::getInstance().getPlayer()->onKeyPress(key);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during key press: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void keyReleaseCallback(unsigned char key, int, int)
{
    try
    {
        Viewer::getInstance().getPlayer()->onKeyRelease(key);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name() << " during key press: " <<
            e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void specialKeyPressCallback(int key, int, int)
{
    try
    {
        Viewer::getInstance().getPlayer()->onSpecialKeyPress(key);
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught " << typeid(e).name()
            << " during special key press: " << e.what() << std::endl;
        glutDestroyWindow(glutGetWindow());
    }
}



void specialKeyReleaseCallback(int key, int, int)
{
    try
    {
        Viewer::getInstance().getPlayer()->onSpecialKeyRelease(key);
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
        Viewer::getInstance().getPlayer()->onMouseClick(button, state, x, y);
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
        Viewer::getInstance().getPlayer()->onMouseMotion(x, y);
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
        Viewer::getInstance().getPlayer()->onMouseDrag(x, y);
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
    glutKeyboardUpFunc(keyReleaseCallback);
    glutSpecialFunc(specialKeyPressCallback);
    glutSpecialUpFunc(specialKeyReleaseCallback);

    glutMouseFunc(mouseClickCallback);
    glutMotionFunc(mouseDragCallback);
    glutPassiveMotionFunc(mouseMotionCallback);
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    initializeGlutWindow(
        glutGet(GLUT_SCREEN_WIDTH),
        glutGet(GLUT_SCREEN_HEIGHT),
        "Folding Atomata - third-party Folding@home simulation viewer"
    );

    try
    {
        createGlContext();
        assertSystemRequirements();
        assignCallbacks();
        glutIgnoreKeyRepeat(true);

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
