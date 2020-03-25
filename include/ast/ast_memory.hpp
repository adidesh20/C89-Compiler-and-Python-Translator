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
};

#endif