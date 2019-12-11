#include "ir_gen.h"

TC::class_table_t* class_table_global;

//ends in space
std::string* class_to_obj_name(const std::string& class_name){
    std::stringstream temp = std::stringstream();
    temp << "obj_" << class_name << " ";
    auto newstr= new std::string(temp.str());
    return newstr;
}

//MUST BE INSIDE
//typedef struct obj_Pt_struct {
void class_def_construct_ivars(TC::Class* tc_class, std::unordered_set<std::string>& finished_vars, head_t& ss){
    auto name = tc_class->name;
    if(tc_class->ivars->size() > 0){
        ss << indent << "//instance vars inherited from class "<<tc_class->name << std::endl;
    }

    for(auto ivar : *(tc_class->ivars)){
        if(finished_vars.find(ivar.first) == finished_vars.end()){
            ss << indent << "obj_" << ivar.second << " " << ivar.first << ";" << std::endl;
            finished_vars.emplace(ivar.first);
        }
    }   
}
// } * obj_Pt;

void class_def_rec_add_ivars(TC::Class* tc_class, TC::class_table_t* class_table, std::unordered_set<std::string>& finished_vars, head_t& ss){
    if(tc_class->name.compare("Obj") == 0 || tc_class->name.compare("___MAIN_CLASS___") == 0){
        return;
    }
    class_def_rec_add_ivars(class_table->at(*(tc_class->parent)), class_table, finished_vars, ss);
    class_def_construct_ivars(tc_class, finished_vars, ss);
}

/*void class_def_add_methods(TC::Class* tc_class, std::unordered_set<std::string>& finished_methods, head_t& ss){
    for(auto m : tc_class->method_order){
        auto mname = m->name;
        auto mret = m->return_type;
        if(finished_methods.find(mname) != finished_methods.end()){
            continue;
        }
        finished_methods.emplace(mname);
        ss << indent;
        ss << *class_to_obj_name(mret) << "(*" << mname << ")(" << "obj_" << tc_class->name;
        auto formals = *(m->formals);
        
            //ss << *class_to_obj_name(formals.at(0));
            
        for(size_t i = 0; i < formals.size(); i++){
            ss << ", " << "obj_" << formals.at(i);
        }
        ss << ");" << std::endl;
        
    }
}

void class_def_rec_add_methods(TC::Class* tc_class, TC::class_table_t* class_table, head_t& ss){
    auto finished_methods = std::unordered_set<std::string>();
    if(tc_class->name.compare("Obj") == 0){
        class_def_add_methods(tc_class, finished_methods, ss);
    }
    else{
        class_def_rec_add_methods(class_table->at(*(tc_class->parent)), class_table, ss);
    }
    class_def_add_methods(tc_class, finished_methods, ss);
}*/

void class_def_rec_populate_method_list(TC::Class* tc_class, TC::class_table_t* class_table, std::vector<TC::Method*>& order_vec, std::unordered_set<std::string>& finished_set){
    if(tc_class->name.compare("Obj") != 0){
        auto parent = class_table->at(*tc_class->parent);
        class_def_rec_populate_method_list(parent, class_table, order_vec, finished_set);
    }
    for(auto m : tc_class->method_order){
        if(finished_set.find(m->name) != finished_set.end()){
            for(size_t i = 0; i < order_vec.size(); i++){
                if(order_vec.at(i)->name.compare(m->name) == 0){
                    order_vec[i] = m;
                }
            }
        }
        else{
            finished_set.emplace(m->name);
            order_vec.push_back(m);
        }
    }
}

void class_def_rec_add_methods_order(TC::Class* tc_class, TC::class_table_t* class_table, head_t& ss){
    std::vector<TC::Method*> order_vec = std::vector<TC::Method*>();
    auto in_vec_methods = std::unordered_set<std::string>();
    class_def_rec_populate_method_list(tc_class, class_table, order_vec, in_vec_methods);
    for(auto m : order_vec){
        ss << indent << *class_to_obj_name(m->return_type) << "(*" << m->name << ")(" << "obj_" << m->this_class;
        auto formals = *(m->formals);
        for(auto f : formals){
            ss << ", obj_" << f;
        }
        ss << ");" << std::endl;
    }
}

void class_def_to_ir(TC::class_table_t* class_table, TC::Class* tc_class, AST::Class* ast_class, head_t& ss, head_t& forward_decl){
    auto name = tc_class->name;

    forward_decl << "//forward declaration for class " << name << std::endl;
    forward_decl << "struct class_" << name << "_struct;" << std::endl;
    forward_decl << "typedef struct class_" << name << "_struct* class_" << name << ";" << std::endl;
    forward_decl << "struct class_" << name << "_struct the_class_" << name << "_struct;" << std::endl << std::endl;
    //setup ivar table
    ss << "//ivar table for class " << name << std::endl;
    ss << "typedef struct obj_" << name << "_struct {" << std::endl;
    ss << indent << "class_" << name << " clazz;" << std::endl;
    /*for(auto ivar : *(tc_class->ivars)){
        ss << indent << "obj_" << ivar.second << " " << ivar.first << ";" << std::endl;
    }*/
    std::unordered_set<std::string> finished_map = std::unordered_set<std::string>();
    class_def_rec_add_ivars(tc_class, class_table, finished_map, ss);
    ss << "} * obj_" << name << ";" << std::endl << std::endl; 

    ss << "//method table for class " << name << std::endl;
    ss << "struct class_" << name << "_struct {" << std::endl;
    ss << indent << *class_to_obj_name(name) << "(*constructor)(";
    auto constructor = tc_class->constructor;
    if(constructor->formals->size() == 0){
        ss << "void);" << std::endl;
    }
    else{
        ss << "obj_" << constructor->formals->at(0);
        for(size_t i = 1; i < constructor->formals->size(); i++){
            ss << ", obj_" << constructor->formals->at(i);
        }
        ss << ");" << std::endl;
    }

    class_def_rec_add_methods_order(tc_class, class_table, ss);
    

    ss << "};" << std::endl << std::endl;
}

//forward declare this like a bum
std::string* expr_inline_to_ir(TC::Class& cur_class, TC::Method& cur_method, AST::Expr* expr, int* last_reg_idx, head_t& tvar_head);

std::string lexpr_to_ir(TC::Class& cur_class, TC::Method& cur_method, AST::LExpr* lexpr, int* last_reg_idx, head_t& tvar_head){
    head_t ss = head_t();
    std::cout << lexpr << std::endl;
    if(dynamic_cast<AST::Dot*>(lexpr) != nullptr){
        AST::Dot* dot = dynamic_cast<AST::Dot*>(lexpr);
        ss << *expr_inline_to_ir(cur_class, cur_method, &(dot->get_left()), last_reg_idx, tvar_head) << "->" << dot->get_ident();
        
    }
    else if(dynamic_cast<AST::Ident*>(lexpr) != nullptr){
        AST::Ident* ident = dynamic_cast<AST::Ident*>(lexpr);
        if(ident->get_ident().compare("true") == 0 || ident->get_ident().compare("True") == 0){
            ss << "lit_true";
        }
        else if(ident->get_ident().compare("false") == 0 || ident->get_ident().compare("False") == 0){
            ss << "lit_false";
        }
        else{
            ss << ident->get_ident();
        }
        
    }
    else{
        std::cout << "IR_GEN_ERROR::lexpr could not by dynamic_cast to either AST::Dot or AST::Ident" << std::endl;
    }
    return ss.str();
}

std::string* recursive_method_find_this(TC::Class& cur_class, const std::string& method_name){
    if(cur_class.name.compare(TC::MAIN_NAME) == 0){
        std::cout << "searching main class for method " << method_name << " ERROR " << std::endl;
        return NULL;
    }
    auto local_methods = cur_class.methods;
    if(local_methods->find(method_name) != local_methods->end()){
        return &(local_methods->at(method_name)->this_class);
    }
    if(cur_class.name.compare("Obj") == 0){
        std::cout << "Unable to find " << method_name << " in Obj or children" << std::endl;
        return NULL;
    }
    std::cout << cur_class.name << std::endl;
    auto parent_name = *(cur_class.parent);
    if(class_table_global->find(parent_name) == class_table_global->end()){
        std::cout << "Unable to find " << parent_name << " in class table" << std::endl;
    }
    auto parent = class_table_global->at(parent_name);
    return recursive_method_find_this(*parent, method_name);
}


std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

//probably have to allocate one variable per expression and then build up statements
//need to pass stream head around

//an expression can by definition be evaluated in line
std::string* expr_inline_to_ir(TC::Class& cur_class, TC::Method& cur_method, AST::Expr* expr, int* last_reg_idx, head_t& tvar_head){
    head_t ss = head_t();
    if(dynamic_cast<AST::IntConst*>(expr) != nullptr){
        auto iconst = dynamic_cast<AST::IntConst*>(expr);
        //ss << "int_literal(" << iconst->get_value() << ")";
        tvar_head << indent << "obj_Int " <<  "r" << *last_reg_idx << " = " << "int_literal(" << iconst->get_value() << ");" << std::endl;
        ss << "r" << *last_reg_idx;
        (*last_reg_idx)++;
    }
    else if(dynamic_cast<AST::Load*>(expr) != nullptr){
        auto load = dynamic_cast<AST::Load*>(expr);
        auto loc = &load->get_loc();
        
        //std::cout << "LOAD TRYING TO LOAD NULL loc, " << expr->str() << std::endl;
        //std::cout << "LOC IDENT = " << loc.get_ident() << std::endl;
        
        ss << lexpr_to_ir(cur_class, cur_method, loc, last_reg_idx, tvar_head);
    }
    else if(dynamic_cast<AST::StrConst*>(expr) != nullptr){
        auto strconst = dynamic_cast<AST::StrConst*>(expr);
        //ss << "str_literal(\"" << strconst->get_value() << "\")";
        tvar_head << indent << "obj_String " << "r" << *last_reg_idx << " = " "str_literal(\"" << ReplaceString(strconst->get_value(), "\n", "\\n") << "\");" << std::endl;
        ss << "r" << *last_reg_idx;
        (*last_reg_idx)++;
    }
    else if(dynamic_cast<AST::NothingConst*>(expr) != nullptr){
        auto nothingconst = dynamic_cast<AST::NothingConst*>(expr);
        ss << "nothing";
    }
    else if(dynamic_cast<AST::Construct*>(expr) != nullptr){
        auto ccon = dynamic_cast<AST::Construct*>(expr);

        //put the constructor in its own stream, build up the arguments, then append the constructor
        head_t cstream = head_t();
        cstream << indent << "obj_" << ccon->get_name() << " " << "r" << *last_reg_idx << " = ";
        cstream << "the_class_" << ccon->get_name() << "_struct.constructor(";
        //ss << "class_" << ccon->get_name() << "_struct->constructor(";
        int constructor_reg = *last_reg_idx;
        (*last_reg_idx)++;

        head_t temp_tvar = head_t();
        auto actuals = ccon->get_actuals().get_actuals();
        if(actuals.size() == 0){
            cstream << ");";
        }
        else{
            int temp_reg = *last_reg_idx;
            (*last_reg_idx)++;
            temp_tvar << indent << "obj_" << actuals.at(0)->get_last_type() << " r" << temp_reg << " = ";
            temp_tvar << *expr_inline_to_ir(cur_class, cur_method, actuals.at(0), last_reg_idx, tvar_head) << ";" << std::endl;
            cstream << "r" << temp_reg;
            for(size_t i = 1; i < actuals.size(); i++){
                int temp_reg = *last_reg_idx;
                (*last_reg_idx)++;
                temp_tvar << indent << "obj_" << actuals.at(i)->get_last_type() << " r" << temp_reg << " = ";
                temp_tvar << *expr_inline_to_ir(cur_class, cur_method, actuals.at(i), last_reg_idx, tvar_head) << ";" << std::endl;
                cstream << ", r" << temp_reg;
            }
            cstream << ");";
        }
        tvar_head << temp_tvar.str() << std::endl;
        tvar_head << cstream.str() << std::endl;
        ss << "r" <<  constructor_reg;
        return new std::string(ss.str());
    }
    else if(dynamic_cast<AST::Call*>(expr) != nullptr){
        auto ccon = dynamic_cast<AST::Call*>(expr);
        head_t cstream = head_t();
        auto receiver_code = *expr_inline_to_ir(cur_class, cur_method, &(ccon->get_receiver()), last_reg_idx, tvar_head);
        
        std::string receiver_type;
        receiver_type = ccon->get_receiver().get_last_type();
        
        int receiver_reg = *last_reg_idx;
        (*last_reg_idx)++;

        cstream << indent << "obj_" << receiver_type << " r" << receiver_reg << " = " << receiver_code << ";" << std::endl;
        //cstream << indent << "void*" << " r" << receiver_reg << " = " << receiver_code << ";" << std::endl;
        
        std::cout << "RECEIVER TYPE " << receiver_type << std::endl;
        auto rec_class = class_table_global->at(receiver_type);
        auto parent_class_this = recursive_method_find_this(*rec_class, ccon->get_method_name());
        head_t temp = head_t();
        head_t temp_tvar = head_t();
        temp << "r" << receiver_reg << "->clazz->" << ccon->get_method_name() << "((obj_" << *parent_class_this << ") " << "r" << receiver_reg;
        auto actuals = ccon->get_actuals().get_actuals();
        if(actuals.size() == 0){
            temp << ")";
        }
        else{
            for(size_t i = 0; i < actuals.size(); i++){
                int cur_temp = *last_reg_idx;
                (*last_reg_idx)++;
                temp_tvar << indent << "obj_"<<actuals.at(i)->get_last_type() << " r" << cur_temp << " = ";
                temp_tvar << *expr_inline_to_ir(cur_class, cur_method, actuals.at(i), last_reg_idx, tvar_head) << ";" << std::endl;
                temp << ", " << "r" << cur_temp;
            }
            temp << ")";
        }
        tvar_head << cstream.str() << std::endl;
        tvar_head << temp_tvar.str();
        //tvar_head << temp_tvar.str() << std::endl;
        //tvar_head << cstream.str() << std::endl;
        
        ss << temp.str();
        return new std::string(ss.str());
    }
    //And/Or must come before BinOp
    else if(dynamic_cast<AST::And*>(expr) != nullptr){
        auto acon = dynamic_cast<AST::And*>(expr);
        //ss << "(" << expr_inline_to_ir(&(acon->get_left())) "->clazz->EQUALS(" <<
        std::stringstream leftss = std::stringstream();
        std::stringstream rightss = std::stringstream();
        int leftreg = *last_reg_idx;
        leftss << indent << "obj_Boolean " << "r" << leftreg << " = ";
        (*last_reg_idx)++;
        leftss << *expr_inline_to_ir(cur_class, cur_method, &acon->get_left(), last_reg_idx, tvar_head) << ";" << std::endl;
        int rightreg = *last_reg_idx;
        rightss << indent << "obj_Boolean " << "r" << rightreg << " = ";
        (*last_reg_idx)++;
        rightss << *expr_inline_to_ir(cur_class, cur_method, &acon->get_right(), last_reg_idx, tvar_head) << ";" << std::endl;

        tvar_head << "//and registers" << std::endl;
        tvar_head << leftss.str() << rightss.str() << std::endl;

        int lefttruereg = *last_reg_idx;
        tvar_head << indent << "obj_Boolean r" << lefttruereg << " = r" << leftreg << "->clazz->EQUALS(r" << leftreg <<  ", lit_true); //left is true" << std::endl;
        (*last_reg_idx)++;
        int righttruereg = *last_reg_idx;
        tvar_head << indent << "obj_Boolean r" << righttruereg << " = r" << rightreg << "->clazz->EQUALS(r" << rightreg << ", lit_true); //right is true" << std::endl;
        (*last_reg_idx)++;

        ss << "((r" << rightreg << "->value == 1 && " << "r" << leftreg << "->value == 1) ? lit_true : lit_false)";
        return new std::string(ss.str());
    }
    else if(dynamic_cast<AST::Or*>(expr) != nullptr){
        //ss << "/* inline IR for or */";
        auto ocon = dynamic_cast<AST::And*>(expr);
        //ss << "(" << expr_inline_to_ir(&(acon->get_left())) "->clazz->EQUALS(" <<
        std::stringstream leftss = std::stringstream();
        std::stringstream rightss = std::stringstream();
        int leftreg = *last_reg_idx;
        leftss << indent << "obj_Boolean " << "r" << leftreg << " = ";
        (*last_reg_idx)++;
        leftss << *expr_inline_to_ir(cur_class, cur_method, &ocon->get_left(), last_reg_idx, tvar_head) << ";" << std::endl;
        int rightreg = *last_reg_idx;
        rightss << indent << "obj_Boolean " << "r" << rightreg << " = ";
        (*last_reg_idx)++;
        rightss << *expr_inline_to_ir(cur_class, cur_method, &ocon->get_right(), last_reg_idx, tvar_head) << ";" << std::endl;

        tvar_head << "//or registers" << std::endl;
        tvar_head << leftss.str() << rightss.str() << std::endl;

        int lefttruereg = *last_reg_idx;
        tvar_head << indent << "obj_Boolean r" << lefttruereg << " = r" << leftreg << "->clazz->EQUALS(r" << leftreg << ", lit_true); //left is true" << std::endl;
        (*last_reg_idx)++;
        int righttruereg = *last_reg_idx;
        tvar_head << indent << "obj_Boolean r" << righttruereg << " = r" << rightreg << "->clazz->EQUALS(r" << rightreg << ", lit_true); //right is true" << std::endl;
        (*last_reg_idx)++;

        ss << "((r" << rightreg << "->value == 1 || " << "r" << leftreg << "->value == 1) ? lit_true : lit_false)";
        return new std::string(ss.str());
    }
    else if(dynamic_cast<AST::BinOp*>(expr) != nullptr){
        auto binop = dynamic_cast<AST::BinOp*>(expr);
        ss << "/* inline IR for " << binop->get_op_sym() <<  "*/";
        return new std::string(ss.str());
    }
    else if(dynamic_cast<AST::Not*>(expr) != nullptr){
        auto vnot = dynamic_cast<AST::Not*>(expr);
        ss << "/* inline IR for not */";
        return new std::string(ss.str());
    }
    else{
        std::cout << "IR_GEN_ERROR::expr could not by dynamic cast to any option" << std::endl;
    }
    return new std::string(ss.str());
}



void statement_to_ir(TC::Class& cur_class, TC::Method& cur_method, AST::Statement* ast_stmnt, TC::var_table_t& local_type_table, head_t& ss, head_t& tvar_head, int* last_reg_idx){
    if(dynamic_cast<AST::Assign*>(ast_stmnt) != nullptr){
        auto assign = dynamic_cast<AST::Assign*>(ast_stmnt);
        if(assign->get_left().has_this_as_left()){
            //don't need to declare type since its in the this table
            
            //    this->ident = |
            ss << indent << "this->" << assign->get_left().get_ident() << " = ";
            ss << *expr_inline_to_ir(cur_class, cur_method, &(assign->get_right()), last_reg_idx, tvar_head)  << ";" << std::endl;
        }
        else if(assign->get_left().is_local()){
            std::stringstream decl = std::stringstream();
            auto local_ident = assign->get_left().get_ident();
            auto type = local_type_table.at(local_ident);
            //int local_alias_reg = *last_reg_idx;
            //tvar_head << indent << "obj_" << type << "r" << *last_reg_idx << " = " << local_ident << ";" << std::endl;
            //(*last_reg_idx)++;
            //decl << indent << "obj_" << type << " r" << local_alias_reg << " = (obj_" << type << ") ";
            decl << indent << local_ident << " = (obj_" << type << ") ";
            decl << *expr_inline_to_ir(cur_class, cur_method, &(assign->get_right()), last_reg_idx, tvar_head);
            decl << ";" << std::endl;
            //decl << std::endl;
            tvar_head << decl.str();
        }
        else{
            ss << "/* NOT LOCAL OR THIS ASSIGN ??? */";
        }
    }
    else if(dynamic_cast<AST::Return*>(ast_stmnt)){
        auto ret = dynamic_cast<AST::Return*>(ast_stmnt);
        ss << indent << "return (obj_" << cur_method.return_type << ") " << *expr_inline_to_ir(cur_class, cur_method, &(ret->get_expr()), last_reg_idx, tvar_head)  << ";" << std::endl;
    }
    else if(dynamic_cast<AST::If*>(ast_stmnt)){
        
        auto ifst = dynamic_cast<AST::If*>(ast_stmnt);
        //ss << "/* if statement */" << std::endl;
        int cond_reg = *last_reg_idx;
        (*last_reg_idx)++;
        std::stringstream temp;
        temp << indent << "obj_Boolean r" << cond_reg << " = " << *expr_inline_to_ir(cur_class, cur_method, &ifst->get_cond(), last_reg_idx, tvar_head) << ";" << std::endl;
        std::stringstream trueblock = std::stringstream();
        std::stringstream trueblock_reg = std::stringstream();
        std::stringstream falseblock = std::stringstream();
        std::stringstream falseblock_reg = std::stringstream();
        tvar_head << temp.str();
        ss << tvar_head.str();
        tvar_head.str("");
        head_t temp_tvar = head_t();
        //std::cout << "TVAR HEAD::" << tvar_head.str() << std::endl;
        
        trueblock_reg << "r" << cond_reg << "trueblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement" << std::endl;
        for(auto s : ifst->get_true_part().get_statements()){
            statement_to_ir(cur_class, cur_method, s, local_type_table, trueblock, trueblock_reg, last_reg_idx);
        }
        trueblock << trueblock_reg.str() << std::endl << indent << "goto r"<<cond_reg<<"end;";
        falseblock_reg << "r" <<cond_reg << "falseblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement" << std::endl;
        for(auto s : ifst->get_false_part().get_statements()){
            statement_to_ir(cur_class, cur_method, s, local_type_table, falseblock, falseblock_reg, last_reg_idx);
        }
        //ss << indent << "goto ((r" << cond_reg << " == lit_true) ? r"<<cond_reg<<"trueblock : r"<<cond_reg<<"falseblock);" << std::endl;
        /*ss << indent << "if(r"<<cond_reg<<"->value == 1){ goto r"<<cond_reg<<"trueblock; }else{ goto r"<<cond_reg<<"falseblock; }" << std::endl;
        ss <<  std::endl << trueblock.str() << std::endl;
        ss << falseblock_reg.str() << std::endl << falseblock.str() << std::endl;
        ss << "r" <<cond_reg<<"end: ;" << std::endl;
        ss << tvar_head.str();*/
        //std::cout << "TVAR_HEAD BEFORE UPDATE::" << tvar_head.str();
        temp_tvar << indent << "if(r"<<cond_reg<<"->value == 1){ goto r"<<cond_reg<<"trueblock; }else{ goto r"<<cond_reg<<"falseblock; }" << std::endl;
        temp_tvar <<  std::endl << trueblock.str() << std::endl;
        temp_tvar << falseblock_reg.str() << std::endl << falseblock.str() << std::endl;
        temp_tvar << "r" <<cond_reg<<"end: ;" << std::endl;
        //std::cout << "TVAR_HEAD AFTER UPDATE::" << tvar_head.str() << std::endl;
        ss << temp_tvar.str();
        return;
    }
    else if(dynamic_cast<AST::While*>(ast_stmnt)){
        auto whl = dynamic_cast<AST::While*>(ast_stmnt);
        int whl_cond = *last_reg_idx;
        (*last_reg_idx)++;
        std::stringstream contents = std::stringstream();
        std::stringstream contents_head = std::stringstream();
        std::stringstream temp = std::stringstream();
        std::stringstream cond_stmnt = std::stringstream();
        cond_stmnt << *expr_inline_to_ir(cur_class, cur_method, &whl->get_cond(), last_reg_idx, tvar_head) << ";" << std::endl; 
        temp << indent << "obj_Boolean r" << whl_cond << " = " << cond_stmnt.str();
        ss << temp.str() << std::endl;
        ss << "if(r" << whl_cond << "->value == 0){ goto r" << whl_cond <<"loop_end;}" << std::endl;
        ss << "r" << whl_cond << "start_label: ;" << std::endl;
        for(auto s : whl->get_body().get_statements()){
            statement_to_ir(cur_class, cur_method, s, local_type_table, contents, contents_head, last_reg_idx);
        }
        ss << contents_head.str() << std::endl;
        ss << contents.str() << std::endl;
        std::stringstream cond_head = std::stringstream();
        std::stringstream cond_body = std::stringstream();
        cond_body << *expr_inline_to_ir(cur_class, cur_method, &whl->get_cond(), last_reg_idx, cond_head) << ";" << std::endl;
        ss << cond_head.str();
        ss << indent << "r" << whl_cond << " = " << cond_body.str();
        ss << "if(r" << whl_cond << "->value == 1){ goto r" << whl_cond << "start_label; }" << std::endl;
        ss << "r" << whl_cond << "loop_end: ;" << std::endl;
        ss << "//end while loop" << std::endl;

    }
    else if(dynamic_cast<AST::Expr*>(ast_stmnt)){
        auto expr = dynamic_cast<AST::Expr*>(ast_stmnt);
        ss << indent << *expr_inline_to_ir(cur_class, cur_method, expr, last_reg_idx, tvar_head) << ";" << std::endl;
    }
    else if(dynamic_cast<AST::Typecase*>(ast_stmnt)){
        ss << "/* typecase statement */";
    }
    else{
        ss << "//ERROR: unable to coerce statement to IR" << std::endl;
        return;
    }
    //ss << ";" << std::endl;
}

void method_to_ir(TC::Class* parent_class, TC::Method* tc_method, AST::Method* ast_method, head_t& ss){
    auto name = tc_method->name;
    auto owner_name = parent_class->name;
    auto ret = *class_to_obj_name(tc_method->return_type);
    
    int local_reg_idx = 0;
    ss << "//method " << name << " for class " << owner_name << std::endl;
    //obj_Pt Pt_method_PLUS(obj_Pt this|
    ss << ret << "" << owner_name << "_method_" << name  << "(" << *class_to_obj_name(owner_name) << "this";
    for(auto arg : ast_method->get_formals()->get_formals()){
        auto arg_name = arg->get_name();
        auto arg_type = arg->get_type_name();

        ss << ", " << *class_to_obj_name(arg_type) << arg_name;
    }
    ss << "){" << std::endl;

    ss << indent << "//local variables for this method" << std::endl;
    for(auto local : *tc_method->locals){
        bool should_skip = false;
        for(auto f : ast_method->get_formals()->get_formals()){
            if(f->get_name().compare(local.first) == 0){
                should_skip = true;
            }
        }
        if(should_skip){
            continue;
        }
        else{
            ss << indent << "obj_" << local.second << " " << local.first << ";" << std::endl;
        }
    }
    ss << indent << "//end local vars" << std::endl << std::endl;
    for(auto statement : ast_method->get_block()->get_statements()){
        head_t tvar_head = head_t();
        head_t body_head = head_t();
        statement_to_ir(*parent_class, *tc_method, statement, *(tc_method->locals), body_head, tvar_head, &local_reg_idx);
        ss << tvar_head.str() << std::endl;
        ss << body_head.str() << std::endl;
    }

    
    

    ss << "}" << std::endl << std::endl; 

}

void constructor_to_ir(TC::Class* parent_class, TC::Constructor* tc_con, AST::Constructor* ast_con, head_t& ss){
    auto name = parent_class->name;
    auto ret = *class_to_obj_name(parent_class->name);
    
   
    int local_reg_idx = 0;
    ss << "//constructor for " << name << std::endl;
    ss << ret << "new_" << name << "(";
    if(tc_con->formals->size() == 0){
        ss << "){" << std::endl;
    }
    else{
        auto formals = ast_con->get_formals()->get_formals();
        ss << *class_to_obj_name(formals.at(0)->get_type_name()) << "" << formals.at(0)->get_name();
        for(size_t i = 1; i < formals.size(); i++){
            ss << ", " << *class_to_obj_name(formals.at(i)->get_type_name()) << "" << formals.at(i)->get_name();
        }
        ss << "){" << std::endl;
    }
    
    ss << indent << "obj_" << parent_class->name << " this = (obj_" << parent_class->name << ")malloc(sizeof(struct obj_" << parent_class->name << "_struct));" << std::endl; 
    ss << indent << "this->clazz = &the_class_" << parent_class->name << "_struct;" << std::endl;
    ss << indent << "//local variables for this constructor" << std::endl;
    for(auto local : *tc_con->locals){
        bool should_skip = false;
        for(auto f : ast_con->get_formals()->get_formals()){
            if(f->get_name().compare(local.first) == 0){
                should_skip = true;
            }
        }
        if(should_skip){
            continue;
        }
        else{
            ss << indent << "obj_" << local.second << " " << local.first << ";" << std::endl;
        }
    }
    ss << indent << "//end local vars" << std::endl << std::endl;
    
    for(auto statement : ast_con->get_block()->get_statements()){
        head_t body_head = head_t();
        head_t tvar_head = head_t();
        statement_to_ir(*parent_class, *tc_con, statement, *(tc_con->locals), body_head, tvar_head, &local_reg_idx);
        ss << tvar_head.str() << std::endl;
        ss << body_head.str() << std::endl;
        
    }
    
    

    ss << indent << "return this;";

    ss << std::endl << "}" << std::endl;
}

void class_methods_to_ir(TC::Class* tc_class, AST::Class* ast_class, head_t& ss){
    ss << std::endl << "//METHOD SET FOR " << tc_class->name << std::endl;
    auto methods = ast_class->get_methods()->get_methods();
    for(auto m : methods){
        auto tc_method = tc_class->methods->at(m->get_name());
        method_to_ir(tc_class, tc_method, m, ss);
    }
}

void main_to_ir(TC::Class& main_class, AST::Block* ast_body, TC::var_table_t& body_table, head_t& ss){
    
    int local_reg_idx = 0;
    ss << std::endl << "//MAIN BODY" << std::endl;
    ss << "int main(){" << std::endl;
        ss << indent << "//local variables for main" << std::endl;
        for(auto local : body_table){
            ss << indent << "obj_" << local.second << " " << local.first << ";" << std::endl;
        }
        ss << indent << "//end local vars" << std::endl << std::endl;

        for(auto statement : ast_body->get_statements()){
            head_t tvar_head = head_t();
            head_t body_head = head_t();
            statement_to_ir(main_class, *main_class.constructor, statement, body_table, body_head, tvar_head, &local_reg_idx);
            ss << tvar_head.str() << std::endl;
            ss << body_head.str() << std::endl;
        }
        

    ss << indent << "return 0;" << std::endl;
    ss << "}" << std::endl;
}

/*void recursive_get_method_defs(TC::class_table_t& class_table, TC::Class* cur_class, head_t& ss){
    auto methods = cur_class->method_order;
    
    if(cur_class->name.compare("Obj") != 0){
        recursive_get_method_defs(class_table, class_table.at(*(cur_class->parent)), ss);
    }

    for(auto m : methods){
        ss << indent << cur_class->name << "_method_" << m->name << "," << std::endl;
    }
}*/



void generate_class_method_vtable(TC::class_table_t& class_table, TC::Class* tc_class, AST::Class* ast_class, head_t& ss){
    std::vector<TC::Method*> method_list;
    std::unordered_set<std::string> finished_set;
    class_def_rec_populate_method_list(tc_class, &class_table, method_list, finished_set);

    ss << "struct class_" << tc_class->name << "_struct the_class_" << tc_class->name << "_struct =";
    ss << std::endl << "{" << std::endl;
    ss << indent << "new_" << tc_class->name << ", /* constructor */" << std::endl;
    for(auto m : method_list){
        ss << indent << m->this_class << "_method_" << m->name << "," << std::endl;
    }
    ss << "};" << std::endl;

}

void generate_program_ir(AST::Program* root, TC::class_table_t* tc_classes, TC::var_table_t* body_table, const std::string& outpath){
    class_table_global = tc_classes;
    auto classes = root->classes_.get_elements();
    head_t ss = head_t();
    head_t forward_decl = head_t();
    forward_decl << "#include \"Builtins.h\"" << std::endl << std::endl;
    forward_decl << "#include <string.h>" << std::endl << std::endl;
    forward_decl << "#include <stdlib.h>" << std::endl << std::endl;
    forward_decl << "//FORWARD DECLARATION" << std::endl;
    ss << std::endl << "//CLASS DEFINITIONS" << std::endl;
    for(auto c : classes){
        class_def_to_ir(tc_classes, tc_classes->at(c->get_class_name()), c, ss, forward_decl);
    }
    for(auto c : classes){
        auto local_class = tc_classes->at(c->get_class_name());
        constructor_to_ir(local_class, local_class->constructor, c->get_constructor(), ss);
        class_methods_to_ir(local_class, c, ss);
        generate_class_method_vtable(*tc_classes, local_class, c, ss);
    }
    main_to_ir(*tc_classes->at(TC::MAIN_NAME), &(root->statements_), *body_table, ss);
    std::cout << forward_decl.str() << ss.str();
    std::ofstream outfile;
    outfile.open(outpath);
    outfile << forward_decl.str() << ss.str();
    outfile.close();
    std::ofstream bih("./ir_out/Builtins.h");
    std::ofstream bic("./ir_out/Builtins.c");
    std::ifstream bih_in("./src/code_gen_resources/Builtins.h", std::ios::binary);
    std::ifstream bic_in("./src/code_gen_resources/Builtins.c", std::ios::binary);
    
    bih << bih_in.rdbuf();
    bic << bic_in.rdbuf();
}