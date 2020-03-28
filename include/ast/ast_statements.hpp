
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
        dst << "#Current Scope Level = " << currentIndent << std::endl;
        if (Body != NULL)
            Body->toMips(dst,mySystem, destReg);
        currentIndent--;
        dst << "#Current Scope Level = " << currentIndent << std::endl;
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(return_val != NULL)
        {
            return_val->toMips(dst, mySystem, destReg);
        }
        dst << "\t" << "b " << function_def_queue.back() << "_function_end_" << function_def_num << "\t #return" << std::endl;
        if(function_def_queue.back() == "main")
        {
            main_returned = true;
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        loop_while = true;

        if(loop_while == true){
        
        //use a free register for condition check
 
        int current_loop = loop_count++;

        dst<<"while_loop_"<<current_loop<<"_begin:"<<"\t#Begin while loop"<<std::endl;
        
        //evalute the condition into the free register
        Condition->toMips(dst, mySystem, destReg);
        //branch to end if condition evaluates false (0)
        dst<<"\t"<<"beq"<<"\t"<<"$0, $"<<destReg<<", end_loop_"<<current_loop<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;

        std::string current_loop_end = "end_loop_" + std::to_string(current_loop);
        loop_ends.push_back(current_loop_end);

        Body->toMips(dst, mySystem, destReg);
        // loop_count--;


        //branch back to top
        dst<<"\t"<<"b"<<"\t"<<"while_loop_"<<current_loop<<"_begin"<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;
        //end of loop
        dst<<current_loop_end<<":"<<"\t#End while loop"<<std::endl;
        loop_ends.pop_back();
        }

        loop_while = false;
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        Body->toMips(dst,mySystem,destReg);
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        std::vector<int> scratchRegs = mySystem.temp_freeRegLookup();
        mySystem.lockReg(scratchRegs[0]);
        int current_if_level = if_level++;
        
        Condition->toMips(dst, mySystem, scratchRegs[0]);
        dst << "\t"<<"beq"<<"\t" << "$zero, $" << mySystem.getRegName(scratchRegs[0]) << ", else_"<<current_if_level << std::endl; //$else condition yet to be filled
		dst << "\t"<<"nop"<<"\t" << std::endl;
		mySystem.unlockReg(scratchRegs[0]);

        if(IfBody != NULL){
            IfBody->toMips(dst, mySystem, destReg);
        }

        dst<<"\t"<<"b"<<"\t"<<"ifelse_end_"<<current_if_level<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;

        dst<<"else_"<<current_if_level<<":"<<std::endl;
        if(ElseBody != NULL){
            ElseBody->toMips(dst, mySystem, destReg);
        }

        dst<<"ifelse_end_"<<current_if_level<<":"<<std::endl;
        
    }
};

class ForStatement: public AST_Node {
private:
    NodePtr Init;
    NodePtr Condition;
    NodePtr Increment;
    NodePtr Body;

public:
    ~ForStatement() {}

    ForStatement (NodePtr _Init, NodePtr _Condition, NodePtr _Increment, NodePtr _Body)
     : Init(_Init), Condition(_Condition) , Increment(_Increment), Body(_Body) {}

    virtual void print (std::ostream &dst) const override {
        dst << "for(";
        Init->print(dst);
        Condition->print(dst);
        dst<<";";
        Increment->print(dst);
        dst<<")"<<std::endl;
        dst<<"\t";
        Body->print(dst);        
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {

        loop_for = true;

        if(loop_for == true){
            //use a free register for condition check
            // std::vector<int> freeRegs = mySystem.FindFreeConstantRegs();
            // mySystem.set_used(freeRegs[0]);

            loop_for = true;
            if (Init != NULL)
                Init->toMips(dst, mySystem, destReg);
            dst<<"for_loop_"<<loop_count<<"_begin:"<<"\t#Begin for loop"<<std::endl;
            
            //evalute the condition into the free register
            if (Condition != NULL)
                Condition->toMips(dst, mySystem, destReg);
            //branch to end if condition evaluates false (0)
            dst<<"\t"<<"beq"<<"\t"<<"$0, $"<<destReg<<", end_loop_"<<loop_count<<std::endl;
            dst<<"\t"<<"nop"<<std::endl;

            std::string current_loop_end = "end_loop_" + std::to_string(loop_count);
            loop_ends.push_back(current_loop_end);
            if (Body != NULL)
                Body->toMips(dst, mySystem, destReg);
            // loop_count--;

            dst<<"for_increment_"<<loop_count<<":"<<"\t#Increment stage of for loop"<<std::endl;
            if (Increment != NULL)
                Increment->toMips(dst, mySystem, destReg);

            //branch back to top
            dst<<"\t"<<"b"<<"\t"<<"for_loop_"<<loop_count<<"_begin"<<std::endl;
            dst<<"\t"<<"nop"<<std::endl;
            //end of loop
            dst<<"end_loop_"<<loop_count<<":"<<"\t#End for loop"<<std::endl;
            loop_ends.pop_back();
            // mySystem.set_unused(freeRegs[0]);
            loop_count++;
        }

        loop_for = false;
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

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {

        std::vector<int> freeParamReg = mySystem.all_freeRegLookup(4, 7);
        mySystem.lockReg(freeParamReg[0]);

        Parameter->toMips(dst, mySystem, destReg);

        dst<<"\t"<<"move"<<"\t"<<"$"<<mySystem.getRegName(freeParamReg[0]) <<", $"<<destReg<<"\t #move param to arg reg"<<std::endl;

        if (RestOf != NULL) {
            RestOf->toMips(dst, mySystem, destReg);
        }

        mySystem.unlockReg(freeParamReg[0]);
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

      virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        functionCallNum++;
        function_call_queue.push_back(name);
        std::vector<int> lockedRegs = mySystem.temp_lockedRegLookup();
        int stack_count = localVarCount;
        
        //store temp registers before function call in stack
        for (unsigned int i = 0; i < lockedRegs.size();i++) {
            stack_count++;
            dst << "\t"<<"sw"<< "\t"<< "$"<<lockedRegs[i]<<", "<<(stack_count)*4+16<< "($fp)";
            dst <<"\t#Storing temp register: "<<lockedRegs[i]<< std::endl;
        }

        if (arg!=NULL)
            arg->toMips(dst,mySystem,destReg);

        //function call
        dst<<"\t"<<"jal"<<"\t"<<name<<"\t#Function called"<<std::endl;

        //restore temp registers
        for ( int i = lockedRegs.size()-1; i >= 0;i--) {
            dst << "\t"<<"lw"<< "\t"<< "$"<<lockedRegs[i]<<", "<<(stack_count)*4+16<< "($fp)";
            dst <<"\t#Loading temp register: "<<lockedRegs[i]<< std::endl;
            dst<<"\t"<<"nop"<<std::endl;
            stack_count--;
        }
    }


   
  
};

#endif