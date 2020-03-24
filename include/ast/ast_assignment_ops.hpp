#ifndef ASS_OP_HPP
#define ASS_OP_HPP

#include "ast.hpp"
#include <string>
#include <iostream>
#include <cmath>

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
};

#endif