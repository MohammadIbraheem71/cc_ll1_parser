#include <iostream>
#include <string>

#include "grammar.h"
#include "parse_table.h"

// prints a set like { a, b, c }
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

static void print_productions(const Grammar& grammar) {
    std::cout << "Productions\n";
    int production_number = 1;
    for (const Production& production : grammar.get_productions()) {
        std::cout << "  " << production_number++ << ". " << production.left_side << " ->";
        if (production.right_side.empty()) std::cout << " " << EPSILON;
        for (const std::string& symbol : production.right_side) std::cout << " " << symbol;
        std::cout << "\n";
    }
}

static void print_first_sets(const Grammar& grammar) {
    std::cout << "FIRST sets\n";
    for (const std::string& nonterminal : grammar.get_nonterminals()) {
        std::set<std::string> first_set = grammar.get_first().at(nonterminal);
        // epsilon is not stored in the first sets, so add it here for nullable symbols
        if (grammar.get_nullable().count(nonterminal)) first_set.insert(EPSILON);
        std::cout << "  FIRST(" << nonterminal << ") = ";
        print_set(first_set);
        std::cout << "\n";
    }
}

static void print_follow_sets(const Grammar& grammar) {
    std::cout << "FOLLOW sets\n";
    for (const std::string& nonterminal : grammar.get_nonterminals()) {
        std::cout << "  FOLLOW(" << nonterminal << ") = ";
        print_set(grammar.get_follow().at(nonterminal));
        std::cout << "\n";
    }
}

int main() {
    const std::string file_path = "grammar.txt";

    // read the grammar from the file
    Grammar grammar;
    std::string error_message;
    if (!grammar.load_file(file_path, error_message)) {
        std::cerr << "Grammar error: " << error_message << "\n";
        return 1;
    }

    // compute nullable, first and follow
    grammar.compute_sets();

    std::cout << "Start symbol: " << grammar.get_start() << "\n\n";
    print_productions(grammar);
    std::cout << "\nTerminals: ";
    print_set(grammar.get_terminals());
    std::cout << "\n\nNullable: ";
    print_set(grammar.get_nullable());
    std::cout << "\n\n";
    print_first_sets(grammar);
    std::cout << "\n";
    print_follow_sets(grammar);

    // build the parse table from the sets and print it
    ParseTable parse_table;
    parse_table.build(grammar);
    std::cout << "\nParse table (terminal=production number)\n";
    parse_table.print(grammar);

    // a non empty conflict list means the grammar is not ll(1)
    return parse_table.get_conflicts().empty() ? 0 : 1;
}
