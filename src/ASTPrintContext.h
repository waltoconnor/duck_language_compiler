//
// Created by Michal Young on 10/25/19.
//

#ifndef QUACK_IMPL_ASTPRINTCONTEXT_H
#define QUACK_IMPL_ASTPRINTCONTEXT_H

namespace AST {
    // Same namespace as ASTNode.h.

    // Json conversion and pretty-printing can pass around a print context object
    // to keep track of indentation, and possibly other things.
    class AST_print_context {
    public:
        int indent_; // Number of spaces to place on left, after each newline
        AST_print_context() : indent_{0} {};

        void indent() { ++indent_; }

        void dedent() { --indent_; }
    };
}
#endif //QUACK_IMPL_ASTPRINTCONTEXT_H
