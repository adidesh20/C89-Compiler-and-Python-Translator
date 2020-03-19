#ifndef  NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include "ast.hpp"

class AST_Node;

typedef const AST_Node *NodePtr;

class AST_Node
{
public:
    virtual ~AST_Node()
    {}

    //! Tell and expression to print itself to the given stream
    virtual void print(std::ostream &dst) const =0;

    //! Evaluate the tree using the given mapping of variables to numbers
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const
    { throw std::runtime_error("Not implemented."); }
};

#endif
