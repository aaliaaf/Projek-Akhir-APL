#ifndef UTILS_H // [MODUL 8] Include Guards
#define UTILS_H

#include <string> // [MODUL 1] Tipe Data Kolektif (string)
#include <ctime>
#include <iostream> // [MODUL 8] Library Internal
#include <stdexcept> // [MODUL 8] Library Internal: runtime_error, invalid_argument
using namespace std;

// [MODUL 8] Debugging Macro dengan __FILE__, __LINE__, __func__
#ifdef DEBUG
    #define ASSERT(cond, msg) \
        do { \
            if (!(cond)) { \
                cerr << "[ASSERT FAILED] " << msg << endl \
                     << "  File: " << __FILE__ << endl \
                     << "  Line: " << __LINE__ << endl \
                     << "  Func: " << __func__ << endl; \
                throw runtime_error(string("Assertion failed: ") + msg); \
            } \
        } while (false)
    #define DEBUG_LOG(msg) \
        cerr << "[DEBUG] " << __func__ << " (" << __FILE__ << ":" << __LINE__ << "): " << msg << endl
#else
    #define ASSERT(cond, msg)
    #define DEBUG_LOG(msg)
#endif

void clearScreen(); // [MODUL 3] Prosedur (void)
void pressEnter(); // [MODUL 3] Prosedur (void)
string getCurrentDate(); // [MODUL 3] Fungsi dengan return value
int stringToInt(const string& str); // [MODUL 3] Pass by Reference (&)
float stringToFloat(const string& str);

// [MODUL 3] Fungsi Overloading: nama sama, parameter beda
string toString(int val); // Overload: versi int
string toString(float val); // Overload: versi float

string formatID(const string& prefix, int num); // [MODUL 3] Default Parameter (prefix memiliki nilai default)

// [MODUL 3] Rekursif: fungsi yang memanggil dirinya sendiri
int faktorial(int n);

// [MODUL 4] Pointer sebagai parameter fungsi (pass by pointer / address-of)
void swapByPointer(int* a, int* b);

// [MODUL 3] Default Parameter
void displaySeparator(char ch = '=', int length = 40);

string getCurrentDateTime();

void logAudit(const string& action, const string& detail);

bool isCancelInput(const string& input);

#endif
