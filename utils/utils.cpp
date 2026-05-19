#include "utils.h"
#include <iostream> // [MODUL 1] I/O: cout, cin
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

// [MODUL 3] Prosedur: void, tanpa return value
void clearScreen() {
    for (int i = 0; i < 40; i++) { // [MODUL 1] Perulangan for
        cout << endl; // [MODUL 1] I/O: cout
    }
}

void pressEnter() {
    cout << endl << "Tekan Enter untuk melanjutkan..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // [MODUL 1] I/O: cin
}

// [MODUL 3] Fungsi dengan return value
string getCurrentDate() {
    time_t t = time(nullptr); // [MODUL 1] Variabel primitif
    struct tm* now = localtime(&t); // [MODUL 4] Pointer: & address-of, * dereference
    stringstream ss;
    ss << (now->tm_year + 1900) << "-" // [MODUL 4] Arrow operator (->) pada struct pointer
       << setw(2) << setfill('0') << (now->tm_mon + 1) << "-"
       << setw(2) << setfill('0') << now->tm_mday;
    return ss.str();
}

// [MODUL 8] Exception Handling: try-catch
int stringToInt(const string& str) {
    try {
        return stoi(str); // [MODUL 1] Tipe Data: int
    } catch (const invalid_argument&) { // [MODUL 8] Tipe Exception: invalid_argument
        return 0;
    } catch (const out_of_range&) { // [MODUL 8] Tipe Exception: out_of_range
        return 0;
    }
}

// [MODUL 8] Exception Handling: try-catch, multiple exception types
float stringToFloat(const string& str) {
    try {
        return stof(str); // [MODUL 1] Tipe Data: float
    } catch (const invalid_argument&) {
        return 0.0f;
    } catch (const out_of_range&) {
        return 0.0f;
    }
}

// [MODUL 3] Fungsi Overloading: toString(int)
string toString(int val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

// [MODUL 3] Fungsi Overloading: toString(float)
string toString(float val) {
    stringstream ss;
    ss << fixed << setprecision(0) << val;
    return ss.str();
}

// [MODUL 3] Default Parameter
string formatID(const string& prefix, int num) {
    stringstream ss;
    ss << prefix;
    ss << setw(3) << setfill('0') << num;
    return ss.str();
}

// [MODUL 3] Default Parameter
void displaySeparator(char ch, int length) {
    for (int i = 0; i < length; i++) { // [MODUL 1] Perulangan for
        cout << ch;
    }
    cout << endl;
}

// [MODUL 3] Rekursif: fungsi memanggil dirinya sendiri
// faktorial(n) = n * faktorial(n-1), base case n <= 1
int faktorial(int n) {
    // [MODUL 1] Percabangan if (base case rekursif)
    if (n <= 1) { // [MODUL 1] Operator Perbandingan: <=
        return 1;
    }
    // [MODUL 3] Rekursif: return memanggil dirinya sendiri
    return n * faktorial(n - 1); // [MODUL 1] Operator Aritmatika: *
}

// [MODUL 4] Pointer sebagai parameter fungsi
// [MODUL 4] Pass by pointer (bukan pass by value, bukan pass by reference)
void swapByPointer(int* a, int* b) {
    int temp = *a; // [MODUL 4] Dereference: mengambil nilai dari alamat pointer
    *a = *b; // [MODUL 4] Dereference: menulis nilai ke alamat pointer
    *b = temp;

    // [MODUL 3] Variabel Lokal: temp hanya ada di dalam fungsi ini
    // [MODUL 3] Variabel Global: a dan b adalah pointer ke variabel di luar fungsi
}

// [MODUL 2] Nested Struct: struct yang memiliki field bertipe struct lain
struct Alamat {
    string jalan;
    string kota;
    string kodePos;
};

string getCurrentDateTime() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    stringstream ss;
    ss << (now->tm_year + 1900) << "-"
       << setw(2) << setfill('0') << (now->tm_mon + 1) << "-"
       << setw(2) << setfill('0') << now->tm_mday << " "
       << setw(2) << setfill('0') << now->tm_hour << ":"
       << setw(2) << setfill('0') << now->tm_min << ":"
       << setw(2) << setfill('0') << now->tm_sec;
    return ss.str();
}

bool isCancelInput(const string& input) {
    return input == "0" || input == "batal" || input == "exit";
}

void logAudit(const string& action, const string& detail) {
    ofstream file("data/audit.log", ios::app);
    if (!file.is_open()) return;
    file << "[" << getCurrentDateTime() << "] " << action << ": " << detail << endl;
    file.close();
}
