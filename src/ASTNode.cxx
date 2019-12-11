#include "ASTNode.h"
#include <unordered_map>
#include <iostream>

namespace AST {

// Abstract syntax tree.  ASTNode is abstract base class for all other nodes.


    // JSON representation of all the concrete node types.
    // This might be particularly useful if I want to do some
    // tree manipulation in Python or another language.  We'll
    // do this by emitting into a stream.

    // --- Utility functions used by node-specific json output methods

    /* Indent to a given level */
    void ASTNode::json_indent(std::ostream& out, AST_print_context& ctx) {
        if (ctx.indent_ > 0) {
            out << std::endl;
        }
        for (int i=0; i < ctx.indent_; ++i) {
            out << "    ";
        }
    }

    /* The head element looks like { "kind" : "block", */
    void ASTNode::json_head(std::string node_kind, std::ostream& out, AST_print_context& ctx) {
        json_indent(out, ctx);
        out << "{ \"kind\" : \"" << node_kind << "\"," ;
        ctx.indent();  // one level more for children
        return;
    }

    void ASTNode::json_close(std::ostream& out, AST_print_context& ctx) {
        // json_indent(out, ctx);
        out << "}";
        ctx.dedent();
    }

    void ASTNode::json_child(std::string field, ASTNode& child, std::ostream& out, AST_print_context& ctx, char sep) {
        json_indent(out, ctx);
        out << "\"" << field << "\" : ";
        child.json(out, ctx);
        out << sep;
    }

    void Stub::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Stub", out, ctx);
        json_indent(out, ctx);
        out  << "\"rule\": \"" <<  name_ << "\"";
        json_close(out, ctx);
    }


    void Program::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Program", out, ctx);
        json_child("classes_", classes_, out, ctx);
        json_child("statements_", statements_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Formal::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Formal", out, ctx);
        json_child("var_", var_, out, ctx);
        json_child("type_", type_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Method::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Method", out, ctx);
        json_child("name_", name_, out, ctx);
        json_child("formals_", formals_, out, ctx);
        json_child("returns_", returns_, out, ctx);
        json_child("statements_", statements_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Assign::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Assign", out, ctx);
        json_child("lexpr_", lexpr_, out, ctx);
        json_child("rexpr_", rexpr_, out, ctx, ' ');
        json_close(out, ctx);
     }

    void AssignDeclare::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Assign", out, ctx);
        json_child("lexpr_", lexpr_, out, ctx);
        json_child("rexpr_", rexpr_, out, ctx);
        json_child("static_type_", static_type_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Return::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Return", out, ctx);
        json_child("expr_", expr_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void If::json(std::ostream& out, AST_print_context& ctx) {
        json_head("If", out, ctx);
        json_child("cond_", cond_, out, ctx);
        json_child("truepart_", truepart_, out, ctx);
        json_child("falsepart_", falsepart_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void While::json(std::ostream& out, AST_print_context& ctx) {
        json_head("While", out, ctx);
        json_child("cond_", cond_, out, ctx);
        json_child("body_", body_, out, ctx, ' ');
        json_close(out, ctx);
    }


    void Typecase::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Typecase", out, ctx);
        json_child("expr_", expr_, out, ctx);
        json_child("cases_", cases_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Type_Alternative::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Type_Alternative", out, ctx);
        json_child("ident_", ident_, out, ctx);
        json_child("classname_", classname_, out, ctx);
        json_child("block_", block_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Load::json(std::ostream &out, AST::AST_print_context &ctx) {
        std::string load_str = std::string("Load ") + get_last_type();
        json_head(load_str, out, ctx);
        //json_head("Load", out, ctx);
        json_child("loc_", loc_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void IdentRoot::json(std::ostream& out, AST_print_context& ctx) {
        json_head("IdentRoot", out, ctx);
        out << "\"text_\" : \"" << text_ << "\"";
        json_close(out, ctx);
    }

    void Ident::json(std::ostream& out, AST_print_context& ctx) {
        std::string ident_str = std::string("Ident ") + get_last_type();
        json_head(ident_str, out, ctx);
        //json_head("Ident", out, ctx);
        out << "\"text_\" : \"" << text_ << "\"";
        json_close(out, ctx);
    }

    void Type::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Type", out, ctx);
        out << "\"text_\" : \"" << text_ << "\"";
        json_close(out, ctx);
    }

    void MethodIdent::json(std::ostream& out, AST_print_context& ctx) {
        json_head("MethodIdent", out, ctx);
        out << "\"text_\" : \"" << text_ << "\"";
        json_close(out, ctx);
    }

    

    void Class::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Class", out, ctx);
        json_child("name_", name_, out, ctx);
        json_child("super_", super_, out, ctx);
        json_child("constructor_", constructor_, out, ctx);
        json_child("methods_", methods_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Call::json(std::ostream &out, AST::AST_print_context &ctx) {
        std::string call_str = std::string("Call ") + get_last_type() + std::string(" obj: ") + receiver_.get_last_type();
        json_head(call_str, out, ctx);
        //json_head("Call", out, ctx);
        json_child("obj_", receiver_, out, ctx);
        json_child("method_", method_, out, ctx);
        json_child("actuals_", actuals_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Construct::json(std::ostream &out, AST::AST_print_context &ctx) {
        json_head("Construct", out, ctx);
        json_child("method_", method_, out, ctx);
        json_child("actuals_", actuals_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void IntConst::json(std::ostream& out, AST_print_context& ctx) {
        json_head("IntConst", out, ctx);
        out << "\"value_\" : " << value_ ;
        json_close(out, ctx);
    }

    void StrConst::json(std::ostream& out, AST_print_context& ctx) {
        json_head("StrConst", out, ctx);
        out << "\"value_\" : \"" << value_  << "\"";
        json_close(out, ctx);
    }

    void NothingConst::json(std::ostream& out, AST_print_context& ctx) {
        json_head("NothingConst", out, ctx);
        out << "\" value_\" : \"" << value_ << "\"";
        json_close(out, ctx);
    }


    void BinOp::json(std::ostream& out, AST_print_context& ctx) {
        json_head(opsym, out, ctx);
        json_child("left_", left_, out, ctx);
        json_child("right_", right_, out, ctx, ' ');
        json_close(out, ctx);
    }


    void Not::json(std::ostream& out, AST_print_context& ctx) {
        json_head("Not", out, ctx);
        json_child("left_", left_, out, ctx, ' ');
        json_close(out, ctx);
    }

    void Dot::json(std::ostream& out, AST_print_context& ctx) {
        std::string dot_head = "Dot " + get_last_type();
        json_head(dot_head, out, ctx);
        //json_head("Dot", out, ctx);
        json_child("left_", left_, out, ctx);
        json_child("right_", right_, out, ctx, ' ');
        json_close(out, ctx);
    }


    /* Convenience factory for operations like +, -, *, / */
    Call* Call::binop(std::string opname, Expr& receiver, Expr& arg) {
        MethodIdent* method = new MethodIdent(*new IdentRoot(opname));
        Actuals* actuals = new Actuals();
        actuals->append(&arg);
        return new Call(receiver, *method, *actuals);
    }



TC::Status Assign::process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    std::cout << "Assign in " << cur_class << std::endl;
    if(class_table.find(cur_class) == class_table.end()){
        return TC::FAIL;
    }
    auto cur_class_o = class_table.at(cur_class);
    auto ident = lexpr_.get_ident();
    auto rtype = rexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto ltype = lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);

    if(rtype.compare(TC::FAIL_NAME) == 0){
        std::cout << "AST::Assign::process_type rtype FAIL" << std::endl;
        return TC::FAIL;
    }

    if(lexpr_.is_local()){
        lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
        if(local_table.find(ident) != local_table.end()){
            auto old_type = local_table.at(ident);
            if(old_type.compare(rtype) == 0){
                std::cout << "Assign: old type and rtype match " << rtype << std::endl;
                return TC::GOOD;
            }
            std::cout << "PRE LCA RTYPE=" << old_type << std::endl;
            auto lca_type = TC::LCA(class_table, old_type, rtype);
            if(lca_type->compare(TC::FAIL_NAME) == 0){
                std::cout << "LCA(" << old_type <<","<< rtype <<") RETURNED " << lca_type << std::endl;
            }
            if(lca_type->compare(old_type) == 0){
                std::cout << "Assign: LCA type and rtype match " << rtype << std::endl;
                return TC::GOOD;
            }
            else{
                std::cout << "Assign: updating local table with LCA type " << *lca_type << " = LCA(" << old_type << "," << rtype << ")" << std::endl;
                local_table.erase(local_table.find(ident));
                local_table.emplace(std::make_pair(ident, *lca_type));
                return TC::CHANGE;
            }
        }
        else{
            std::cout << "Assign: inserting new var in to local table " << ident << ":" <<rtype << std::endl;
            local_table.emplace(std::make_pair(ident, rtype));
            return TC::CHANGE;
        }
    }

    if(lexpr_.has_this_as_left()){
        lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
        auto recursive_lookup_type = TC::recursive_lookup(class_table, cur_class, ident);
        if(TC::is_subclass_of(class_table, rtype, recursive_lookup_type)){
            return TC::GOOD;
        }
        std::cout << "AST::Assign::process_type rtype not subclass of LOCAL instance var lookup type FAIL" << std::endl;
        return TC::FAIL;
    }
    
    //if we are here it means that the variable is an instance variable of another class
    auto recursive_lookup_type = TC::recursive_lookup(class_table, ltype, ident);
    if(TC::is_subclass_of(class_table, rtype, recursive_lookup_type)){
        return TC::GOOD;
    }
    std::cout << "AST::Assign::process_type rtype not subclass of REMOTE instance var lookup type FAIL" << std::endl;
    return TC::FAIL;
}
TC::Status Assign::process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    //std::cout << "Assign(constructor) in " << cur_class << std::endl;
    //std::cout << "Local table size " << local_table.size() << std::endl;
    //std::cout << "This table size " << this_table.size() << std::endl;
    if(class_table.find(cur_class) == class_table.end()){
        return TC::FAIL;
    }
    auto cur_class_o = class_table.at(cur_class);
    auto ident = lexpr_.get_ident();
    auto rtype = rexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto ltype = lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);

    if(rtype.compare(TC::FAIL_NAME) == 0){
        std::cout << "AST::Assign::process_type_constructor rtype " << ltype << "." <<ident << "="<<rtype << " FAIL" << std::endl;
        return TC::FAIL;
        //std::cout << "INSERTING " << ident << " as " << rtype;
    }

    if(lexpr_.is_local()){
        lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
        if(local_table.find(ident) != local_table.end()){
            auto old_type = local_table.at(ident);
            if(old_type.compare(rtype) == 0){
                std::cout << "Assign: old type and rtype match " << rtype << std::endl;
                return TC::GOOD;
            }
            auto lca_type = TC::LCA(class_table, old_type, rtype);
            if(lca_type->compare(old_type) == 0){
                std::cout << "Assign: LCA type and rtype match " << rtype << std::endl;
                return TC::GOOD;
            }
            else{
                std::cout << "Assign: updating local table with LCA type " << *lca_type << std::endl;
                local_table.erase(local_table.find(ident));
                local_table.emplace(std::make_pair(ident, *lca_type));
                return TC::CHANGE;
            }
        }
        else{
            std::cout << "Assign: inserting new var in to local table " << ident << ":" <<rtype << std::endl;
            local_table.emplace(std::make_pair(ident, rtype));
            return TC::CHANGE;
        }
    }

    bool lexpr_has_this_as_left = lexpr_.has_this_as_left();
    if(lexpr_has_this_as_left){
        lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
        if(this_table.find(ident) != this_table.end()){
            auto old_type = this_table.at(ident);
            if(old_type.compare(rtype) == 0){
                return TC::GOOD;
            }
            std::cout << "LCA old_type=" << old_type << ", rtype=" << rtype << std::endl;
            auto lca_type = TC::LCA(class_table, old_type, rtype);
            if(lca_type->compare(old_type) == 0){
                return TC::GOOD;
            }
            else{
                this_table.erase(this_table.find(ident));
                this_table.emplace(std::make_pair(ident, *lca_type));
                return TC::CHANGE;
            }
        }
        else{
            std::cout << "Assign adding " << ident << ":" << rtype << " to this table" << std::endl;
            this_table.emplace(std::make_pair(ident, rtype));
            return TC::CHANGE;
        }
        //return TC::recursive_update(class_table, cur_class, ident, rtype);
    }
    //std::cout << "Assign::process_type_constructor left_ is not a this statement" << std::endl;
    //std::cout << "Assing::process_type_constructor left_.has_this_as_left() = " << lexpr_has_this_as_left << std::endl;
    
    //if we are here it means that the variable is an instance variable of another class
    auto recursive_lookup_type = TC::recursive_lookup(class_table, ltype, ident);
    if(TC::is_subclass_of(class_table, rtype, recursive_lookup_type)){
        return TC::GOOD;
    }
    std::cout << "AST::Assign::process_type_constructor rtype not subclass of REMOTE instance var lookup type FAIL" << std::endl;
    return TC::FAIL;
}



TC::Status AssignDeclare::process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    //std::cout << "AssignDeclare in " << cur_class << std::endl;
    TC::Status stat = Assign::process_type(cur_class, class_table, this_table, local_table);
    auto rtype = rexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto ltype = lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    if(rtype.compare(TC::FAIL_NAME) == 0 || ltype.compare(TC::FAIL_NAME) == 0){
        std::cout << "AST::AssignDeclare::rtype or ltype FAIL" << std::endl;
        return TC::FAIL;
    }

    if(!TC::is_subclass_of(class_table, rtype, ltype)){
        std::cout << "AST::AssignDeclare::rtype " << rtype << " not subclass of ltype " << ltype << std::endl;
        return TC::FAIL;
    }
    return stat;
}
TC::Status AssignDeclare::process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    //std::cout << "AssignDeclare(Constructor) in " << cur_class << std::endl;
    TC::Status stat = Assign::process_type_constructor(cur_class, class_table, this_table, local_table);
    auto rtype = rexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto ltype = lexpr_.get_type_tc(cur_class, class_table, this_table, local_table);
    if(rtype.compare(TC::FAIL_NAME) == 0 || ltype.compare(TC::FAIL_NAME) == 0){
        std::cout << "AST::AssignDeclare::process_type_constructor rtype " << rtype << " or ltype " << ltype << " FAIL" << std::endl;
        return TC::FAIL;
    }

    if(!TC::is_subclass_of(class_table, rtype, ltype)){
        std::cout << "AST::AssignDeclare::process_type_constructor rtype " << rtype << " not subclass of ltype " << ltype << std::endl;
        return TC::FAIL;
    }
    return stat;
}

TC::class_n_t Construct::get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //std::cout << "Construct in " << cur_class << std::endl;
        for(auto a : actuals_.get_actuals()){
            a->get_type_tc(cur_class, class_table, this_table, local_table);
        }
        auto name = method_.get_name();
        if(class_table.find(name) == class_table.end()){
            std::cout << "Construct::get_type_tc class " << name << " not in table" << std::endl;
            return TC::FAIL_NAME;
        }
        auto actuals_list = actuals_.get_actuals();
        TC::actual_types_t actual_types = TC::actual_types_t();
        for(auto a : actuals_list){
            actual_types.push_back(a->get_type_tc(cur_class, class_table, this_table, local_table));
        }
        auto formals = class_table.at(name)->constructor->formals;
        if(formals->size() != actual_types.size()){
            std::cout << "Construct::get_type_tc formals size (" << formals->size() << ") does not match actuals size (" << actuals_list.size() << ")" << std::endl;
            return TC::FAIL_NAME;
        }
        for(size_t i = 0; i < formals->size(); i++){
            //if(actual_types.at(i).compare(formals.at(i)) != 0){
            if(!TC::is_subclass_of(class_table, actual_types.at(i), formals->at(i))){
                std::cout << "Construct::get_type_tc actual type (" << actual_types.at(i) << ") not subclass of formal type (" << formals->at(i) << ")" << std::endl;
                return TC::FAIL_NAME;
            }
        }

        return name;
    }

TC::class_n_t Call::get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    //std::cout << "Call in " << cur_class << std::endl;
    for(auto a : actuals_.get_actuals()){
        a->get_type_tc(cur_class, class_table, this_table, local_table);
    }
    receiver_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto receiver_type = receiver_.get_type_tc(cur_class, class_table, this_table, local_table);
    std::cout << "CALL RECIEVER TYPE " << receiver_type << std::endl;
    if(receiver_type.compare(TC::FAIL_NAME) == 0){
        std::cout << "Call reciever type unable to be determined FAIL" << std::endl;
        return TC::FAIL_NAME;
    }
    
    auto name = method_.get_name();
    std::cout << "CALL METHOD NAME: " << name << " RECIEVER: " << receiver_type << std::endl;
    if(class_table.find(receiver_type) == class_table.end()){
        std::cout << "Call reciever type " << receiver_type << " not found in class table" << std::endl;
        return TC::FAIL_NAME;
    }
    auto cur_class_obj = class_table.at(receiver_type);

    if(cur_class_obj->methods->find(name) == cur_class_obj->methods->end()){
        std::cout << "Call method " << name << " not found in local class table" << std::endl;
        //return TC::FAIL_NAME;
    }
    auto test_method = TC::recursive_lookup_method(class_table, receiver_type, name);
    if(test_method == NULL){
        std::cout << "Call method " << name << " not found in any parent classes" << std::endl;
        return TC::FAIL_NAME;
    }

    auto actuals_list = actuals_.get_actuals();
    TC::actual_types_t* actual_types = new TC::actual_types_t();
    for(auto a : actuals_list){
        actual_types->push_back(a->get_type_tc(cur_class, class_table, this_table, local_table));
    }
    auto formals = test_method->formals;

    
    if(formals->size() != actual_types->size()){
        std::cout << "Call formals mismatch" << std::endl;
        auto formal_name = name + std::string(" FORMALS LIST");
        TC::print_actual_list(formal_name, *formals);
        auto actual_name = name + std::string(" ACTUAL LIST");
        TC::print_actual_list(actual_name, *actual_types);
        return TC::FAIL_NAME;
    }
    for(size_t i = 0; i < formals->size(); i++){
        //if(actual_types.at(i).compare(formals.at(i)) != 0){
        if(!TC::is_subclass_of(class_table, actual_types->at(i), formals->at(i))){
            std::cout << "Call formals mismatch" << std::endl;
            auto formal_name = name + std::string(" FORMALS LIST");
            TC::print_actual_list(formal_name, *formals);
            auto actual_name = name + std::string(" ACTUAL LIST");
            TC::print_actual_list(actual_name, *actual_types);
            return TC::FAIL_NAME;
        }
    }

    last_type = new std::string(test_method->return_type);
    return test_method->return_type;
}

TC::Status If::process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    auto cond_type = cond_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto bool_str = std::string("Boolean");
    if(!TC::is_subclass_of(class_table, cond_type, bool_str)){
        std::cout << "IF CONDITION IS OF TYPE " << cond_type << " WHICH IS NOT A SUBCLASS OF BOOLEAN" << std::endl;
        return TC::FAIL;
    }
    
    TC::var_table_t* true_table = new TC::var_table_t();
    TC::var_table_t* false_table = new TC::var_table_t();

    TC::copy_var_table(&local_table, true_table);
    TC::copy_var_table(&local_table, false_table);

    auto tstatements = truepart_.get_statements();
    auto fstatements = falsepart_.get_statements();

    std::cout << "IF PROCESSING TRUE PART" << std::endl;
    int iter_count = 0;
    TC::Status last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        last_stat = TC::GOOD;
        for(auto s : tstatements){
            TC::Status cur_stat = s->process_type(cur_class, class_table, this_table, *true_table);
            if(cur_stat == TC::FAIL){
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter_count++;
        if(iter_count > 100){
            std::cout << "If iter count above 100 (truepart), probably stuck in loop" << std::endl;
            return TC::FAIL;
        }
    }

    std::cout << "IF PROCESSING FALSE PART" << std::endl;
    last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        last_stat = TC::GOOD;
        for(auto s : fstatements){
            TC::Status cur_stat = s->process_type(cur_class, class_table, this_table, *false_table);
            if(cur_stat == TC::FAIL){
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter_count++;
        if(iter_count > 100){
            std::cout << "If iter count above 100 (falsepart), probably stuck in loop" << std::endl;
            return TC::FAIL;
        }
    }

    auto ttable_name = std::string("TRUE TABLE");
    auto ftable_name = std::string("FALSE TABLE");
    TC::print_var_table(ttable_name, *true_table);
    TC::print_var_table(ftable_name, *false_table);

    TC::Status t_update_stat = TC::LCA_type_table(class_table, true_table,  &local_table);
    TC::Status f_update_stat = TC::LCA_type_table(class_table, false_table, &local_table);

    if(t_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE FAILED (TRUE)" << std::endl;
        return TC::FAIL;
    }
    if(f_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE FAILED (FALSE)" << std::endl;
        return TC::FAIL;
    }
    TC::Status merge_stat = TC::LCA_merge_var_table(class_table, true_table, false_table, &local_table);
    if(merge_stat == TC::FAIL){
        std::cout << "IF LCA MERGE FAILED" << std::endl;
        return TC::FAIL;
    }
    delete true_table;
    delete false_table;

    if(t_update_stat == TC::CHANGE || f_update_stat == TC::CHANGE || merge_stat == TC::CHANGE){
        return TC::CHANGE;
    }
    return TC::GOOD;
}
TC::Status If::process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    auto cond_type = cond_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto bool_str = std::string("Boolean");
    if(!TC::is_subclass_of(class_table, cond_type, bool_str)){
        std::cout << "IF CONDITION IS OF TYPE " << cond_type << " WHICH IS NOT A SUBCLASS OF BOOLEAN" << std::endl;
        return TC::FAIL;
    }
    
    TC::var_table_t* true_table = new TC::var_table_t();
    TC::var_table_t* false_table = new TC::var_table_t();
    TC::var_table_t* true_this_table = new TC::var_table_t();
    TC::var_table_t* false_this_table = new TC::var_table_t();

    TC::copy_var_table(&local_table, true_table);
    TC::copy_var_table(&local_table, false_table);
    TC::copy_var_table(&this_table, true_this_table);
    TC::copy_var_table(&this_table, false_this_table);

    auto tstatements = truepart_.get_statements();
    auto fstatements = falsepart_.get_statements();

    int iter_count = 0;
    TC::Status last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        last_stat = TC::GOOD;
        for(auto s : tstatements){
            TC::Status cur_stat = s->process_type_constructor(cur_class, class_table, *true_this_table, *true_table);
            if(cur_stat == TC::FAIL){
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter_count++;
        if(iter_count > 100){
            std::cout << "If iter count above 100 (truepart), probably stuck in loop" << std::endl;
            return TC::FAIL;
        }
    }

    last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        last_stat = TC::GOOD;
        for(auto s : fstatements){
            TC::Status cur_stat = s->process_type_constructor(cur_class, class_table, *false_this_table, *false_table);
            if(cur_stat == TC::FAIL){
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter_count++;
        if(iter_count > 100){
            std::cout << "If iter count above 100 (falsepart), probably stuck in loop" << std::endl;
            return TC::FAIL;
        }
    }

    TC::Status t_update_stat = TC::LCA_type_table(class_table, true_table,  &local_table);
    TC::Status f_update_stat = TC::LCA_type_table(class_table, false_table, &local_table);
    TC::Status t_this_update_stat = TC::LCA_type_table(class_table, true_this_table,  &this_table);
    TC::Status f_this_update_stat = TC::LCA_type_table(class_table, false_this_table, &this_table);

    if(t_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE FAILED (TRUE)" << std::endl;
        return TC::FAIL;
    }
    if(f_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE FAILED (FALSE)" << std::endl;
        return TC::FAIL;
    }
    if(t_this_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE THIS FAILED (TRUE)" << std::endl;
        return TC::FAIL;
    }
    if(f_this_update_stat == TC::FAIL){
        std::cout << "IF LCA UPDATE THIS FAILED (FALSE)" << std::endl;
        return TC::FAIL;
    }

    TC::Status merge_stat = TC::LCA_merge_var_table(class_table, true_table, false_table, &local_table);
    if(merge_stat == TC::FAIL){
        std::cout << "IF LCA MERGE FAILED" << std::endl;
        return TC::FAIL;
    }

    TC::Status this_merge_stat = TC::LCA_merge_var_table(class_table, true_this_table, false_this_table, &this_table);
    if(merge_stat == TC::FAIL){
        std::cout << "IF LCA THIS MERGE FAILED" << std::endl;
        return TC::FAIL;
    }
    
    delete true_table;
    delete false_table;
    delete true_this_table;
    delete false_this_table;

    if(t_this_update_stat == TC::CHANGE || f_this_update_stat == TC::CHANGE || t_update_stat == TC::CHANGE || f_update_stat == TC::CHANGE || merge_stat == TC::CHANGE || this_merge_stat == TC::CHANGE){
        return TC::CHANGE;
    }
    return TC::GOOD;
}

TC::Status While::process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    auto cond = cond_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto bool_str = std::string("Boolean");
    if(!TC::is_subclass_of(class_table, cond, bool_str)){
        std::cout << "WHILE CONDITION IS NOT Boolean, is " << cond << std::endl;
        return TC::FAIL;
    }

    auto temp_table = new TC::var_table_t();
    TC::copy_var_table(&local_table, temp_table);

    auto statements = body_.get_statements();

    int iter = 0;
    TC::Status last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        for(auto s : statements){
            TC::Status cur_stat = s->process_type(cur_class, class_table, this_table, *temp_table);
            if(cur_stat == TC::FAIL){
                std::cout << "WHILE FAILED DURING PROCESSING" << std::endl;
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter++;
        if(iter > class_table.size()){
            std::cout << "While loop has processed [CLASS TABLE SIZE] times, at fixed point" << std::endl;
            return TC::GOOD;
        }
    }

    TC::Status update_stat = TC::LCA_type_table(class_table, temp_table, &local_table);
    if(update_stat == TC::FAIL){
        std::cout << "While updating local table failed" << std::endl;
    }

    return update_stat;
}
TC::Status While::process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
    auto cond = cond_.get_type_tc(cur_class, class_table, this_table, local_table);
    auto bool_str = std::string("Boolean");
    if(!TC::is_subclass_of(class_table, cond, bool_str)){
        std::cout << "WHILE CONDITION IS NOT Boolean, is " << cond << std::endl;
        return TC::FAIL;
    }

    auto temp_table = new TC::var_table_t();
    TC::copy_var_table(&local_table, temp_table);

    auto statements = body_.get_statements();

    int iter = 0;
    TC::Status last_stat = TC::CHANGE;
    while(last_stat == TC::CHANGE){
        for(auto s : statements){
            TC::Status cur_stat = s->process_type_constructor(cur_class, class_table, this_table, *temp_table);
            if(cur_stat == TC::FAIL){
                std::cout << "WHILE FAILED DURING PROCESSING" << std::endl;
                return TC::FAIL;
            }
            if(cur_stat == TC::CHANGE){
                last_stat = TC::CHANGE;
            }
        }
        iter++;
        if(iter > 100){
            std::cout << "While stuck in type resolution loop" << std::endl;
            return TC::FAIL;
        }
    }

    TC::Status update_stat = TC::LCA_type_table(class_table, temp_table, &local_table);
    if(update_stat == TC::FAIL){
        std::cout << "While updating local table failed" << std::endl;
    }

    return update_stat;
}

}