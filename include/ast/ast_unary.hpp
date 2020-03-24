#ifndef ast_unary_hpp
#define ast_unary_hpp

#include <string>
#include <iostream>
#include "ast.hpp"
class Unary
    : public AST_Node
{
private:
    NodePtr expr;
    std::string position;
protected:
    Unary(const NodePtr _expr, std::string _position)
        : expr(_expr), position(_position)
    {}
public:
    virtual ~Unary()
    {
        delete expr;
    }

    virtual const char *getOpcode() const =0;

    NodePtr getExpr() const
    { return expr; }

    virtual void print(std::ostream &dst) const override
    {
        if(position == "pre")
        {
            dst << "( ";
            dst << getOpcode();
            expr->print(dst);
            dst << " )";
        }
        else
        {
            dst << "( ";
            expr->print(dst);
            dst << getOpcode();
            dst << " )";
        }
    }

    virtual void toPython(std::ostream &dst) const override
    {
        if(position == "pre")
        {
            dst << getOpcode();
            expr->print(dst);
        }
        else
        {
            expr->print(dst);
            dst << getOpcode();
        }
    }
};

class NegOperator
    : public Unary
{
public:
    NegOperator(const NodePtr _expr, std::string _position)
        : Unary(_expr, _position)
    {}

    virtual const char *getOpcode() const override
    { return "-"; }

    virtual double evaluate(
        const std::map<std::string, double> &bindings
    ) const override
    {
       
        //throw std::runtime_error("NegOperator::evaluate is not implemented.");
	 double vl=getExpr()->evaluate(bindings);
        
	return -vl;
    }
};

class IncrementOperator
    : public Unary
{
public:
    IncrementOperator(const NodePtr _expr, std::string _position)
        : Unary(_expr, _position)
    {}

    virtual const char *getOpcode() const override
    { return "++"; }

};

class DecrementOperator
    : public Unary
{
public:
    DecrementOperator(const NodePtr _expr, std::string _position)
        : Unary(_expr, _position)
    {}

    virtual const char *getOpcode() const override
    { return "--"; }

};

#endif
