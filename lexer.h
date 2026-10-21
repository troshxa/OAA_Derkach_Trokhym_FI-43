#pragma once

#include <string>
#include <vector>

enum class TokenType{
    Create, Insert, PrintIndex,
    Search, Where, Collection,
    String, Semicolon, Int,
    Greater, Less, Star, End
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenizer(const std::string& text);
std::string toString(TokenType type);