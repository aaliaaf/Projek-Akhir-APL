#ifndef USER_H // [MODUL 8] Include Guards
#define USER_H

#include <string>
using namespace std;

#define MAX_USERS 100

// [MODUL 2] Nested Struct: struct dalam struct
struct Alamat {
    string jalan;
    string kota;
    string kodePos;
};

// [MODUL 2] Struct: tipe data abstrak
struct User {
    // [MODUL 2] Field struct
    string id;
    string name;
    string phone;
    string username;
    string password;
    string role;
    bool isVIP; // [MODUL 1] Tipe Data Primitif: bool
    Alamat alamat; // [MODUL 2] Nested Struct: field bertipe struct Alamat
};

// [MODUL 2] Array of Struct
extern User daftarUser[MAX_USERS];
extern int jumlahUser;

// [MODUL 3] Prosedur (void)
void login();
void registerUser();
void loadUsers();
void saveUsers();
int findUserByUsername(const string& username);
int findUserIndexByID(const string& id);
void tambahUser();
void editUser();
void sortUsersByName();                    // [MODUL 5] Insertion Sort
void quickSortUser(int low, int high);     // [MODUL 5] Quick Sort untuk User
int partitionUser(int low, int high);      // [MODUL 5] Partisi Quick Sort
void searchUserLinear();                   // [MODUL 6] Linear Search User
void searchUserBinary();                   // [MODUL 6] Binary Search User
void searchUserJump();                     // [MODUL 6] Jump Search User
void displayAllUsers();
void deleteUser();
void setVIPStatus();
void changePassword(const string& userID);

#endif
