#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "ast_memory.hpp"
#include "common.hpp"


/*
MIPS Registers:
$0      |       $0      |       Always Zero
$1      |       $at     |       Pseudo Instructions
$2      |       $v0     |       Return Values
$3      |       $v1     |       Return Values
$4      |       $a0     |       Function Arguments
$5      |       $a1     |       Function Arguments
$6      |       $a2     |       Function Arguments
$7      |       $a3     |       Function Arguments
$8      |       $t0     |       Temporary
$9      |       $t1     |       Temporary
$10     |       $t2     |       Temporary
$11     |       $t3     |       Temporary
$12     |       $t4     |       Temporary
$13     |       $t5     |       Temporary
$14     |       $t6     |       Temporary
$15     |       $t7     |       Temporary
$16     |       $s0     |       Saved (Preserved)
$17     |       $s1     |       Saved (Preserved)
$18     |       $s2     |       Saved (Preserved)
$19     |       $s3     |       Saved (Preserved)
$20     |       $s4     |       Saved (Preserved)
$21     |       $s5     |       Saved (Preserved)
$22     |       $s6     |       Saved (Preserved)
$23     |       $s7     |       Saved (Preserved)
$24     |       $t8     |       Temporary
$25     |       $t9     |       Temporary
$26     |       $k0     |       Kernal
$27     |       $k1     |       Kernal
$28     |       $gp     |       Global Area Pointer
$29     |       $sp     |       Stack Pointer
$30     |       $fp     |       Frame Pointer
$31     |       $ra     |       Return Address
*/

class System
{
public:
    std::map<int, std::string> registerNames;
    Memory systemMemory;
    bool registerStatus[32];
    std::vector<std::string> functionParameters;
    

    ~System(){}

    System()
    {
        registerNames_init();
        localVarCount = 0;
        globalVarCount = 0;
        for(int i = 0; i < 32; i++)
        {
            if((i <= 3) || (i >= 26))
            {
                registerStatus[i] = false;
            }
            else
            {
                registerStatus[i] = true;
            }
            
        }
    }

    std::string getRegName(int reg)
    {
        return registerNames.find(reg)->second;
    }

    void unlockReg(unsigned int reg)
    {
        if(reg > 3 && reg < 26)
        {
            std::cout << "#unlock $" << getRegName(reg) << std::endl;
            registerStatus[reg] = true;
        }
        else
        {
            throw std::runtime_error("Illegal Register Access");
        }
        
    }

    void lockReg(unsigned int reg)
    {
        if(reg > 3 && reg < 26)
        {
            std::cout << "#lock $" << getRegName(reg) << std::endl;
            registerStatus[reg] = false;
        }
        else
        {
            throw std::runtime_error("Illegal Register Access");
        }
        
    }

    std::vector<int> all_freeRegLookup(int min, int max)
    {
        std::vector<int> available;
        for(int i = min; i <= max; i++)
        {
            if(registerStatus[i] == true)
            {
                available.push_back(i);
            }
        }
        if(available.size() == 0)
        {
            throw std::runtime_error("Unable to find free registers in requested range");
        }

        return available;
    }

    std::vector<int> all_lockedRegLookup(int min, int max)
    {
        std::vector<int> unavailable;
        for(int i = min; i <= max; i++)
        {
            if(registerStatus[i] == false)
            {
                unavailable.push_back(i);
            }
        }

        return unavailable;
    }

    std::vector<int> temp_freeRegLookup()
    {
        std::vector<int> available;
        std::vector<int> temp1 = all_freeRegLookup(8,15);
        std::vector<int> temp2 = all_freeRegLookup(24,25);

        for(int i = 0; i < temp1.size(); i++)
        {
            available.push_back(temp1[i]);
        }
        for(int i = 0; i < temp2.size(); i++)
        {
            available.push_back(temp2[i]);
        }

        return available;
    }

    std::vector<int> temp_lockedRegLookup()
    {
        std::vector<int> unavailable;
        std::vector<int> temp1 = all_lockedRegLookup(8,15);
        std::vector<int> temp2 = all_lockedRegLookup(24,25);

        for(int i = 0; i < temp1.size(); i++)
        {
            unavailable.push_back(temp1[i]);
        }
        for(int i = 0; i < temp2.size(); i++)
        {
            unavailable.push_back(temp2[i]);
        }

        return unavailable;
    }

    std::vector<int> args_freeRegLookup()
    {
        return all_freeRegLookup(4,7);
    }

    std::vector<int> preserved_freeRegLookup()
    {
        return all_freeRegLookup(16,23);
    }

    void pushParameter(std::string param)
    {
        if(functionParameters.size() > 4)
        {
            throw std::runtime_error("More than four parameters");
        }
        else
        {
            functionParameters.push_back(param);
        }
    }

    int lookupParameter(std::string param)
    {
        for(int i = 0; i < functionParameters.size(); i++)
        {
            if(param == functionParameters[i])
            {
                return i;
            }
        }
        return -1;
    }
   
    void resetParameters()
    {
        functionParameters.clear();
    }

    

    void NewGlobalVar (std::string varname) {
        systemMemory.globalvariables.emplace(varname,++globalVarCount);

    }

    void DeleteLocalVar (std::string varname) {
        systemMemory.localvariables.erase(varname);
    }

    void NewLocalVar (std::string varname) {
       
        systemMemory.localvariables.emplace(varname,localVarCount);
    }

     void store_var_val(std::string var, int val) {
         
        systemMemory.variable_bindings.erase(var);
        systemMemory.variable_bindings.emplace(var, val);
    }

    void clear_var_val(std::string var) {
        systemMemory.variable_bindings.erase(var);
    }

    int find_var_val(std::string var) {
        if (systemMemory.variable_bindings.find(var) != systemMemory.variable_bindings.end())
        {
            return (systemMemory.variable_bindings.find(var)->second);
        }
        return 0;
    }

    void resetArgRegs()
    {
        for(int i = 4; i <= 7; i++)
        {
            registerStatus[i] = true;
        }
    }



private:
    void registerNames_init()
    {
        registerNames.insert(std::pair<int, std::string>(0, "zero"));
        registerNames.insert(std::pair<int, std::string>(1, "at"));
        registerNames.insert(std::pair<int, std::string>(2, "v0"));
        registerNames.insert(std::pair<int, std::string>(3, "v1"));
        registerNames.insert(std::pair<int, std::string>(4, "a0"));
        registerNames.insert(std::pair<int, std::string>(5, "a1"));
        registerNames.insert(std::pair<int, std::string>(6, "a2"));
        registerNames.insert(std::pair<int, std::string>(7, "a3"));
        registerNames.insert(std::pair<int, std::string>(8, "t0"));
        registerNames.insert(std::pair<int, std::string>(9, "t1"));
        registerNames.insert(std::pair<int, std::string>(10, "t2"));
        registerNames.insert(std::pair<int, std::string>(11, "t3"));
        registerNames.insert(std::pair<int, std::string>(12, "t4"));
        registerNames.insert(std::pair<int, std::string>(13, "t5"));
        registerNames.insert(std::pair<int, std::string>(14, "t6"));
        registerNames.insert(std::pair<int, std::string>(15, "t7"));
        registerNames.insert(std::pair<int, std::string>(16, "s0"));
        registerNames.insert(std::pair<int, std::string>(17, "s1"));
        registerNames.insert(std::pair<int, std::string>(18, "s2"));
        registerNames.insert(std::pair<int, std::string>(19, "s3"));
        registerNames.insert(std::pair<int, std::string>(20, "s4"));
        registerNames.insert(std::pair<int, std::string>(21, "s5"));
        registerNames.insert(std::pair<int, std::string>(22, "s6"));
        registerNames.insert(std::pair<int, std::string>(23, "s7"));
        registerNames.insert(std::pair<int, std::string>(24, "t8"));
        registerNames.insert(std::pair<int, std::string>(25, "t9"));
        registerNames.insert(std::pair<int, std::string>(26, "k0"));
        registerNames.insert(std::pair<int, std::string>(27, "k1"));
        registerNames.insert(std::pair<int, std::string>(28, "gp"));
        registerNames.insert(std::pair<int, std::string>(29, "sp"));
        registerNames.insert(std::pair<int, std::string>(30, "fp"));
        registerNames.insert(std::pair<int, std::string>(31, "ra"));


    }
};



#endif