#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_node.hpp"
#include "ast/ast_primitives.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_unary.hpp"
//#include "ast/ast_functions.hpp"
#include "ast/ast_statements.hpp"
#include "ast/ast_function_def.hpp"
#include "ast/ast_variable_def.hpp"
#include "ast/ast_assignment_ops.hpp"
#include "ast/ast_system.hpp"
#include "ast/ast_memory.hpp"

extern const AST_Node *parseAST();

#endif
