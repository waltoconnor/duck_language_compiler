//
// Created by Michal Young on 9/12/18.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "Symbols.h"
#include "ASTPrintContext.h"
#include "Shared.h"
#include "type_shared.h"





namespace AST {
    // Abstract syntax tree.  ASTNode is abstract base class for all other nodes.
    enum Status { CHANGE, NO_CHANGE, FAIL};

    typedef const std::string type_name_t;
    typedef const std::string method_name_t;
    //typedef std::pair<type_name_t, Class*> type_t;

    //type_name_t FAIL_NAME = std::string("__TYPE_FAIL__");



    /*class Context {
        std::unordered_map<type_name_t, type_t> class_map;
        type_t main;
    public:
        Context();

        void add_class(Class& c);
        type_t& get_class(type_name_t& name);
        type_t& LCA(type_t& c1, type_t& c2);
    };*/

    class ASTNode {
    public:
        virtual void json(std::ostream& out, AST_print_context& ctx) = 0;  // Json string representation
        std::string str() {
            std::stringstream ss;
            AST_print_context ctx;
            json(ss, ctx);
            return ss.str();
        }
    protected:
        void json_indent(std::ostream& out, AST_print_context& ctx);
        void json_head(std::string node_kind, std::ostream& out, AST_print_context& ctx);
        void json_close(std::ostream& out, AST_print_context& ctx);
        void json_child(std::string field, ASTNode& child, std::ostream& out, AST_print_context& ctx, char sep=',');
    };

    class Stub : public ASTNode {
        std::string name_;
    public:
        explicit Stub(std::string name) : name_{name} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
    };


    /*
     * Abstract base class for nodes that have sequences
     * of children, e.g., block of statements, sequence of
     * classes.  These may be able to share some operations,
     * especially when applying a method to the sequence just
     * means applying the method to each element of the sequence.
     * We need a different kind of sequence depending on type of
     * elements if we want to access elements without casting while
     * still having access to their fields.
     * (Will replace 'Seq')
     */
    template<class Kind>
    class Seq : public ASTNode {
    protected:
        std::string kind_;
        std::vector<Kind *> elements_;
    public:
        Seq(std::string kind) : kind_{kind}, elements_{std::vector<Kind *>()} {}

        void append(Kind *el) { 
            elements_.push_back(el); 
        }

        void json(std::ostream &out, AST_print_context &ctx) {
            json_head(kind_, out, ctx);
            out << "\"elements_\" : [";
            auto sep = "";
            for (Kind *el: elements_) {
                out << sep;
                el->json(out, ctx);
                sep = ", ";
            }
            out << "]";
            json_close(out, ctx);
        }

    };

    /* L_Expr nodes are AST nodes that can be evaluated for location.
     * Most can also be evaluated for value_.  An example of an L_Expr
     * is an identifier, which can appear on the left_ hand or right_ hand
     * side of an assignment.  For example, in x = y, x is evaluated for
     * location and y is evaluated for value_.
     *
     * For now, a location is just a name, because that's what we index
     * the symbol table with.  In a full compiler, locations can be
     * more complex, and typically in code generation we would have
     * LExpr evaluate to an address in a register.
     */
    class LExpr : public ASTNode {
        /* Abstract base class */
    public:
        TC::class_n_t* last_type;
        virtual TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL;}
        virtual TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){std::cout << "ERROR INVOKED ABSTRACT CLASS METHOD LExpr" << std::endl; return TC::FAIL;}
        virtual TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL_NAME; }
        virtual TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL_NAME; }
        virtual bool is_this(){ std::cout << "LExpr is_this() invoked, ERROR" << std::endl; return false; };
        virtual bool is_local(){ return false; };
        virtual const std::string& get_ident(){ return *new std::string("VIRTUAL_CLASS_IDENT"); };
        virtual bool has_this_as_left(){ return false; }
        virtual TC::class_n_t get_last_type(){ return *new std::string("VIRTUAL get_last_type LEXPR"); }
        void set_last_type(TC::class_n_t& new_type){ last_type = new std::string(new_type); }
        LExpr(){};
        ~LExpr(){};
    };

    class Statement : public ASTNode {
    public:
        Statement(){};
        ~Statement(){};
        virtual TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL;}
        virtual TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){std::cout << "ERROR INVOKED ABSTRACT CLASS METHOD Statement" << std::endl; return TC::FAIL; return TC::FAIL;}
    };

    /* A statement could be just an expression ... but
     * we might want to interpose a node here.
     */
    class Expr : public Statement {
    
    public:
        TC::class_n_t* last_type;
        Expr(){};
        ~Expr(){};
        //TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL;};
        //TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){std::cout << "ERROR INVOKED ABSTRACT CLASS METHOD Expr" << std::endl; return TC::FAIL;}
        virtual bool is_this(){return false; };
        virtual TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL_NAME; }
        virtual TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL_NAME; }
        virtual TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::FAIL;}
        virtual TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){std::cout << "ERROR INVOKED ABSTRACT CLASS METHOD Statement" << std::endl; return TC::FAIL; return TC::FAIL;}
        virtual TC::class_n_t get_last_type(){ return *new std::string("VIRTUAL get_last_type EXPR"); }
        void set_last_type(TC::class_n_t& new_type){ last_type = new std::string(new_type); }
    };


    /* Identifiers like x and literals like 42 are the
    * leaves of the AST.  A literal can only be evaluated
    * for value_ (the 'eval' method), but an identifier
    * can also be evaluated for location (when we want to
    * store something in it).
    */

    class IdentRoot : public ASTNode{
        std::string text_;
    public:
        explicit IdentRoot(std::string txt) : text_{txt} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        const std::string get_txt(){ return text_; }
        
    };

    class Ident : public LExpr {
        std::string text_;
    public:
        TC::class_n_t* last_type;
        explicit Ident(IdentRoot& root) : text_{root.get_txt()} {
            last_type = new std::string("Ident uninit");
        }
        void json(std::ostream& out, AST_print_context& ctx) override;
        bool is_this(){
            bool is_this_val = text_.compare(std::string("this")) == 0;
            //std::cout << "Ident probed for is_this, Ident=" << text_ << ", is_this=" << is_this_val << std::endl;
            return is_this_val; 
        }
        const std::string& get_ident(){ return text_; }
        bool is_local(){ return true; }
        bool has_this_as_left(){ return false; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){ 
            last_type = new std::string(get_type_tc(cur_class, class_table, this_table, local_table));
            if(is_this()){
                last_type = new std::string(cur_class);
            }
            else{
                last_type = new std::string(local_table.at(text_));
            }
            return TC::GOOD; 
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){ 
            return process_type(cur_class, class_table, this_table, local_table); 
        }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            if(text_.compare("true") == 0 || text_.compare("false") == 0 || text_.compare("True") == 0 || text_.compare("False") == 0){
                auto tbool = new std::string("Boolean");
                last_type = tbool;
                return *tbool;
            }
            if(is_this()){
                last_type = new std::string(cur_class);
                return cur_class;
            }
            if(local_table.find(text_) != local_table.end()){
                auto type = local_table.at(text_);
                last_type = new std::string(type);
                return type;
            }
            std::cout << "Ident did not find " << text_ << " in local table" << std::endl;
            //last_type = &TC::FAIL_NAME;
            return TC::FAIL_NAME;
        }
        TC::class_n_t get_last_type(){ return *last_type; }
        void set_last_type(TC::class_n_t& name){
            last_type = new std::string(name);
        }
        /*TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            return get_type_tc(cur_class, class_table, this_table, local_table);
        }*/
    };

    class MethodIdent : public ASTNode {
        method_name_t text_;
    public:
        explicit MethodIdent(IdentRoot& root) : text_{root.get_txt()} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        method_name_t& get_name(){
            return text_;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){ return TC::GOOD; }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){ return TC::GOOD; }
        
    };

    class Type : public ASTNode {
        type_name_t text_;
    public:
        explicit Type(IdentRoot& root) : text_{root.get_txt()} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::class_n_t& get_type_name(){
            return text_;
        }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            
            return text_;
        }
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return text_;
        // }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::GOOD;}
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){return TC::GOOD;}

        //in case where method and type are same, we can convert the type in to the method name
        MethodIdent& constructor_type_to_ident(){
            IdentRoot idroot = IdentRoot(text_);
            MethodIdent* mid = new MethodIdent(idroot);
            return *mid;
        }
    };

    


    /* A block is a sequence of statements or expressions.
     * For simplicity we'll just make it a sequence of ASTNode,
     * and leave it to the parser to build valid structures.
     */
    class Block : public Seq<Statement> {
    public:
        explicit Block() : Seq("Block") {}
        std::vector<Statement*>& get_statements(){ return elements_; }
     };



    /* Formal arguments list is a list of
     * identifier: type pairs.
     */
    class Formal : public ASTNode {
        Ident& var_;
        Type& type_;
    public:
        explicit Formal(Ident& var, Type& type_) :
            var_{var}, type_{type_} {};
        void json(std::ostream& out, AST_print_context&ctx) override;
        const std::string& get_name(){ return var_.get_ident(); }
        TC::class_n_t& get_type_name(){ return type_.get_type_name(); }
    };

    class Formals : public Seq<Formal> {
    public:
        explicit Formals() : Seq("Formals") {}
        std::vector<Formal*>& get_formals(){ return elements_; }
    };

    class Method : public ASTNode {
    protected:
        MethodIdent& name_;
        Formals& formals_;
        Type& returns_;
        Block& statements_;
        // Symbol table information
        std::unordered_map<std::string, type_t&> variables;
    public:
        explicit Method(MethodIdent& name, Formals& formals, Type& returns, Block& statements) :
          name_{name}, formals_{formals}, returns_{returns}, statements_{statements} {}
        void json(std::ostream& out, AST_print_context&ctx) override;
        const std::string& get_name(){ return name_.get_name(); }
        Formals* get_formals(){ return &formals_; }
        Block* get_block(){ return &statements_; }
        TC::class_n_t& get_return_type_name(){ return returns_.get_type_name(); }
    };

    class Methods : public Seq<Method> {

    public:
        explicit Methods() : Seq("Methods") {
        }
        std::vector<Method*>& get_methods(){ return elements_; }
    };

    class Constructor : public Method {
    public:
        explicit Constructor(Type& name_returns, Formals& formals, Block& statements) :
        Method(name_returns.constructor_type_to_ident(), formals, name_returns, statements) {}
        
    };




    /* An assignment has an lvalue (location to be assigned to)
     * and an expression.  We evaluate the expression and place
     * the value_ in the variable.  An assignment may also place a
     * static type constraint on a variable.  This is logically a
     * distinct operation, and could be represented as a separate statement,
     * but it's convenient to keep it in the assignment since our syntax
     * puts it there.
     */
    class Assign : public Statement {
    protected:  // But inherited by AssignDeclare
        LExpr &lexpr_;
        Expr &rexpr_;
    public:
        explicit Assign(LExpr &lexpr, Expr &rexpr) :
           lexpr_{lexpr}, rexpr_{rexpr} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        LExpr& get_left(){ return lexpr_; }
        Expr& get_right(){ return rexpr_; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
    };


    class AssignDeclare : public Assign {
        Type &static_type_;
    public:
        explicit AssignDeclare(LExpr &lexpr, Expr &rexpr, Type &static_type) :
            Assign(lexpr, rexpr), static_type_{static_type} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        
    };



    /* When an expression is an LExpr, we
     * the LExpr denotes a location, and we
     * need to load it.
     */
    class Load : public Expr {
        LExpr &loc_;
    public:
        TC::class_n_t* last_type;
        Load(LExpr &loc) : loc_{loc} {
            last_type = new std::string("Load uninit");
        }
        void json(std::ostream &out, AST_print_context &ctx) override;
        LExpr& get_loc(){ return loc_; }
        bool is_this(){ 
            bool is_this_val = loc_.is_this();
            //std::cout << "Load was probed for is_this, retunring " << is_this_val << std::endl;
            if(loc_.get_last_type().compare(TC::FAIL_NAME) == 0){
                loc_.last_type = last_type;
            }
            return is_this_val; 
        }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class,TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            //std::cout << "Load in " << cur_class << std::endl;
            loc_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(loc_.is_this()){
                //std::cout << "Loading \"this\", returning type " << cur_class << std::endl;
                last_type = new std::string(cur_class);
                return cur_class;
            }
            auto type = loc_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(type.compare(TC::FAIL_NAME) == 0){
                std::cout << "Load::get_type_tc got fail type" << std::endl;
            }
            last_type = new std::string(type);
            return type;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            loc_.get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ 
            return loc_.get_last_type(); 
        }
        void set_last_type(TC::class_n_t& new_type){
            last_type = new std::string(new_type);
            loc_.set_last_type(new_type);
        }
    };



    /* 'return' statement returns value from method */
    class Return : public Statement {
        Expr &expr_;
    public:
        explicit Return(Expr& expr) : expr_{expr}  {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        Expr& get_expr(){ return expr_; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            expr_.get_type_tc(cur_class, class_table, this_table, local_table);
            expr_.process_type(cur_class, class_table, this_table, local_table);
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            expr_.get_type_tc_constructor(cur_class, class_table, this_table, local_table);
            expr_.process_type_constructor(cur_class, class_table, this_table, local_table);
            return TC::GOOD;
        }
    };

    class If : public Statement {
        Expr &cond_; // The boolean expression to be evaluated
        Block &truepart_; // Execute this block if the condition is true
        Block &falsepart_; // Execute this block if the condition is false
    public:
        explicit If(Expr& cond, Block& truepart, Block& falsepart) :
            cond_{cond}, truepart_{truepart}, falsepart_{falsepart} { };
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        Expr& get_cond(){ return cond_; }
        Block& get_true_part(){ return truepart_; }
        Block& get_false_part(){ return falsepart_; }
    };

    class While : public Statement {
        Expr& cond_;  // Loop while this condition is true
        Block&  body_;     // Loop body
    public:
        explicit While(Expr& cond, Block& body) :
            cond_{cond}, body_{body} { };
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        Expr& get_cond(){ return cond_; }
        Block& get_body(){ return body_; }
    };


    /* A class has a name, a list of arguments, and a body
    * consisting of a block (essentially the constructor)
    * and a list of methods.
    */
    class Class : public ASTNode {
        Type& name_;
        Type& super_;
        Constructor& constructor_;
        Methods& methods_;
        std::unordered_map<std::string, type_t&> instance_vars;
    public:
        explicit Class(Type& name, Type& super,
                 Constructor& constructor, Methods& methods) :
            name_{name},  super_{super},
            constructor_{constructor}, methods_{methods} {};
        
        void json(std::ostream& out, AST_print_context& ctx) override;
        Methods* get_methods(){ return &methods_; }
        Constructor* get_constructor(){ return &constructor_; }
        TC::class_n_t& get_class_name(){ return name_.get_type_name(); }
        TC::class_n_t& get_super_name(){ return super_.get_type_name(); }
    };

    /* A Quack program begins with a sequence of zero or more
     * class definitions.
     */
    class Classes : public Seq<Class> {
    public:
        explicit Classes() : Seq<Class>("Classes") {}
        std::vector<Class*>& get_elements(){ return elements_; }
    };

    class IntConst : public Expr {
        int value_;
        TC::class_n_t class_name = std::string("Int");
    public:
        TC::class_n_t* last_type;
        explicit IntConst(int v) : value_{v} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        int get_value(){ return value_; }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class,TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            return class_name;
        }
        TC::class_n_t get_last_type(){ return class_name; }
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return get_type_tc(cur_class, class_table, this_table, local_table);
        // }
    };

    class Type_Alternative : public ASTNode {
        Ident& ident_;
        Type& classname_;
        Block& block_;
    public:
        explicit Type_Alternative(Ident& ident, Type& classname, Block& block) :
                ident_{ident}, classname_{classname}, block_{block} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
    };

    class Type_Alternatives : public Seq<Type_Alternative> {
    public:
        explicit Type_Alternatives() : Seq("Type_Alternatives") {}
    };

    class Typecase : public Statement {
        Expr& expr_; // An expression we want to downcast to a more specific class
        Type_Alternatives& cases_;    // A case for each potential type
    public:
        explicit Typecase(Expr& expr, Type_Alternatives& cases) :
                expr_{expr}, cases_{cases} {};
        void json(std::ostream& out, AST_print_context& ctx) override;
        
    };


    class StrConst : public Expr {
        std::string value_;
        TC::class_n_t class_name = std::string("String");
    public:
        TC::class_n_t* last_type;
        explicit StrConst(std::string v) : value_{v} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        std::string get_value(){ return value_; }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class,TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            //std::cout << "StrConst in " << cur_class << std::endl;
            return class_name;
        }
        TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            return class_name;
        }
        bool is_this(){ return false; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ return class_name; }

    };

    class NothingConst : public Expr {
        std::string value_;
        TC::class_n_t class_name = std::string("Nothing");
    public:
        TC::class_n_t* last_type;
        explicit NothingConst(std::string v) : value_{v} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class,TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            //std::cout << "NothingConst in " << cur_class << std::endl;
            return class_name;
        }
        TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            return class_name;
        }
        bool is_this(){ return false; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ return class_name; }
    };

    class Actuals : public Seq<Expr> {
    public:
        explicit Actuals() : Seq("Actuals") {}
        std::vector<Expr*>& get_actuals(){ return elements_; }
    };


    /* Constructors are different from other method calls. They
      * are static (not looked up in the vtable), have no receiver
      * object, and have their own type-checking rules.
      */
    class Construct : public Expr {
        MethodIdent&  method_;           /* Method name is same as class name */
        Actuals& actuals_;    /* Actual arguments to constructor */
    public:
        TC::class_n_t* last_type;
        explicit Construct(MethodIdent& method, Actuals& actuals) :
                method_{method}, actuals_{actuals} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        std::string get_name(){ return method_.get_name(); }
        Actuals& get_actuals(){ return actuals_; }
        bool is_this(){ return false; }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            return get_type_tc(cur_class, class_table, this_table, local_table);
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            for(auto a : actuals_.get_actuals()){
                a->get_type_tc(cur_class, class_table, this_table, local_table);
            }
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ return method_.get_name(); }
    };


    /* Method calls are central to type checking and code
     * generation ... and for us, the operators +, -, etc
     * are method calls to specially named methods.
     */
    class Call : public Expr {
        Expr& receiver_;        /* Expression computing the receiver object */
        MethodIdent& method_;         /* Identifier of the method */
        Actuals& actuals_;     /* List of actual arguments */
    public:
        TC::class_n_t* last_type;
        explicit Call(Expr& receiver, MethodIdent& method, Actuals& actuals) :
                receiver_{receiver}, method_{method}, actuals_{actuals} {
                    last_type = new std::string("Call uninit");
                };
        // Convenience factory for the special case of a method
        // created for a binary operator (+, -, etc).
        static Call* binop(std::string opname, Expr& receiver, Expr& arg);
        void json(std::ostream& out, AST_print_context& ctx) override;
        Expr& get_receiver(){ return receiver_; }
        std::string get_method_name(){ return method_.get_name(); }
        Actuals& get_actuals(){ return actuals_; }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table);
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return get_type_tc(cur_class, class_table, this_table, local_table);
        // }
        bool is_this(){ return false; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            receiver_.get_type_tc(cur_class, class_table, this_table, local_table);
            for(auto a : actuals_.get_actuals()){
                a->get_type_tc(cur_class, class_table, this_table, local_table);
            }
            auto type = get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = type.compare(TC::FAIL_NAME) == 0;
            std::cout << "Call resolved to type " << type << std::endl;
            if(fail){
                return TC::FAIL;
            }
            last_type = new std::string(type);
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ return *last_type; }
    };


    // Virtual base class for binary operations.
    // Does NOT include +, -, *, /, etc, which
    // are method calls.
    // Does include And, Or, Dot, ...
   class BinOp : public Expr {
    protected:
        std::string opsym;
        Expr &left_;
        Expr &right_;
        BinOp(std::string sym, Expr &l, Expr &r) :
                opsym{sym}, left_{l}, right_{r} {
                    last_type = new std::string("BinOp uninit");
                };
    public:
        TC::class_n_t* last_type;
        void json(std::ostream& out, AST_print_context& ctx) override;
        Expr& get_left(){ return left_; }
        Expr& get_right(){ return right_; }
        std::string get_op_sym(){ return opsym; }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){ std::cout << "ERROR INVOKED ABSTRACT CLASS METHOD BinOp" << std::endl;  return TC::FAIL_NAME; };
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return TC::FAIL_NAME;
        // }
        bool is_this(){ return false; }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ return *last_type; }
    };

   class And : public BinOp {
   public:
        explicit And(Expr& left, Expr& right) :
            BinOp("And", left, right) {}
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            //std::cout << "And in " << cur_class << std::endl;
            auto ltype = left_.get_type_tc(cur_class, class_table, this_table, local_table);
            auto rtype = right_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(ltype.compare(TC::FAIL_NAME) == 0 || rtype.compare(TC::FAIL_NAME) == 0){
                return TC::FAIL_NAME;
            }

            if(TC::is_subclass_of(class_table, ltype, std::string("Boolean"))
            && TC::is_subclass_of(class_table, rtype, std::string("Boolean"))){
                last_type = new std::string("Boolean");
                return std::string("Boolean");
            }

            return TC::FAIL_NAME;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            left_.get_type_tc(cur_class, class_table, this_table, local_table);
            right_.get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return get_type_tc(cur_class, class_table, this_table, local_table);
        // }
        
   };

    class Or : public BinOp {
    public:
        explicit Or(Expr& left, Expr& right) :
                BinOp("Or", left, right) {}
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            //std::cout << "Or in " << cur_class << std::endl;
            auto ltype = left_.get_type_tc(cur_class, class_table, this_table, local_table);
            auto rtype = right_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(ltype.compare(TC::FAIL_NAME) == 0 || rtype.compare(TC::FAIL_NAME) == 0){
                return TC::FAIL_NAME;
            }

            if(TC::is_subclass_of(class_table, ltype, std::string("Boolean"))
            && TC::is_subclass_of(class_table, rtype, std::string("Boolean"))){
                last_type = new std::string("Boolean");
                return std::string("Boolean");
            }

            return TC::FAIL_NAME;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            left_.get_type_tc(cur_class, class_table, this_table, local_table);
            right_.get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return get_type_tc(cur_class, class_table, this_table, local_table);
        // }
        
    };

    class Not : public Expr {
        Expr& left_;
    public:
        TC::class_n_t* last_type;
        explicit Not(Expr& left ):
            left_{left}  {}
        void json(std::ostream& out, AST_print_context& ctx) override;
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            std::cout << "Not in " << cur_class << std::endl;
            auto ltype = left_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(ltype.compare(TC::FAIL_NAME) == 0){
                return TC::FAIL_NAME;
            }

            if(TC::is_subclass_of(class_table, ltype, std::string("Boolean"))){
                last_type = new std::string("Boolean");
                return std::string("Boolean");
            }

            return TC::FAIL_NAME;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            left_.get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        // TC::class_n_t get_type_tc_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
        //     return get_type_tc(cur_class, class_table, this_table, local_table);
        // }
        bool is_this(){ return false; }
    };


    /* Can a field de-reference (expr . IDENT) be a binary
     * operation?  It can be evaluated to a location (l_exp),
     * whereas an operation like * and + cannot, but maybe that's
     * ok.  We'll tentatively group it with Binop and consider
     * changing it later if we need to make the distinction.
     */

    class Dot : public LExpr {
        Expr& left_;
        Ident& right_;
    public:
        TC::class_n_t* last_type;
        explicit Dot (Expr& left, Ident& right) :
           left_{left},  right_{right} {
               last_type = new std::string("Dot uninit");
           }
        void json(std::ostream& out, AST_print_context& ctx) override;
        Expr& get_left(){ return left_; };
        Ident& get_right(){ return right_; }
        bool is_this(){ return false; }
        bool has_this_as_left(){
            bool l_is_this = left_.is_this();
            
            //std::cout << "Dot being probed for left_is_this, returning " << l_is_this << std::endl; 
            return l_is_this; 
        }
        
        bool is_local(){ return false; }
        const std::string& get_ident(){ return right_.get_ident(); }
        TC::class_n_t get_type_tc(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            
            auto ltype = left_.get_type_tc(cur_class, class_table, this_table, local_table);
            if(ltype.compare(TC::FAIL_NAME) == 0){
                std::cout << "Dot [BAD_TYPE]." << right_.get_ident() << " GOT BAD TYPE" << std::endl; 
                return TC::FAIL_NAME;
            }
            auto r_ident = right_.get_ident();
            std::cout << "Dot in " << cur_class << "(" << ltype << "." << r_ident << ")" << std::endl;

            if(left_.is_this()){
                auto type = TC::recursive_lookup(class_table, cur_class, r_ident);
                if(type.compare(TC::FAIL_NAME) == 0){
                    std::cout << "Dot recursive lookup (" << cur_class << ", " << r_ident << ") FAILED" << std::endl;
                }
                left_.set_last_type(ltype);
                last_type = new std::string(type);
                return type;
            }

            auto type =  TC::recursive_lookup(class_table, ltype, r_ident);
            if(type.compare(TC::FAIL_NAME) == 0){
                std::cout << "Dot recursive lookup (" << cur_class << ", " << r_ident << ") FAILED" << std::endl;
            }
            left_.set_last_type(ltype);
            right_.set_last_type(type);
            last_type = new std::string(type);
            return type;
        }
        TC::Status process_type(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            left_.get_type_tc(cur_class, class_table, this_table, local_table);
            bool fail = get_type_tc(cur_class, class_table, this_table, local_table).compare(TC::FAIL_NAME) == 0;
            if(fail){
                return TC::FAIL;
            }
            return TC::GOOD;
        }
        TC::Status process_type_constructor(TC::class_n_t& cur_class, TC::class_table_t& class_table, TC::var_table_t& this_table, TC::var_table_t& local_table){
            process_type(cur_class, class_table, this_table, local_table);
        }
        TC::class_n_t get_last_type(){ 
            return *last_type; 
        }
    };


    /* A program has a set of classes (in any order) and a block of
     * statements.
     */
    class Program : public ASTNode {
    public:
        Classes& classes_;
        Block& statements_;
        explicit Program(Classes& classes, Block& statements) :
                classes_{classes}, statements_{statements} {}
        void json(std::ostream& out, AST_print_context& ctx) override;
    };



}


#endif //ASTNODE_H
