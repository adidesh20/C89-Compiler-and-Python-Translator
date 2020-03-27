#ifndef COMMON_HPP
#define COMMON_HPP

//variables available to all files:
#include "ast.hpp"


#include <vector>
#include <string>

static std::vector<std::string> global_variables_names;
static int currentIndent = 0;
static int functionCallNum;
static int main_returned;

static unsigned int localVarCount;
static unsigned int globalVarCount;

static bool isGlobal;
static int stackVarCount;
static int paramCount = 16;
static std::vector<std::string> function_def_queue;
static int function_def_num;



static std::vector<std::string> function_call_queue;
static std::vector<std::string> loop_ends;


static int loop_count;
static int if_level;


static bool loop_while = false;
static bool loop_for = false;

#endif
