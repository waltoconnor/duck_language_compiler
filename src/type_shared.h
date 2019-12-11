#ifndef TYPE_SHARED_H
#define TYPE_SHARED_H
#include <unordered_map>
#include <string>
#include <vector>

namespace TC{

typedef const std::string class_n_t;

enum Status {
    GOOD,
    CHANGE,
    FAIL
};

class_n_t FAIL_NAME = class_n_t("___FAIL_CLASS___");
class_n_t MAIN_NAME = class_n_t("___MAIN_CLASS___");

typedef std::unordered_map<std::string, std::string> var_table_t;
typedef std::vector<std::string> actual_types_t;
class Method {
public:
    std::string name;
    var_table_t* locals;
    actual_types_t* formals;
    std::string return_type;
    std::string this_class;
    //AST::Method* node;
    Method(const std::string& name_, const std::string& this_class_){
        name = name_;
        locals = new var_table_t();
        formals = new actual_types_t();
        this_class = this_class_;
    }
    ~Method(){
        delete locals;
        delete formals;
    }
};

class Constructor : public Method{
public:
    var_table_t* locals;
    actual_types_t* formals;
    //AST::Constructor* node;
    Constructor(const std::string& name_) : Method(name_, name_){
        locals = new var_table_t();
        formals = new actual_types_t();
        return_type = name_;
    }
    ~Constructor(){
        delete locals;
        delete formals;
    }
};

typedef std::unordered_map<std::string, Method*> method_table_t;

class Class {
public:
    std::string name;
    var_table_t* ivars;
    method_table_t* methods;
    std::vector<Method*> method_order;
    class_n_t* parent = NULL;
    Constructor* constructor;
    //AST::Class* node;
    Class(const std::string& name_){
        name = name_;
        ivars = new var_table_t();
        methods = new method_table_t();
        method_order = std::vector<Method*>();
    }
    ~Class(){
        delete ivars;
        delete methods;
    }
    void append_method(std::pair<std::string, Method*> method_pair){
        methods->emplace(method_pair);
        method_order.push_back(method_pair.second);
    }
};

typedef std::unordered_map<std::string, Class*> class_table_t;

std::vector<std::string*>* get_super_list(class_table_t& class_table, class_n_t& child_class);

class_n_t* LCA(class_table_t& class_table, class_n_t& c1, class_n_t& c2);


bool is_subclass_of(class_table_t& class_table, class_n_t& subclass, class_n_t& superclass);

class_n_t& recursive_lookup(class_table_t& class_table, class_n_t& cur_class, const std::string& ident);

Status recursive_update(class_table_t& class_table, class_n_t& cur_class, const std::string& ident, class_n_t& new_type);

Method* recursive_lookup_method(class_table_t& class_table, class_n_t& cur_class, const std::string& method_ident);

void copy_var_table(var_table_t* src, var_table_t* dst);
Status LCA_merge_var_table(class_table_t& class_table, var_table_t* src1, var_table_t* src2, var_table_t* dst);
Status LCA_type_table(class_table_t& class_table, var_table_t* src, var_table_t* dst);

void print_var_table(std::string& table_title, var_table_t& locals);
void print_actual_list(std::string& method_name, actual_types_t& list);
void print_method_table(std::string& parent_class_name, method_table_t& methods);
void print_method(Method& method);
void print_constructor(const std::string& class_name, Constructor& constructor);
void print_full_class(Class& class_data);
}

#endif
