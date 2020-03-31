#ifndef STRUCT_HPP
#define STRUCT_HPP

#include "ast.hpp"
#include "common.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

static std::map<std::string, std::vector<std::pair<std::string, NodePtr>>> structDefaults;
static std::vector<std::pair<std::string, NodePtr>> varDetails;

//DEFINING STRUCT TEMPLATE

class StructDefinition: public AST_Node
{
public:
    std::string structName;
    NodePtr varDefList;

    ~StructDefinition(){}

    StructDefinition(std::string _structName, NodePtr _varDefList): structName(_structName), varDefList(_varDefList){}

    virtual void print(std::ostream &dst) const override
    {
        for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        dst << "struct " << structName << "{" << std::endl;
        varDefList->print(dst);
        for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        dst << "}" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        varDetails.clear();
        dst << "#Defining struct " << structName << std::endl;
        varDefList->toMips(dst, mySystem, destReg); 

        //Storing Details
        structDefaults.insert(std::pair<std::string, std::vector<std::pair<std::string, NodePtr>>>(structName, varDetails));
    }
};

class StructList: public AST_Node
{
public:
    std::string varName;
    NodePtr expression;
    NodePtr RestOfDefs;

    ~StructList(){}

    StructList(std::string _varName, NodePtr _expression, NodePtr _RestOfDefs): varName(_varName), expression(_expression), RestOfDefs(_RestOfDefs){}
    
    virtual void print(std::ostream &dst) const override
    {
       for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        dst << "\t" << varName;
        if(expression != NULL)
        {
            System dummy;
            dst << " = " << expression->evaluate(dummy);
        }
        dst << ";" << std::endl;
        if(RestOfDefs != NULL)
        {
            RestOfDefs->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destRegs) const override
    {

        varDetails.push_back(std::make_pair(varName, expression));

        if(RestOfDefs != NULL)
        {
            RestOfDefs->toMips(dst, mySystem, destRegs);
        }
    }

};

// CREATING AN INSTANCE OF A STRUCT

class GlobalStructInstance: public AST_Node
{
public:
    std::string structName;
    std::string instanceName;
    
    ~GlobalStructInstance(){}

    GlobalStructInstance(std::string _structName, std::string _instanceName): structName(_structName), instanceName(_instanceName){}

    virtual void print(std::ostream &dst) const override
    {
        dst << "struct " << structName << " " << instanceName << ";" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destRegs) const override
    {
        dst << "#Creating instance of struct " << structName << " called " << instanceName << " in the global scope" << std::endl;
        std::vector<std::pair<std::string, NodePtr>> details = structDefaults.find(structName)->second;

        std::vector<std::pair<std::string, NodePtr>>::iterator it;

        for(it = details.begin(); it !=  details.end(); it++)
        {
            std::string newIdentifier = instanceName + it->first;
            NodePtr current = new GlobalVariable_Definition(newIdentifier, it->second);
            current->toMips(dst, mySystem, destRegs);
            delete current;
        }

    }
    

};

class LocalStructInstance: public AST_Node
{
public:
    std::string structName;
    std::string instanceName;
    
    ~LocalStructInstance(){}

    LocalStructInstance(std::string _structName, std::string _instanceName): structName(_structName), instanceName(_instanceName){}

    virtual void print(std::ostream &dst) const override
    {
        dst << "struct " << structName << " " << instanceName << ";" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destRegs) const override
    {
        dst << "#Creating instance of struct " << structName << " called " << instanceName << " in a local scope" << std::endl;
        std::vector<std::pair<std::string, NodePtr>> details = structDefaults.find(structName)->second;

        std::vector<std::pair<std::string, NodePtr>>::iterator it;

        for(it = details.begin(); it !=  details.end(); it++)
        {
            std::string newIdentifier = instanceName + it->first;
            NodePtr current = new LocalVariable_Definition(newIdentifier, it->second);
            current->toMips(dst, mySystem, destRegs);
            delete current;
        }

    }
    

};

#endif