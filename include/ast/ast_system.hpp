#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <string>
#include <vector>
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
    Memory systemMemory;
    bool registerStatus[32];

    ~System(){}

    System()
    {
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

    void unlockReg(unsigned int reg)
    {
        if(reg > 3 && reg < 26)
        {
            std::cout << "\t\t\t\t #unlock $" << reg << std::endl;
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
            std::cout << "\t\t\t\t #lock $" << reg << std::endl;
            registerStatus[reg] = false;
        }
        else
        {
            throw std::runtime_error("Illegal Register Access");
        }
        
    }
};

#endif