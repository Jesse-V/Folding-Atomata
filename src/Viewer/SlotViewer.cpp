
#include "SlotViewer.hpp"
#include "../Modeling/Shading/ShaderManager.hpp"
#include <memory>
#include <algorithm>
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
    addBondsToScene();
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

    const float SCALE = 0.1f; //todo: don't scale the atoms
    std::transform(vertices.begin(), vertices.end(), vertices.begin(), 
        [&](const glm::vec3& vertex)
        {
            return vertex * SCALE;
        }
    );

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
        glm::vec3(0,            0, 8),
        glm::vec3(1,            0, 8),
        glm::vec3(0.5, sqrt3over2, 8),
    };

    std::vector<GLuint> indices = {
        2, 5, 4, 1,
        3, 5, 2, 0,
        1, 4, 3, 0/*,
        0, 2, 1, 0, //this and next line are the end caps
        3, 4, 5, 3*/
    };

    const float SCALE = 0.05f;
    const glm::vec3 OFFSET = glm::vec3(0.25, sqrt3over4, 0);
    std::transform(vertices.begin(), vertices.end(), vertices.begin(), 
        [&](const glm::vec3& vertex)
        {
            return (vertex - OFFSET) * SCALE;
        }
    );

    auto vBuffer = std::make_shared<VertexBuffer>(vertices);
    auto iBuffer = std::make_shared<IndexBuffer>(indices, GL_QUADS);
    mesh = std::make_shared<Mesh>(vBuffer, iBuffer, GL_QUADS);

    std::cout << "done. Cached the result." << std::endl;
    return mesh;
}



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
        auto model = std::make_shared<Model>(generateAtomMesh());
        //add color here
        auto position = snapshotZero->getPosition(j);
        model->setModelMatrix(glm::translate(glm::mat4(), position));

        addAtom(atoms[j], model);
        atomModels_.push_back(model);
    }
    auto model = std::make_shared<Model>(generateAtomMesh());
    addAtom(atoms[0], model);

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
        auto position = snapshotZero->getPosition(bonds[j]->getAtomA());
        //set rotation
        model->setModelMatrix(glm::translate(glm::mat4(), position));

        addBond(bonds[j], model);
        bondModels_.push_back(model);
    }

    std::cout << "... done adding bonds for that trajectory." << std::endl;

    /*
    glm::vec3 z(0, 0, 1);
    glm::vec3 p = a - b;
    glm::vec3 t = glm::cross(z, p);
    double angle = 180 / 3.1415926 * acos((glm::dot(z, p) / ))*/
    /*
    //http://www.thjsmith.com/40/cylinder-between-two-points-opengl-c
    Vector3D a, b; (the two points you want to draw between)

// This is the default direction for the cylinders to face in OpenGL
Vector3D z = Vector3D(0,0,1);         
// Get diff between two points you want cylinder along
Vector3D p = (a - b);                               
// Get CROSS product (the axis of rotation)
Vector3D t = CROSS_PRODUCT (z , p); 

// Get angle. LENGTH is magnitude of the vector
double angle = 180 / PI * acos ((DOT_PRODUCT(z, p) / p.LENGTH());

glTranslated(b.x,b.y,b.z);
glRotated(angle,t.x,t.y,t.z);

gluQuadricOrientation(YourQuadric,GLU_OUTSIDE);
gluCylinder(YourQuadric, RADIUS, RADIUS, p.LENGTH(), SEGS1, SEGS2);*/
}



void SlotViewer::addAtom(const AtomPtr& atom, const ModelPtr& model)
{
    static std::unordered_map<char, ProgramPtr> programCache;

    auto value = programCache.find(atom->getElement());
    if (value == programCache.end())
    { //not in cache
        std::cout << "Program for element " << atom->getElement()
            << " is not cached. Generating..." << std::endl;
        auto program = ShaderManager::createProgram(model,
            scene_->getVertexShaderGLSL(),
            scene_->getFragmentShaderGLSL(), scene_->getLights()
        );

        std::cout << "... done generating Program for " << 
            atom->getElement() << " type." << std::endl;

        scene_->addModel(model, program, true); //add to Scene and save
        programCache[atom->getElement()] = program; //add to cache
        std::cout << "Saved Model and cached Program." << std::endl;
    }
    else
    { //already in cache
        scene_->addModel(model, value->second, false); //just add, !save
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
