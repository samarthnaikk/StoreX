
#pragma once
#include <vector>
#include <string>
#include <unordered_map>

enum class FieldType {
    String,
    Binary
};

struct FieldValue {
    FieldType type;
    std::string strValue;
    std::vector<unsigned char> binValue;

    FieldValue() : type(FieldType::String) {}
    FieldValue(const std::string& s) : type(FieldType::String), strValue(s) {}
    FieldValue(const std::vector<unsigned char>& b) : type(FieldType::Binary), binValue(b) {}
};

struct Record {
    int id;
    std::unordered_map<std::string, FieldValue> fields;
};

class Database {
public:
    bool createRecord(const Record& record);
    Record* readRecord(int id);
    bool updateRecord(int id, const std::unordered_map<std::string, FieldValue>& newFields);
    bool deleteRecord(int id);
    std::vector<Record> getAllRecords() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
private:
    std::vector<Record> records;
};
