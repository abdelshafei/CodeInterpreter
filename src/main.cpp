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

string read_file_contents(const string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    cout << unitbuf;
    cerr << unitbuf;

    if (argc < 3) {
        cerr << "Usage: ./your_program <command> <filename>\n"
        << "    choose one of the available commands:\n"
        << "        1- tokenize\n"
        << "        2- parse\n"
        << "        3- evaluate\n"
        << "        4- run (beta)" << endl;
        return 1;
    }

    const string command = argv[1];

    if (command == "tokenize") { // lexical analysis
        string file_contents = read_file_contents(argv[2]);
        
        try {
            Scanner scanner(file_contents);
            if(!scanner.getErrStatus()) {
                scanner.print();
                return 0;
            } else 
                return 65;
        } catch (invalid_argument& err) {
            cerr << err.what();
            return 65;
        }
        
    } else if(command == "parse") { // expression analysis
        string file_contents = read_file_contents(argv[2]);

        try { // checks for any unterminated comment blocks
            Scanner scan(file_contents);
        } catch (invalid_argument& err) {
            cerr << err.what();
            return 65;
        }

        Scanner scanner(file_contents);
        if(scanner.getErrStatus())
            return 65;

        Parser parser(scanner.getTokens());
        AstPrinter printer;
        try {
            cout << printer.print(parser.parseExpr()) << endl;
        } catch (runtime_error& err) {
            cerr << err.what();
            return 65;
        }

        return 0;

    } else if(command == "evaluate") { // evaluation analysis
        string file_contents = read_file_contents(argv[2]);

        try { // checks for any unterminated comment blocks
            Scanner scan(file_contents);
        } catch (invalid_argument& err) {
            cerr << err.what();
            return 65;
        }

        Scanner scanner(file_contents);
        if(scanner.getErrStatus())
            return 65;

        Parser parser(scanner.getTokens());
        Interpreter interpret;

        try {
            string eval = interpret.to_string(interpret.evaluate(*parser.parseExpr()));
            cout << eval << endl;
        } catch (runtime_error& err) {
            cerr << err.what();
            return 70;
        }
    } else if(command == "run") {
                string file_contents = read_file_contents(argv[2]);

        try { // checks for any unterminated comment blocks
            Scanner scan(file_contents);
        } catch (invalid_argument& err) {
            cerr << err.what();
            return 65;
        }

        Scanner scanner(file_contents);
        if(scanner.getErrStatus())
            return 65;

        Parser parser(scanner.getTokens());
        Interpreter interpret;

        try {
            interpret.interpret(parser.parseStmt());
        } catch (runtime_error& err) {
            cerr << err.what();
            return 70;
        }
    } else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }

    return 0;

}

string read_file_contents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error reading file: " << filename << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
