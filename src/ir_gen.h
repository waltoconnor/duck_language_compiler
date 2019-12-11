#ifndef IR_GEN_H
#define IR_GEN_H
#include <sstream>
#include <unordered_set>
#include <fstream>
#include "ASTNode.h"
#include "type_shared.h"

/*  STATEMENT_LIST:
 *  Assign
 *  AssignDeclare (subclass to assign)
 *  Load (Expr)
 *  Return
 *  If
 *  While
 *  Typecase
 *  IntConst (Expr)
 *  StrConst (Expr)
 *  NothingConst (Expr)
 *  Construct (Expr)
 *  Call (Expr)
 *  BinOp (Expr)
 *  And (BinOp)
 *  Or (BinOp)
 *  Not (Expr)
 *  
 *  LEXPR_LIST:
 *  Dot
 *  Ident
 * 
 *  WE dynamic_cast<> TO EXTRACT THE TREE DATA FROM ITS OOP HELL
 *  AST::LExpr* lexpr_ = ...;
 *  if(dynamic_cast<AST::Dot*>(lexpr_) != nullptr){
 *      //handle Dot
 *      
 *  }
 *  else if(dynamic_cast<AST::Ident*>(lexpr_) != nullptr){
 *      //handle Ident
 *  }
 *  else{
 *      //print error
 *  }
 *  
 */


typedef std::stringstream head_t;

const std::string indent = std::string("    ");

void generate_program_ir(AST::Program* root, TC::class_table_t* tc_classes, TC::var_table_t* body_table, const std::string& outpath);

#endif