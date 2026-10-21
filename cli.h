#pragma once
#include "command.h"
#include "database.h"
#include <string>

class Cli {
private:
    Database& db;

    bool readCommand(std::string& text);
    void execute(const Command& command);

public:
    explicit Cli(Database& db) : db(db) {}
    void run();
};
