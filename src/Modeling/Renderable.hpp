
#ifndef RENDERABLE
#define RENDERABLE

#include <GL/glew.h>
//#include <GL/glut.h>

class Renderable
{
    public:
        virtual void render(GLint modelMatrixID) = 0;
};

#endif
