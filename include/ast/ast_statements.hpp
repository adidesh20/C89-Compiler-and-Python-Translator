
#ifndef ast_statements_hpp
#define ast_statements_hpp
#include "ast.hpp"
#include "common.hpp"

extern int currentIndent;
class ScopeBody : public AST_Node {
private:
    NodePtr Body;
public:
    ~ScopeBody () {}

    ScopeBody (NodePtr _Body) : Body(_Body) {}

    virtual void print(std::ostream &dst) const override {
        dst << "{ " << std::endl;
        // std::cout<<"scope level: "<<currentIndent<<std::endl;
        currentIndent++;
        if (Body!=NULL)
            Body->print(dst);
        currentIndent--;
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        dst << "}";
    }

    virtual void toPython(std::ostream &dst) const override {
        dst << std::endl;
        currentIndent++;
        if (Body != NULL)
            Body->toPython(dst);
        currentIndent--;
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        dst << std::endl;
        currentIndent++;
        if (Body != NULL)
            Body->toMips(dst,mySystem, destReg);
        currentIndent--;
      
	}

   
};

class ScopeStatements: public AST_Node {
private:
    NodePtr Singular_statement;
    NodePtr Rest_of_statements;
public:
    ~ScopeStatements () {}

    ScopeStatements (NodePtr _Singular_statement, NodePtr _Rest_of_statements)
     : Singular_statement(_Singular_statement), Rest_of_statements(_Rest_of_statements) {}

    virtual void print(std::ostream &dst) const override {
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        Singular_statement->print(dst);
        dst<<std::endl;
        if (Rest_of_statements != NULL) {
            Rest_of_statements->print(dst);
        }
    }

    virtual void toPython(std::ostream &dst) const override {
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        Singular_statement->toPython(dst);
        dst<<std::endl;
        if (Rest_of_statements != NULL) {
            Rest_of_statements->toPython(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        Singular_statement->toMips(dst, mySystem, destReg);
        dst<<std::endl;
        if (Rest_of_statements != NULL) {
            Rest_of_statements->toMips(dst, mySystem, destReg);
        }
    }

};

class ReturnStatement : public AST_Node {
public:
    NodePtr return_val;

    ~ReturnStatement() {}
    ReturnStatement (NodePtr _return_val): return_val(_return_val) {}

    virtual void print(std::ostream &dst) const override {
        dst<<"return";
        if (return_val != NULL) {
            dst<<" ";
            return_val->print(dst);
            dst<<";";
        }
        else {
            dst<<";";
        }
    }

    virtual void toPython(std::ostream &dst) const override {
        dst << "return";
        if (return_val != NULL) {
            dst << " ";
            return_val->toPython(dst);
        }
    }

    
};

class WhileStatement: public AST_Node {
private:
    NodePtr Condition;
    NodePtr Body;

public:
    ~WhileStatement() {}
    WhileStatement (NodePtr _Condition, NodePtr _Body) : Condition(_Condition) , Body(_Body) {}

    virtual void print (std::ostream &dst) const override {
        dst << "while(";
        Condition->print(dst);
        dst <<") ";
        Body->print(dst);
    }

    virtual void toPython (std::ostream &dst) const override {
        dst<< "while(";
        Condition->toPython(dst);
        dst<<"): ";
        Body->toPython(dst);
    }

 
};
class NoBraces : public AST_Node
{
  private:
    NodePtr Body;

  public:
    ~NoBraces() {}

    NoBraces(NodePtr _Body) : Body(_Body) {}

    virtual void print(std::ostream &dst) const override
    {
    

        currentIndent++;
        for (int i = 0; i < currentIndent; i++)  
        { 
            dst << "\t"; 
        }
        Body->print(dst);
        currentIndent--;
        for (int i = 0; i < currentIndent; i++)  
        { 
            dst << "\t"; 
        }

    }

    virtual void toPython(std::ostream &dst) const override
    {
        dst << std::endl;
        currentIndent++;
        for (int i = 0; i < currentIndent; i++)  
        { 
            dst << "\t"; 
        }
        Body->toPython(dst);
        currentIndent--;
        for (int i = 0; i < currentIndent; i++)  
        { 
            dst << "\t"; 
        }

    }

};
class IfElseStatement : public AST_Node {
private:
    NodePtr Condition;
    NodePtr IfBody;
    NodePtr ElseBody;

public:
    ~IfElseStatement () {}

    IfElseStatement (NodePtr _Condition, NodePtr _IfBody, NodePtr _ElseBody) : Condition(_Condition), IfBody(_IfBody), ElseBody(_ElseBody) {}

    virtual void print (std::ostream &dst) const override {
        dst << "if (";
        Condition->print(dst);
        dst<<") ";
        IfBody->print(dst);
        if (ElseBody != NULL) {
            dst<<std::endl;
            for (int i = 0; i < currentIndent; i++) 
            {
                dst<<"\t";
            }
            dst<<"else";
            ElseBody->print(dst);
        }

    }

    virtual void toPython(std::ostream &dst) const override {
        dst<< "if(";
        Condition->toPython(dst);
        dst<<"):";
        IfBody->toPython(dst);

        if(ElseBody != NULL){
            dst<<std::endl;
            for(int i = 0; i < currentIndent; i++)
            {
                dst<<"\t";
            }
            dst<<"else:";
            ElseBody->toPython(dst);
        }
    }

};


class Parameter_In_List : public AST_Node {
private:
    NodePtr Parameter;
    NodePtr RestOf;

public:
    ~Parameter_In_List() {}

    Parameter_In_List (NodePtr _Parameter, NodePtr _RestOf) : Parameter(_Parameter), RestOf(_RestOf) {}

    virtual void print (std::ostream &dst) const override {
        Parameter->print(dst);
        if (RestOf != NULL) {
            dst << ",";
            RestOf->print(dst);
        }
    }

    virtual void toPython (std::ostream &dst) const override {
        Parameter->toPython(dst);
        if (RestOf != NULL) {
            dst << ",";
            RestOf->toPython(dst);
        }
    }

  
};


class FunctionCall : public AST_Node
{
  private:
    std::string name;
    NodePtr arg;


  public:
    FunctionCall(std::string _name, NodePtr _arg) : name(_name), arg(_arg) {     
    }
    ~FunctionCall() {}

    virtual void print(std::ostream &dst) const override {
        dst << name << "(";
        if (arg != NULL) {
            arg->print(dst);
        }
        
        dst << ")";
    }

    virtual void toPython(std::ostream &dst) const {
        dst << name << "(";
        if (arg != NULL) {
            arg->toPython(dst);
        }

        dst << ")";
    }


   
  
};

#endif