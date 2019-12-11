//
// The parser driver just glues together a parser object
// and a lexer object.
//

#include "lex.yy.h"
#include "ASTNode.h"
#include "Messages.h"
#include "ir_gen.h"

#include "type_shared.h"
#include "TypeChecker.h"
#include <iostream>
#include <unistd.h>  // getopt is here

class Driver {
    int debug_level = 0;
public:
    explicit Driver(const reflex::Input in) : lexer(in), parser(new yy::parser(lexer, &root)) { root = nullptr; }

    ~Driver() { delete parser; }

    void debug() { debug_level = 1; }

    AST::ASTNode *parse() {
        parser->set_debug_level(debug_level); // 0 = no debugging, 1 = full tracing
        // std::cout << "Running parser\n";
        int result = parser->parse();
        if (result == 0 && report::ok()) {  // 0 == success, 1 == failure
            // std::cout << "Extracting result\n";
            if (root == nullptr) {
                std::cout << "But I got a null result!  How?!\n";
            }
            return root;
        } else {
            // std::cout << "Parse failed, no tree\n";
            return nullptr;
        }
    }

private:
    yy::Lexer lexer;
    yy::parser *parser;
    AST::ASTNode *root;
};

int main(int argc, char **argv) {
    std::string filename;
    std::string outfile;
    char c;
    FILE *f;
    int index;
    int debug; // 0 = no debugging, 1 = full tracing

    if(argc < 2){
        std::cout << "Usage = parser <input_path>" << std::endl;
        std::cout << "<input_path> = path to input file, no default" << std::endl;
        std::cout << "path to output file by default is ./ir_out/[input_file_name].c" << std::endl;
        return 1;
    }

    while ((c = getopt(argc, argv, "t")) != -1) {
        if (c == 't') {
            std::cerr <<  "Debugging mode\n";
            debug = 1;
        }
    }

    for (index = optind; index < argc; ++index) {
        if( !(f = fopen(argv[index], "r"))) {
            perror(argv[index]);
            exit(1);
        }
        
        Driver driver(f);
        if (debug) driver.debug();
        AST::ASTNode *root = driver.parse();
        if (root != nullptr) {
            // std::cout << "Parsed!\n";
            //AST::AST_print_context context;
            //root->json(std::cout, context);
            //std::cout << "\n\n " << std::endl;
            TC::class_table_t* classes;
            TC::var_table_t* body_table;
            auto status = TC::check_program(*((AST::Program*) root), &classes, &body_table);
            
            AST::AST_print_context context;
            root->json(std::cout, context);
            std::cout << "\n\n " << std::endl;

            std::cout << "FAIL = " << TC::FAIL << std::endl;
            std::cout << "GOOD = " << TC::GOOD << std::endl;
            std::cout << "FINAL STATUS = " << status << std::endl;
            if(status == TC::GOOD){
                std::string in_path = std::string(argv[index]);
                std::string out_path = std::string("./ir_out/") + in_path.substr(in_path.find_last_of('/') + 1, in_path.find_last_of('.')) + std::string(".c");
                generate_program_ir(((AST::Program*) root), classes, body_table, out_path);
                std::cout << "writing to " << out_path << std::endl;
            }

        } else {
            std::cout << "No tree produced." << std::endl;
        }
    }

}
