#include "ast.hpp"

int main()
{
    const AST_Node *ast=parseAST();

    System *mySystem = new System();
    ast->toMips(std::cout, *mySystem, 2);
    std::cout << std::endl;
}