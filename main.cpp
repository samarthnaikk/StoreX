#include <iostream>
#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include "Database.h"

int main() {
    Database db;
    if (!db.loadFromFile("db_app.storex")) {
        std::cout << "No database file found. Nothing to extract." << std::endl;
        return 1;
    }

    bool found = false;
    for (const auto& r : db.getAllRecords()) {
        for (const auto& f : r.fields) {
            if (f.first == "image" && f.second.type == FieldType::Binary) {
                std::ofstream out("sample.png", std::ios::binary);
                if (out.is_open()) {
                    out.write(reinterpret_cast<const char*>(f.second.binValue.data()), f.second.binValue.size());
                    out.close();
                    std::cout << "Extracted sample.png from database." << std::endl;
                    found = true;
                }
            }
        }
    }
    if (!found) {
        std::cout << "sample.png not found in database." << std::endl;
    }
    return 0;
}
