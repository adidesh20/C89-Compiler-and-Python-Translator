#include "ast.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <string>

class EnumDefinition: public AST_Node
{
public:
    std::string name;
    NodePtr list;

    ~EnumDefinition(){}
    
    EnumDefinition(std::string _name, NodePtr _list): name(_name), list(_list){}

    virtual void print(std::ostream &dst) const override
    {
        dst << "enum" << name << "{" << std::endl;
        list->print(dst);
        dst << "};" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        enumCounter = 0;
        dst << "#Defining enum " << name << std::endl;
        list->toMips(dst, mySystem, destReg);
    }

};

class GlobalEnumList: public AST_Node
{
public:
    std::string elementName;
    NodePtr elementValue;
    NodePtr nextElement;

    ~GlobalEnumList(){}

    GlobalEnumList(std::string _elementName, NodePtr _elementValue, NodePtr _nextElement): elementName(_elementName), elementValue(_elementValue), nextElement(_nextElement){}
    
    virtual void print(std::ostream &dst) const override
    {
        dst<< elementName;
        if(elementValue != NULL)
        {
            dst << " = ";
            elementValue->print(dst);
        }
        if(nextElement != NULL)
        {
            dst << ", ";
            nextElement->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        globalVarCount++;
        mySystem.NewGlobalVar(elementName);

        dst << "\t" << ".globl"<< "\t" << elementName <<"\t\t #Defining global variable "<< elementName << "in enum" << std::endl;
        dst << "\t" << ".data"<< "\t" << std::endl;
        dst << elementName << ":" << std::endl;

        if(elementValue !=NULL)
        {
            enumCounter = elementValue->evaluate(mySystem);
        }
        dst << "\t"<< ".word"<< "\t"<< enumCounter;

       dst <<std::endl;
       enumCounter++;

       if(nextElement != NULL)
        {
           nextElement->toMips(dst, mySystem, destReg);
        }

        
    }
};

class LocalEnumList: public AST_Node
{
public:
    std::string elementName;
    NodePtr elementValue;
    NodePtr nextElement;

    ~LocalEnumList(){}

    LocalEnumList(std::string _elementName, NodePtr _elementValue, NodePtr _nextElement): elementName(_elementName), elementValue(_elementValue), nextElement(_nextElement){}
    
    virtual void print(std::ostream &dst) const override
    {
        dst<< elementName;
        if(elementValue != NULL)
        {
            dst << " = ";
            elementValue->print(dst);
        }
        if(nextElement != NULL)
        {
            dst << ", ";
            nextElement->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        localVarCount++;
        mySystem.NewLocalVar(elementName + std::to_string(currentIndent));

        

        if(elementValue !=NULL)
        {
            enumCounter = elementValue->evaluate(mySystem);
        }

       dst <<std::endl;
       enumCounter++;

        dst << "\t" << "li" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << enumCounter;
        dst << "\t\t#enum variable " << elementName <<" assigned value "<< enumCounter <<std::endl;

        enumCounter++;

        int varAddress = mySystem.systemMemory.variableSearch(elementName, currentIndent);

        dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) <<", "<<varAddress<<"($fp)";
        dst<<"\t\t#Store enum variable "<< elementName << std::endl;

       if(nextElement != NULL)
       {
           std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
           mySystem.lockReg(scratchRegs[0]);
           nextElement->toMips(dst, mySystem, destReg);
           mySystem.unlockReg(scratchRegs[0]);
       }
    }
};


