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

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    cerr << "Logs from your program will appear here!" << endl;

    if (argc < 3) {
        cerr << "Usage: ./your_program (tokenize || parse || evaluate) <filename>" << endl;
        return 1;
    }

    const string command = argv[1];

    if (command == "tokenize") {
        string file_contents = read_file_contents(argv[2]);
        
        Scanner scanner(file_contents);
        scanner.print();
        if(!scanner.getErrStatus())
            return 0;
        else 
            return 65;
        
    } else if(command == "parse") {
        string file_contents = read_file_contents(argv[2]);

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
        string file_contents = read_file_contents(argv[2]);

        Scanner scanner(file_contents);
        if(scanner.getErrStatus())
            return 65;

        Parser parser(scanner.getTokens());
        Interpreter interpret;
        try {
            T eval = interpret.evaluate(*parser.parse());
            cout << interpret.to_string(eval) << endl;
            parser.cleanUpExpressions();
        } catch (runtime_error& err) {
            parser.cleanUpExpressions();
            cerr << err.what();
            return 65;
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
