// [MODUL 1] Tipe Data Kolektif: vector
// [MODUL 2] Vector: array dinamis

#include "csv_helper.h"
#include <sstream>

vector<string> splitCSVRow(const string& row) {
    vector<string> fields; // [MODUL 2] Vector: tipe data kolektif dinamis
    stringstream ss(row);
    string field;

    while (getline(ss, field, ',')) { // [MODUL 1] Perulangan while
        fields.push_back(trimString(field)); // [MODUL 2] CRUD Vector: Create (push_back)
    }

    return fields;
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
        lower += tolower(c); // [MODUL 1] Operator Penugasan: +=
    }
    return lower == "true";
}
