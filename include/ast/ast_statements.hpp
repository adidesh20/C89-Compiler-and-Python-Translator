
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
    NodePtr statement;
    NodePtr RestOf;
public:
    ~ScopeStatements () {}

    ScopeStatements (NodePtr _statement, NodePtr _RestOf)
     : statement(_statement), RestOf(_RestOf) {}

    virtual void print(std::ostream &dst) const override {
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        statement->print(dst);
        dst<<std::endl;
        if (RestOf != NULL) {
            RestOf->print(dst);
        }
    }

    virtual void toPython(std::ostream &dst) const override {
        for (int i = 0; i < currentIndent; i++) {
            dst << "\t";
        }
        statement->toPython(dst);
        dst<<std::endl;
        if (RestOf != NULL) {
            RestOf->toPython(dst);
        }
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
        statement->toMips(dst, mySystem, destReg);
        dst<<std::endl;
        if (RestOf != NULL) {
            RestOf->toMips(dst, mySystem, destReg);
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
        dst << "\t" << "b " << function_def_queue.back() << "_function_end_" << function_def_num << "\t\t #return" << std::endl;
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

        //if(loop_while == true){
    
        int current_loop = loop_count++;

        dst<<"while_loop_"<<current_loop<<"_begin:"<<"\t#Begin while loop"<<std::endl;
 
        Condition->toMips(dst, mySystem, destReg);
    
        dst<<"\t"<<"beq"<<"\t"<<"$0, $"<<mySystem.getRegName(destReg)<<", end_loop_"<<current_loop<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;

        std::string current_loop_end = "end_loop_" + std::to_string(current_loop);
        loop_ends.push_back(current_loop_end);

        Body->toMips(dst, mySystem, destReg);
  
        dst<<"\t"<<"b"<<"\t"<<"while_loop_"<<current_loop<<"_begin"<<std::endl;
        dst<<"\t"<<"nop"<<std::endl;
  
        dst<<current_loop_end<<":"<<"\t#End while loop"<<std::endl;
        loop_ends.pop_back();
        //}

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
        dst << "\t"<<"beq"<<"\t" << "$zero, $" << mySystem.getRegName(scratchRegs[0]) << ", else_"<<current_if_level << std::endl; 
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
         
            loop_for = true;
            if (Init != NULL)
                Init->toMips(dst, mySystem, destReg);
            dst<<"for_loop_"<<loop_count<<"_begin:"<<"\t#Begin for loop"<<std::endl;
            
            if (Condition != NULL)
                Condition->toMips(dst, mySystem, destReg);
       
            dst<<"\t"<<"beq"<<"\t"<<"$0, $"<<mySystem.getRegName(destReg)<<", end_loop_"<<loop_count<<std::endl;
            dst<<"\t"<<"nop"<<std::endl;

            std::string current_loop_end = "end_loop_" + std::to_string(loop_count);
            loop_ends.push_back(current_loop_end);
            if (Body != NULL)
                Body->toMips(dst, mySystem, destReg);
      

            dst<<"for_increment_"<<loop_count<<":"<<"\t#Increment stage of for loop"<<std::endl;
            if (Increment != NULL)
                Increment->toMips(dst, mySystem, destReg);

            dst<<"\t"<<"b"<<"\t"<<"for_loop_"<<loop_count<<"_begin"<<std::endl;

            dst<<"end_loop_"<<loop_count<<":"<<"\t#End for loop"<<std::endl;
            loop_ends.pop_back();

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

        dst<<"\t"<<"move"<<"\t"<<"$"<<mySystem.getRegName(freeParamReg[0]) <<", $"<<mySystem.getRegName(destReg)<<"\t\t #move parameter to arg reg"<<std::endl;

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
    NodePtr parameters;


  public:
    FunctionCall(std::string _name, NodePtr _parameters) : name(_name), parameters(_parameters) {}
    ~FunctionCall() {}

    virtual void print(std::ostream &dst) const override {
        dst << name << "(";
        if (parameters != NULL) {
            parameters->print(dst);
        }
        
        dst << ")";
    }

    virtual void toPython(std::ostream &dst) const {
        dst << name << "(";
        if (parameters != NULL) {
            parameters->toPython(dst);
        }

        dst << ")";
    }

      virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override {
       
        std::vector<int> lockedRegs = mySystem.temp_lockedRegLookup();
        int stack_bottom = localVarCount;
        
        function_call_queue.push_back(name);
        functionCallNum++;
        for (unsigned int i = 0; i < lockedRegs.size();i++) {
            stack_bottom++;
            dst << "\t"<<"sw"<< "\t"<< "$"<<mySystem.getRegName(lockedRegs[i])<<", "<<(stack_bottom)*4+16<< "($fp)";
            dst <<"\t\t#Storing used temporary register: "<<mySystem.getRegName(lockedRegs[i])<< std::endl;
        }

        if (parameters!=NULL)
            parameters->toMips(dst,mySystem,destReg);

       
        dst<<"\t"<<"jal"<<"\t"<<name<<"\t\t#Function call"<<std::endl;


        for ( int i = lockedRegs.size()-1; i >= 0;i--) {
            dst << "\t"<<"lw"<< "\t"<< "$"<<mySystem.getRegName(lockedRegs[i])<<", "<<(stack_bottom)*4+16<< "($fp)";
            dst <<"\t\t#Loading used temporary register: "<<mySystem.getRegName(lockedRegs[i])<< std::endl;
            dst<<"\t"<<"nop"<<std::endl;
            stack_bottom--;
        }

        
    }
    virtual int evaluate(System &mySystem) const override
    {
          return 0;
    }


   
  
};

class Break: public AST_Node
{
public:

    ~Break(){}

    Break(){}
    virtual void print(std::ostream &dst) const override
    {
        dst << "break;" << std::endl;
    }

    virtual void toPython(std::ostream &dst) const override
    {
        dst << "break" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(inSwitch)
        {
            switchFallthroughAllowed = false;
        }
        else
        {
            dst << "\t" << "b" << "\t" << loop_ends.back() << "\t\t #break" << std::endl;
        }
    }
};

class Continue: public AST_Node
{
public:

    ~Continue(){}

    Continue(){}

    virtual void print(std::ostream &dst) const override
    {
        dst << "continue;" << std::endl;
    }

    virtual void toPython(std::ostream &dst) const override
    {
        dst << "continue" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {
        if(loop_while)
        {
            dst << "\t" << "b" << "\t" << "while_loop_"<<loop_count-1<<"_begin" << "\t\t #continue to next iteration of while loop" << std::endl;
            return;
        }
        if(loop_for)
        {
            dst << "\t" << "b" << "\t" << "for_increment_" << loop_count << "\t\t #continue to next iteration of for loop"<<std::endl;
        }
    }
};

class Typedef: public AST_Node
{
public:
    std::string type;
    std::string newName;
    
    ~Typedef(){}
    Typedef(std::string _type, std::string _newName): type(_type), newName(_newName){}

    virtual void print (std::ostream &dst) const override
    {
        dst << "typedef " << type << " " << newName << ";" << std::endl;
    }

    virtual void toMips(std::ostream &dst, System &mySystem, int destReg) const override
    {

    }
   

};

#endif