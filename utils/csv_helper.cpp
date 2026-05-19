// [MODUL 1] Tipe Data Kolektif: vector
// [MODUL 2] Vector: array dinamis

#include "csv_helper.h"
#include <sstream>
#include <cctype>

vector<string> splitCSVRow(const string& row) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < row.size(); i++) {
        char ch = row[i];
        if (inQuotes) {
            if (ch == '"') {
                if (i + 1 < row.size() && row[i + 1] == '"') {
                    field.push_back('"');
                    i++;
                } else {
                    inQuotes = false;
                }
            } else {
                field.push_back(ch);
            }
        } else {
            if (ch == '"') {
                inQuotes = true;
            } else if (ch == ',') {
                fields.push_back(trimString(field));
                field.clear();
            } else {
                field.push_back(ch);
            }
        }
    }

    fields.push_back(trimString(field));

    return fields;
}

string escapeCSVField(const string& field) {
    bool needQuotes = false;
    for (char ch : field) {
        if (ch == ',' || ch == '"' || ch == '\n' || ch == '\r') {
            needQuotes = true;
            break;
        }
    }

    if (!needQuotes) return field;

    string escaped = "\"";
    for (char ch : field) {
        if (ch == '"') escaped += "\"\"";
        else escaped.push_back(ch);
    }
    escaped += "\"";
    return escaped;
}

string trimString(const string& str) {
    int start = 0;
    int end = str.length() - 1;

    // [MODUL 1] while loop + operator logika
    while (start <= end && (str[start] == ' ' || str[start] == '\r')) {
        start++; // [MODUL 1] Operator Increment: ++
    }

    while (end >= start && (str[end] == ' ' || str[end] == '\r')) {
        end--;
    }

    return str.substr(start, end - start + 1);
}

bool parseBoolean(const string& str) {
    string lower = "";
    for (char c : str) { // [MODUL 1] foreach loop
        lower += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return lower == "true";
}
