
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

#ifndef SCENE
#define SCENE

#include "Camera.hpp"
#include "Light.hpp"
#include "Modeling/Shading/Program.hpp"
#include "Modeling/Model.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>
#include <vector>

typedef std::shared_ptr<Model> ModelPtr;
typedef std::unordered_multimap<ProgramPtr, ModelPtr> ProgramModelMultimap;

/**
    A Scene is basically a high-level container for Models, Lights, and a Camera.
    This allows for an application to use multiple Scenes for different environments
    or locations. Another advantage is that Scene::render() will handle the
    management of rendering all Models with all the Lights, as seen by the given
    Camera.
**/
class Scene
{
    public:
        Scene(const std::shared_ptr<Camera>& camera);
        void addModel(const ModelPtr& model);
        void addModel(const ModelPtr& model, const ProgramPtr& program, 
                                                              bool save = true);
        void addLight(const std::shared_ptr<Light>& light);
        void setCamera(const std::shared_ptr<Camera>& camera);
        void setAmbientLight(const glm::vec3& rgb);
        void sync();
        float render(); //iterate through all Programs to render

        std::shared_ptr<Camera> getCamera();
        int getModelCount();
        //std::vector<ModelPtr> getModels();
        //std::vector<ProgramPtr> getPrograms();
        //int getModelCount(), getProgramCount();
        LightList getLights();
        glm::vec3 getAmbientLight();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        void syncLighting(GLuint handle);

    private:
        ProgramModelMultimap map_; //1-to-many map between Programs and Models
        LightList lights_;
        std::shared_ptr<Camera> camera_;
        glm::vec3 ambientLight_;
};


#endif
