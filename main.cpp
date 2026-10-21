#include "cli.h"
#include "database.h"

int main() {
    Database db;
    Cli cli(db);
    cli.run();
    return 0;
}