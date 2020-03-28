#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include <iostream>
#include <cmath>
#include "ast.hpp"
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
    virtual const char *getOpcodePython() const {};

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
    
    virtual int evaluate (System &mySystem) const override
    {
        int vl = getLeft()->evaluate(mySystem);
        int vr = getRight()->evaluate(mySystem);
        return vl + vr;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if (isGlobal)
        {
            int evalLeft = getLeft()->evaluate(mySystem);
            int evalRight = getRight()->evaluate(mySystem);
            dst << evalLeft + evalRight;
        }
        else
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            mySystem.lockReg(scratchRegs[0]);
            mySystem.lockReg(scratchRegs[1]);

            int currentDepth = functionCallNum;
            getLeft()->toMips(dst, mySystem, scratchRegs[0]);
            if(currentDepth != functionCallNum)
            {
                dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
            }

            currentDepth = functionCallNum;
            getRight()->toMips(dst, mySystem, scratchRegs[1]);
            if(currentDepth != functionCallNum)
            {
                dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
            }
            dst << "\t"<< "addu"<< "\t"<< "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << std::endl;

            mySystem.unlockReg(scratchRegs[0]);
            mySystem.unlockReg(scratchRegs[1]);    
        
        }
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
    
    virtual int evaluate (System &mySystem) const override
        {
            int vl = getLeft()->evaluate(mySystem);
            int vr = getRight()->evaluate(mySystem);
            return vl - vr;
        }

     virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
        {
            if (isGlobal)
            {
                int evalLeft = getLeft()->evaluate(mySystem);
                int evalRight = getRight()->evaluate(mySystem);
                dst << evalLeft - evalRight;
            }
            else
            {
                std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
                mySystem.lockReg(scratchRegs[0]);
                mySystem.lockReg(scratchRegs[1]);

                int currentDepth = functionCallNum;
                getLeft()->toMips(dst, mySystem, scratchRegs[0]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }

                currentDepth = functionCallNum;
                getRight()->toMips(dst, mySystem, scratchRegs[1]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }
                dst << "\t"<< "subu"<< "\t"<< "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << std::endl;

                mySystem.unlockReg(scratchRegs[0]);
                mySystem.unlockReg(scratchRegs[1]);

                    
            }
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

  virtual int evaluate (System &mySystem) const override
        {
            int vl = getLeft()->evaluate(mySystem);
            int vr = getRight()->evaluate(mySystem);
            return vl * vr;
        }

     virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
        {
            if (isGlobal)
            {
                int evalLeft = getLeft()->evaluate(mySystem);
                int evalRight = getRight()->evaluate(mySystem);
                dst << evalLeft * evalRight;
            }
            else
            {
                std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
                mySystem.lockReg(scratchRegs[0]);
                mySystem.lockReg(scratchRegs[1]);

                int currentDepth = functionCallNum;
                getLeft()->toMips(dst, mySystem, scratchRegs[0]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }

                currentDepth = functionCallNum;
                getRight()->toMips(dst, mySystem, scratchRegs[1]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }
                dst << "\t" << "mult" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << std::endl;
                 dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;

                mySystem.unlockReg(scratchRegs[0]);
                mySystem.unlockReg(scratchRegs[1]);
            }
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

    virtual int evaluate (System &mySystem) const override
        {
            int vl = getLeft()->evaluate(mySystem);
            int vr = getRight()->evaluate(mySystem);
            return vl / vr;
        }

     virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
        {
            if (isGlobal)
            {
                int evalLeft = getLeft()->evaluate(mySystem);
                int evalRight = getRight()->evaluate(mySystem);
                dst << evalLeft / evalRight;
            }
            else
            {
                std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
                mySystem.lockReg(scratchRegs[0]);
                mySystem.lockReg(scratchRegs[1]);

                int currentDepth = functionCallNum;
                getLeft()->toMips(dst, mySystem, scratchRegs[0]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }

                currentDepth = functionCallNum;
                getRight()->toMips(dst, mySystem, scratchRegs[1]);
                if(currentDepth != functionCallNum)
                {
                    dst << "\t" << "move" << "\t" << "$" << mySystem.getRegName(scratchRegs[1]) << ", $" << mySystem.getRegName(destReg) << "\t\t #storing result of function call in temporary register" << std::endl;
                }
                dst << "\t" << "div" << "\t" << "$" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(scratchRegs[1]) << std::endl;
                 dst << "\t" << "mflo" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;

                mySystem.unlockReg(scratchRegs[0]);
                mySystem.unlockReg(scratchRegs[1]);
            }
        }
};

class ModOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "%"; }

    virtual const char *getOpcodePython() const override
    { return getOpcode(); }
public:
    ModOperator(NodePtr _left, NodePtr _right)
        : Operator(_left, _right)
    {}
    
    virtual int evaluate (System &mySystem) const override
    {
        int vl = getLeft()->evaluate(mySystem);
        int vr = getRight()->evaluate(mySystem);
        return vl % vr;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if (isGlobal)
        {
            int evalLeft = getLeft()->evaluate(mySystem);
            int evalRight = getRight()->evaluate(mySystem);
            dst << evalLeft % evalRight;
        }
        else
        {
            std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
            mySystem.lockReg(scratchRegs[0]);

            getLeft()->toMips(dst, mySystem, scratchRegs[0]);
            getRight()->toMips(dst, mySystem, destReg);
            
            dst << "\t" << "div" << "\t" << "$" << scratchRegs[0] << ", $" << mySystem.getRegName(destReg) << std::endl;
            dst << "\t" << "mfhi" << "\t" << "$" << mySystem.getRegName(destReg) << std::endl;

            mySystem.unlockReg(scratchRegs[0]); 
        
        }
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl==vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);
            
        dst << "\t" << "xor" << "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #comparison (==)" << std::endl;
        dst << "\t" << "sltiu" << "\t" << "$" <<mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", 1" << std::endl;

        mySystem.unlockReg(scratchRegs[0]); 
        
        
    }    
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl==vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {

        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);
            
        dst << "\t" << "xor" << "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #comparison (!=)" << std::endl;
        dst << "\t" << "sltu" << "\t" << "$" << mySystem.getRegName(destReg) << ", $zero , $" << mySystem.getRegName(destReg) << std::endl;

        mySystem.unlockReg(scratchRegs[0]); 

    }    

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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl < vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst << "\t" << "slt"<< "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #comparison (<)" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl <= vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst << "\t" << "slt"<< "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #comparison (<=) less than check" << std::endl;
        dst << "\t" << "xori" << "\t" << "$" << mySystem.getRegName(destReg) <<", $"<< mySystem.getRegName(destReg)<<", 1"<<"\t#equals check" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl > vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst << "\t" << "slt"<< "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << ", $" << mySystem.getRegName(destReg) << "\t\t #comparison (>)" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }

    
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl >= vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst << "\t" << "slt"<< "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #comparison (>=) less than check" << std::endl;
        dst << "\t" << "xori" << "\t" << "$" << mySystem.getRegName(destReg) <<", $"<< mySystem.getRegName(destReg)<<", 1"<<"\t#equals check" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }

    
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl || vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst<<"\t"<<"or"<<"\t"<<"$"<<mySystem.getRegName(destReg)<<", $"<<mySystem.getRegName(destReg)<<", $"<<mySystem.getRegName(scratchRegs[0])<<"\t#&& logical operator" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }

    
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

    virtual int evaluate(System &mySystem) const override
    {
        double vl=getLeft()->evaluate(mySystem);
        double vr=getRight()->evaluate(mySystem);
        return (vl && vr);
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);

        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, scratchRegs[0]);

        dst << "\t" << "and"<< "\t" << "$" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(destReg) << ", $" << mySystem.getRegName(scratchRegs[0]) << "\t\t #logical or" << std::endl;
        
        mySystem.unlockReg(scratchRegs[0]); 
    }
};


class BitwiseComplement : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return "~"; }

  public:
    BitwiseComplement(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        // double vl=left->evaluate(bindings);
        int vr=getRight()->evaluate(mySystem);
        return (~vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        getRight()->toMips(dst, mySystem, destReg);
        dst<<"\t"<<"nor"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $zero"<<"\t#~ operator" << std::endl;
    }  
};

class BitwiseOrOperator : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return "|"; }

  public:
    BitwiseOrOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        int vl=getLeft()->evaluate(mySystem);
        int vr=getRight()->evaluate(mySystem);
        return (vl|vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> freeReg = mySystem.temp_freeRegLookup(); //finds available registers
        mySystem.lockReg(freeReg[0]);                      //locks the registers for use of the function
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, freeReg[0]);
     
        //checks equivalence, if they are the same will result in zero
        dst<<"\t"<<"or"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $"<<freeReg[0]<<"\t#| bitwise operator" << std::endl;
        //or operation ensures that if the result is non zero it becomes 1
        mySystem.unlockReg(freeReg[0]);
    }   
};

class BitwiseXorOperator : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return "^"; }

  public:
    BitwiseXorOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        int vl=getLeft()->evaluate(mySystem);
        int vr=getRight()->evaluate(mySystem);
        return (vl^vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> freeReg = mySystem.temp_freeRegLookup(); //finds available registers
        mySystem.lockReg(freeReg[0]);                      //locks the registers for use of the function
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, freeReg[0]);
     
        //checks equivalence, if they are the same will result in zero
        dst<<"\t"<<"xor"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $"<<freeReg[0]<<"\t#^ bitwise operator" << std::endl;
        //or operation ensures that if the result is non zero it becomes 1
        mySystem.unlockReg(freeReg[0]);
    }   
};

class BitwiseAndOperator : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return "&"; }

  public:
    BitwiseAndOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        int vl=getLeft()->evaluate(mySystem);
        int vr=getRight()->evaluate(mySystem);
        return (vl|vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> freeReg = mySystem.temp_freeRegLookup(); //finds available registers
        mySystem.lockReg(freeReg[0]);                      //locks the registers for use of the function
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, freeReg[0]);
     
        //checks equivalence, if they are the same will result in zero
        dst<<"\t"<<"and"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $"<<freeReg[0]<<"\t#& bitwise operator" << std::endl;
        //or operation ensures that if the result is non zero it becomes 1
        mySystem.unlockReg(freeReg[0]);
    }   
};

class LeftShiftOperator : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return "<<"; }

  public:
    LeftShiftOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        int vl=getLeft()->evaluate(mySystem);
        int vr=getRight()->evaluate(mySystem);
        return (vl<<vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> freeReg = mySystem.temp_freeRegLookup(); //finds available registers
        mySystem.lockReg(freeReg[0]);                      //locks the registers for use of the function
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, freeReg[0]);
     
        //checks equivalence, if they are the same will result in zero
        dst<<"\t"<<"sllv"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $"<<freeReg[0]<<"\t# << operator" << std::endl;
        //or operation ensures that if the result is non zero it becomes 1
        mySystem.unlockReg(freeReg[0]);
    }   
};

class RightShiftOperator : public Operator {
  protected:
    virtual const char *getOpcode() const override
    { return ">>"; }

  public:
    RightShiftOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right)
    {}

    virtual int evaluate(System &mySystem) const override {
        int vl=getLeft()->evaluate(mySystem);
        int vr=getRight()->evaluate(mySystem);
        return (vl>>vr);
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> freeReg = mySystem.temp_freeRegLookup(); //finds available registers
        mySystem.lockReg(freeReg[0]);                      //locks the registers for use of the function
        getLeft()->toMips(dst, mySystem, destReg);
        getRight()->toMips(dst, mySystem, freeReg[0]);
     
        //checks equivalence, if they are the same will result in zero
        dst<<"\t"<<"srlv"<<"\t"<<"$"<<destReg<<", $"<<destReg<<", $"<<freeReg[0]<<"\t# << operator" << std::endl;
        //or operation ensures that if the result is non zero it becomes 1
        mySystem.unlockReg(freeReg[0]);
    }   
};


#endif
