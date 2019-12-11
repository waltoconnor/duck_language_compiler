#include "ASTNode.h"
#include "type_shared.h"
#include <iostream>

namespace TC{



Status process_statement_constructor(AST::Statement* statement, class_n_t& cur_class, class_table_t& class_table, var_table_t& this_table, var_table_t& local_table){
    //std::cout << "Processing constructor for " << cur_class << std::endl;
    return statement->process_type_constructor(cur_class, class_table, this_table, local_table);
}

Status process_statement(AST::Statement* statement, class_n_t& cur_class, class_table_t& class_table, var_table_t& this_table, var_table_t& local_table){
    return statement->process_type(cur_class, class_table, this_table, local_table);
}

Status process_block_constructor(AST::Block* block_node, class_n_t& cur_class, class_table_t& class_table, var_table_t& this_table, var_table_t& local_table){
    //std::cout << "process block constructor this_table size=" << this_table.size() << std::endl;
    //std::cout << "process_block_constructor local table size " << local_table.size() << std::endl;
    Status last_stat = CHANGE;
    int i = 0;
    while(last_stat == CHANGE){
        last_stat = GOOD;
        for(auto s : block_node->get_statements()){
            Status statement_stat = process_statement_constructor(s, cur_class, class_table, this_table, local_table);
            if(statement_stat == FAIL){
                //std::cout << "process_block_constructor statement_stat FAIL" << std::endl;
                return FAIL;
            }
            if(statement_stat == CHANGE){
                last_stat = CHANGE;
            }
        }
        i++;
        if(i > class_table.size()){
            std::cout << "process_block_constructor has iterated [CLASS_TABLE_SIZE] times, fixed point reached" << std::endl;
            return GOOD;
        }
    }
    
    return GOOD;
}

Status process_block(AST::Block* block_node, class_n_t& cur_class, class_table_t& class_table, var_table_t& this_table, var_table_t& local_table){
    Status last_stat = CHANGE;
    int i = 0;
    while(last_stat == CHANGE){
        last_stat = GOOD;
        for(auto s : block_node->get_statements()){
            
            Status statement_stat = process_statement(s, cur_class, class_table, this_table, local_table);
            if(statement_stat == FAIL){
                std::cout << "process_block statement_stat FAIL" << std::endl;
                return FAIL;
            }
            if(statement_stat == CHANGE){
                last_stat = CHANGE;
            }
        }
        i++;
        if(i > class_table.size()){
            std::cout << "process_block_constructor has iterated [CLASS_TABLE_SIZE] times, fixed point reached" << std::endl;
            return GOOD;
        }
    }
    
    return GOOD;
}

//constructors are fully processed by end of warmup
Status warmup_constructor(AST::Constructor* constructor_node, Constructor& constructor_data, class_n_t& cur_class, class_table_t& class_table, var_table_t& ivar_table){
    auto formals = constructor_node->get_formals();
    //constructor_data.node = constructor_node;
    //std::cout << "warmup constructor ivar_table size=" << ivar_table.size() << std::endl;
    for(auto f : formals->get_formals()){
        constructor_data.locals->emplace(std::make_pair(f->get_name(), f->get_type_name()));
        //std::cout << "warmup_constructor " << cur_class << " added formal " << f->get_name() << ":" << f->get_type_name() << std::endl;
        constructor_data.formals->push_back(f->get_type_name());
    }
    
    //std::cout << "warmup constructor ivar_table size2=" << ivar_table.size() << std::endl;
    return process_block_constructor(constructor_node->get_block(), cur_class, class_table, ivar_table, *constructor_data.locals);
}

Status warmup_method(AST::Method* method_node, Method& method_data, class_n_t& cur_class, class_table_t& class_table, var_table_t& ivar_table){
    auto formals = method_node->get_formals();
    //method_data.node = method_node;

    for(auto f : formals->get_formals()){
        method_data.locals->emplace(std::make_pair(f->get_name(), f->get_type_name()));
        method_data.formals->push_back(f->get_type_name());
    }

    method_data.return_type = method_node->get_return_type_name();

    return GOOD;
    //return process_block(method_node->get_block(), class_table, ivar_table, method_data.locals);
}

Status process_method(AST::Method* method_node, Method& method_data, class_n_t& cur_class, class_table_t& class_table, var_table_t& ivar_table){
    return process_block(method_node->get_block(), cur_class, class_table, ivar_table, *method_data.locals);
}

Status warmup_methods(AST::Methods* methods_node, method_table_t& method_table, class_n_t& cur_class, class_table_t& class_table, var_table_t& ivar_table){
    for(auto m : methods_node->get_methods()){
        Method* method_data = new Method(m->get_name(), cur_class);
        Status method_stat = warmup_method(m, *method_data, cur_class, class_table, ivar_table);
        if(method_stat == FAIL){
            std::cout << "warmup_method method_stat FAIL" << std::endl;
            return FAIL;
        }
        class_table.at(cur_class)->append_method(std::make_pair(m->get_name(), method_data));
        //method_table.emplace(std::make_pair(m->get_name(), method_data));
    }
    return GOOD;
}

Status process_methods(AST::Methods* methods_node, method_table_t& method_table, class_n_t& cur_class, class_table_t& class_table, var_table_t& ivar_table){
    for(auto m : methods_node->get_methods()){
        Method* method_data = method_table.at(m->get_name());
        Status procstat = process_method(m, *method_data, cur_class, class_table, ivar_table);
        if(procstat == FAIL){
            std::cout << "METHOD FAILED PROCESSING: " << m->get_name() << std::endl;
            return FAIL;
        }
    }
    return GOOD;
}

Status warmup_class(AST::Class* class_node, Class& class_data, class_table_t& class_table){
    std::cout << "::::Warming up class " << class_data.name << std::endl;
    auto constructor = new Constructor(class_data.name);
    class_data.constructor = constructor;
    //std::cout << "warmup class ivar_table size=" << class_data.ivars.size() << std::endl;
    //class_data.ivars.emplace(std::make_pair(std::string("DUMMY_IVAR"), std::string("Obj")));
    Status constwarmup = warmup_constructor(class_node->get_constructor(), *constructor, class_node->get_class_name(), class_table, *class_data.ivars);
    if(constwarmup == FAIL){
        std::cout << "warmup_class constwarmup FAIL" << std::endl;
        return FAIL;
    }
    //class_data.node = class_node;
    print_constructor(class_data.name, *constructor);
    auto ivar_name = std::string("INSTANCE VARS");
    print_var_table(ivar_name, *class_data.ivars);

    Status methodwarmup = warmup_methods(class_node->get_methods(), *class_data.methods, class_node->get_class_name(), class_table, *class_data.ivars);
    if(methodwarmup == FAIL){
        std::cout << "warmup_class methodwarmup FAIL" << std::endl;
        return FAIL;
    }
    std::cout << "::::END CLASS WARMUP" << std::endl;

    return GOOD;
}

Status process_class(AST::Class* class_node, Class& class_data, class_table_t& class_table){
    std::cout << "::::Processing class " << class_data.name << std::endl;
    Status stat = process_methods(class_node->get_methods(), *class_data.methods, class_node->get_class_name(), class_table, *class_data.ivars);
    std::cout << "::::END PROCESS CLASS" << std::endl;
    print_full_class(class_data);
    
    return stat;
}

Status warmup_class_table(AST::Classes& classes, class_table_t& class_table){

    //TODO: CLASSES MUST BE PROCESSED IN TOPO SORT ORDER STARTING WITH OBJ
    for(auto c : classes.get_elements()){
        auto class_data = class_table.at(c->get_class_name());
        Status stat = warmup_class(c, *class_data, class_table);
        if(stat == FAIL){
            //std::cout << "warmup_class_table stat FAIL" << std::endl;
            return FAIL;
        }
    }
    return GOOD;
}

Status process_class_table(AST::Classes& classes, class_table_t& class_table){
    for(auto c : classes.get_elements()){
        auto class_data = class_table.at(c->get_class_name());
        Status stat = process_class(c, *class_data, class_table);
        if(stat == FAIL){
            //std::cout << "process class table stat FAIL" << std::endl;
            return FAIL;
        }
    }
    return GOOD;
}

Status populate_class_table(AST::Classes& classes, class_table_t& table){
    for(auto c : classes.get_elements()){
        Class* class_data = new Class(c->get_class_name());
        std::cout << "Adding class " << c->get_class_name() << ": (" << c->get_super_name() << ")" << std::endl;
        table.emplace(std::make_pair(c->get_class_name(), class_data));
    }
    for(auto c : classes.get_elements()){
        Class& cur_class = *table.at(c->get_class_name());
        if(table.find(c->get_super_name()) == table.end()){
            //std::cout << "populate_class_table table.find(c->get_super_name()) : "<< c->get_super_name() << " FAIL" << std::endl;
            return FAIL;
        }
        cur_class.parent = &(c->get_super_name());
    }
}

void insert_default_stuff(class_table_t& class_table){
    auto obj_class_name = new std::string("Obj");
    auto int_class_name = new std::string("Int");
    auto ntg_class_name = new std::string("Nothing");
    auto bol_class_name = new std::string("Boolean");
    auto str_class_name = new std::string("String");

    auto obj_arg_name = new std::string("arg_Obj");
    auto int_arg_name = new std::string("arg_Int");
    auto ntg_arg_name = new std::string("arg_Nothing");
    auto bol_arg_name = new std::string("arg_Boolean");
    auto str_arg_name = new std::string("arg_String");

    Class* obj_class = new Class(*obj_class_name);
    Class* int_class = new Class(*int_class_name);
    Class* ntg_class = new Class(*ntg_class_name);
    Class* bol_class = new Class(*bol_class_name);
    Class* str_class = new Class(*str_class_name);

    int_class->parent = obj_class_name;
    ntg_class->parent = obj_class_name;
    bol_class->parent = obj_class_name;
    str_class->parent = obj_class_name;

    std::string* print_name  = new std::string("PRINT");
    std::string* string_name = new std::string("STRING");
    std::string* equals_name = new std::string("EQUALS");
    std::string* less_name   = new std::string("LESS");
    std::string* plus_name   = new std::string("PLUS");

    Constructor* obj_constructor = new Constructor(*obj_class_name);

    Method* obj_STRING = new Method(*string_name, *obj_class_name);
    obj_STRING->return_type = *str_class_name;
    
    Method* obj_PRINT = new Method(*print_name, *obj_class_name);
    obj_PRINT->return_type = *obj_class_name;

    Method* obj_EQUALS = new Method(*equals_name, *obj_class_name);
    obj_EQUALS->formals->push_back(*obj_class_name);
    obj_EQUALS->return_type = *bol_class_name;

    obj_class->constructor = obj_constructor;
    obj_class->append_method(std::make_pair(*string_name, obj_STRING));
    obj_class->append_method(std::make_pair(*print_name,  obj_PRINT ));
    obj_class->append_method(std::make_pair(*equals_name, obj_EQUALS));
    

    class_table.emplace(std::make_pair(*obj_class_name, obj_class));

    Constructor* int_constructor = new Constructor(*int_class_name);

    Method* int_STRING = new Method(*string_name, *int_class_name);
    int_STRING->return_type = *str_class_name;
    
    //Method* int_PRINT = new Method(*print_name, *int_class_name);
    Method* int_PRINT = new Method(*print_name, *obj_class_name);
    //int_PRINT->return_type = *int_class_name;
    int_PRINT->return_type = *obj_class_name;

    Method* int_EQUALS = new Method(*equals_name, *int_class_name);
    //int_EQUALS->formals->push_back(*int_class_name);
    int_EQUALS->formals->push_back(*obj_class_name);
    int_EQUALS->return_type = *bol_class_name;

    Method* int_LESS = new Method(*less_name, *int_class_name);
    int_LESS->formals->push_back(*int_class_name);
    int_LESS->return_type =      *bol_class_name;

    Method* int_ADD = new Method(*plus_name, *int_class_name);
    int_ADD->formals->push_back(*int_class_name);
    int_ADD->return_type =      *int_class_name;

    int_class->constructor = int_constructor;
    int_class->append_method(std::make_pair(*string_name, int_STRING));
    int_class->append_method(std::make_pair(*print_name,  int_PRINT ));
    int_class->append_method(std::make_pair(*equals_name, int_EQUALS));
    int_class->append_method(std::make_pair(*less_name,   int_LESS  ));
    int_class->append_method(std::make_pair(*plus_name,   int_ADD   ));

    class_table.emplace(std::make_pair(*int_class_name, int_class));

    Method* bol_STRING = new Method(*string_name, *bol_class_name);
    bol_STRING->return_type = *str_class_name;

    Method* bol_PRINT = new Method(*print_name, *obj_class_name);
    //Method* bol_PRINT = new Method(*print_name, *bol_class_name);
    //bol_PRINT->return_type = *bol_class_name;
    bol_PRINT->return_type = *obj_class_name;

    Method* bol_EQUALS = new Method(*equals_name, *obj_class_name);
    //Method* bol_EQUALS = new Method(*equals_name, *bol_class_name);
    bol_EQUALS->return_type = *bol_class_name;
    bol_EQUALS->formals->push_back(*bol_class_name);

    Constructor* bol_constructor = new Constructor(*bol_class_name);

    bol_class->constructor = bol_constructor;
    bol_class->append_method(std::make_pair(*string_name, bol_STRING));
    bol_class->append_method(std::make_pair(*print_name,  bol_PRINT ));
    bol_class->append_method(std::make_pair(*equals_name, bol_EQUALS));
    
    class_table.emplace(std::make_pair(*bol_class_name, bol_class));

    Constructor* str_constructor = new Constructor(*str_class_name);

    Method* str_STRING = new Method(*string_name, *str_class_name);
    str_STRING->return_type = *str_class_name;
    
    Method* str_PRINT = new Method(*print_name, *str_class_name);
    str_PRINT->return_type = *obj_class_name;
    //str_PRINT->return_type = *str_class_name;

    Method* str_EQUALS = new Method(*equals_name, *obj_class_name);
    //Method* str_EQUALS = new Method(*equals_name, *str_class_name);
    str_EQUALS->formals->push_back(*str_class_name);
    str_EQUALS->return_type = *bol_class_name;

    Method* str_ADD = new Method(*plus_name, *str_class_name);
    str_ADD->formals->push_back(*str_class_name);
    str_ADD->return_type =      *str_class_name;

    Method* str_LESS = new Method(*less_name, *str_class_name);
    str_LESS->formals->push_back(*str_class_name);
    str_LESS->return_type = *bol_class_name;

    str_class->constructor = str_constructor;
    str_class->append_method(std::make_pair(*string_name, str_STRING));
    str_class->append_method(std::make_pair(*print_name,  str_PRINT ));
    str_class->append_method(std::make_pair(*equals_name, str_EQUALS));
    str_class->append_method(std::make_pair(*plus_name,   str_ADD   ));
    str_class->append_method(std::make_pair(*less_name,   str_LESS  ));

    class_table.emplace(std::make_pair(*str_class_name, str_class));



    Method* ntg_STRING = new Method(*string_name, *ntg_class_name);
    ntg_STRING->return_type = *str_class_name;

    //Method* ntg_PRINT = new Method(*print_name, *ntg_class_name);
    //ntg_PRINT->return_type = *ntg_class_name;
    Method* ntg_PRINT = new Method(*print_name, *obj_class_name);
    ntg_PRINT->return_type = *obj_class_name;

    Method* ntg_EQUALS = new Method(*equals_name, *obj_class_name);
    //Method* ntg_EQUALS = new Method(*equals_name, *ntg_class_name);
    ntg_EQUALS->return_type = *bol_class_name;
    //ntg_EQUALS->formals->push_back(*ntg_class_name);

    Constructor* ntg_constructor = new Constructor(*ntg_class_name);

    ntg_class->constructor = ntg_constructor;
    ntg_class->append_method(std::make_pair(*string_name, ntg_STRING));
    ntg_class->append_method(std::make_pair(*print_name,  ntg_PRINT ));
    ntg_class->append_method(std::make_pair(*equals_name, ntg_EQUALS));
    

    class_table.emplace(std::make_pair(*ntg_class_name, ntg_class));

    auto main_class_obj = new Class(MAIN_NAME);
    //no methods
    Constructor* main_constructor = new Constructor(MAIN_NAME);
    main_class_obj->constructor = main_constructor;
    class_table.emplace(std::make_pair(MAIN_NAME, main_class_obj));
}

Status check_program(AST::Program& prgm_root, class_table_t** outer_classes, var_table_t** outer_body){
    class_table_t* class_table = new class_table_t();
    var_table_t* body_table = new var_table_t();
    *outer_classes = class_table;
    *outer_body = body_table;

    //INSERT DEFAULT CLASSES IN TO CLASS TABLE
    insert_default_stuff(*class_table);

    for(auto c : *class_table){
        print_full_class(*c.second);
    }

    populate_class_table(prgm_root.classes_, *class_table);

    

    //PUT ALL THE BASE CLASSES IN THE CLASS TABLE
    Status class_warmup_stat = warmup_class_table(prgm_root.classes_, *class_table);
    if(class_warmup_stat == FAIL){
        std::cout << "check_program class_warmup_stat FAIL" << std::endl;
        return FAIL;
    }
    Status proc_stat = process_class_table(prgm_root.classes_, *class_table);
    if(proc_stat == FAIL){
        std::cout << "check_program proc_stat FAIL" << std::endl;
        return FAIL;
    }

    std::cout << std::endl << std::endl << "=======FINAL CLASS LIST=======" << std::endl;
    for(auto c : *class_table){
        print_full_class(*c.second);
    }
    std::cout << "======CHECKING MAIN BODY======" << std::endl;
    Status body_stat = process_block(&prgm_root.statements_, MAIN_NAME, *class_table, *body_table, *body_table);
    if(body_stat == FAIL){
        std::cout << "check_program body_stat FAIL" << std::endl;
        auto body_name = std::string("MAIN BODY TABLE");
        print_var_table(body_name, *body_table);
    }

    return body_stat;
}

} //END NAMESPACE TC