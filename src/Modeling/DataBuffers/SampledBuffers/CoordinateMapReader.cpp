
#include "CoordinateMapReader.hpp"
#include "Modeling/Mesh/PlyParser.hpp"
#include <sstream>
#include <iostream>


std::vector<GLfloat> CoordinateMapReader::getMap(const std::string& fileName)
{
    std::cout << "Loading coordinate map from " << fileName << " ... ";
    std::string fileContents = PlyParser::readFile(fileName);

    std::vector<GLfloat> map;
    std::stringstream mapStream(fileContents);

    GLfloat temp;
    mapStream >> temp;
    while (mapStream.good())
    {
        map.push_back(temp);
        mapStream >> temp;
    }

    std::cout << "done. " << std::endl;
    return map;
}
