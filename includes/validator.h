#ifndef VALIDATOR_H // [MODUL 8] Include Guards
#define VALIDATOR_H

#include <string>
using namespace std;

bool isValidPhoneNumber(const string& phone);
bool isValidDate(const string& date);
int getValidInt(const string& prompt);
float getValidFloat(const string& prompt);
string getValidString(const string& prompt);
int getMenuChoice(int min, int max);
string getValidDateSplit();

#endif
