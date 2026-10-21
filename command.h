#pragma once
#include <variant>
#include <string>
#include <optional>

struct KeywordQuery  { std::string word; };
struct PrefixQuery   { std::string prefix; };
struct DistanceQuery { std::string first; std::string second; int distance=0; };

using Query = std::variant<KeywordQuery, PrefixQuery, DistanceQuery>;

struct CreateCommand {std::string collection;};
struct InsertCommand {std::string collection; std::string document;};
struct PrintIndexCommand {std::string collection;};
struct SearchCommand {std::string collection; std::optional<Query> query;};

using Command = std::variant<CreateCommand, InsertCommand, PrintIndexCommand, SearchCommand>;