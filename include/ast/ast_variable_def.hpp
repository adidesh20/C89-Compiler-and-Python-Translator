#ifndef V_DEFINITIONS_HPP 
#define V_DEFINITIONS_HPP

#include <iostream>
#include <vector>
#include <string>
#include "ast.hpp"

extern std::vector<std::string> global_variables;

class GlobalVariable_Definition: public AST_Node
{
public:
    std::string returnType;
    std::string varIdenifier;
    ExpressionPtr varValue;

    GlobalVariable_Definition(std::string _returnType, std::string _varIdenifier, ExpressionPtr _varValue)
    {
        returnType = _returnType;
        varIdenifier = _varIdenifier;
        varValue = _varValue;
        global_variables.push_back(varIdenifier);
        
    }

     virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << varIdenifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
        out << ";" << std::endl;
    }

    ~GlobalVariable_Definition(){}


};

class LocalVariable_Definition: AST_Node
{
    public:
    std::string returnType;
    std::string varIdenifier;
    ExpressionPtr varValue;

    LocalVariable_Definition(std::string _returnType, std::string _varIdenifier, ExpressionPtr _varValue)
    {
        returnType = _returnType;
        varIdenifier = _varIdenifier;
        varValue = _varValue;
        
    }

     virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << varIdenifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
        out << ";" << std::endl;
    }

    ~LocalVariable_Definition(){}
};

#endif