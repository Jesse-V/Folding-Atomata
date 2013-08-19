
#include "SlotViewer.hpp"
#include "../Modeling/Shading/ShaderManager.hpp"
#include "../Modeling/DataBuffers/ColorBuffer.hpp"
#include <memory>
#include <algorithm>
#include <cmath>
#include <iostream>


SlotViewer::SlotViewer(const TrajectoryPtr& trajectory, 
                       const std::shared_ptr<Scene>& scene) :
    trajectory_(trajectory), scene_(scene)
{
    if (trajectory_->countSnapshots() == 0)
        throw std::runtime_error("No snapshots to work with!");

    std::cout << std::endl;
    addAtomsToScene();
    std::cout << std::endl;
    //addBondsToScene();
    std::cout << std::endl;
}



std::shared_ptr<Mesh> SlotViewer::generateAtomMesh()
{
    static std::shared_ptr<Mesh> mesh = nullptr;

    if (mesh)
        return mesh;

    std::cout << "Generating atom mesh... ";

    std::vector<glm::vec3> vertices = {
        glm::vec3(-1, -1, -1),
        glm::vec3(-1, -1,  1),
        glm::vec3(-1,  1, -1),
        glm::vec3(-1,  1,  1),
        glm::vec3( 1, -1, -1),
        glm::vec3( 1, -1,  1),
        glm::vec3( 1,  1, -1),
        glm::vec3( 1,  1,  1)
    };

    std::vector<GLuint> indices = {
        4, 5, 1, 0, //front
        2, 3, 7, 6, //back
        6, 4, 0, 2, //top
        3, 1, 5, 7, //bottom
        0, 1, 3, 2, //left
        6, 7, 5, 4  //right
    };

    auto vBuffer = std::make_shared<VertexBuffer>(vertices);
    auto iBuffer = std::make_shared<IndexBuffer>(indices, GL_QUADS);
    mesh = std::make_shared<Mesh>(vBuffer, iBuffer, GL_QUADS);

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
    float sqrt3over2 = 0.86602540378f;
    float sqrt3over4 = sqrt3over2 / 2;
    std::vector<glm::vec3> vertices = {
        glm::vec3(0,            0, 0),
        glm::vec3(1,            0, 0),
        glm::vec3(0.5, sqrt3over2, 0),
        glm::vec3(0,            0, 1),
        glm::vec3(1,            0, 1),
        glm::vec3(0.5, sqrt3over2, 1),
    };

    std::vector<GLuint> indices = {
        2, 5, 4, 1,
        3, 5, 2, 0,
        1, 4, 3, 0,
        0, 2, 1, 0, //this and next line are the end caps
        3, 4, 5, 3
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


#include "../Modeling/DataBuffers/NormalBuffer.hpp"
void SlotViewer::addAtomsToScene()
{
    auto atoms = trajectory_->getTopology()->getAtoms();
    atomModels_.reserve(atoms.size());
    std::cout << "Trajectory consists of " << atoms.size() 
        << " atoms." << std::endl;

    std::cout << "Adding Atoms to Scene..." << std::endl;
    auto snapshotZero = trajectory_->getSnapshot(0);
    for (std::size_t j = 0; j < atoms.size(); j++)
    {
        //if (atoms[j]->getElement() == 'H')
        {
            auto position = snapshotZero->getPosition((int)j);
            auto matrix = glm::translate(glm::mat4(), position);
            matrix      = glm::scale(matrix, glm::vec3(0.1f));
            auto model = addAtom(atoms[j], matrix);
            atomModels_.push_back(model);
        }
        
    }

    std::cout << "... done adding atoms for that trajectory." << std::endl;
}



void SlotViewer::addBondsToScene()
{
    auto bonds = trajectory_->getTopology()->getBonds();
    bondModels_.reserve(bonds.size());
    std::cout << "Trajectory consists of " << bonds.size() 
        << " bonds." << std::endl;

    std::cout << "Adding Bonds to Scene..." << std::endl;
    auto snapshotZero = trajectory_->getSnapshot(0);
    for (std::size_t j = 0; j < bonds.size(); j++)
    {
        auto model = std::make_shared<Model>(generateBondMesh());
        auto positionA = snapshotZero->getPosition(bonds[j]->getAtomA());
        auto positionB = snapshotZero->getPosition(bonds[j]->getAtomB());
        float distance = getMagnitude(positionA - positionB);

        auto matrix = alignBetween(positionA, positionB);
        matrix = glm::scale(matrix, glm::vec3(0.01f, 0.01f, 2));
        model->setModelMatrix(matrix);

        addBond(bonds[j], model);
        bondModels_.push_back(model);
    }

    std::cout << "... done adding bonds for that trajectory." << std::endl;
}



ModelPtr SlotViewer::addAtom(const AtomPtr& atom, const glm::mat4& matrix)
{
    static std::unordered_map<char, std::pair<ProgramPtr, BufferList>> programCache;

    auto value = programCache.find(atom->getElement());
    if (value == programCache.end())
    { //not in cache
        std::cout << "Program for element " << atom->getElement()
            << " is not cached. Generating..." << std::endl;

        auto color = atom->getColor();
        std::cout << color.x << ", " << color.y << ", " << color.z << std::endl;
        BufferList list = { std::make_shared<ColorBuffer>(color, 8) };
        auto model = std::make_shared<Model>(generateAtomMesh(), list);
        model->setModelMatrix(matrix);

        auto program = ShaderManager::createProgram(model,
            scene_->getVertexShaderGLSL(),
            scene_->getFragmentShaderGLSL(), scene_->getLights()
        );

        std::cout << "... done generating Program for " << 
            atom->getElement() << " type." << std::endl;

        scene_->addModel(model, program, true); //add to Scene and save
        programCache[atom->getElement()] = std::make_pair(program, list); //add to cache
        std::cout << "Saved Model and cached Program." << std::endl;
        return model;
    }
    else
    { //already in cache
        auto model = std::make_shared<Model>(generateAtomMesh(), value->second.second);
        model->setModelMatrix(matrix);
        scene_->addModel(model, value->second.first, false); //just add, !save
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



SlotViewer::~SlotViewer()
{
    std::cout << "SlotViewer destructing..." << std::endl;
    std::cout.flush();
}



void SlotViewer::update()
{

}



glm::mat4 SlotViewer::alignBetween(const glm::vec3& ptA, const glm::vec3& ptB)
{ //adapted from http://www.thjsmith.com/40/cylinder-between-two-points-opengl-c
    
    glm::vec3 z(0, 0, 1);
    glm::vec3 p = ptB - ptA;

    float radians = (float)acos(getDotProduct(z, p) / getMagnitude(p));
    double angle = 180 / 3.1415926 * radians;

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
