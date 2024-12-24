#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Scanner/Scanner.hpp"
#include "Parser/Parser.hpp"
#include "Parser/AstPrinter.hpp"
#include "Interpreter/Interpreter.hpp"

using namespace std;

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program (tokenize || parse || evaluate) <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        Scanner scanner(file_contents);
        scanner.print();
        if(!scanner.getErrStatus())
            return 0;
        else 
            return 65;
        
    } else if(command == "parse") {
        std::string file_contents = read_file_contents(argv[2]);

        Scanner scanner(file_contents);
        if(scanner.getErrStatus())
            return 65;

        Parser parser(scanner.getTokens());
        AstPrinter printer;
        try {
            cout << printer.print(parser.parse()) << endl;
            parser.cleanUpExpressions();
        } catch (runtime_error& err) {
            parser.cleanUpExpressions();
            cerr << err.what();
            return 65;
        }

        return 0;

    } else if(command == "evaluate") {

    } else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }

    return 0;

}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
