#include "cli.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <stdexcept>

void Cli::run(){
    std::string text;
    while (readCommand(text)) {
        try {
            execute(parser(tokenizer(text)));
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

bool Cli::readCommand(std::string& text){
    text.clear();
    bool inString = false;
    std::string line;
    while (true) {
        std::cout << (text.empty() ? "> " : "... ");
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            if (!text.empty()) {
                std::cout << "Error: unexpected end of input, command must end with ';'\n";
            }
            return false;
        }
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '"') {
                inString = !inString;
            } else if (line[i] == ';' && !inString) {
                text += line.substr(0, i + 1);
                return true;
            }
        }
        if (text.empty() && line.find_first_not_of(" \t\r") == std::string::npos) {
            continue;
        }
        text += line + "\n";
    }
}

void Cli::execute(const Command& command){
    if (const auto* create = std::get_if<CreateCommand>(&command)) {
        db.create(create->collection);
        std::cout << "Collection " << create->collection << " has been created\n";
    } else if (const auto* insert = std::get_if<InsertCommand>(&command)) {
        db.insert(insert->collection, insert->document);
        std::cout << "Document has been added to " << insert->collection << "\n";
    } else if (const auto* print = std::get_if<PrintIndexCommand>(&command)) {
        std::cout << db.printIndex(print->collection);
    } else if (const auto* search = std::get_if<SearchCommand>(&command)) {
        std::vector<std::string> documents = db.search(search->collection, search->query);
        if (documents.empty()) {
            std::cout << "No documents found\n";
        }
        for (const std::string& document : documents) {
            std::cout << "\"" << document << "\"\n";
        }
    }
}
