#ifndef V_DEFINITIONS_HPP 
#define V_DEFINITIONS_HPP

#include <iostream>
#include <vector>
#include <string>
#include "ast.hpp"
#include "common.hpp"

class GloTypeset: public AST_Node {
public:
    std::string type;
    NodePtr VarDefList;

    ~GloTypeset() {}

    GloTypeset(std::string _type, NodePtr _VarDefList) {
        type=_type;
        VarDefList=_VarDefList;

    }

    virtual void print (std::ostream &dst) const override {
        
            dst << type;
           VarDefList->print(dst);
           dst << ";" << std::endl;
        
    }

    virtual void toPython (std::ostream &dst) const override {
        
        VarDefList->toPython(dst);
        dst << std::endl;
    }

     virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        
        VarDefList->toMips(dst, mySystem, destReg);
        dst << std::endl;
    }

 

};
class GloVarList: public AST_Node {
private:
    NodePtr Var;
    NodePtr Rest_of_Vars;
public:
    ~GloVarList() {}

    GloVarList(NodePtr _Var, NodePtr _Rest_of_Vars) {
        Var=_Var;
        Rest_of_Vars=_Rest_of_Vars;

    }

    virtual void print (std::ostream &dst) const override {
        Var->print(dst);
        if (Rest_of_Vars != NULL) {
            dst << ",";
            Rest_of_Vars->print(dst);
            
        }
         
    }

    virtual void toPython (std::ostream &dst) const override {
        Var->toPython(dst);
        if (Rest_of_Vars != NULL) {
            
            Rest_of_Vars->toPython(dst);
            
        }
    }

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        Var->toMips(dst, mySystem, destReg);
        if (Rest_of_Vars != NULL) {
            
            Rest_of_Vars->toMips(dst, mySystem, destReg);
            
        }
    }

 

};

class GlobalVariable_Definition: public AST_Node
{
public:
   
    std::string varIdentifier;
    NodePtr varValue;

    GlobalVariable_Definition(std::string _varIdentifier, NodePtr _varValue)
    {
        
        varIdentifier = _varIdentifier;
        varValue = _varValue;
        global_variables_names.push_back(varIdentifier);
        
    }

    virtual void print(std::ostream &out) const override
    {
        out <<" "<< varIdentifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
       
    }

    virtual void toPython(std::ostream &out) const override
    {
        out << varIdentifier << " = ";
        if(varValue == NULL)
        {
            out<< "0" << std::endl;
        }
        else
        {
           varValue->toPython(out);
        }
        
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
        {
            dst << "\t"<< ".globl "<<varIdentifier << std::endl;
            dst << "\t"<< ".data" << std::endl;
            if (varValue != NULL)
            {
                dst << varIdentifier << ":" << std::endl;
                dst << "\t"<< ".word"<< "\t";
                isGlobal = true;
                varValue->toMips(dst, mySystem, destReg);
                isGlobal = false;
                dst<< "\t#New global variable"<< std::endl;

                mySystem.store_var_val(varIdentifier, varValue->evaluate());
             
            }
            else {
                dst<<varIdentifier<<":"<<std::endl;
                dst<<"\t"<<".word"<<"\t"<<"0" << "\t#New global variable"<< std::endl;

            }

            mySystem.NewGlobalVar(varIdentifier);
        }
    

    ~GlobalVariable_Definition(){}


};


class LocalTypeset: public AST_Node {
public:
    std::string type;
    NodePtr VarDefList;

    ~LocalTypeset() {}

    LocalTypeset(std::string _type, NodePtr _VarDefList) {
        type=_type;
        VarDefList=_VarDefList;

    }

    virtual void print (std::ostream &dst) const override {
        
            dst << type;
           VarDefList->print(dst);
           dst << ";" << std::endl;
        
    }

    virtual void toPython (std::ostream &dst) const override {
        
        VarDefList->toPython(dst);
        dst << std::endl;
    }

 

};
class LocalVarList: public AST_Node {
private:
    NodePtr Var;
    NodePtr Rest_of_Vars;
public:
    ~LocalVarList() {}

    LocalVarList(NodePtr _Var, NodePtr _Rest_of_Vars) {
        Var=_Var;
        Rest_of_Vars=_Rest_of_Vars;

    }

    virtual void print (std::ostream &dst) const override {
        Var->print(dst);

        if (Rest_of_Vars != NULL) {
        
            dst << ",";
            Rest_of_Vars->print(dst);
            
        }
         
    }

    virtual void toPython (std::ostream &dst) const override {
        Var->toPython(dst);
        if (Rest_of_Vars != NULL) {
            for(int j = 0; j < currentIndent; j++)
            {
                dst << "\t";
            }
            
            Rest_of_Vars->toPython(dst);
            
        }
    }

 

};

class LocalVariable_Definition: public AST_Node
{
public:
   
    std::string varIdentifier;
    NodePtr varValue;

    LocalVariable_Definition(std::string _varIdentifier, NodePtr _varValue)
    {
        
        varIdentifier = _varIdentifier;
        varValue = _varValue;
        
        
    }

    virtual void print(std::ostream &out) const override
    {
        out <<" "<< varIdentifier;
        if(varValue != NULL)
        {
            out << " = ";
            varValue->print(out);
        }
       
    }

    virtual void toPython(std::ostream &out) const override
    {
        out << varIdentifier << " = ";
        if(varValue == NULL)
        {
            out<< "0" << std::endl;
        }
        else
        {
           varValue->toPython(out);
        }
        
    }

    ~LocalVariable_Definition(){}


};

#endif