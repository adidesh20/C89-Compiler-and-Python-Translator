#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>
#include "ast.hpp"
class Unary
    : public AST_Node
{
protected:
    std::string var;
    std::string position;

    Unary( std::string _var, std::string _position)
        : var(_var), position(_position)
    {}
public:
    virtual ~Unary()
    {
        
    }

    virtual const char *getOpcode() const =0;


    virtual void print(std::ostream &dst) const override
    {
        if(position == "pre")
        {
            dst << "( ";
            dst << getOpcode()<<var;
            dst << " )";
        }
        else
        {
            dst << "( ";
            dst << var<<getOpcode();
            dst << " )";
        }
    }

    virtual void toPython(std::ostream &dst) const override
    {
        if(position == "pre")
        {
            dst << getOpcode()<<var;
        }
        else
        {
            
            dst << var<<getOpcode();
        }
    }

};



class IncrementOperator
    : public Unary
{
public:
    IncrementOperator( std::string var, std::string _position)
        : Unary(var, _position)
    {}

    virtual const char *getOpcode() const override
    { return "++"; }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(position == "pre")
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            int stackSize = (stackVarCount*4) + paramCount + 62;
            mySystem.lockReg(scratchRegs[0]);
            

            int varAddress = mySystem.systemMemory.variableSearch(var, currentIndent);

            if(varAddress > 0) //local variable
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << var <<std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" << "\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << var << " after increment"<<std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)" <<std::endl;
                dst << "\t" << "nop" << std::endl;
            }
            else if(mySystem.lookupParameter(var) > 1)
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << var << std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" << "\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(var))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << var << " after increment"<<std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << std::endl;
                dst << "\t" << "nop" << std::endl;
            
            }
            else
            {
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" << "\t\t #loading global variable: " << var << std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" <<"\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << " %lo("<< var << ")($" << mySystem.getRegName(destReg) <<")" << "\t\t #storing " << var << " after increment" << std::endl;
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" <<  std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "nop" << std::endl;
            }
        
            mySystem.unlockReg(scratchRegs[0]);
        }
        else
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            int stackSize = (stackVarCount*4) + paramCount + 62;
            mySystem.lockReg(scratchRegs[0]);
            

            int varAddress = mySystem.systemMemory.variableSearch(var, currentIndent);

            if(varAddress > 0) //local variable
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << var <<std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" << "\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << var << " after increment"<<std::endl;
            }
            else if(mySystem.lookupParameter(var) > 1)
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << var << std::endl;
                dst << "\t" << "nop" << std::endl;
               dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" << "\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(var))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << var << " after increment"<<std::endl;
            
            }
            else
            {
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" << "\t\t #loading global variable: " << var << std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", 1" <<"\t\t #incrementing by 1 due to ++ operator" << std::endl;
                dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << " %lo("<< var << ")($" << mySystem.getRegName(destReg) <<")" << "\t\t #storing " << var << " after increment" << std::endl;
            }
        
            mySystem.unlockReg(scratchRegs[0]);
     
        }
    }

};

class DecrementOperator
    : public Unary
{
public:
    DecrementOperator( std::string var, std::string _position)
        : Unary(var, _position)
    {}

    virtual const char *getOpcode() const override
    { return "--"; }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(position == "pre")
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            int stackSize = (stackVarCount*4) + paramCount + 62;
            mySystem.lockReg(scratchRegs[0]);
            

            int varAddress = mySystem.systemMemory.variableSearch(var, currentIndent);

            if(varAddress > 0) //local variable
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << var <<std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" << "\t\t #decrementing by 1 due to -- operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << var << " after decrement"<<std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)" <<std::endl;
                dst << "\t" << "nop" << std::endl;
            }
            else if(mySystem.lookupParameter(var) > 1)
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << var << std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" << "\t\t #decrementing by 1 due to -- operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(var))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << var << " after decrement"<<std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << std::endl;
                dst << "\t" << "nop" << std::endl;
            
            }
            else
            {
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" << "\t\t #loading global variable: " << var << std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" <<"\t\t #decrementing by -1 due to -- operator" << std::endl;
                dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << " %lo("<< var << ")($" << mySystem.getRegName(destReg) <<")" << "\t\t #storing " << var << " after decrement" << std::endl;
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" <<  std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "nop" << std::endl;
            }
        
            mySystem.unlockReg(scratchRegs[0]);
        }
        else
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            int stackSize = (stackVarCount*4) + paramCount + 62;
            mySystem.lockReg(scratchRegs[0]);
            

            int varAddress = mySystem.systemMemory.variableSearch(var, currentIndent);

            if(varAddress > 0) //local variable
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " <<varAddress <<"($fp)"<<"\t\t #loading local variable " << var <<std::endl;
                dst << "\t" << "nop" << std::endl;
                dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" << "\t\t #decrementing by 1 due to -- operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< varAddress <<"($fp)"<<"\t\t #storing " << var << " after increment"<<std::endl;
            }
            else if(mySystem.lookupParameter(var) > 1)
            {
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << ((mySystem.lookupParameter(var))*4 + stackSize) << "($fp)" << "\t\t #loading function parameter " << var << std::endl;
                dst << "\t" << "nop" << std::endl;
               dst << "\t" << "addiu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" << "\t\t #decrementing by 1 due to -- operator" << std::endl;
                dst << "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) <<", "<< ((mySystem.lookupParameter(var))*4 + stackSize) <<"($fp)"<<"\t\t #storing " << var << " after decrement"<<std::endl;
            
            }
            else
            {
                dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << var << ")" << "\t\t #loading global variable: " << var << std::endl;
                dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", %lo(" << var << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
                dst << "\t" << "addu" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", -1" <<"\t\t #incrementing by -1 due to -- operator" << std::endl;
                dst<< "\t" << "sw" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", " << " %lo("<< var << ")($" << mySystem.getRegName(destReg) <<")" << "\t\t #storing " << var << " after decrement" << std::endl;
            }
        
            mySystem.unlockReg(scratchRegs[0]);
     
        }
    }

};

class NotOperator: public AST_Node {
public:
    
    NodePtr val;

    ~NotOperator() {}

    NotOperator(NodePtr _val) {
        val=_val;

    }

    virtual void print (std::ostream &dst) const override {
        
            dst << "!";
            val->print(dst);
          
        
    }

    virtual void toPython (std::ostream &dst) const override {
        
        dst << "!";
        val->toPython(dst);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        val->toMips(dst, mySystem, destReg);
        dst << "\t" << "sltiu" << "\t" << "$" << mySystem.getRegName(destReg) << ", 1" << "\t\t # logical not operator" << std::endl;
    }
 

};



class NegOperator: public AST_Node {
public:
    
    NodePtr val;

    ~NegOperator() {}

    NegOperator(NodePtr _val) {
        val=_val;

    }

    virtual void print (std::ostream &dst) const override {
        
            dst << "-";
            val->print(dst);
          
        
    }

    virtual void toPython (std::ostream &dst) const override {
        
        dst << "-";
        val->toPython(dst);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(isGlobal)
        {
            int v = val->evaluate(mySystem);
            dst << -v;
        }
        else
        {
            val->toMips(dst, mySystem, destReg);
            dst << "\t" << "sub" << "\t" << "$" << mySystem.getRegName(destReg) << ", $zero, $" << mySystem.getRegName(destReg) << "\t\t #negative operator " << std::endl;
        }
    }

 

};
#endif
