#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H
#include "type_shared.h"
#include "ASTNode.h"
#include <iostream>

namespace TC {
    Status check_program(AST::Program& prgm_root, class_table_t** outer_classes, var_table_t** outer_body);
}

#endif