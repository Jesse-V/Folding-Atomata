
#include "SlotViewer.hpp"
#include "../Modeling/Shading/ShaderManager.hpp"
#include <memory>
#include <algorithm>
#include <iostream>


SlotViewer::SlotViewer(const TrajectoryPtr& trajectory, 
                       const std::shared_ptr<Scene>& scene) :
    trajectory_(trajectory), scene_(scene)
{
    addAtomsToScene();
    addBondsToScene();
}



std::shared_ptr<Mesh> SlotViewer::generateAtomMesh()
{
    static std::shared_ptr<Mesh> mesh = nullptr;

    if (mesh)
        return mesh;

    std::cout << "Generating atom mesh... ";

    std::vector<glm::vec3> vertices = {
            glm::vec3(-0.5, -0.5, -0.5),
            glm::vec3(-0.5, -0.5,  0.5),
            glm::vec3(-0.5,  0.5, -0.5),
            glm::vec3(-0.5,  0.5,  0.5),
            glm::vec3( 0.5, -0.5, -0.5),
            glm::vec3( 0.5, -0.5,  0.5),
            glm::vec3( 0.5,  0.5, -0.5),
            glm::vec3( 0.5,  0.5,  0.5)
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

    std::cout << "done." << std::endl;
    return mesh;
}



void SlotViewer::addAtomsToScene()
{
    auto atoms = trajectory_->getTopology()->getAtoms();
    std::cout << "Trajectory consists of " << atoms.size() 
        << " atoms." << std::endl;

    if (trajectory_->countSnapshots() == 0)
        throw std::runtime_error("No snapshots to work with!");

    std::cout << std::endl << "Adding Atoms to Scene..." << std::endl;
    auto snapshotZero = trajectory_->getSnapshot(0);
    auto mesh = generateAtomMesh();
    for (int j = 0; j < atoms.size(); j++)
    {
        auto model = std::make_shared<Model>(mesh);
        //add color here
        auto position = snapshotZero->getPosition(j);
        model->setModelMatrix(glm::translate(glm::mat4(), position));

        const AtomPtr atom = atoms[j];
        auto value = programCache_.find(atom->getElement());
        if (value == programCache_.end())
        { //not in cache
            auto program = ShaderManager::createProgram(model,
                scene_->getVertexShaderGLSL(),
                scene_->getFragmentShaderGLSL(), scene_->getLights()
            );

            std::cout << "Done generating Program for " << 
                atom->getElement() << " type." << std::endl;

            scene_->addModel(model, program, true); //add to Scene and save
            programCache_[atom->getElement()] = program; //add to cache
            std::cout << "Saved Model and cached Program." << std::endl;
        }
        else
        { //already in cache
            scene_->addModel(model, value->second, false); //just add, !save
        }
    }

    std::cout << "... done adding atoms for that trajectory.\n" << std::endl;
}



void SlotViewer::addBondsToScene()
{
    
}



SlotViewer::~SlotViewer()
{
    std::cout << "SlotViewer destructing..." << std::endl;
    std::cout.flush();
}



void SlotViewer::update()
{

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
