#ifndef CSV_HELPER_H // [MODUL 8] Include Guards
#define CSV_HELPER_H

#include <string>
#include <vector> // [MODUL 8] Library Internal: <vector>
using namespace std;

vector<string> splitCSVRow(const string& row);
string escapeCSVField(const string& field);
string trimString(const string& str);
bool parseBoolean(const string& str);

#endif
