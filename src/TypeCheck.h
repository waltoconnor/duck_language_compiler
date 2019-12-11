#ifndef TYPECHECK_H
#define TYPECHECK_H
#include <vector>
#include <unordered_map>
#include <string>
#include "Shared.h"

namespace AST{
    class Class;
};

namespace TC {

    static type_t TYPE_FAIL = std::string("TYPE_FAIL");
    static type_t MAIN_CLASS = std::string("_MAIN_");
    static method_name_t MAIN_METHOD = std::string("main");
    static type_t OBJ_CLASS = std::string("Obj");
    static type_t VIRTUAL_ROOT = std::string("_VIRTUAL_ROOT_CLASS_");

    class Context{
        std::unordered_map<type_t, AST::Class*> class_map;
        std::vector<AST::Class*> class_list;
        AST::Class* main_;
    public:
        AST::Class* get_class(type_t name){
            if(name.compare(MAIN_CLASS) == 0){
                return main_;
            }
            return class_map.at(name);
        }

    };
};

#endif