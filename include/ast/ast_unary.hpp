#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>
#include "ast.hpp"
class Unary
    : public AST_Node
{
private:
    std::string var;
    std::string position;
protected:
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

 

};
#endif
