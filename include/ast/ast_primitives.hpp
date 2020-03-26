#ifndef ast_primitives_hpp
#define ast_primitives_hpp
#include "ast.hpp"
#include <string>
#include <iostream>

class Variable
    : public AST_Node
{
private:
    std::string id;
public:
    Variable(const std::string &_id)
        : id(_id)
    {}

    const std::string getId() const
    { return id; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<id;
    }
    
    virtual void toPython(std::ostream &dst) const override
    {
        dst<<id;
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // TODO-B : Run bin/eval_expr with a variable binding to make sure you understand how this works.
        // If the binding does not exist, this will throw an error
        return bindings.at(id);
    }    

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg)
    {
        int varAddress = mySystem.systemMemory.variableSearch(id, currentIndent);
        int stackTop = (stackVarCount*4) + paramCount + 62;
        std::vector<int> argumentRegs = mySystem.args_freeRegLookup();

        if(varAddress > 0) //Local Variable defined in a scope
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << varAddress << "($fp)";
            dst << "\t\t #loading local (scope) variable: " << id << std::endl; 
            dst << "\t" << "nop" << std::endl;
        }
        else if(varAddress > -1) //Function Parameter (use stack)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << ((mySystem.lookupParameter(id))*4 + stackTop) << "($fp)";
            dst << "\t\t #variable " << id << " is a function parameter" << std::endl;
            dst << "\t" << "nop" << std::endl;
        }
        else //Global Variable
        {
            dst << "\t" << "lui" << "$" << mySystem.getRegName(destReg) << ", %hi(" << id << ")";
            dst << "\t\t #loading global variable: " << id << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", %lo(" << id << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
            dst << "\t" << "nop" << std::endl;
        }
        
    }
};

class Number
    : public AST_Node
{
private:
    double value;
public:
    Number(double _value)
        : value(_value)
    {}

    double getValue() const
    { return value; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<value;
    }
    
    virtual void toPython(std::ostream &dst) const override
    {
        dst<<value;
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // TODO-A : Run bin/eval_expr with a numeric AST_Node to make sure you understand how this works.
        return value;
    }
      virtual int evaluate()const override
    {
        // TODO-A : Run bin/eval_expr with a numeric AST_Node to make sure you understand how this works.
        return value;
    }
    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if (isGlobal) {
            dst<<value<<"\t#Global variable"<<std::endl;
        }
        else if (!isGlobal) {
            dst <<"\t"<< "li" << "\t" << "$"<<destReg<<", " << value<<std::endl;
        }
	}
};


#endif
