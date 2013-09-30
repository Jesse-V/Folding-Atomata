
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

#include "Scene.hpp"
#include "Modeling/Shading/ShaderManager.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>


Scene::Scene(const std::shared_ptr<Camera>& camera):
    camera_(camera)
{}



void Scene::addModel(const ModelPtr& model)
{
    addModel(model, ShaderManager::createProgram(model,
        getVertexShaderGLSL(), getFragmentShaderGLSL(), lights_), true
    );
}



void Scene::addModel(const ModelPtr& model, const ProgramPtr& program, bool save)
{
    if (save)
        model->saveAs(program->getHandle());

    auto programIt = std::find(programs_.begin(), programs_.end(), program);
    int index = 0;
    if (programIt == programs_.end())
    {
        index = programs_.size();
        programs_.push_back(program);
    }
    else
        index = std::distance(programs_.begin(), programIt);

    models_.push_back(std::make_pair((std::size_t)index, model));
}



void Scene::addLight(const std::shared_ptr<Light>& light)
{
    lights_.push_back(light);

    std::cout << "Successfully added a Light to the Scene." << std::endl;
}



void Scene::setCamera(const std::shared_ptr<Camera>& sceneCamera)
{
    camera_ = sceneCamera;
}



void Scene::setAmbientLight(const glm::vec3& rgb)
{
    ambientLight_ = rgb;
}



void Scene::update()
{
    for (auto program : programs_)
    {
        GLuint handle = program->getHandle();
        glUseProgram(handle);
        camera_->sync(handle);
        syncLighting(handle);
    }
}



float Scene::render()
{
    update();

    using namespace std::chrono;
    auto start = steady_clock::now();
    for (auto pair : models_)
    {
        auto programHandle = programs_[pair.first]->getHandle();
        auto model = pair.second;
        model->render(programHandle);
    }

    auto diff = duration_cast<microseconds>(steady_clock::now() - start).count();
    return diff / 1000.0f;
}



std::shared_ptr<Camera> Scene::getCamera()
{
    return camera_;
}



int Scene::getModelCount()
{
    return (int)models_.size();
}



LightList Scene::getLights()
{
    return lights_;
}



glm::vec3 Scene::getAmbientLight()
{
    return ambientLight_;
}



void Scene::syncLighting(GLuint handle)
{
    GLint ambientLightUniform = glGetUniformLocation(handle, "ambientLight");
    glUniform3fv(ambientLightUniform, 1, glm::value_ptr(ambientLight_));

    for (std::size_t j = 0; j < lights_.size(); j++)
        lights_[j]->sync(handle, j);
}



SnippetPtr Scene::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            #extension GL_ARB_draw_instanced : require
            // ********* VERTEX SHADER ********* \\

            //Scene fields
            attribute vec3 vertex; //position of the vertex
            uniform mat4 viewMatrix, projMatrix; //Camera view and projection matrices
            uniform mat4 modelMatrix; //matrix transforming model mesh into world space
        ).",
        R".(
            //Scene methods
            vec4 projectVertex()
            {
                //https://www.opengl.org/registry/specs/ARB/draw_instanced.txt
                float id = 0.2 * gl_InstanceIDARB;
                mat4 modelMatrix2 = mat4(
                    vec4(1, 0, 0, 0),
                    vec4(0, 1, 0, 0),
                    vec4(0, 0, 1, 0),
                    vec4(id, id, id, 1)
                );
                mat4 MVP = projMatrix * viewMatrix * modelMatrix2; //Calculate the Model-View-Projection matrix
                return MVP * vec4(vertex, 1); // Convert from model space to clip space
            }
        ).",
        R".(
            //Scene main method code
            gl_Position = projectVertex();
        )."
    );
}



SnippetPtr Scene::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            // ********* FRAGMENT SHADER ********* \\

            //Scene fields
            uniform vec3 ambientLight;
            uniform mat4 viewMatrix, projMatrix; //Camera view and projection matrices
            uniform mat4 modelMatrix; //matrix transforming model mesh into world space

            struct Colors
            {
                vec3 material, lightBlend;
            };
        ).",
        R".(
            //Scene methods
        ).",
        R".(
            //Scene main method code
            Colors colors;
            colors.material = vec3(-1); //init to invalid if not needed
            colors.lightBlend = vec3(-1);
        )."
    );
}
