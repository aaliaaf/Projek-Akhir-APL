// MODUL 1-4: Variabel, percabangan, perulangan, fungsi, pointer
// MODUL 6: Interpolation Search untuk cari posisi antrian
// MODUL 8: try, catch, throw dengan invalid_argument & runtime_error
// MODUL 8: <vector>, <algorithm>, pencatatan error dengan __FILE__, __LINE__, __func__

#include "../includes/helpers.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// ========== LOGIN ==========
// MODUL 8: Exception handling dengan invalid_argument
bool login(string& outUserId, string& outRole) {
    try {
        string username, password;

        cout << "\n+------------------------------------------+\n";
        cout << "|                LOGIN                     |\n";
        cout << "+------------------------------------------+\n";
        cout << "| Username: ";
        cin >> username;
        cout << "| Password: ";
        cin >> password;
        cout << "+------------------------------------------+\n";

        if (username.empty()) {
            throw invalid_argument("Username tidak boleh kosong!");
        }
        if (password.empty()) {
            throw invalid_argument("Password tidak boleh kosong!");
        }

        // Case-insensitive username comparison
        string userLower = mylib::toLower(username);
        for (size_t i = 0; i < usersDB.size(); i++) {
            if (mylib::toLower(usersDB[i].username) == userLower &&
                usersDB[i].password == password) {

                cout << "\n  Selamat datang, " << username << "!" << endl;
                cout << "  Role: " << usersDB[i].role << endl;

                outUserId = usersDB[i].id;
                outRole = usersDB[i].role;
                return true;
            }
        }

        throw runtime_error("Username atau password salah!");
    }
    catch (const invalid_argument& e) {
        cerr << "\n[VALIDASI] " << e.what() << endl;
        LOG_ERROR(string("Login validation: ") + e.what());
        return false;
    }
    catch (const runtime_error& e) {
        cerr << "\n[LOGIN GAGAL] " << e.what() << endl;
        LOG_ERROR(string("Login failed: ") + e.what());
        return false;
    }
}

// ========== REGISTER ==========

void registerUser() {
    // MODUL 8: try-catch
    try {
        registerUser(usersDB);
    }
    catch (const exception& e) {
        cerr << "Registrasi gagal: " << e.what() << endl;
        LOG_ERROR(string("Register failed: ") + e.what());
    }
}

void registerUser(vector<User>& data) {
    // MODUL 8: throw invalid_argument
    User newUser;
    newUser.id = "U" + to_string(data.size() + 1);

    cout << "\n+------------------------------------------+\n";
    cout << "|              REGISTER                    |\n";
    cout << "+------------------------------------------+\n";
    cout << "| Username: ";
    cin >> newUser.username;

    if (newUser.username.empty()) {
        throw invalid_argument("Username tidak boleh kosong!");
    }

        // Cek duplikat username (case-insensitive)
        string newLower = mylib::toLower(newUser.username);
        for (const auto& u : data) {
            if (mylib::toLower(u.username) == newLower) {
                throw invalid_argument("Username '" + newUser.username + "' sudah terdaftar!");
            }
        }

    cout << "| Password: ";
    cin >> newUser.password;
    if (newUser.password.empty()) {
        throw invalid_argument("Password tidak boleh kosong!");
    }

    newUser.role = "Customer";
    newUser.level = BARU;
    newUser.totalSewa = 0;

    data.push_back(newUser);
    cout << "+------------------------------------------+\n";
    cout << "  Registrasi berhasil! ID User: " << newUser.id << endl;
}

// ========== CARI USER ==========
// Sequential Search: O(n)
User* cariUser(const vector<User>& data, string id) {
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].id == id) {
            return const_cast<User*>(&data[i]);
        }
    }
    return nullptr;
}

// ========== TAMPILKAN USER ==========

void tampilkanDetailUser(const User& u) {
    cout << "\n+------------------------------------------+\n";
    cout << "|             DETAIL USER                  |\n";
    cout << "+------------------------------------------+\n";
    cout << "| ID       : " << setw(28) << left << u.id << " |\n";
    cout << "| Username : " << setw(28) << left << u.username << " |\n";
    cout << "| Role     : " << setw(28) << left << u.role << " |\n";
    cout << "| Level    : " << setw(28) << left
         << (u.level == VIP ? "VIP" : u.level == REGULAR ? "Regular" : "Baru") << " |\n";
    cout << "| Total Sewa: " << setw(27) << left << u.totalSewa << " |\n";
    cout << "+------------------------------------------+\n";
}

void tampilkanRiwayatUser(const vector<Transaksi>& trx, const vector<Reservasi>& res, string userId) {
    header("RIWAYAT PENGGUNA");

    cout << "\nTransaksi Selesai:\n";
    bool ada = false;
    for (const auto& t : trx) {
        if (t.userId == userId && t.tglKembali != "-" && !t.tglKembali.empty()) {
            cout << "  " << t.idTransaksi
                 << " | iPhone: " << t.iPhoneId
                 << " | " << t.tglMulai << " - " << t.tglSelesai
                 << " | Rp " << t.totalBayar << endl;
            ada = true;
        }
    }
    if (!ada) cout << "  (Belum ada transaksi selesai)\n";

    cout << "\nReservasi Aktif:\n";
    ada = false;
    for (const auto& r : res) {
        if (r.userId == userId && r.isActive) {
            cout << "  " << r.idReservasi
                 << " | iPhone: " << r.iPhoneId
                 << " | " << r.tglMulai << " - " << r.tglSelesai << " | Aktif\n";
            ada = true;
        }
    }
    if (!ada) cout << "  (Tidak ada reservasi aktif)\n";
}

void lihatPosisiAntrian(const vector<Reservasi>& res, string userId) {
    header("POSISI ANTRIAN");

    // MODUL 6: Interpolation Search — cari posisi reservasi user di antrian
    // Urutkan salinan antrian berdasarkan waktuBooking (prasyarat interpolation search)
    vector<Reservasi> sorted = res;
    sort(sorted.begin(), sorted.end(), [](const Reservasi& a, const Reservasi& b) {
        return a.waktuBooking < b.waktuBooking;
    });

    bool found = false;
    for (const auto& r : res) {
        if (r.userId == userId && r.isActive) {
            // MODUL 6: Gunakan Interpolation Search untuk cari posisi
            int pos = cariReservasiInterpolation(sorted, r.waktuBooking);
            if (pos >= 0) {
                cout << "  " << r.idReservasi
                     << " | iPhone: " << r.iPhoneId
                     << " | Posisi antrian: ke-" << (pos + 1)
                     << " (Interpolation Search, O(log log n))" << endl;
            }
            found = true;
        }
    }

    if (!found) {
        cout << "Anda tidak memiliki reservasi dalam antrian." << endl;
    }
}

void tampilkanDetailReservasi(const Reservasi& r) {
    cout << "\n+------------------------------------------+\n";
    cout << "|           DETAIL RESERVASI               |\n";
    cout << "+------------------------------------------+\n";
    cout << "| ID        : " << setw(28) << left << r.idReservasi << " |\n";
    cout << "| User      : " << setw(28) << left << r.userId << " |\n";
    cout << "| iPhone    : " << setw(28) << left << r.iPhoneId << " |\n";
    cout << "| Periode   : " << setw(28) << left
         << (r.tglMulai + " - " + r.tglSelesai) << " |\n";
    cout << "| Status    : " << setw(28) << left
         << (r.isActive ? "Aktif" : "Tidak Aktif") << " |\n";
    cout << "+------------------------------------------+\n";
}

// ========== CRUD USER (ADMIN) ==========

void tambahUser(vector<User>& data) {
    // MODUL 8: try-catch
    try {
        User newUser;
        newUser.id = "U" + to_string(data.size() + 1);

        cout << "Username: ";
        cin >> newUser.username;
        if (newUser.username.empty()) {
            throw invalid_argument("Username tidak boleh kosong!");
        }

        cout << "Password: ";
        cin >> newUser.password;
        if (newUser.password.empty()) {
            throw invalid_argument("Password tidak boleh kosong!");
        }

        cout << "Role (Admin/Customer): ";
        cin >> newUser.role;
        if (newUser.role != "Admin" && newUser.role != "Customer") {
            throw invalid_argument("Role harus Admin atau Customer!");
        }

        cout << "Level (1=VIP, 2=Regular, 3=Baru): ";
        int levelChoice;
        cin >> levelChoice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Input level harus angka!");
        }

        switch (levelChoice) {
        case 1: newUser.level = VIP; break;
        case 2: newUser.level = REGULAR; break;
        case 3: newUser.level = BARU; break;
        default: throw out_of_range("Level harus 1, 2, atau 3!");
        }

        newUser.totalSewa = 0;
        data.push_back(newUser);
        cout << "User berhasil ditambahkan! ID: " << newUser.id << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        LOG_ERROR(string("Tambah user gagal: ") + e.what());
    }
    catch (const out_of_range& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void editUser(vector<User>& data) {
    try {
        string id;
        cout << "Masukkan ID User: ";
        cin >> id;

        User* user = cariUser(data, id);
        if (user == nullptr) {
            throw runtime_error("User dengan ID " + id + " tidak ditemukan!");
        }

        cout << "Username baru (" << user->username << "): ";
        cin >> user->username;
        cout << "Password baru: ";
        cin >> user->password;

        cout << "User berhasil diupdate!" << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void hapusUser(vector<User>& data) {
    try {
        string id;
        cout << "Masukkan ID User: ";
        cin >> id;

        User* user = cariUser(data, id);
        if (user == nullptr) {
            throw runtime_error("User dengan ID " + id + " tidak ditemukan!");
        }

        // MODUL 8: <algorithm> find_if
        auto it = find_if(data.begin(), data.end(), [&id](const User& u) {
            return u.id == id;
        });

        if (it != data.end()) {
            data.erase(it);
            cout << "User berhasil dihapus!" << endl;
        }
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
