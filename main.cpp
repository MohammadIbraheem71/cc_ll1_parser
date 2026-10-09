#include <iostream>
#include <string>

#include "grammar.h"

static void print_set(const std::set<std::string>& symbol_set) {
    std::cout << "{ ";
    bool is_first_item = true;
    for (const std::string& symbol : symbol_set) {
        if (!is_first_item) std::cout << ", ";
        std::cout << symbol;
        is_first_item = false;
    }
    std::cout << " }";
}

int main() {
    const std::string file_path = "grammar.txt";

    Grammar grammar;
    std::string error_message;
    if (!grammar.load_file(file_path, error_message)) {
        std::cerr << "Grammar error: " << error_message << "\n";
        return 1;
    }
    grammar.compute_sets();

    std::cout << "Start symbol: " << grammar.get_start() << "\n\n";

    std::cout << "Productions\n";
    int production_number = 1;
    for (const Production& production : grammar.get_productions()) {
        std::cout << "  " << production_number++ << ". " << production.left_side << " ->";
        if (production.right_side.empty()){
            std::cout << " " << EPSILON;
        }

        for (const std::string& symbol : production.right_side){
            std::cout << " " << symbol;
        }

        std::cout << "\n";
    }

    std::cout << "\nTerminals: ";
    print_set(grammar.get_terminals());
    std::cout << "\n\nNullable: ";
    print_set(grammar.get_nullable());

    // printing out the first sets
    std::cout << "\n\nFIRST sets\n";
    for (const std::string& nonterminal : grammar.get_nonterminals()) {
        std::set<std::string> first_set = grammar.get_first().at(nonterminal);
        if (grammar.get_nullable().count(nonterminal)){
            first_set.insert(EPSILON);
        }
        std::cout << "  FIRST(" << nonterminal << ") = ";
        print_set(first_set);
        std::cout << "\n";
    }

    // printing out the follow sets
    std::cout << "\nFOLLOW sets\n";
    for (const std::string& nonterminal : grammar.get_nonterminals()) {
        std::cout << "  FOLLOW(" << nonterminal << ") = ";
        print_set(grammar.get_follow().at(nonterminal));
        std::cout << "\n";
    }
    return 0;
}
