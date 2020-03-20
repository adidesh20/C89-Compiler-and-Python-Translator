#ifndef F_DEFINITIONS_HPP
#define F_DEFINITIONS_HPP

#include <iostream>
#include<vector>
#include <string>
#include "ast.hpp"
#include "common.hpp"

extern int variable_count;
extern std::vector<std::string> global_variables;



class Function_Declaration: public AST_Node
{
public:
    std::string returnType;
    std::string functionIdentifier;
    ExpressionPtr parameters;


    Function_Declaration(std::string _returnType, std::string _functionIdentifier, ExpressionPtr _parameters)
    {
        returnType = _returnType;
        functionIdentifier = _functionIdentifier;
        parameters = _parameters;
    }

    virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << functionIdentifier << " (";
        if(parameters != NULL)
        {
            parameters->print(out);
        }
        out << ")" << std::endl;
    } 

    ~Function_Declaration(){}
};

class Function_Definition: public AST_Node
{
public:
    std::string returnType;
    std::string functionIdentifier;
    ExpressionPtr parameters;
    NodePtr functionImplementation;

    Function_Definition(std::string _returnType, std::string _functionIdentifier, ExpressionPtr _parameters, NodePtr _functionImplementation)
    {
        returnType = _returnType;
        functionIdentifier = _functionIdentifier;
        parameters = _parameters;
        functionImplementation = _functionImplementation;
    }

    virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << functionIdentifier << " (";
        if(parameters != NULL)
        {
            parameters->print(out);
        }
        out << ") {";
        if(functionImplementation != NULL)
        {
            functionImplementation->print(out);
        }
        out << "}" << std::endl;
    } 
};

#endif