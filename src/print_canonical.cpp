#include "ast.hpp"
#include "common.hpp"
int main()
{
    const AST_Node *ast=parseAST();

    ast->print(std::cout);
    std::cout<<std::endl;

    return 0;
}
