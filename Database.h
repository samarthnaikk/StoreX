#pragma once
#include <vector>
#include <string>
#include <unordered_map>

struct Record {
    int id;
    std::unordered_map<std::string, std::string> fields;
};

class Database {
public:
    bool createRecord(const Record& record);
    Record* readRecord(int id);
    bool updateRecord(int id, const std::unordered_map<std::string, std::string>& newFields);
    bool deleteRecord(int id);
    std::vector<Record> getAllRecords() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
private:
    std::vector<Record> records;
};
