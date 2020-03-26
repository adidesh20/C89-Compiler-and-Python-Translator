#ifndef AST_Node_hpp
#define AST_Node_hpp

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "ast_system.hpp"
class AST_Node;

typedef const AST_Node *NodePtr;

class AST_Node{
public:
    virtual ~AST_Node()
    {}

    //! Tell a node to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;

    //! Evaluate the tree using the given mapping of variables to numbers
    virtual double evaluate(const std::map<std::string,double> &bindings) const
    { throw std::runtime_error("Evaluate not implemented."); }

    virtual int evaluate () const {
        throw std::runtime_error("Eval Not implemented");
    }

    virtual void toPython(std::ostream &dst) const {
        throw std::runtime_error("Translate not implemented.");
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const {
        throw std::runtime_error("Compilation not implemented.");
    }

    

};


#endif