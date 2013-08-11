
#include "Scene.hpp"
#include "Modeling/Shading/ShaderManager.hpp"
#include "Modeling/DataBuffers/NormalBuffer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iostream>


typedef std::unordered_multimap<char, int> MyMap;


Scene::Scene(const std::shared_ptr<Camera>& camera):
    camera_(camera)
{}



void Scene::addModel(const ModelPtr& model)
{
    addModel(model, ShaderManager::createProgram(model,
        getVertexShaderGLSL(), getFragmentShaderGLSL(), lights_)
    );
}



void Scene::addModel(const ModelPtr& model, const ProgramPtr& program, bool save)
{
    if (save)
        model->saveAs(program);
        
    map_.insert(ProgramModelMultimap::value_type(program, model));
}



void Scene::addLight(const std::shared_ptr<Light>& light)
{
    assertModelsContainNormalBuffers();
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



void Scene::sync()
{
    for (auto keyIterator = map_.begin(); keyIterator != map_.end(); 
        keyIterator = map_.equal_range(keyIterator->first).second)
    {
        auto program = keyIterator->first;
        GLuint handle = program->getHandle();

        glUseProgram(handle);
        camera_->sync(handle);
        syncLighting(handle);
    }
}



float Scene::render()
{
    using namespace std::chrono;
    auto start = steady_clock::now();

    sync();

    for_each (map_.begin(), map_.end(),
        [&](const std::pair<ProgramPtr, ModelPtr>& pair)
        {
            auto programHandle = pair.first->getHandle();
            auto model = pair.second;

            model->render(glGetUniformLocation(programHandle, "modelMatrix"));
        }
    );

    auto diff = duration_cast<microseconds>(steady_clock::now() - start).count();
    return diff / 1000.0f;
}



std::shared_ptr<Camera> Scene::getCamera()
{
    return camera_;
}



int Scene::getModelCount()
{
    return (int)map_.size();
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
            // ********* VERTEX SHADER ********* \\

            //Scene fields
            in vec3 vertex; //position of the vertex
            uniform mat4 viewMatrix, projMatrix; //Camera view and projection matrices
            uniform mat4 modelMatrix; //matrix transforming model mesh into world space
        ).",
        R".(
            //Scene methods
            vec4 projectVertex()
            {
                mat4 MVP = projMatrix * viewMatrix * modelMatrix; //Calculate the Model-View-Projection matrix
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
                vec3 textureColor, lightBlend;
            };
        ).",
        R".(
            //Scene methods
        ).",
        R".(
            //Scene main method code
            Colors colors;
            colors.textureColor = vec3(-1); //init to invalid if not needed
            colors.lightBlend = vec3(-1);
        )."
    );
}


void Scene::assertModelsContainNormalBuffers()
{/*
    std::vector<glm::vec3> emptyVec;
    auto normBuffer = std::make_shared<NormalBuffer>(emptyVec);
    for_each (models_.begin(), models_.end(),
        [&](std::shared_ptr<Model>& model)
        {
            auto optionalBuffers = model->getOptionalDataBuffers();
            bool containsNormalBuffer = false;
            for_each (optionalBuffers.begin(), optionalBuffers.end(),
                [&](std::shared_ptr<OptionalDataBuffer>& buffer)
                {
                    if (typeid(*buffer) == typeid(*normBuffer))
                        containsNormalBuffer = true;
                }
            );

            if (!containsNormalBuffer)
            {
                std::stringstream stream;
                stream << "Light added to Scene, yet ";
                stream << typeid(*model).name();
                stream << " does not contain a NormalBuffer!";

                throw std::runtime_error(stream.str());
            }
        }
    );*/
}
