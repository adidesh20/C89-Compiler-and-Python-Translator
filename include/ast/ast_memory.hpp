#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Memory
{
public:
    std::unordered_map<std::string, unsigned int> localVarBindings;
    std::unordered_map<std::string, unsigned int> globalVarBindings;
    std::unordered_map<std::string, unsigned int> variableValues;

    ~Memory(){}
    Memory(){}

    int variableSearch(std::string identifier, int currentScope)
    {
        for(int i = currentScope; i >= 0; i--)
        {
            std::string key =  identifier = std::to_string(currentScope);
            if(localVarBindings.find(key) != localVarBindings.end())
            {
                return (((localVarBindings.find(key))->second-1)*4 + 16); //returning memory address
            }
        }
        return 0;

    }
};

#endif