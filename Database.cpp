#include "Database.h"
#include <fstream>

bool Database::createRecord(const Record& record) {
    for (const auto& r : records) {
        if (r.id == record.id) return false; // Duplicate ID
    }
    records.push_back(record);
    return true;
}

Record* Database::readRecord(int id) {
    for (auto& r : records) {
        if (r.id == id) return &r;
    }
    return nullptr;
}

bool Database::updateRecord(int id, const std::unordered_map<std::string, std::string>& newFields) {
    for (auto& r : records) {
        if (r.id == id) {
            r.fields = newFields;
            return true;
        }
    }
    return false;
}

bool Database::deleteRecord(int id) {
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->id == id) {
            records.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Record> Database::getAllRecords() const {
    return records;
}

bool Database::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& r : records) {
        out << r.id;
        for (const auto& f : r.fields) {
            out << "|" << f.first << "=" << f.second;
        }
        out << "\n";
    }
    out.close();
    return true;
}

bool Database::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;
    records.clear();
    std::string line;
    while (std::getline(in, line)) {
        size_t pos = line.find('|');
        if (pos == std::string::npos) continue;
        int id = std::stoi(line.substr(0, pos));
        std::unordered_map<std::string, std::string> fields;
        size_t start = pos + 1;
        while (start < line.size()) {
            size_t next = line.find('|', start);
            std::string pair = line.substr(start, next - start);
            size_t eq = pair.find('=');
            if (eq != std::string::npos) {
                fields[pair.substr(0, eq)] = pair.substr(eq + 1);
            }
            if (next == std::string::npos) break;
            start = next + 1;
        }
        records.push_back({id, fields});
    }
    in.close();
    return true;
}
