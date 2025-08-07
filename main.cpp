#include <iostream>
#include "Database.h"

int main() {
    Database db;
    Record r1{1, {{"name", "Alice"}, {"email", "alice@example.com"}}};
    Record r2{2, {{"name", "Bob"}, {"email", "bob@example.com"}}};

    db.createRecord(r1);
    db.createRecord(r2);

    Record* rec = db.readRecord(1);
    if (rec) {
        std::cout << "Record 1: " << rec->fields["name"] << ", " << rec->fields["email"] << std::endl;
    }

    db.updateRecord(2, {{"name", "Bob Updated"}, {"email", "bob@newmail.com"}});
    rec = db.readRecord(2);
    if (rec) {
        std::cout << "Record 2: " << rec->fields["name"] << ", " << rec->fields["email"] << std::endl;
    }

    db.deleteRecord(1);
    std::cout << "All records after deletion:" << std::endl;
    for (const auto& r : db.getAllRecords()) {
        std::cout << r.id << ": " << r.fields.at("name") << ", " << r.fields.at("email") << std::endl;
    }
    return 0;
}
