
#include "Database.h"
#include <fstream>
#include <sstream>


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

bool Database::updateRecord(int id, const std::unordered_map<std::string, FieldValue>& newFields) {
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

// Custom .storex format: id|field:type=value|field:type=binary_base64|...
// Binary data is base64 encoded for text file compatibility
static std::string base64_encode(const std::vector<unsigned char>& data) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    int val=0, valb=-6;
    for (unsigned char c : data) {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) {
            out.push_back(table[(val>>valb)&0x3F]);
            valb-=6;
        }
    }
    if (valb>-6) out.push_back(table[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

static std::vector<unsigned char> base64_decode(const std::string& in) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T[table[i]] = i;
    std::vector<unsigned char> out;
    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0) {
            out.push_back((unsigned char)((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}

bool Database::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& r : records) {
        out << r.id;
        for (const auto& f : r.fields) {
            out << "|" << f.first << ":";
            if (f.second.type == FieldType::String) {
                out << "S=" << f.second.strValue;
            } else if (f.second.type == FieldType::Binary) {
                out << "B=" << base64_encode(f.second.binValue);
            }
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
        std::unordered_map<std::string, FieldValue> fields;
        size_t start = pos + 1;
        while (start < line.size()) {
            size_t next = line.find('|', start);
            std::string pair = line.substr(start, next - start);
            size_t colon = pair.find(':');
            size_t eq = pair.find('=');
            if (colon != std::string::npos && eq != std::string::npos) {
                std::string key = pair.substr(0, colon);
                char type = pair[colon+1];
                std::string value = pair.substr(eq+1);
                if (type == 'S') {
                    fields[key] = FieldValue(value);
                } else if (type == 'B') {
                    fields[key] = FieldValue(base64_decode(value));
                }
            }
            if (next == std::string::npos) break;
            start = next + 1;
        }
        records.push_back({id, fields});
    }
    in.close();
    return true;
}
