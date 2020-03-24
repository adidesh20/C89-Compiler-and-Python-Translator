#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include <iostream>
#include <cmath>
class Operator
    : public AST_Node
{
private:
    NodePtr left;
    NodePtr right;
protected:
    Operator(NodePtr _left, NodePtr _right)
        : left(_left)
        , right(_right)
    {}
public:
    virtual ~Operator()
    {
        delete left;
        delete right;
    }

    virtual const char *getOpcode() const =0;
    virtual const char *getOpcodePython() const =0;

    NodePtr getLeft() const
    { return left; }

    NodePtr getRight() const
    { return right; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<"( ";
        left->print(dst);
        dst<<" ";
        dst<<getOpcode();
        dst<<" ";
        right->print(dst);
        dst<<" )";
    }

    virtual void toPython(std::ostream &dst) const override
    {
        dst<<"( ";
        left->toPython(dst);
        dst<<" ";
        dst<<getOpcodePython();
        dst<<" ";
        right->toPython(dst);
        dst<<" )";
    }
};

    // ARITHMETIC OPERATORS //

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    AddOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override 
    {
        // TODO-C : Run bin/eval_expr with something like 5+a, where a=10, to make sure you understand how this works
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl+vr;
    }
};

class SubOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "-"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    SubOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override 
    {
        // TODO-D : Implement this, based on AddOperator::evaluate
        //throw std::runtime_error("MulOperator::evaluate is not implemented.");
	double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl-vr;
    }
};


class MulOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "*"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    MulOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
         //throw std::runtime_error("MulOperator::evaluate is not implemented.");
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl*vr;
    }
};

class DivOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "/"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    DivOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        //throw std::runtime_error("DivOperator::evaluate is not implemented.");
       double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return vl/vr;
    }
};

/*
class ExpOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "^"; }
public:
    ExpOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        //throw std::runtime_error("ExpOperator::evaluate is not implemented.");
        double vl=getLeft()->evaluate(bindings);
        double vr=getRight()->evaluate(bindings);
        return std::pow(vl,vr);
    }
};
*/

/*
class TernaryOperator : public AST_Node {
  private:
    NodePtr condition;
    NodePtr con_true;
    NodePtr con_false;
  public:
    
    ~TernaryOperator() {}
    
    TernaryOperator(NodePtr _condition, NodePtr _con_true, NodePtr _con_false)
     : condition(_condition), con_true(_con_true), con_false(_con_false) {}
    
    virtual void print(std::ostream &dst) const override {
        condition->print(dst);
        dst<<" ? ";
        if (con_true != NULL) 
            con_true->print(dst);
        dst<<" : ";
        con_false->print(dst);
    }

    
    
};
*/
    // COMPARISON OPERATORS //

class EqualToOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "=="; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    EqualToOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class NotEqualToOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "!="; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    NotEqualToOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class LessThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "<"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    LessThanOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class LessThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "<="; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    LessThanEqualOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class GreaterThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return ">"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    GreaterThanOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class GreaterThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return ">="; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    GreaterThanEqualOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

    // LOGICAL OPERATORS //

class LogicalOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "||"; }

    virtual const char *getOpcodePython() const override
    { return "or"; }
public:
    LogicalOrOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};

class LogicalAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "&&"; }

    virtual const char *getOpcodePython() const override
    { return "and"; }
public:
    LogicalAndOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
};



#endif
