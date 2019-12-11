#include "type_shared.h"
#include <iostream>
namespace TC {
std::vector<std::string*>* get_super_list(class_table_t& class_table, class_n_t& child_class){
    if(child_class.compare(MAIN_NAME) == 0){
        std::vector<std::string*>* vec = new std::vector<std::string*>();
        vec->push_back((std::string*) &FAIL_NAME);
        return vec;
    }
    if(child_class.compare("Obj") == 0){
        std::vector<std::string*>* vec = new std::vector<std::string*>();
        auto ostr = new std::string("Obj");
        vec->push_back(ostr);
        return vec;
    }

    if(class_table.find(child_class) == class_table.end()){
        auto errvec = new std::vector<std::string*>();
        errvec->push_back((std::string*) &FAIL_NAME);
        return errvec;
    }

    auto class_obj = class_table.at(child_class);
    auto recursive_vec = get_super_list(class_table, *(class_obj->parent));
    recursive_vec->push_back((std::string*) &child_class);
    return recursive_vec;
}

class_n_t* LCA(class_table_t& class_table, class_n_t& c1, class_n_t& c2){
    if(c1.compare(MAIN_NAME) == 0 || c2.compare(MAIN_NAME) == 0){
        return &FAIL_NAME;
    }
    if(c1.compare(FAIL_NAME) == 0 || c2.compare(FAIL_NAME) == 0){
        std::cout << "LCA GOT FAIL NAME c1="<<c1<<", c2=" << c2 << std::endl;
        return &FAIL_NAME;
    }
    auto c1_vec = get_super_list(class_table, c1);
    auto c2_vec = get_super_list(class_table, c2);
    
    for(int i = c1_vec->size() - 1; i >= 0; i--){
        for(int j = c2_vec->size() - 1; j >= 0; j--){
            if(c1_vec->at(i)->compare(*(c2_vec->at(j))) == 0){
                auto cache = c1_vec->at(i);
                delete c1_vec;
                delete c2_vec;
                return cache;
            }
        }
    }
    delete c1_vec;
    delete c2_vec;

    return &FAIL_NAME;
}


bool is_subclass_of(class_table_t& class_table, class_n_t& subclass, class_n_t& superclass){
    if(subclass.compare(MAIN_NAME) == 0 || superclass.compare(MAIN_NAME) == 0){
        return false;
    }
    
    //if either don't exist, fail
    if(class_table.find(subclass) == class_table.end() || class_table.find(superclass) == class_table.end()){
        return false;
    }

    //all objects are subclasses of themselves
    if(subclass.compare(superclass) == 0){
        return true;
    }

    //if the superclass is not obj but the subclass is, fail
    if(subclass.compare("Obj") == 0){
        return false;
    }
    
    auto subclass_o = class_table.at(subclass);

    return is_subclass_of(class_table, *(subclass_o->parent), superclass);
}

class_n_t& recursive_lookup(class_table_t& class_table, const std::string& cur_class, const std::string& ident){
    if(cur_class.compare(MAIN_NAME) == 0){
        return FAIL_NAME;
    }
    
    //std::cout << "doing recursive lookup on " << cur_class << std::endl;
    if(class_table.find(cur_class) == class_table.end()){
        std::cout << "recursive_lookup did not find cur class " << cur_class << " in table " << std::endl;
        return FAIL_NAME;
    }
    auto cur_class_obj = class_table.at(cur_class);
    if(cur_class_obj == NULL){
        std::cout << "LOOKED UP " << cur_class << ", GOT NULL" << std::endl << std::endl;
        return FAIL_NAME;
    }
    auto cur_class_table = cur_class_obj->ivars; //source_of_error
    if(cur_class_table->find(ident) != cur_class_table->end()){
        return cur_class_table->at(ident);
    }

    //if we don't find it in the cur class table and the class is obj, do not recurse
    if(cur_class.compare("Obj") == 0){
        std::cout << "recursive_lookup failed for ident "<< ident << " in class " << cur_class << std::endl;
        return FAIL_NAME;
    }

    return recursive_lookup(class_table, (class_n_t) *(cur_class_obj->parent), ident);
}

Status recursive_update(class_table_t& class_table, class_n_t& cur_class, const std::string& ident, class_n_t& new_type){
    if(cur_class.compare(MAIN_NAME) == 0){
        return FAIL;
    }
    
    if(class_table.find(cur_class) == class_table.end()){
        return FAIL;
    }
    auto cur_class_obj = class_table.at(cur_class);
    auto cur_class_table = cur_class_obj->ivars;
    if(cur_class_table->find(ident) != cur_class_table->end()){
        auto old_type = cur_class_table->at(ident);
        auto lca_type = LCA(class_table, old_type, new_type);
        if(lca_type->compare(old_type) == 0){
            return GOOD;
        }
        cur_class_table->erase(cur_class_table->find(ident));
        cur_class_table->emplace(std::make_pair(ident, *lca_type));
        return CHANGE;
    }

    //if we don't find it in the cur class table and the class is obj, do not recurse
    if(cur_class.compare("Obj") == 0){
        return FAIL;
    }

    return recursive_update(class_table, *(cur_class_obj->parent), ident, new_type);
}

Method* recursive_lookup_method(class_table_t& class_table, class_n_t& cur_class, const std::string& method_ident){
    if(cur_class.compare(MAIN_NAME) == 0){
        return NULL;
    }
    
    if(class_table.find(cur_class) == class_table.end()){
        return NULL;
    }
    auto cur_class_obj = class_table.at(cur_class);
    auto cur_class_table = cur_class_obj->methods;
    //std::cout << "recursive lookup in " <<cur_class << " looking for " << method_ident << "|" << std::endl;
    /*for(auto m : *cur_class_table){
        std::cout << " -" << m.first << "|" << std::endl; 
        if(m.first.compare(method_ident) == 0){
            return cur_class_table->at(method_ident);
        }
    }*/
    if(cur_class_table->find(method_ident) != cur_class_table->end()){
        return cur_class_table->at(method_ident);
    }

    //if we don't find it in the cur class table and the class is obj, do not recurse
    if(cur_class.compare("Obj") == 0){
        return NULL;
    }

    return recursive_lookup_method(class_table, *(cur_class_obj->parent), method_ident);
    
}

void copy_var_table(var_table_t* src, var_table_t* dst){
    for(auto s : *src){
        dst->emplace(s);
    }
}

Status LCA_type_table(class_table_t& class_table, var_table_t* src, var_table_t* dst){
    Status last_stat = GOOD;
    for(auto s : *src){
        if(dst->find(s.first) != dst->end()){
            auto old_type = dst->at(s.first);
            auto lca_type = LCA(class_table, s.second, old_type);
            if(lca_type->compare(FAIL_NAME) == 0){
                std::cout << "LCA_type_table failed to LCA " << old_type << " and " << s.second << std::endl;
                return FAIL;
            }
            if(old_type.compare(*lca_type) == 0){
                //no change needed
            }
            else{
                dst->erase(dst->find(s.first));
                dst->emplace(std::make_pair(s.first, *lca_type));
                last_stat = CHANGE;
            }
        }
    }
    return last_stat;
}

//merges true and false table together and writes it to dst
Status LCA_merge_var_table(class_table_t& class_table, var_table_t* src1, var_table_t* src2, var_table_t* dst){
    Status last_stat = GOOD;
    for(auto s : *src1){
        if(src2->find(s.first) != src2->end()){
            auto lca_type = LCA(class_table, src2->at(s.first), s.second);
            if(lca_type->compare(FAIL_NAME) == 0){
                std::cout << "LCA MERGE COULD NOT RESOLVE " << src2->at(s.first) << " and " << s.second << std::endl;
                return FAIL;
            }
            if(dst->find(s.first) == dst->end()){
                //in both tables but not in dst
                dst->emplace(std::make_pair(s.first, *lca_type));
                last_stat = CHANGE;
            }   
            else{
                auto dst_type = dst->at(s.first);
                auto lca_type_2 = LCA(class_table, dst_type, *lca_type);
                if(lca_type_2->compare(FAIL_NAME) == 0){
                    std::cout << "LCA MERGE COULD NOT RESOLVE " << *lca_type << " and " << dst_type << std::endl;
                    return FAIL;
                }
                if(lca_type_2->compare(dst_type) == 0){
                    //leave last_stat as good
                }
                else{
                    dst->erase(dst->find(s.first));
                    dst->emplace(std::make_pair(s.first, *lca_type_2));
                    last_stat = CHANGE;
                }  
            }
        }
    }
    return last_stat;
}



void print_var_table(std::string& table_title, var_table_t& locals){
    std::cout << "VAR TABLE [" << table_title << "]:" << std::endl;
    for(auto v : locals){
        std::cout << v.first << ":" << v.second << std::endl;
    }
    std::cout << std::endl;
}
void print_actual_list(std::string& method_name, actual_types_t& list){
    std::cout  << "TYPE_LIST [" << method_name << "]" << std::endl;
    for(auto a : list){
        std::cout << a << std::endl;
    }
    std::cout << std::endl;
}

void print_constructor(const std::string& class_name, Constructor& constructor){
    std::cout << "CONSTRUCTOR INFO [" << class_name << "]" << std::endl;
    auto actual_title = std::string("Actuals");
    auto local_title = std::string("Locals");
    print_actual_list(actual_title, *constructor.formals);
    print_var_table(local_title, *constructor.locals);
    std::cout << "RETURNS: " << class_name << std::endl;
    std::cout << "----------------------" << std::endl;
}
void print_method(Method& method){
    std::cout << "METHOD INFO [" << method.name << "]" << std::endl;
    auto actual_title = std::string("Actuals");
    auto local_title = std::string("Locals");
    print_actual_list(actual_title, *method.formals);
    print_var_table(local_title, *method.locals);
    std::cout << "RETURNS: " << method.return_type << std::endl;
    std::cout << "----------------------" << std::endl;
}
void print_method_table(std::string& parent_class_name, method_table_t& methods){
    std::cout << "METHOD TABLE [" << parent_class_name << "]" << std::endl;
    for(auto m : methods){
        print_method(*m.second);
    }
}
void print_full_class(Class& class_data){
    std::cout << "CLASS INFO [" << class_data.name << "]" << std::endl;
    auto ivar_title = std::string("Instance Variables");

    print_var_table(ivar_title, *class_data.ivars);
    std::cout << "......................." << std::endl;
    print_constructor(class_data.name, *class_data.constructor);
    std::cout << "......................." << std::endl;
    print_method_table(class_data.name, *class_data.methods);
    std::cout << "=======================" << std::endl << std::endl;
}
    

}