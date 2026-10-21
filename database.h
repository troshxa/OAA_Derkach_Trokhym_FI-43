#pragma once
#include "command.h"
#include <optional>
#include <string>
#include <vector>

class Database {
public:
    void create(const std::string& name);
    void insert(const std::string& name, const std::string& document);
    std::string printIndex(const std::string& name);
    std::vector<std::string> search(const std::string& name, const std::optional<Query>& query);
};
