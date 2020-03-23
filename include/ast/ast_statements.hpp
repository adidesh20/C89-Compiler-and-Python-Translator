
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

};

#endif
