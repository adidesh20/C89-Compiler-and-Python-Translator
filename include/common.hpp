#ifndef COMMON_HPP
#define COMMON_HPP

//variables available to all files:
#include "ast.hpp"


#include <vector>
#include <string>

static std::vector<NodePtr> global_variables_values;
static std::vector<std::string> global_variables_names;

static int currentIndent = 0;
static int function_call_num;
static int main_returned;

static unsigned int localVarCount;
static unsigned int globalVarCount;

static bool isGlobal;
static int stackVarCount;
static int paramCount = 16;

#endif
