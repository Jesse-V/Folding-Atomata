
#ifndef TEXTURE_BUFFER
#define TEXTURE_BUFFER

#include "SampledBuffer.hpp"

/**
    A TextureBuffer maps a texture image onto a fragments of a polygon.
    See the SampledBuffer documentation for further details on how it does this.
    In the end, each fragment can retrieve data from its corresponding location
    in the image. In this class, this is used for determining the raw color that
    the fragment should be. This is then applied as an influence for the final 
    coloring calculation that also applies any light effects.
**/
class TextureBuffer : public SampledBuffer
{
    public:
        TextureBuffer(const std::string& imagePath,
                      const std::vector<GLfloat>& coordinateMap);

        virtual void initialize(GLuint programHandle);
        virtual void enable();
        virtual void disable();
        virtual void store();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        GLint texCoordAttrib_;
};

#endif
