
#ifndef MAPPED_BUFFER
#define MAPPED_BUFFER

#include "../OptionalDataBuffer.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <string>

/**
    A SampledBuffer maps an image onto a fragments of a polygon.
    An image is first generated CPU-side or loaded from a file. It's then
    saved into the GPU's VRAM, and is then applied to a set of fragments on the
    screen. This last step is done with UV coordinates: a coordinate system of
    (0, 0) to (1, 1) that specifies a point on the image. A sampler is used in
    the fragment shader to convert from camera-space to UV coordinates in 
    image-space. This results in each fragment knowing its corresponding
    location in the image, and the data there can the be used in ways
    defined by the classes which inherit this class. When this mapping causes
    the image to appear to be enlarged or reduced, linear minimap filters are
    applied for a smooth blending effect. 
**/
class SampledBuffer : public OptionalDataBuffer
{
    public:
        SampledBuffer(const std::string& imagePath,
                      const std::vector<GLfloat>& coordinateMap);
        ~SampledBuffer();
        //default copy constructor should be fine

        void loadBMP(const std::string& imagePath);
        void loadPNG(const std::string& imagePath);
        void deleteBufferFromRAM();

        virtual void initialize(GLuint handle);
        virtual void store();

    protected:
        void storeImage();
        void storeCoordMap();

        bool strHasEnding(const std::string& string, const std::string& ending);

    protected:
        int imgWidth_, imgHeight_;
        unsigned char* data_;
        bool isValid_ = false;

        std::vector<GLfloat> coordinateMap_;
        GLuint vbo_coords_;
};

#endif
