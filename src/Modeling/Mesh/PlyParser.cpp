
#include "PlyParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <stdexcept>


// **************  Public functions:  **************

/*  Given the address to the PLY file,
    this function loads, parses, and assembles the vertices and triangle data.
    Nothing is returned, as the vertices and triangle are remembered internally.
    Use getVertices() and getTriangles(), respectively, to return that data.
*/
MeshPtr PlyParser::getMesh(const std::string& fileName)
{
    VertexBufferPtr vertices;
    IndexCollection indices;

    std::cout << "Parsing " << fileName << " ... ";
    auto pieces = seperatePly(readFile(fileName));

    //use threading to parallelize the parsing
    std::thread t1([&]() {
        vertices = parseVertices(pieces[1]);
    });

    std::thread t2( [&]() {
        indices = parseIndices(pieces[2]);
    });

    t1.join(); //wait for both threads to complete
    t2.join();

    std::cout << "done (";

    if (indices.second == GL_TRIANGLES)
        std::cout << "Triangles";
    else if (indices.second == GL_QUADS)
        std::cout << "Quads";

    std::cout << ")" << std::endl;

    return std::make_shared<Mesh>(vertices, indices.first, indices.second);
}



// **************  Private helper functions:  **************

/*  Reads the given file and returns the contents as a String */
std::string PlyParser::readFile(const std::string& fileName)
{
    std::string fileContents;
    std::ifstream fin(fileName, std::ios::in);

    if (!fin.is_open())
        throw std::runtime_error("Unable to open file at " + fileName);

    fin.seekg(0, std::ios::end);
    fileContents.resize((unsigned long)fin.tellg()); //allocate enough space
    fin.seekg(0, std::ios::beg);
    fin.read(&fileContents[0], (long)fileContents.size()); //read entire file
    fin.close();

    return fileContents;
}



/*  Accepts the contents of the PLY file,
    returns three strings: the header, the vertices, and the triangle.
    Note that no parsing is done, this function just separates the raw data.
*/
std::vector<std::string> PlyParser::seperatePly(const std::string& fileContents)
{
    std::string headerDelimiter = "end_header";
    std::vector<std::string> pieces;
    unsigned long headerEnd = fileContents.find(headerDelimiter);
    unsigned long dataBegin = headerEnd + headerDelimiter.size() + 1;

    std::string header = fileContents.substr(0, headerEnd - 1);
    pieces.push_back(header); //add header
    auto sizes = getSizes(header); //fetches count of vertices and triangle

    //skips from the end of the header to the end of vertices data
    unsigned long location = dataBegin;
    for (int lineCount = 0; lineCount < sizes.first; lineCount++)
        location = fileContents.find('\n', location + 1);

    //add vertex data
    pieces.push_back(fileContents.substr(dataBegin, location - dataBegin));
    pieces.push_back(fileContents.substr(location + 1)); //add index data

    return pieces;
}



/*  Accepts the raw characters that represent the vertices,
    parses the characters into a list of Points and returns the result.
*/
VertexBufferPtr PlyParser::parseVertices(const std::string& verticesStr)
{
    std::vector<glm::vec3> vertices;
    std::stringstream sstream(verticesStr);

    //loop through each line, pull out and store the relevant data
    std::string line;
    while (std::getline(sstream, line))
    {
        std::stringstream lineStream(line);

        glm::vec3 pt;
        lineStream >> pt.x >> pt.y >> pt.z;
        vertices.push_back(pt);
    }

    return std::make_shared<VertexBuffer>(vertices);
}



/*  Accepts the raw characters that represent the triangle,
    parses the characters into a list of ints and returns the result.
*/
IndexCollection PlyParser::parseIndices(const std::string& facesStr)
{
    std::vector<GLuint> indices;
    std::stringstream sstream(facesStr);
    
    bool determinedType = false;
    GLenum type = GL_TRIANGLES;

    //loop through each line, pull out and store the relevant data
    std::string line;
    while (std::getline(sstream, line))
    {
        std::stringstream lineStream(line);

        int faceDimensionality; //3 for GL_TRIANGLES, 4 for GL_QUADS
        lineStream >> faceDimensionality;
        
        GLenum faceType;
        if (faceDimensionality == 3)
            faceType = GL_TRIANGLES;
        else if (faceDimensionality == 4)
            faceType = GL_QUADS;
        else
            throw std::runtime_error("Unrecognized face dimensionality.");

        if (determinedType && type != faceType)
            throw std::runtime_error("Mixed face dimensionality.");

        if (!determinedType)
        {
            type = faceType;
            determinedType = true;
        }

        for (int j = 0; j < faceDimensionality; j++)
        {
            GLuint index;
            lineStream >> index;
            indices.push_back(index);
        }
    }

    return std::make_pair(std::make_shared<IndexBuffer>(indices, type), type);
}


/**
 * Accepts the characters that make up the header,
 * returns the count of how many vertices and
 * how many triangle make up the rest of the file.
 */
std::pair<int, int> PlyParser::getSizes(const std::string& header)
{
    std::stringstream stream(header);
    std::string line;
    int vertexCount = -1, indexCount = -1;

    while (std::getline(stream, line))
    {
        if (line.find("element vertex") != std::string::npos)
            vertexCount = getLastWordAsInt(line);

        if (line.find("element face") != std::string::npos)
            indexCount = getLastWordAsInt(line);
    }

    return std::make_pair(vertexCount, indexCount);
}



/*  Given a string,
    this takes the last word (space delimited),
    converts it to an int, and returns the result.
*/
int PlyParser::getLastWordAsInt(const std::string& str)
{
    std::stringstream sstream(str.substr(str.find_last_of(' ')));

    int var;
    sstream >> var;
    return var;
}
