//
// Created by Michal Young on 10/21/19.
//

#ifndef QUACK_IMPL_SYMBOLS_H
#define QUACK_IMPL_SYMBOLS_H

#include <string>

/* Symbol table information ---
 * information from the AST that we keep
 * in table form for type inference and
 * code generation.
 */

class Symbol {
    std::string name;
    /* Eventually there will be more. */
};

/* Depth first class list is a list of the classes in a program in
 * depth first order, starting with the root class Object.  This is
 * the order in which we need to generate code for classes, as well
 * as the best order for type inference.
 *
 */
class DFClassList {

};



#endif //QUACK_IMPL_SYMBOLS_H
