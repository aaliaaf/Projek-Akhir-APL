#include "validator.h"
#include "utils.h"
#include <iostream> // [MODUL 1] I/O: cout, cin
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

static bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

static int daysInMonth(int year, int month) {
    if (month < 1 || month > 12) return 0;
    const int normalDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return normalDays[month - 1];
}

// [MODUL 1] Tipe Data Primitif: bool, string
// [MODUL 1] Percabangan if dengan operator logika (&&, ||)
bool isValidPhoneNumber(const string& phone) {
    // [MODUL 1] Operator Perbandingan: <, >, ||
    if (phone.length() < 10 || phone.length() > 14) { // [MODUL 1] Operator Logika: OR (||)
        return false;
    }

    // [MODUL 1] Perulangan foreach (range-based for)
    for (char c : phone) { // [MODUL 1] foreach loop
        // [MODUL 1] Operator Perbandingan, Operator Logika AND (&&)
        if (c < '0' || c > '9') { // [MODUL 1] Operator Logika: OR (||)
            return false;
        }
    }

    return true;
}

// [MODUL 1] Nested if: if di dalam if
bool isValidDate(const string& date) {
    if (date.length() != 10) {
        return false;
    }

    // [MODUL 1] Operator Perbandingan: ==
    if (date[4] != '-' || date[7] != '-') {
        return false;
    }

    // [MODUL 1] Perulangan for dengan nested if
    for (int i = 0; i < 10; i++) { // [MODUL 1] Operator Increment: i++
        // [MODUL 1] Nested if: if di dalam for
        if (i == 4 || i == 7) continue; // [MODUL 1] continue (skip iterasi)
        // [MODUL 1] Nested if (lanjutan)
        if (date[i] < '0' || date[i] > '9') {
            return false;
        }
    }

    int year = stringToInt(date.substr(0, 4));
    int month = stringToInt(date.substr(5, 2));
    int day = stringToInt(date.substr(8, 2));

    // [MODUL 1] Operator Logika AND (&&)
    if (month < 1 || month > 12) return false;
    int maxDay = daysInMonth(year, month);
    if (day < 1 || day > maxDay) return false;

    return true;
}

// [MODUL 1] while loop untuk validasi input
int getValidInt(const string& prompt) {
    int val;
    string input;

    // [MODUL 1] Perulangan while
    while (true) {
        cout << prompt; // [MODUL 1] I/O: cout
        if (!getline(cin, input)) {
            cout << "Input stream ditutup. Program berhenti." << endl;
            exit(0);
        }

        // [MODUL 1] Operator Logika: &&
        stringstream ss(input);
        if (ss >> val && ss.eof()) { // [MODUL 1] Operator Logika: AND (&&)
            return val; // [MODUL 3] keyword return
        }

        cout << "Input harus berupa angka!" << endl;
    }
}

float getValidFloat(const string& prompt) {
    float val;
    string input;

    while (true) {
        cout << prompt;
        if (!getline(cin, input)) {
            cout << "Input stream ditutup. Program berhenti." << endl;
            exit(0);
        }
        stringstream ss(input);
        if (ss >> val && ss.eof()) {
            return val;
        }
        cout << "Input harus berupa angka!" << endl;
    }
}

string getValidString(const string& prompt) {
    string input;
    cout << prompt;
    if (!getline(cin, input)) {
        cout << "Input stream ditutup. Program berhenti." << endl;
        exit(0);
    }
    return input;
}

// [MODUL 1] while loop + if untuk validasi rentang menu
int getMenuChoice(int min, int max) {
    int choice;
    string input;

    // [MODUL 1] Perulangan while
    while (true) {
        cout << "Pilih: ";
        if (!getline(cin, input)) {
            cout << "Input stream ditutup. Program berhenti." << endl;
            exit(0);
        }

        // [MODUL 1] Operator Perbandingan: >=, <=
        // [MODUL 1] Operator Logika: AND (&&)
        stringstream ss(input);
        if (ss >> choice && ss.eof() && choice >= min && choice <= max) {
            return choice;
        }

        cout << "Pilihan tidak valid! Masukkan " << min << "-" << max << endl;
    }
}

string getValidDateSplit() {
    int tahun, bulan, hari;
    while (true) {
        tahun = getValidInt("Tahun (YYYY): ");
        if (tahun >= 2020 && tahun <= 2040) break;
        cout << "Tahun harus antara 2020-2040!" << endl;
    }
    while (true) {
        bulan = getValidInt("Bulan (1-12): ");
        if (bulan >= 1 && bulan <= 12) break;
        cout << "Bulan harus 1-12!" << endl;
    }
    while (true) {
        int maxDay = daysInMonth(tahun, bulan);
        hari = getValidInt("Tanggal (sesuai bulan): ");
        if (hari >= 1 && hari <= maxDay) break;
        cout << "Tanggal harus 1-" << maxDay << "!" << endl;
    }
    stringstream ss;
    ss << tahun << "-"
       << setw(2) << setfill('0') << bulan << "-"
       << setw(2) << setfill('0') << hari;
    return ss.str();
}
