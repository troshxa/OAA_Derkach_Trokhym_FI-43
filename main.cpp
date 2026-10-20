#include "lexer.h"
#include <iostream>
#include <stdexcept>

std::string toString(TokenType type){
    switch (type)
    {
    case TokenType::Create:     return "Create";
    case TokenType::Insert:     return "Insert";
    case TokenType::PrintIndex: return "PrintIndex";
    case TokenType::Search:     return "Search";
    case TokenType::Where:      return "Where";
    case TokenType::Collection: return "Collection";
    case TokenType::String:     return "String";
    case TokenType::Semicolon:  return "Semicolon";
    case TokenType::Int:        return "Int";
    case TokenType::Greater:    return "Greater";
    case TokenType::Less:       return "Less";
    case TokenType::Star:       return "Star";
    case TokenType::End:        return "End";
    }
    return "Unknown";
}

void printTokens(const std::string& text){
    std::cout << "> " << text << "\n";
    try {
        for (const Token& token : tokenizer(text)) {
            std::cout << "  " << toString(token.type) << " [" << token.value << "]\n";
        }
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }
}

int main() {
    printTokens("CREATE wiki;");
    printTokens("INSERT wiki \"to be ; or not\";");
    printTokens("search wiki where \"has\" <2> \"roots\";");
    printTokens("SEARCH wiki WHERE \"lat\"*;");
    printTokens("INSERT wiki \"abc;");
    printTokens("CREATE wi-ki;");
    return 0;
}
