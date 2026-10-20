#include "lexer.h"
#include <stdexcept>
#include <unordered_map>
#include <cctype>


namespace 
{
    bool isDigit(char c){
        return c >= '0' && c <= '9';
    }
    bool isLetter(char c){
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    bool isWordChar(char c){
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')||(c >= '0' && c <= '9')|| (c=='_');
    }
    const std::unordered_map<std::string, TokenType> keyWords={
        {"CREATE", TokenType::Create},
        {"INSERT", TokenType::Insert},
        {"PRINT_INDEX", TokenType::PrintIndex},
        {"SEARCH", TokenType::Search},
        {"WHERE", TokenType::Where}
    };
}


std::vector<Token> tokenizer(const std::string& text){
    std::vector<Token> tokens;
    Token temp;
    std::string word;
    size_t i = 0;
    size_t j=0;
    while (i < text.length()){
        if (text[i] == ' ' || text[i] == '\t' || text[i] == '\r' || text[i] == '\n' )
        {
            i++;
            continue;
        }
        switch (text[i])
        {
        case ';':
            temp.type = TokenType::Semicolon;
            temp.value = std::string(1, text[i]);
            tokens.push_back(temp);
            i++;
            continue;
        case '<':
            temp.type = TokenType::Less;
            temp.value = std::string(1, text[i]);
            tokens.push_back(temp);
            i++;
            continue;
        case '>':
            temp.type = TokenType::Greater;
            temp.value = std::string(1, text[i]);
            tokens.push_back(temp);
            i++;
            continue;
        case '*':
            temp.type = TokenType::Star;
            temp.value = std::string(1, text[i]);
            tokens.push_back(temp);
            i++;
            continue;
        default:
            break;
        }
        if(text[i]=='"'){
            j = i+1;
            while(j < text.length() && text[j]!='"'){
                if(text[j]=='\n'){
                    throw std::runtime_error("string must not span multiple lines");
                }
                j++;
            }
            if(j==text.length()){
                throw std::runtime_error("missing closing quote");
            }
            temp.type = TokenType::String;
            temp.value = text.substr(i + 1, j - i - 1);
            tokens.push_back(temp);
            i = j+1;
            continue;
        }
        if(isLetter(text[i])) {
            j = i+1;
            while(j<text.length() && isWordChar(text[j])){
                j++;
            }
            word = text.substr(i, j - i);
            std::string upper = word;
            for(size_t k =0;k<word.length();k++) upper[k]=std::toupper(upper[k]);
            auto it = keyWords.find(upper);
            if (it !=keyWords.end()){
                temp.type = it->second;
            }else{
                temp.type = TokenType::Collection;
            }
            temp.value = word;
            tokens.push_back(temp);
            i = j;
            continue;
        }
        if(isDigit(text[i])){
            j=i+1;
            while(j<text.length() && isDigit(text[j])){
                j++;
            }
            temp.value = text.substr(i, j-i);
            temp.type = TokenType::Int;
            tokens.push_back(temp);
            i = j;
            continue;
        }
        throw std::runtime_error("unexpected character '" + std::string(1, text[i]) + "'");
    }
    temp.value = "end";
    temp.type = TokenType::End;
    tokens.push_back(temp);
    return tokens;
}