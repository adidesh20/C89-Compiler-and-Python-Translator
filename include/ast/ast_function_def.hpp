#ifndef F_DEFINITIONS_HPP
#define F_DEFINITIONS_HPP

#include <iostream>
#include<vector>
#include <string>
#include "ast.hpp"
#include "common.hpp"



extern std::vector<std::string> global_variables_names;


extern int function_call_num;
extern int main_returned;

class Program : public AST_Node
{
  private:
    NodePtr ExternalDec;
    NodePtr Rest_of_program;

public:
    ~Program() {}

    Program (NodePtr _ExternalDec, NodePtr _Rest_of_program)  {
        ExternalDec=_ExternalDec;
        Rest_of_program=_Rest_of_program;
    
        
        function_call_num = 0;
        main_returned = false;
    }

    virtual void print (std::ostream &out) const override {
        ExternalDec->print(out);
        if (Rest_of_program != NULL) {
            Rest_of_program->print(out);
        }
    }

    virtual void toPython (std::ostream &out) const override {
        ExternalDec->toPython(out);
        if (Rest_of_program != NULL) {
            Rest_of_program->toPython(out);
        }
    }   

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        ExternalDec->toMips(dst, mySystem, destReg);
        if (Rest_of_program != NULL) {
            Rest_of_program->toMips(dst, mySystem, destReg);
        }
    }

};

class Function_Declaration: public AST_Node
{
public:
    std::string returnType;
    std::string functionIdentifier;
    NodePtr parameters;


    Function_Declaration(std::string _returnType, std::string _functionIdentifier, NodePtr _parameters)
    {
        returnType = _returnType;
        functionIdentifier = _functionIdentifier;
        parameters = _parameters;
    }

    virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << functionIdentifier << " (";
        if(parameters != NULL)
        {
            parameters->print(out);
        }
        out << ");" << std::endl;
    } 
    virtual void toPython(std::ostream &out) const override
    { 
        out <<std::endl;
    }



    ~Function_Declaration(){}
};

class Function_Definition: public AST_Node
{
public:
    std::string returnType;
    std::string functionIdentifier;
    NodePtr parameters;
    NodePtr functionImplementation;

    Function_Definition(std::string _returnType, std::string _functionIdentifier, NodePtr _parameters, NodePtr _functionImplementation)
    {
        returnType = _returnType;
        functionIdentifier = _functionIdentifier;
        parameters = _parameters;
        functionImplementation = _functionImplementation;
    }

    virtual void print(std::ostream &out) const override
    {
        out << returnType << " " << functionIdentifier << " (";
        if(parameters != NULL)
        {
            parameters->print(out);
        }
        out << ")";
        if(functionImplementation != NULL)
        {
            functionImplementation->print(out);
        }
        out << std::endl;
    } 

    virtual void toPython(std::ostream &out) const override
    {
        out << "def " << functionIdentifier <<"(";
        if(parameters != NULL)
        {
            parameters->toPython(out);
        }
        out <<"):" << std::endl;
        if(functionImplementation == NULL)
        {
            out << std::endl << "\t" << "pass" << std::endl;
        }
        else
        {
            //Deal with Globals Variables first
            for(int i = 0; i< global_variables_names.size(); i++)
            {
                out << std::endl;
                for(int j = 0; j < currentIndent; j++)
                {
                    out << "\t";
                }
                out << "\t" << "global " << global_variables_names[i] << std::endl;
            }

            //Then toPython to python
            functionImplementation->toPython(out);
        }

        //Main Case
        if(functionIdentifier == "main")
        {
            out << std::endl;
            out << "if __name__ == \"__main__\": " << std::endl;
            out << "\timport sys" << std::endl;
            out << "\tret=main()" << std::endl;
            out << "\tsys.exit(ret)" << std::endl;
        }
        
       


        
    }
};



class Parameter_list: public AST_Node {
private:
    NodePtr Parameter;
    NodePtr Rest_of_Parameters;
public:
    ~Parameter_list() {}

    Parameter_list(NodePtr _Parameter, NodePtr _Rest_of_Parameters) {
        Parameter=_Parameter;
        Rest_of_Parameters=_Rest_of_Parameters;

    }

    virtual void print (std::ostream &out) const override {
        Parameter->print(out);
        if (Rest_of_Parameters != NULL) {
            out << ",";
            Rest_of_Parameters->print(out);
        }
    }

    virtual void toPython (std::ostream &out) const override {
        Parameter->toPython(out);
        if (Rest_of_Parameters != NULL) {
            out << ",";
            Rest_of_Parameters->toPython(out);
        }
    }

 

};

class Parameter: public AST_Node {
private:
    std::string Type;
    std::string Var;

public:
    ~Parameter () {}

    Parameter(std::string _Type, std::string _Var){
        Type=_Type;
        Var=_Var;
    }

    virtual void print (std::ostream &out) const override {
        out << Type <<" "<< Var;
    }

    virtual void toPython (std::ostream &out) const override {
        out<< Var;
    }

   
};

#endif