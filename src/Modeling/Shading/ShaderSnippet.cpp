
#include "ShaderSnippet.hpp"


ShaderSnippet::ShaderSnippet():
    ShaderSnippet("", "", "")
{}



ShaderSnippet::ShaderSnippet(const std::string& fields,
    const std::string& methods, const std::string& mainBodyCode):
    fields_(fields), methods_(methods), mainBodyCode_(mainBodyCode)
{}



std::string ShaderSnippet::getFields()
{
    return fields_;
}



std::string ShaderSnippet::getMethods()
{
    return methods_;
}



std::string ShaderSnippet::getMainBodyCode()
{
    return mainBodyCode_;
}
