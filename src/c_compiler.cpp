#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
#include "common.hpp"

int main(int argc, char* argv[])
{
    freopen(argv[2], "r", stdin); //Opening C file

    const AST_Node *ast = parseAST(); //Generating parse tree
    std::cout << std::endl;
    
    std::string option = argv[1];
    if(option == "--print")
    {
        std::cout << "Printing parsed C code: " << std::endl;
        ast->print(std::cout);
    }
    else if(option == "--translate")
    {
        std::cout << "Converting " << argv[2] << " to Python..." << std::endl;
        freopen(argv[4], "w", stdout);
        ast->toPython(std::cout);
        std::cout << std::endl;
    }
    else if(option == "-S")
    {
        std::cout << "Compiling " << argv[2] << " to MIPS..." << std::endl;
        freopen(argv[4], "w", stdout);
        System *mySystem = new System;
        ast->toMips(std::cout, *mySystem, 2);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
        std::exit(-1);
    }
    

    fclose(stdin);
    fclose(stdout);

    return 0;
    
    
}