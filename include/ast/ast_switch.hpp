#include "ast.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>
#include <string>

class Switch: public AST_Node 
{
private:
    NodePtr Expression;
    NodePtr CaseList;

public:
    ~Switch() {}

    Switch (NodePtr _Expression, NodePtr _CaseList) : Expression(_Expression) , CaseList(_CaseList) {}

    virtual void print (std::ostream &dst) const override 
    {
        dst<<"switch(";
        Expression->print(dst);
        dst <<") {"<<std::endl;
        // dst<<"\t";
        currentIndent++;
        CaseList->print(dst);
        currentIndent--;
        for (int i = 0; i < currentIndent; i++) {dst << "\t";}
        dst<<"}"<<std::endl;
    }
    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        inSwitch = true;
        switchActivated = false;
        switchFallthroughAllowed = true;
        //Expression->compile(dst, contxt, destReg);
        dst << "#Switch Case" << std::endl;
        switchRef = Expression->evaluate(mySystem);
        CaseList->toMips(dst, mySystem, destReg);

        switchActivated = false;
        switchFallthroughAllowed = true;
        inSwitch = false;
    }
};

class SwitchCaseList: public AST_Node
{
public:
    NodePtr SingularCase;
    NodePtr RestOfCases;
    NodePtr DefaultCase;

    ~SwitchCaseList(){}

    SwitchCaseList(NodePtr _SingularCase, NodePtr _RestOfCases, NodePtr _DefaultCase) : SingularCase(_SingularCase), RestOfCases(_RestOfCases), DefaultCase(_DefaultCase){}

    virtual void print(std::ostream &dst) const override
    {
        for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        SingularCase->print(dst);
        if(RestOfCases != NULL)
        {
            RestOfCases->print(dst);
        }
        if(DefaultCase != NULL)
        {
            DefaultCase->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {

        if(SingularCase != NULL)
        {
            SingularCase->toMips(dst, mySystem, destReg);
        }

        if(RestOfCases != NULL)
        {
            RestOfCases->toMips(dst, mySystem, destReg);
        }

        if(DefaultCase != NULL)
        {
            DefaultCase->toMips(dst, mySystem, destReg);
        }

    }

};

class SwitchCase: public AST_Node
{
public:
    double value;
    NodePtr Block;

    ~SwitchCase(){}

    SwitchCase(double _value, NodePtr _Block): value(_value), Block(_Block) {}

    virtual void print(std::ostream &dst) const override
    {
        for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        
        dst << "\t" << "case " << value << ":" << std::endl;

        if(Block != NULL)
        {
            Block->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        
        if((switchRef == value && !switchActivated) ||(switchFallthroughAllowed && switchActivated))
        {
            //This is the case to execute
            dst << "#Executing Case " << value << std::endl;
            Block->toMips(dst, mySystem, destReg);

            switchActivated = true;

            
        }
    }

};

class SwitchDefaultCase: public AST_Node
{
public:
    NodePtr Block;

    ~SwitchDefaultCase(){}

    SwitchDefaultCase(NodePtr _Block): Block(_Block){}

    virtual void print(std::ostream &dst) const override
    {
        for(int i = 0; i < currentIndent; i++)
        {
            dst << "\t";
        }
        
        dst << "\t" << "default: " << std::endl;

        if(Block != NULL)
        {
            Block->print(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(!switchActivated)
        {
            dst << "#Executing Default Case " << std::endl;
            Block->toMips(dst, mySystem, destReg);
        }
    }

};