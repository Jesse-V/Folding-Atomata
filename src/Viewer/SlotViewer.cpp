
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
    scene_(scene), trajectory_(trajectory),
    snapshotIndexA_(0), snapshotIndexB_(1)
{
    std::cout << std::endl;

    if (trajectory_->countSnapshots() == 0)
        throw std::runtime_error("No snapshots to work with!");
    else
        std::cout << "Creating viewer for trajectory with " <<
            trajectory_->countSnapshots() << " snapshots..." << std::endl;

    const auto RENDER_MODE = Options::getInstance().getRenderMode();
    if (RENDER_MODE == Options::RenderMode::BALL_N_STICK)
    {
        addAllAtoms();
        std::cout << std::endl;
    }

    addAllBonds();
    std::cout << std::endl;

    /*std::thread thread( [&] {
        ProteinAnalysis proteinAnalysis(trajectory_);
        proteinAnalysis.fixProteinSplits();
    });
    thread.detach();*/

    std::cout << "... done creating SlotViewer." << std::endl;
}



void SlotViewer::addAllAtoms()
{
    const auto ATOMS = trajectory_->getTopology()->getAtoms();

    std::cout << "Trajectory consists of " << ATOMS.size()
        << " atoms." << std::endl;
    std::cout << "Adding Atoms to Scene..." << std::endl;

    auto snapshotZero = trajectory_->getSnapshot(0);
    typedef std::pair<std::size_t, InstancedModelPtr> Instance;
    std::unordered_map<char, Instance> elementMap;
    elementMap.reserve(8);
    for (std::size_t j = 0; j < ATOMS.size(); j++)
    {
        auto atom = ATOMS[j];
        auto matrix = generateAtomMatrix(snapshotZero->getPosition(j), atom);
        auto element = atom->getElement();

        if (elementMap.find(element) == elementMap.end()) //not in cache
        {
            std::cout << "Generating model type " << element << "..." << std::endl;
            auto model = generateAtomModel(atom, matrix);
            scene_->addModel(model);
            elementMap[element] = std::make_pair(atomInstances_.size(), model);
            elementIndexes_.push_back(ElementIndex(atomInstances_.size(), 0));
            atomInstances_.push_back(model);
            std::cout << "... done generating data for " << element << std::endl;
        }
        else //already in cache
        {
            elementMap[element].second->addInstance(matrix);
            elementIndexes_.push_back(ElementIndex(elementMap[element].first,
                elementMap[element].second->getInstanceCount()));
        }
    }

    std::cout << "... done adding atoms for that trajectory." << std::endl;
}



void SlotViewer::addAllBonds()
{
    const auto BONDS = trajectory_->getTopology()->getBonds();

    std::cout << "Trajectory consists of " << BONDS.size()
        << " bonds." << std::endl;
    std::cout << "Adding Bonds to Scene..." << std::endl;

    BufferList list = { std::make_shared<ColorBuffer>(BOND_COLOR, 6) };
    bondInstance_ = std::make_shared<InstancedModel>(getBondMesh(), list);

    auto snapshotZero = trajectory_->getSnapshot(0);
    for (auto bond : BONDS)
    {
        auto positionA = snapshotZero->getPosition(bond.first);
        auto positionB = snapshotZero->getPosition(bond.second);
        bondInstance_->addInstance(generateBondMatrix(positionA, positionB));
    }

    scene_->addModel(bondInstance_);
    std::cout << "... done adding bonds for that trajectory." << std::endl;
}



bool SlotViewer::animate(int deltaTime)
{
    if (trajectory_->countSnapshots() <= 1)
        return false; //can't animate with one snapshot

    if (atomInstances_.size() == 0 && bondInstance_->getInstanceCount() == 0)
        return false; //we have nothing to animate

    int b = updateSnapshotIndexes(deltaTime);
    auto newPositions = animateAtoms(b);
    animateBonds(newPositions);

    return true;
}



int SlotViewer::updateSnapshotIndexes(int deltaTime)
{
    transitionTime_ += deltaTime;

    int a = transitionTime_ / ANIMATION_SPEED; //int division on purpose
    int b = transitionTime_ % ANIMATION_SPEED;
    transitionTime_ = b;

    int snapshotCount = trajectory_->countSnapshots();
    if (snapshotCount > 2 && a > 0)
    {
        if (Options::getInstance().bounceAnimation())
        { //FAHViewer-like bouncing animation
            if (snapshotIndexA_ < snapshotIndexB_)
            { //going forward
                snapshotIndexA_ = snapshotIndexA_ + a;
                snapshotIndexB_ = snapshotIndexA_ + 1;
                if (snapshotIndexB_ == snapshotCount)
                    snapshotIndexB_ -= 2;
            }
            else
            { //going backwards
                snapshotIndexA_ = snapshotIndexA_ - a;
                snapshotIndexB_ = snapshotIndexA_ - 1;
                if (snapshotIndexB_ == -1)
                    snapshotIndexB_ += 2;
            }
        }
        else
        { //default jump-to-first-snapshot animation
            snapshotIndexA_ = (snapshotIndexA_ + a) % (snapshotCount - 1);
            snapshotIndexB_ = snapshotIndexA_ + 1;
        }
    }

    return b;
}



std::vector<glm::vec3> SlotViewer::animateAtoms(int b)
{
    auto snapA = trajectory_->getSnapshot(snapshotIndexA_);
    auto snapB = trajectory_->getSnapshot(snapshotIndexB_);

    const auto atoms = trajectory_->getTopology()->getAtoms();
    std::vector<glm::vec3> newPositions;
    newPositions.reserve(atoms.size());

    for (std::size_t j = 0; j < atoms.size(); j++)
    {
        auto startPosition = snapA->getPosition(j);
        auto endPosition   = snapB->getPosition(j);
        auto position = (endPosition - startPosition) *
                                   (b / (float)ANIMATION_SPEED) + startPosition;

        newPositions.push_back(position);
        const ElementIndex index = elementIndexes_[j];
        if (atomInstances_.size() > 0)
        {
            atomInstances_[index.elementIndex]->setModelMatrix(
                index.instanceIndex, generateAtomMatrix(position, atoms[j]));
        }
    }

    return newPositions;
}



void SlotViewer::animateBonds(const std::vector<glm::vec3>& atomPositions)
{
    const auto bonds = trajectory_->getTopology()->getBonds();
    for (std::size_t j = 0; j < bonds.size(); j++)
    {
        auto positionA = atomPositions[bonds[j].first];
        auto positionB = atomPositions[bonds[j].second];
        bondInstance_->setModelMatrix(j, generateBondMatrix(positionA, positionB));
    }
}



std::shared_ptr<ColorBuffer> SlotViewer::generateColorBuffer(const AtomPtr& atom)
{
    static auto N_VERTICES = (ATOM_STACKS + 1) * ATOM_SLICES;
    std::vector<glm::vec3> colors(N_VERTICES, atom->getColor());

    auto vertices = getAtomMesh()->getVertices();
    for (std::size_t j = 0; j < N_VERTICES; j++)
    {
        float distance = getMagnitude(vertices[j] - ATOM_LIGHT_POSITION);
        float scaledDistance = distance / ATOM_LIGHT_POWER;
        glm::vec3 luminosity = ATOM_LIGHT_COLOR * (1 - scaledDistance);

        if (luminosity.x > 0 && luminosity.y > 0 && luminosity.z > 0)
            colors[j] += luminosity;
    }

    return std::make_shared<ColorBuffer>(colors);
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



InstancedModelPtr SlotViewer::generateAtomModel(const AtomPtr& atom,
                                                const glm::mat4& matrix)
{
    BufferList list = { generateColorBuffer(atom) };
    return std::make_shared<InstancedModel>(getAtomMesh(), matrix, list);
}



glm::mat4 SlotViewer::generateAtomMatrix(const glm::vec3& position,
                                         const AtomPtr& atom)
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
    if (distance <= FLT_EPSILON)
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
