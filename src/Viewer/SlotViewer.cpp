
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

#include "SlotViewer.hpp"
#include "Modeling/Shading/ShaderManager.hpp"
#include "Trajectory/ProteinAnalysis.hpp"
#include "Options.hpp"
#include <algorithm>
#include <thread>
#include <iostream>


SlotViewer::SlotViewer(const TrajectoryPtr& trajectory,
                       const std::shared_ptr<Scene>& scene) :
    ATOM_STACKS(Options::getInstance().getAtomStacks()),
    ATOM_SLICES(Options::getInstance().getAtomSlices()),
    trajectory_(trajectory), scene_(scene),
    snapshotA_(0), snapshotB_(1)
{
    std::cout << "... done creating SlotViewer." << std::endl;
}



void SlotViewer::update(int deltaTime)
{

}



std::shared_ptr<Mesh> SlotViewer::getAtomMesh()
{
    static std::shared_ptr<Mesh> mesh = nullptr;

    if (mesh)
        return mesh;

    std::cout << "Generating atom mesh... ";

    //adapted from sandy_bence's 2005 post over at:
    //http://www.gamedev.net/topic/350823-rendering-a-sphere-using-triangle-strips/

    std::vector<glm::vec3> vertices;
    for (unsigned int stack = 0; stack <= ATOM_STACKS; stack++)
    {
        for (unsigned int slice = 0; slice < ATOM_SLICES; slice++)
        {
            float theta = stack * PI / ATOM_STACKS;
            float phi   = slice * 2 * PI / ATOM_SLICES;

            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            vertices.push_back(glm::vec3(
                cosPhi * sinTheta,
                sinPhi * sinTheta,
                cosTheta
            ));
        }
    }

    std::vector<GLuint> indices;
    for (unsigned int stack = 0; stack < ATOM_STACKS; stack++)
    {
        for (unsigned int slice = 0; slice <= ATOM_SLICES; slice++)
        {
            auto sliceMod = slice % ATOM_SLICES;
            indices.push_back((stack       * ATOM_SLICES) + sliceMod);
            indices.push_back(((stack + 1) * ATOM_SLICES) + sliceMod);
        }
    }

    auto vBuffer = std::make_shared<VertexBuffer>(vertices);
    auto iBuffer = std::make_shared<IndexBuffer>(indices, GL_TRIANGLE_STRIP);
    mesh = std::make_shared<Mesh>(vBuffer, iBuffer, GL_TRIANGLE_STRIP);

    std::cout << "done. Cached the result." << std::endl;
    return mesh;
}



std::shared_ptr<Mesh> SlotViewer::getBondMesh()
{
    static std::shared_ptr<Mesh> mesh = nullptr;

    if (mesh)
        return mesh;

    std::cout << "Generating bond mesh... ";

    // http://in.answers.yahoo.com/question/index?qid=20060907224537AA8MBBH
    const float sqrt3over2 = 0.86602540378f;
    const float sqrt3over4 = sqrt3over2 / 2;
    std::vector<glm::vec3> vertices = {
        glm::vec3(0,            0, 0),
        glm::vec3(1,            0, 0),
        glm::vec3(0.5, sqrt3over2, 0),
        glm::vec3(0,            0, 1),
        glm::vec3(1,            0, 1),
        glm::vec3(0.5, sqrt3over2, 1)
    };

    std::vector<GLuint> indices = {
        2, 5, 4, 1,
        3, 5, 2, 0,
        1, 4, 3, 0,
        0, 2, 1, 0/*, //this and next line are the end caps
        3, 4, 5, 3*/
    };

    const glm::vec3 OFFSET = glm::vec3(0.25, sqrt3over4, 0);
    std::transform(vertices.begin(), vertices.end(), vertices.begin(),
        [&](const glm::vec3& vertex)
        {
            return vertex - OFFSET;
        }
    );

    auto vBuffer = std::make_shared<VertexBuffer>(vertices);
    auto iBuffer = std::make_shared<IndexBuffer>(indices, GL_QUADS);
    mesh = std::make_shared<Mesh>(vBuffer, iBuffer, GL_QUADS);

    std::cout << "done. Cached the result." << std::endl;
    return mesh;
}



glm::mat4 SlotViewer::generateAtomMatrix(const glm::vec3& position, const AtomPtr& atom)
{
    auto matrix = glm::translate(glm::mat4(), position);
    auto shellCount = glm::vec3(atom->getElectronShellCount());
    return glm::scale(matrix, glm::vec3(ATOM_SCALE) * shellCount);
}



glm::mat4 SlotViewer::generateBondMatrix(const glm::vec3& startPosition,
                                         const glm::vec3& endPosition
)
{
    float distance = getMagnitude(startPosition - endPosition);
    if (distance == 0.0f)
        return glm::scale(glm::mat4(), glm::vec3(glm::vec2(BOND_SCALE), FLT_EPSILON));

    auto matrix = alignBetween(startPosition, endPosition);
    return glm::scale(matrix, glm::vec3(glm::vec2(BOND_SCALE), distance));
}



glm::mat4 SlotViewer::alignBetween(const glm::vec3& ptA, const glm::vec3& ptB)
{ //adapted from http://www.thjsmith.com/40/cylinder-between-two-points-opengl-c

    glm::vec3 z(0, 0, 1);
    glm::vec3 p = ptB - ptA;

    float radians = (float)std::acos(getDotProduct(z, p) / getMagnitude(p));
    float angle = 180 / 3.1415926f * radians;

    auto translated = glm::translate(glm::mat4(), ptA);
    return glm::rotate(translated, (float)angle, glm::cross(z, p));
}



float SlotViewer::getDotProduct(const glm::vec3& vecA, const glm::vec3& vecB)
{
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}



float SlotViewer::getMagnitude(const glm::vec3& vector)
{
    return (float)std::sqrt(getDotProduct(vector, vector));
}



/* save this for https://github.com/Jesse-V/Folding-Atomata/issues/16
    std::thread snapshotAdder([]() {
        try
        {
            std::cout << "New thread for " << idStream.str() <<
                ", awaiting incoming snapshots... " << std::endl;

            TrajectoryParser parser;
            while (true)
            {
                std::string positionsStr = readResponse(*socket_);
                std::cout << idStream.str() << "'s thread read new snapshot. Adding... ";
                addSnapshot(parser.parseSnapshot(positionsStr));
                std::cout << "done." << std::endl;

                std::chrono::milliseconds duration(5000);
                std::this_thread::sleep_for(duration);
            }
        }
    catch (SocketException& e)
    {
        std::cout << "In SlotViewer's thread: " << e.description() << "\n";
    }
    });
    snapshotAdder.detach(); //run thread detached from main execution
    */
