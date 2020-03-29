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

     virtual int evaluate(System &mySystem) const override
    {
        // TODO-B : Run bin/eval_expr with a variable binding to make sure you understand how this works.
        // If the binding does not exist, this will throw an error
        return mySystem.systemMemory.variable_bindings.at(id);
    }  

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    
    {   
       
        int varAddress = mySystem.systemMemory.variableSearch(id, currentIndent);
        int stackSize = (stackVarCount*4) + paramCount + 62;
        std::vector<int> argumentRegs = mySystem.args_freeRegLookup();
        std::cout << "\t #varAddress = " << varAddress << std::endl;

        if(varAddress > 0) //Local Variable defined in a scope
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << varAddress << "($fp)";
            dst << "\t\t #loading local (scope) variable: " << id << std::endl; 
            dst << "\t" << "nop" << std::endl;
        }
        else if(mySystem.lookupParameter(id) > -1) //Function Parameter (use stack)
        {
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", " << ((mySystem.lookupParameter(id))*4 + stackSize) << "($fp)";
            dst << "\t\t #variable " << id << " is a function parameter" << std::endl;
            dst << "\t" << "nop" << std::endl;
        }
        else //Global Variable
        {
            dst << "\t" << "lui" << "\t" << "$" << mySystem.getRegName(destReg) << ", %hi(" << id << ")";
            dst << "\t\t #loading global variable: " << getId() << std::endl;
            dst << "\t" << "lw" << "\t" << "$" << mySystem.getRegName(destReg) << ", %lo(" << getId() << ")($" << mySystem.getRegName(destReg) << ")" << std::endl;
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
      virtual int evaluate(System &mySystem)const override
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
            dst <<"\t"<< "li" << "\t" << "$"<<mySystem.getRegName(destReg)<<", " << value<<std::endl;
        }
	}
};

class ArrayElement : public AST_Node {
    private:
    std::string id;
    NodePtr index;

    public:
    ~ArrayElement() {};
    ArrayElement(std::string _id, NodePtr _index) : id(_id), index(_index) {}
    virtual void print (std::ostream &dst) const override {
        dst<<id<<"[";
        if (index!=NULL)
            index->print(dst);
        dst<<"]";
    }

    virtual int evaluate (System &mySystem) const override {
        return index->evaluate(mySystem);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override{
        int index_no = index->evaluate(mySystem);
        
        int varAddress = mySystem.systemMemory.variableSearch(id+std::to_string(index_no), currentIndent);

        if(varAddress > 0) {  
            
            dst<<"\t"<<"lw"<<"\t"<<"$"<<mySystem.getRegName(destReg) <<", "<< varAddress<<"($fp)";
            dst<<"\t\t#Accessing index "<<index_no<<" of array "<<id<<std::endl;
            dst<<"\t"<<"nop"<<std::endl;
        }
        else {
            //global array
            dst<<"\t"<<"lui"<<"\t"<<"$"<<mySystem.getRegName(destReg)<<", "<<"%hi("<<id<<")"<<"\t\t #Loading in array: "<<id<<std::endl;
            dst<<"\t"<<"addiu"<<"\t"<<"$"<<mySystem.getRegName(destReg)<<", $"<<mySystem.getRegName(destReg)<<", %lo("<<id<<")"<<std::endl;
            dst<<"\t"<<"nop"<<std::endl;
            dst<<"\t"<<"addiu"<<"\t"<<"$"<<mySystem.getRegName(destReg)<<", $"<<mySystem.getRegName(destReg)<<", "<<index_no*4<<"\t\t #Add offset for index "<<index_no<<std::endl;
            dst<<"\t"<<"lw"<<"\t"<<"$"<<mySystem.getRegName(destReg)<<", "<<"0($"<<mySystem.getRegName(destReg)<<")"<<"\t\t #Load in index "<<index_no<<" of global array "<<id<<std::endl;
        }
    }
};




#endif
