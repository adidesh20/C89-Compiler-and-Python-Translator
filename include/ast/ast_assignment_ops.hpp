#ifndef ASS_OP_HPP
#define ASS_OP_HPP

#include "ast.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <vector>

class AssignmentOperator
    : public AST_Node
{
private:
    std::string left;
    NodePtr right;
protected:
    AssignmentOperator(std::string _left, NodePtr _right)
        : left(_left)
        , right(_right)
    {}
public:
    virtual ~AssignmentOperator()
    {
        //delete left;
        delete right;
    }

    virtual const char *getOpcode() const =0;

    std::string getLeft() const
    { return left; }

    NodePtr getRight() const
    { return right; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<left;
        dst<<" ";
        dst<<getOpcode();
        dst<<" ";
        right->print(dst);
    }

    virtual void toPython(std::ostream &dst) const override
    {
        dst<<left;
        dst<<" ";
        dst<<getOpcode();
        dst<<" ";
        right->toPython(dst);
    }
};


class AssOpEqual: public AssignmentOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "="; }

public:
    AssOpEqual(std::string _left, NodePtr _right)
        : AssignmentOperator(_left, _right)
    {}

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        int stackSize = (stackVarCount*4) + paramCount + 62;
        getRight()->toMips(dst, mySystem, destReg);
        mySystem.store_var_val(getLeft(), getRight()->evaluate(mySystem));

        int varAddress = mySystem.systemMemory.variableSearch(getLeft(), currentIndent);

        if(varAddress > 0) //local variable
        {
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << varAddress << "($fp)" << "\t\t#assigning value " << getRight()->evaluate(mySystem) << " to local variable " << getLeft() << std::endl;
        }
        else if(mySystem.lookupParameter(getLeft()) > 1) //function parameter
        {
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << ((mySystem.lookupParameter(getLeft()))*4 + stackSize) << "($fp)" << "\t\t#assigning value " << getRight()->evaluate(mySystem) << " to function parameter " << getLeft() << std::endl;
        }
        else
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            mySystem.lockReg(scratchRegs[0]);

            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << getLeft() << ")" << "\t\t #loading global variable: " << getLeft() << std::endl;
            getRight()->toMips(dst, mySystem, scratchRegs[0]);
            dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<" %lo("<<getLeft() <<")($"<< mySystem.getRegName(destReg) <<")"<<std::endl;
            dst << "\t" << "nop" << std::endl;

            mySystem.unlockReg(scratchRegs[0]);
        }
        
    }
};

class AssOpAdd: public AssignmentOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "+="; }

public:
    AssOpAdd(std::string _left, NodePtr _right)
        : AssignmentOperator(_left, _right)
    {}

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        int stackSize = (stackVarCount*4) + paramCount + 62;
        mySystem.lockReg(scratchRegs[0]);
        mySystem.lockReg(scratchRegs[1]);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        int varAddress = mySystem.systemMemory.variableSearch(getLeft(), currentIndent);

        if(varAddress > 0) //local variable
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << getLeft() <<std::endl;
            dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #incrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
        }
        else if(mySystem.lookupParameter(getLeft()) > 1)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " << ((mySystem.lookupParameter(getLeft()))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << getLeft() << std::endl;
            dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #incrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(getLeft()))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
            
        }
        else
        {
            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << getLeft() << ")" << "\t\t #loading global variable: " << getLeft() << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", %lo(" << getLeft() << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
            dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #incrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << " %lo("<< getLeft() << ")($" << mySystem.getRegName(destReg) <<")" << "\t\t #storing " << getLeft() << " after increment" << std::endl;
        }
        
        mySystem.unlockReg(scratchRegs[0]);
        mySystem.unlockReg(scratchRegs[1]);

    }
};

class AssOpSub: public AssignmentOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "-="; }

public:
    AssOpSub(std::string _left, NodePtr _right)
        : AssignmentOperator(_left, _right)
    {}

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        int stackSize = (stackVarCount*4) + paramCount + 62;
        mySystem.lockReg(scratchRegs[0]);
        mySystem.lockReg(scratchRegs[1]);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        int varAddress = mySystem.systemMemory.variableSearch(getLeft(), currentIndent);

        if(varAddress > 0) //local variable
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << getLeft() <<std::endl;
            dst << "\t" << "sub" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #decrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
        }
        else if(mySystem.lookupParameter(getLeft()) > 1)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " << ((mySystem.lookupParameter(getLeft()))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << getLeft() << std::endl;
            dst << "\t" << "sub" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #decrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(getLeft()))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
            
        }
        else
        {
            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << getLeft() << ")" << "\t\t #loading global variable: " << getLeft() << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", %lo(" << getLeft() << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
            dst << "\t" << "sub" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) <<"\t\t #decrementing by " << getRight()->evaluate(mySystem) << std::endl;
            dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<" %lo("<<getLeft() <<")($"<< mySystem.getRegName(destReg) <<")" << "\t\t #storing " << getLeft() << " after increment" << std::endl;
        }
        
        mySystem.unlockReg(scratchRegs[0]);
        mySystem.unlockReg(scratchRegs[1]);

    }

    
};

class AssOpMul: public AssignmentOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "*="; }

public:
    AssOpMul(std::string _left, NodePtr _right)
        : AssignmentOperator(_left, _right)
    {}

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        int stackSize = (stackVarCount*4) + paramCount + 62;
        mySystem.lockReg(scratchRegs[0]);
        mySystem.lockReg(scratchRegs[1]);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        int varAddress = mySystem.systemMemory.variableSearch(getLeft(), currentIndent);

        if(varAddress > 0) //local variable
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << getLeft() <<std::endl;
            dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << "\t\t #multiplying by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
        }
        else if(mySystem.lookupParameter(getLeft()) > 1)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " << ((mySystem.lookupParameter(getLeft()))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << getLeft() << std::endl;
            dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << "\t\t #multiplying by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) <<", "<< ((mySystem.lookupParameter(getLeft()))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
            
        }
        else
        {
            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << getLeft() << ")" << "\t\t #loading global variable: " << getLeft() << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", %lo(" << getLeft() << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
            dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << "\t\t #multiplying by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " <<" %lo("<<getLeft() <<")($"<< mySystem.getRegName(destReg) <<")" << "\t\t #storing " << getLeft() << " after increment" << std::endl;
        }
        
        mySystem.unlockReg(scratchRegs[0]);
        mySystem.unlockReg(scratchRegs[1]);

    }
};

class AssOpDiv: public AssignmentOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "/="; }

public:
    AssOpDiv(std::string _left, NodePtr _right)
        : AssignmentOperator(_left, _right)
    {}

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        int stackSize = (stackVarCount*4) + paramCount + 62;
        mySystem.lockReg(scratchRegs[0]);
        mySystem.lockReg(scratchRegs[1]);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        int varAddress = mySystem.systemMemory.variableSearch(getLeft(), currentIndent);

        if(varAddress > 0) //local variable
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << getLeft() <<std::endl;
            dst << "\t" << "div" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #dividing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
        }
        else if(mySystem.lookupParameter(getLeft()) > 1)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", " << ((mySystem.lookupParameter(getLeft()))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << getLeft() << std::endl;
            dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #dividing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) <<", "<< ((mySystem.lookupParameter(getLeft()))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << getLeft() << " after increment"<<std::endl;
            
        }
        else
        {
            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << getLeft() << ")" << "\t\t #loading global variable: " << getLeft() << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", %lo(" << getLeft() << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
            dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #dividing by " << getRight()->evaluate(mySystem) << std::endl;
            dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;
            dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " <<" %lo("<<getLeft() <<")($"<< mySystem.getRegName(destReg) <<")" << "\t\t #storing " << getLeft() << " after increment" << std::endl;
        }

        mySystem.unlockReg(scratchRegs[0]);
        mySystem.unlockReg(scratchRegs[1]);
       

    }

    
};

#endif