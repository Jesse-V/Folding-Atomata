
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

#include "ShaderManager.hpp"
#include "Program.hpp"
#include <sstream>
#include <algorithm>
#include <thread>
#include <iostream>


ProgramPtr ShaderManager::createProgram(
    const std::shared_ptr<Model>& model, const SnippetPtr& sceneVertexShader,
    const SnippetPtr& sceneFragmentShader, const LightList& lights
)
{
    std::cout << "Creating vertex and fragment shaders for Model"
        << " with " << lights.size() << " light(s)... ";

    auto buffers = model->getOptionalDataBuffers();
    auto vertexShaderStr = assembleVertexShaderStr(buffers,
                                              sceneVertexShader, lights);
    auto fragmentShaderStr = assembleFragmentShaderStr(buffers,
                                              sceneFragmentShader, lights);

    std::cout << "done." << std::endl;
    return cs5400::makeProgram(
        cs5400::makeVertexShaderStr(vertexShaderStr),
        cs5400::makeFragmentShaderStr(fragmentShaderStr)
    );
}



std::string ShaderManager::assembleVertexShaderStr(
    const BufferList& buffers, const SnippetPtr& sceneVertexShader,
    const LightList& lights
)
{
    auto vertexSnippets = assembleVertexSnippets(sceneVertexShader,
                                                 buffers, lights);
    return buildShader(
        assembleFields(vertexSnippets),
        assembleMethods(vertexSnippets),
        assembleMainBodyCode(vertexSnippets)
    );
}



std::string ShaderManager::assembleFragmentShaderStr(
    const BufferList& buffers, const SnippetPtr& sceneFragmentShader,
    const LightList& lights
)
{
    auto fragmentSnippets = assembleFragmentSnippets(sceneFragmentShader,
                                                     buffers, lights);
    return buildShader(
        assembleFields(fragmentSnippets),
        assembleMethods(fragmentSnippets),
        assembleMainBodyCode(fragmentSnippets) + R".(
            //final fragment shader main body code from ShaderManager
            if (colors.material == vec3(-1))
                colors.material = vec3(1);

            if (colors.lightBlend == vec3(-1))
                colors.lightBlend = vec3(0);

            vec3 lighting = ambientLight + colors.lightBlend;
            vec3 color = colors.material * lighting;

            gl_FragColor = vec4(color, 1);
        )."
    );
}



std::vector<SnippetPtr> ShaderManager::assembleVertexSnippets(
    const SnippetPtr& sceneVertexShader, const BufferList& buffers,
    const LightList& lights
)
{
    std::vector<SnippetPtr> vertexSnippets;
    vertexSnippets.reserve(1 + buffers.size() + lights.size());
    vertexSnippets.push_back(sceneVertexShader);

    for_each (buffers.begin(), buffers.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            vertexSnippets.push_back(buffer->getVertexShaderGLSL());
        }
    );

    if (lights.size() > 0) //only need one instance of light code
        vertexSnippets.push_back(lights[0]->getVertexShaderGLSL());

    return vertexSnippets;
}



std::vector<SnippetPtr> ShaderManager::assembleFragmentSnippets(
    const SnippetPtr& sceneFragmentShader, const BufferList& buffers,
    const LightList& lights
)
{
    std::vector<SnippetPtr> fragmentSnippets;
    fragmentSnippets.reserve(1 + buffers.size() + lights.size());
    fragmentSnippets.push_back(sceneFragmentShader);

    for_each (buffers.begin(), buffers.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            fragmentSnippets.push_back(buffer->getFragmentShaderGLSL());
        }
    );

    if (lights.size() > 0) //only need one instance of light code
        fragmentSnippets.push_back(lights[0]->getFragmentShaderGLSL());

    return fragmentSnippets;
}



std::string ShaderManager::assembleFields(const SnippetList& snippets)
{
    std::stringstream stream("");

    for_each (snippets.begin(), snippets.end(),
        [&](const SnippetPtr& snippet)
        {
            stream << snippet->getFields();
        }
    );

    return stream.str();
}



std::string ShaderManager::assembleMethods(const SnippetList& snippets)
{
    std::stringstream stream("");

    for_each (snippets.begin(), snippets.end(),
        [&](const SnippetPtr& snippet)
        {
            stream << snippet->getMethods();
        }
    );

    return stream.str();
}



std::string ShaderManager::assembleMainBodyCode(const SnippetList& snippets)
{
    std::stringstream stream("");

    for_each (snippets.begin(), snippets.end(),
        [&](const SnippetPtr& snippet)
        {
            stream << snippet->getMainBodyCode();
        }
    );

    return stream.str();
}



std::string ShaderManager::buildShader(const std::string& fields,
    const std::string& methods, const std::string& mainBodyCode
)
{
    //return fields + methods + mainBodyCode;
    return R".(
            #version 120
        )."
        + fields
        + "\n"
        + methods
        + R".(
            void main()
            {
        )."
        + mainBodyCode
        + R".(
            }
        ).";
}
