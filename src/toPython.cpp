#include "ast.hpp"

int main () {
    //Lexer debugging 
    extern int yy_flex_debug;
    yy_flex_debug = 0;  //set to 1 to debug

    const AST_Node *ast=parseAST();

    ast->toPython(std::cout);
    std::cout<<std::endl;

    
    return 0;
}