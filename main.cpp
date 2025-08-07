#include <iostream>
#include "Database.h"

int main() {
    Database db;
    if (db.loadFromFile("db_app.storex")) {
        std::cout << "Records in db_app.storex:" << std::endl;
        for (const auto& r : db.getAllRecords()) {
            std::cout << r.id;
            for (const auto& f : r.fields) {
                std::cout << ", " << f.first << ": " << f.second;
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Failed to load database or file is empty." << std::endl;
    }
    return 0;
}
