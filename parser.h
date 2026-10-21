#pragma once
#include "command.h"
#include "lexer.h"
Command parser(const std::vector<Token>& tokens);
