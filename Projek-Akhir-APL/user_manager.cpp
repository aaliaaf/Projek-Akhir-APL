#include "helpers.h"
#include <iostream>

using namespace std;

vector<User> usersDB = {
    // default admin
    {"U001", "admin", "admin", "Admin", UserLevel::BARU, 0}
};

bool login() {
    string username, password;
    cout << endl;
    cout << "--- Login ---" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (const auto& user : usersDB) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!" << endl;
            return true;
        }
    }

    cout << "Invalid credentials. Please try again." << endl;
    return false;
}

void registerUser() {
    string username, password;
    cout << endl;
    cout << "--- Register ---" << endl;
    cout << "Enter new username: ";
    cin >> username;

    for (const auto& user : usersDB) {
        if (user.username == username) {
            cout << "Username already exists. Please try another one." << endl;
            return;
        }
    }

    cout << "Enter new password: ";
    cin >> password;

    string newId = "U" + to_string(usersDB.size() + 1);
    usersDB.push_back({newId, username, password, "Customer", UserLevel::BARU, 0});
    cout << "Registration successful! You can now log in." << endl;
}


