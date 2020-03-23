#ifndef V_DEFINITIONS_HPP 
#define V_DEFINITIONS_HPP

#include <iostream>
#include <vector>
#include <string>
#include "ast.hpp"
#include "common.hpp"


class GlobalVariable_Definition: public AST_Node
{
public:
    std::string varType;
    std::string varIdentifier;
    NodePtr varValue;

    GlobalVariable_Definition(std::string _varType, std::string _varIdentifier, NodePtr _varValue)
    {
        varType = _varType;
        varIdentifier = _varIdentifier;
        varValue = _varValue;
        global_variables_names.push_back(varIdentifier);
        
    }

    virtual void print(std::ostream &out) const override
    {
        out << varType << " " << varIdentifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
        out << ";" << std::endl;
    }

    virtual void toPython(std::ostream &out) const override
    {
        out << varIdentifier << " = ";
        if(varValue == NULL)
        {
            out<< "0" << std::endl;
        }
        else
        {
           varValue->toPython(out);
        }
        
    }

    ~GlobalVariable_Definition(){}


};

class LocalVariable_Definition: public AST_Node
{
    public:
    std::string returnType;
    std::string varIdentifier;
    NodePtr varValue;

    LocalVariable_Definition(std::string _returnType, std::string _varIdentifier, NodePtr _varValue)
    {
        returnType = _returnType;
        varIdentifier = _varIdentifier;
        varValue = _varValue;
        
    }

     virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << varIdentifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
        out << ";" << std::endl;
    }

    virtual void toPython(std::ostream &out) const override
    {
        out << varIdentifier << " = ";
        if(varValue == NULL)
        {
            out<< "0" << std::endl;
        }
        else
        {
           varValue->toPython(out);
        }
        
    }

    ~LocalVariable_Definition(){}
};

#endif