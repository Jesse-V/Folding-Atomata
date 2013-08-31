
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

#include "SlotViewer.hpp"
#include "../Modeling/Shading/ShaderManager.hpp"
#include "../Modeling/DataBuffers/ColorBuffer.hpp"
#include <memory>
#include <algorithm>
#include <cmath>
#include <iostream>


SlotViewer::SlotViewer(const TrajectoryPtr& trajectory, 
                       const std::shared_ptr<Scene>& scene) :
    trajectory_(trajectory), scene_(scene),
    snapshotA_(0), snapshotB_(1)
{
    std::cout << std::endl;

    if (trajectory_->countSnapshots() == 0)
        throw std::runtime_error("No snapshots to work with!");
    else
        std::cout << "Creating viewer for trajectory with " <<
            trajectory_->countSnapshots() << " snapshots..." << std::endl;

    addAllAtoms();
    std::cout << std::endl;
    addAllBonds();
    std::cout << std::endl;

    std::cout << "... done creating SlotViewer." << std::endl;
}



std::shared_ptr<Mesh> SlotViewer::generateAtomMesh()
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



std::shared_ptr<Mesh> SlotViewer::generateBondMesh()
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



void SlotViewer::addAllAtoms()
{
    auto atoms = trajectory_->getTopology()->getAtoms();
    atomModels_.reserve(atoms.size());

    std::cout << "Trajectory consists of " << atoms.size() 
        << " atoms." << std::endl;
    std::cout << "Adding Atoms to Scene..." << std::endl;

    auto snapshotZero = trajectory_->getSnapshot(0);
    for (std::size_t j = 0; j < atoms.size(); j++)
    {
        auto matrix = generateAtomMatrix(snapshotZero->getPosition((int)j));        
        auto atomModel = addAtom(atoms[j], matrix);
        atomModels_.push_back(atomModel);
    }

    std::cout << "... done adding atoms for that trajectory." << std::endl;
}



void SlotViewer::addAllBonds()
{
    auto bonds = trajectory_->getTopology()->getBonds();
    bondModels_.reserve(bonds.size());

    std::cout << "Trajectory consists of " << bonds.size() 
        << " bonds." << std::endl;
    std::cout << "Adding Bonds to Scene..." << std::endl;

    auto snapshotZero = trajectory_->getSnapshot(0);
    BufferList list = { std::make_shared<ColorBuffer>(BOND_COLOR, 6) };
    for (std::size_t j = 0; j < bonds.size(); j++)
    {
        auto model = std::make_shared<Model>(generateBondMesh(), list);
        auto positionA = snapshotZero->getPosition(bonds[j]->getAtomA());
        auto positionB = snapshotZero->getPosition(bonds[j]->getAtomB());
        float distance = getMagnitude(positionA - positionB);

        auto matrix = alignBetween(positionA, positionB);
        matrix = glm::scale(matrix, glm::vec3(glm::vec2(BOND_SCALE), distance));
        model->setModelMatrix(matrix);

        addBond(bonds[j], model);
        bondModels_.push_back(model);
    }

    std::cout << "... done adding bonds for that trajectory." << std::endl;
}



ModelPtr SlotViewer::addAtom(const AtomPtr& atom, const glm::mat4& matrix)
{
    static std::unordered_map<char, AtomModelInfo> cache;
    static auto N_VERTICES = (ATOM_STACKS + 1) * ATOM_SLICES;

    auto value = cache.find(atom->getElement());
    if (value == cache.end())
    { //not in cache
        std::cout << "Program for element " << atom->getElement()
            << " is not cached. Generating..." << std::endl;

        auto cBuffer = std::make_shared<ColorBuffer>(atom->getColor(), N_VERTICES);
        auto model = generateAtomModel(cBuffer, matrix);
        auto program = ShaderManager::createProgram(model,
            scene_->getVertexShaderGLSL(),
            scene_->getFragmentShaderGLSL(), scene_->getLights()
        );

        std::cout << "... done generating Program for " << 
            atom->getElement() << " type." << std::endl;

        scene_->addModel(model, program, true); //add to Scene and save
        cache[atom->getElement()] = std::make_pair(program, cBuffer);

        std::cout << "Saved Model and cached Program." << std::endl;
        return model;
    }
    else
    { //already in cache
        auto model = generateAtomModel(value->second.second, matrix);
        scene_->addModel(model, value->second.first, false); //don't save
        return model;
    }
}



void SlotViewer::addBond(const BondPtr& bond, const ModelPtr& model)
{
    static ProgramPtr bondProgram = nullptr;

    if (!bondProgram)
    {
        std::cout << "Bond Program is not cached. Generating..." << std::endl;
        bondProgram = ShaderManager::createProgram(model,
            scene_->getVertexShaderGLSL(),
            scene_->getFragmentShaderGLSL(), scene_->getLights()
        );

        std::cout << "... done generating Bond Program." << std::endl;

        scene_->addModel(model, bondProgram, true); //add to Scene and save
        std::cout << "Saved Model and cached Bond Program." << std::endl;
    }
    else
        scene_->addModel(model, bondProgram, false); //just add, !save
}



ModelPtr SlotViewer::generateAtomModel(const ColorPtr& cBuffer,
                                       const glm::mat4& matrix)
{
    BufferList list = { cBuffer };
    auto model = std::make_shared<Model>(generateAtomMesh(), list);
    model->setModelMatrix(matrix);
    return model;
}



glm::mat4 SlotViewer::generateAtomMatrix(const glm::vec3& position)
{
    auto matrix = glm::translate(glm::mat4(), position);
    return glm::scale(matrix, glm::vec3(ATOM_SCALE));
}



void SlotViewer::update(int deltaTime)
{
    const int snapshotCount = trajectory_->countSnapshots();

    if (snapshotCount <= 1)
        return; //can't animate as we don't have enough snapshots

    transitionTime_ += deltaTime;
    int a = transitionTime_ / 2000; //int division on purpose
    int b = transitionTime_ % 2000;
    transitionTime_ = b;

    if (snapshotCount > 2)
    {
        snapshotA_ = (snapshotA_ + a) % (snapshotCount - 2);
        snapshotB_ = snapshotA_ + 1;
    }

    auto snapA = trajectory_->getSnapshot(snapshotA_);
    auto snapB = trajectory_->getSnapshot(snapshotB_);
    for (std::size_t j = 0; j < atomModels_.size(); j++)
    {
        auto startPosition = snapA->getPosition((int)j);
        auto endPosition   = snapB->getPosition((int)j);
        auto position = (endPosition - startPosition) * (b / 2000.0f) + startPosition;

        /*std::cout << j << "," << (b / 2000.0f) << std::endl;
        std::cout << startPosition.x << "," << startPosition.y << "," << startPosition.z << std::endl;
        std::cout << endPosition.x << "," << endPosition.y << "," << endPosition.z << std::endl;
        std::cout << "  " << position.x << "," << position.y << "," << position.z << std::endl;*/

        atomModels_[j]->setModelMatrix(generateAtomMatrix(position));
    }
}



glm::mat4 SlotViewer::alignBetween(const glm::vec3& ptA, const glm::vec3& ptB)
{ //adapted from http://www.thjsmith.com/40/cylinder-between-two-points-opengl-c
    
    glm::vec3 z(0, 0, 1);
    glm::vec3 p = ptB - ptA;

    float radians = (float)acos(getDotProduct(z, p) / getMagnitude(p));
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
    return (float)sqrt(getDotProduct(vector, vector));
}




/*
    2 seconds between snapshots. When the accumulated time has exceeded this,
    calculate where we should be between the next pair
*/




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
