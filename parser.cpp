#include "parser.h"
#include <stdexcept>

namespace {
    class Parser
    {
    private:
        const std::vector<Token>& tokens;
        size_t pos = 0;
        const Token& peek(){
            return tokens[pos];
        }
        const Token& advance(){
            if (tokens[pos].type == TokenType::End) {
                return tokens[pos];
            }
            pos++;
            return tokens[pos-1];
        }
        const Token& expect(TokenType type, const std::string& what){
            if(tokens[pos].type ==type){
                return advance();
            }
            if(peek().type == TokenType::End){
                throw std::runtime_error("expected " + what + ", got end of command");
            }
            throw std::runtime_error("expected " + what + ", got '" + peek().value + "'");
        }
        CreateCommand parseCreate(){
            return CreateCommand{expect(TokenType::Collection, "collection name").value};
        }
        InsertCommand parseInsert(){
            std::string coll = expect(TokenType::Collection, "collection name").value;
            return InsertCommand{coll, expect(TokenType::String, "document text in quotes").value};
        }
        PrintIndexCommand parsePrintIndex(){
            return PrintIndexCommand{expect(TokenType::Collection, "collection name").value};
        }
        SearchCommand parseSearch(){
            SearchCommand cmd{expect(TokenType::Collection, "collection name").value};
            if(peek().type == TokenType::Where){
                advance();
                cmd.query = parseQuery();
            }
            return cmd;
        }
        Query parseQuery(){
            std::string word = expect(TokenType::String, "search word").value;
            switch (peek().type)
            {
            case TokenType::Star:
                advance();
                return PrefixQuery{word};
            case TokenType::Less: {
                advance();
                int num;
                try {
                    num = std::stoi(expect(TokenType::Int, "distance").value);
                } catch (const std::out_of_range&) {
                    throw std::runtime_error("distance is too large");
                }
                if (num <= 0) {
                    throw std::runtime_error("distance must be a positive integer");
                }
                expect(TokenType::Greater, "'>'");
                return DistanceQuery{word,expect(TokenType::String, "second search word").value, num};
            }
            default:
                return KeywordQuery{word};
            }
        }

    public:
        explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
        Command parse(){
            Command cmd;
            switch (peek().type)
            {
            case TokenType::Create:
                advance();
                cmd = parseCreate();
                break;
            case TokenType::Insert:
                advance();
                cmd = parseInsert();
                break;
            case TokenType::PrintIndex:
                advance();
                cmd = parsePrintIndex();
                break;
            case TokenType::Search:
                advance();
                cmd = parseSearch();
                break;
            default:
                throw std::runtime_error("expected a command (CREATE, INSERT, PRINT_INDEX, SEARCH)");
            }
            expect(TokenType::Semicolon, "';'");
            expect(TokenType::End, "end of command");
            return cmd;
        }

    };
}

Command parser(const std::vector<Token>& tokens){
    Parser a(tokens);
    return a.parse();
}