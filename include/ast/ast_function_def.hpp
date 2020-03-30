#ifndef F_DEFINITIONS_HPP
#define F_DEFINITIONS_HPP

#include <iostream>
#include<vector>
#include <string>
#include "ast.hpp"
#include "common.hpp"






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
    
        
        functionCallNum = 0;
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

     virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {}



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


    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        function_def_num++;
        function_def_queue.push_back(functionIdentifier);
        if (functionIdentifier == "main") {
            function_call_queue.insert(function_call_queue.begin(),"main");
        }
       
        dst<<"\t"<<".text"<<std::endl;
        dst<<std::endl;
        dst<<"#----------FUNCTION "<<functionIdentifier<<"----------"<<std::endl;
    
    
        dst<<"\t"<<".globl"<<"\t"<<functionIdentifier<<std::endl;
        
        dst<<"\t"<<".ent"<<"\t"<<functionIdentifier<<std::endl;
        dst<<"\t"<<".type"<<"\t"<<functionIdentifier<<", @function"<<std::endl;

        dst<<functionIdentifier<<":"<<std::endl;
        //space allocated in stack
        dst<<"#ALLOCATING STACK"<<std::endl;
        
        if (functionImplementation != NULL)
        {
            std::ostream tmp(0);
            functionImplementation->print(tmp);
           
        }
        int stack_end = (stackVarCount*4) +78;
        dst<<"\t"<<"addiu"<<"\t"<<"$sp, $sp,-"<<stack_end<<std::endl; //restoring sp
        dst<<"\t"<<"sw"<<"\t"<<"$ra,"<<stack_end-4<<"($sp)"<<std::endl; //store return address at end of stack frame
        dst<<"\t"<<"sw"<<"\t"<<"$fp,"<<stack_end-8<<"($sp)"<<std::endl; //old fp = top of stack address - 4
        dst<<"\t"<<"move"<<"\t"<<"$fp, $sp"<<std::endl;

        
        if(parameters != NULL){
            mySystem.resetArgRegs();
            
            for(int i =4; i<8; i++){
                dst<<"\t"<<"sw"<<"\t"<<"$"<<mySystem.getRegName(i)<<", "<<stack_end+(4*(i-4))<<"($fp)"<<"\t"<<"#storing param argument register"<<std::endl;
            }
           
            parameters->toMips(dst, mySystem, destReg);
        }

        if(functionImplementation != NULL){
            functionImplementation->toMips(dst, mySystem, destReg);
        }
        else if(functionIdentifier == "main" && functionImplementation == NULL){
            dst<<"\t"<<"move"<<"\t"<<"$v0, $0"<<std::endl; //empty main should return zero in $2
        }
        else{
            dst<<"\t"<<"nop"<<"\t"<<std::endl; //if a function is declared as empty
        }

        
        dst<<functionIdentifier<<"_function_end_"<<function_def_num<<":"<<std::endl;
        dst<<"#DEALLOCATING STACK"<<std::endl;

        if(parameters != NULL){
            //int stack_end = (var_count*4) +parameter_count+12+50;
            int argreg = 4;
            for(int i =0; i<4; i++){
                dst<<"\t"<<"lw"<<"\t"<<"$"<<argreg++<<", "<<stack_end+(4*i)<<"($fp)"<<"\t"<<"#loading param argument register"<<std::endl;
                dst<<"\t"<<"nop"<<std::endl;
            }
        }

        dst<<"\t"<<"move"<<"\t"<<"$sp, $fp"<<std::endl; //deallocating stack
        dst<<"\t"<<"lw"<<"\t"<<"$ra,"<<stack_end-4<<"($sp)"<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;
        dst<<"\t"<<"lw"<<"\t"<<"$fp,"<<stack_end-8<<"($sp)"<<std::endl; //old fp = top of stack address - 4
        dst<<"\t"<<"nop"<<std::endl;
        dst<<"\t"<<"addiu"<<"\t"<<"$sp, $sp,"<<stack_end<<std::endl; //restoring sp
        //returns 0 if no return defined for main
        if (functionIdentifier == "main" && main_returned == false) {
            dst<<"\t"<<"li"<<"\t"<<"$2"<<", 0"<<"\t#No return defined, return 0 by default"<<std::endl;
        }
        dst<<"\t"<<"j"<<"\t"<<"$ra"<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;
        dst<<std::endl;
        

        dst<<"\t"<<".end"<<"\t" <<functionIdentifier<<std::endl;
        function_def_queue.pop_back();
        stackVarCount=0;
        
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

    virtual void toMips (std::ostream &dst, System &mySystem, int destReg) const override {
        Parameter->toMips(dst, mySystem, destReg);
        if (Rest_of_Parameters != NULL) {
            Rest_of_Parameters->toMips(dst, mySystem, destReg);
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        mySystem.pushParameter(Var);

        
    } 

   
};

#endif