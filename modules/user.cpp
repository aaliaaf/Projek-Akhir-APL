// [MODUL 1] I/O: cout, cin, getline, Operator, Percabangan, Perulangan
// [MODUL 2] Array, Struct, Array of Struct, CRUD Array
// [MODUL 3] Prosedur, Fungsi, Pass by Reference, Variabel Global/Lokal
// [MODUL 5] Insertion Sort
// [MODUL 8] Exception Handling, Include Guards, Library

#include "user.h"
#include "menu.h"
#include "csv_helper.h"
#include "utils.h"
#include "validator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>    // [MODUL 8] Library Internal: <cmath> (sqrt)
#include <algorithm> // [MODUL 8] Library Internal: <algorithm> (min)
#include <tabulate/table.hpp> // [MODUL 8] Library External (tabulate)
using namespace std;
using namespace tabulate;

// [MODUL 3] Variabel Global: dapat diakses oleh semua fungsi dalam file ini
User daftarUser[MAX_USERS]; // [MODUL 2] Array of Struct
int jumlahUser = 0;

// [MODUL 3] Shadowing: jika fungsi mendeklarasikan variabel lokal dengan nama yang sama
// Contoh (tidak dijalankan): int jumlahUser = 5; // akan menimpa (shadow) variabel global jumlahUser

// [MODUL 8] Exception Handling: try-catch
void loadUsers() {
    try {
        ifstream file("data/users.csv");
        if (!file.is_open()) {
            return;
        }

        string line;
        getline(file, line);

        jumlahUser = 0;
        while (getline(file, line) && jumlahUser < MAX_USERS) {
            vector<string> fields = splitCSVRow(line);
            if (fields.size() < 7) continue;

            // [MODUL 2] CRUD Array: Create
            daftarUser[jumlahUser].id = fields[0];
            daftarUser[jumlahUser].name = fields[1];
            daftarUser[jumlahUser].phone = fields[2];
            daftarUser[jumlahUser].username = fields[3];
            daftarUser[jumlahUser].password = fields[4];
            daftarUser[jumlahUser].role = fields[5];
            daftarUser[jumlahUser].isVIP = parseBoolean(fields[6]);
            daftarUser[jumlahUser].alamat.jalan = fields.size() > 7 ? fields[7] : "";
            daftarUser[jumlahUser].alamat.kota = fields.size() > 8 ? fields[8] : "";
            daftarUser[jumlahUser].alamat.kodePos = fields.size() > 9 ? fields[9] : "";
            jumlahUser++;
        }

        file.close();
    } catch (const exception& e) {
        cerr << "Error loading users: " << e.what() << endl;
        jumlahUser = 0;
    }
}

void saveUsers() {
    ofstream file("data/users.csv");
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file users.csv!" << endl;
        return;
    }

    file << "id,name,phone,username,password,role,is_vip,jalan,kota,kode_pos" << endl;

    for (int i = 0; i < jumlahUser; i++) {
        file << daftarUser[i].id << ","
             << daftarUser[i].name << ","
             << daftarUser[i].phone << ","
             << daftarUser[i].username << ","
             << daftarUser[i].password << ","
             << daftarUser[i].role << ","
             << (daftarUser[i].isVIP ? "true" : "false") << ","
             << daftarUser[i].alamat.jalan << ","
             << daftarUser[i].alamat.kota << ","
             << daftarUser[i].alamat.kodePos << endl;
    }

    file.close();
}

// [MODUL 3] Fungsi dengan return value (int)
// [MODUL 3] Pass by Value: username dikirim sebagai salinan
// [MODUL 1] Perulangan for: sequential search
int findUserByUsername(const string& username) {
    for (int i = 0; i < jumlahUser; i++) {
        if (daftarUser[i].username == username) {
            return i;
        }
    }
    return -1;
}

int findUserIndexByID(const string& id) {
    for (int i = 0; i < jumlahUser; i++) {
        if (daftarUser[i].id == id) {
            return i;
        }
    }
    return -1;
}

// [MODUL 3] Prosedur: void, tidak mengembalikan nilai
void login() {
    string username, password;

    cout << "=================================" << endl;
    cout << "           LOGIN                 " << endl;
    cout << "=================================" << endl;
    cout << "Masukkan username: ";
    getline(cin, username);
    cout << "Masukkan password: ";
    getline(cin, password);

    // [MODUL 1] Percabangan if-else
    int idx = findUserByUsername(username);
    if (idx == -1) { // [MODUL 1] Operator Perbandingan: ==
        cout << "Username tidak ditemukan!" << endl;
        pressEnter();
        return;
    }

    if (daftarUser[idx].password != password) {
        cout << "Password salah!" << endl;
        pressEnter();
        return;
    }

    cout << "Selamat datang, " << daftarUser[idx].name << "!" << endl;

    // [MODUL 1] Percabangan if-else
    if (daftarUser[idx].role == "admin") {
        showAdminMenu();
    } else {
        showCustomerMenu(daftarUser[idx].id);
    }
}

void registerUser() {
    if (jumlahUser >= MAX_USERS) {
        cout << "Maaf, kapasitas user penuh!" << endl;
        pressEnter();
        return;
    }

    string name, phone, username, password;

    cout << "=================================" << endl;
    cout << "        REGISTER CUSTOMER        " << endl;
    cout << "=================================" << endl;

    cout << "Nama: ";
    getline(cin, name);

    // [MODUL 1] Perulangan while untuk validasi input
    while (true) {
        cout << "Nomor HP: ";
        getline(cin, phone);
        if (isValidPhoneNumber(phone)) break;
        cout << "Nomor HP harus 10-14 digit angka!" << endl;
    }

    while (true) {
        cout << "Username: ";
        getline(cin, username);
        if (findUserByUsername(username) == -1) break;
        cout << "Username sudah digunakan!" << endl;
    }

    cout << "Password: ";
    getline(cin, password);

    string jalan, kota, kodePos;
    cout << "Alamat - Jalan: ";
    getline(cin, jalan);
    cout << "Alamat - Kota: ";
    getline(cin, kota);
    cout << "Alamat - Kode Pos: ";
    getline(cin, kodePos);

    int newID = jumlahUser + 1;
    string id = formatID("USR", newID);

    // [MODUL 2] Inisialisasi field struct dengan operator .
    daftarUser[jumlahUser].id = id;
    daftarUser[jumlahUser].name = name;
    daftarUser[jumlahUser].phone = phone;
    daftarUser[jumlahUser].username = username;
    daftarUser[jumlahUser].password = password;
    daftarUser[jumlahUser].role = "customer";
    daftarUser[jumlahUser].isVIP = false;
    daftarUser[jumlahUser].alamat.jalan = jalan;
    daftarUser[jumlahUser].alamat.kota = kota;
    daftarUser[jumlahUser].alamat.kodePos = kodePos;
    jumlahUser++;

    saveUsers();
    logAudit("REGISTER_USER", "ID: " + id + ", Username: " + username);
    cout << "Registrasi berhasil! ID Anda: " << id << endl;
    pressEnter();
}

void tambahUser() {
    if (jumlahUser >= MAX_USERS) {
        cout << "Maaf, kapasitas user penuh!" << endl;
        pressEnter();
        return;
    }

    string name, phone, username, password, role;

    cout << "=================================" << endl;
    cout << "        TAMBAH USER              " << endl;
    cout << "=================================" << endl;

    cout << "Nama: ";
    getline(cin, name);

    while (true) {
        cout << "Nomor HP: ";
        getline(cin, phone);
        if (isValidPhoneNumber(phone)) break;
        cout << "Nomor HP harus 10-14 digit angka!" << endl;
    }

    while (true) {
        cout << "Username: ";
        getline(cin, username);
        if (findUserByUsername(username) == -1) break;
        cout << "Username sudah digunakan!" << endl;
    }

    cout << "Password: ";
    getline(cin, password);

    // [MODUL 1] while loop + if untuk validasi role
    while (true) {
        cout << "Role (admin/customer): ";
        getline(cin, role);
        if (role == "admin" || role == "customer") break; // [MODUL 1] Operator Logika: ||
        cout << "Role harus 'admin' atau 'customer'!" << endl;
    }

    string jalan, kota, kodePos;
    cout << "Alamat - Jalan: ";
    getline(cin, jalan);
    cout << "Alamat - Kota: ";
    getline(cin, kota);
    cout << "Alamat - Kode Pos: ";
    getline(cin, kodePos);

    int newID = jumlahUser + 1;
    string id = formatID("USR", newID);

    daftarUser[jumlahUser].id = id;
    daftarUser[jumlahUser].name = name;
    daftarUser[jumlahUser].phone = phone;
    daftarUser[jumlahUser].username = username;
    daftarUser[jumlahUser].password = password;
    daftarUser[jumlahUser].role = role;
    daftarUser[jumlahUser].isVIP = false;
    daftarUser[jumlahUser].alamat.jalan = jalan;
    daftarUser[jumlahUser].alamat.kota = kota;
    daftarUser[jumlahUser].alamat.kodePos = kodePos;
    jumlahUser++;

    saveUsers();
    logAudit("TAMBAH_USER", "ID: " + id + ", Username: " + username);
    cout << "User berhasil ditambahkan! ID: " << id << endl;
    pressEnter();
}

void editUser() {
    if (jumlahUser == 0) {
        cout << "Belum ada user terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "      PILIH USER (EDIT)          " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < jumlahUser; i++) {
        cout << (i + 1) << ". " << daftarUser[i].name
             << " (" << daftarUser[i].role << ")" << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih user (1-" << jumlahUser << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= jumlahUser) break;
        cout << "Masukkan angka 1 - " << jumlahUser << "!" << endl;
    }

    int idx = pilihan - 1;
    string temp;

    cout << "=================================" << endl;
    cout << "   EDIT USER (" << daftarUser[idx].name << ")" << endl;
    cout << "=================================" << endl;
    cout << "Kosongkan jika tidak ingin diubah." << endl;

    cout << "Nama (" << daftarUser[idx].name << "): ";
    getline(cin, temp);
    if (!temp.empty()) daftarUser[idx].name = temp; // [MODUL 2] CRUD: Update field

    while (true) {
        cout << "No. HP (" << daftarUser[idx].phone << "): ";
        getline(cin, temp);
        if (temp.empty()) break;
        if (isValidPhoneNumber(temp)) {
            daftarUser[idx].phone = temp;
            break;
        }
        cout << "Nomor HP harus 10-14 digit angka!" << endl;
    }

    while (true) {
        cout << "Username (" << daftarUser[idx].username << "): ";
        getline(cin, temp);
        if (temp.empty()) break;
        if (findUserByUsername(temp) == -1 || temp == daftarUser[idx].username) {
            daftarUser[idx].username = temp;
            break;
        }
        cout << "Username sudah digunakan!" << endl;
    }

    cout << "Password (kosongkan jika tidak diubah): ";
    getline(cin, temp);
    if (!temp.empty()) daftarUser[idx].password = temp;

    cout << "Alamat - Jalan (" << daftarUser[idx].alamat.jalan << "): ";
    getline(cin, temp);
    if (!temp.empty()) daftarUser[idx].alamat.jalan = temp;

    cout << "Alamat - Kota (" << daftarUser[idx].alamat.kota << "): ";
    getline(cin, temp);
    if (!temp.empty()) daftarUser[idx].alamat.kota = temp;

    cout << "Alamat - Kode Pos (" << daftarUser[idx].alamat.kodePos << "): ";
    getline(cin, temp);
    if (!temp.empty()) daftarUser[idx].alamat.kodePos = temp;

    saveUsers();
    logAudit("EDIT_USER", "ID: " + daftarUser[idx].id);
    cout << "User berhasil diedit!" << endl;
    pressEnter();
}

// [MODUL 5] Insertion Sort - O(n²)
// [MODUL 5] Ascending (A-Z)
void sortUsersByName() {
    for (int i = 1; i < jumlahUser; i++) {
        User key = daftarUser[i];
        int j = i - 1;
        while (j >= 0 && daftarUser[j].name > key.name) {
            daftarUser[j + 1] = daftarUser[j];
            j--;
        }
        daftarUser[j + 1] = key;
    }
}

// [MODUL 5] Quick Sort untuk User (divide & conquer)
int partitionUser(int low, int high) {
    string pivot = daftarUser[high].alamat.kota;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (daftarUser[j].alamat.kota < pivot) {
            i++;
            User temp = daftarUser[i];
            daftarUser[i] = daftarUser[j];
            daftarUser[j] = temp;
        }
    }
    User temp = daftarUser[i + 1];
    daftarUser[i + 1] = daftarUser[high];
    daftarUser[high] = temp;
    return i + 1;
}

void quickSortUser(int low, int high) {
    if (low < high) {
        int pi = partitionUser(low, high);
        quickSortUser(low, pi - 1);
        quickSortUser(pi + 1, high);
    }
}

// [MODUL 6] Linear Search User (substring case-insensitive)
void searchUserLinear() {
    if (jumlahUser == 0) {
        cout << "Belum ada user terdaftar." << endl;
        pressEnter();
        return;
    }
    cout << "Masukkan nama user yang dicari: ";
    string keyword;
    getline(cin, keyword);

    Table table;
    table.add_row({"No", "Nama", "No. HP", "Kota", "Role", "VIP"});

    int count = 0;
    for (int i = 0; i < jumlahUser; i++) {
        string lowerName = "";
        string lowerKeyword = "";
        for (char c : daftarUser[i].name) lowerName += tolower(c);
        for (char c : keyword) lowerKeyword += tolower(c);
        if (lowerName.find(lowerKeyword) != string::npos) {
            count++;
            table.add_row({
                toString(count),
                daftarUser[i].name,
                daftarUser[i].phone,
                daftarUser[i].alamat.kota,
                daftarUser[i].role,
                daftarUser[i].isVIP ? "Ya" : "Tidak"
            });
        }
    }

    if (count == 0) {
        cout << "Tidak ada user dengan nama mengandung \"" << keyword << "\"" << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(18);
        table.column(2).format().width(14);
        table.column(3).format().width(12);
        table.column(4).format().width(8);
        table.column(5).format().width(6);
        cout << table << endl;
    }
    pressEnter();
}

// [MODUL 6] Binary Search User (exact match, requires sorted data)
void searchUserBinary() {
    if (jumlahUser == 0) {
        cout << "Belum ada user terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "Masukkan nama user yang dicari (tepat): ";
    string target;
    getline(cin, target);

    sortUsersByName();

    int left = 0, right = jumlahUser - 1;
    int foundIdx = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (daftarUser[mid].name == target) {
            foundIdx = mid;
            break;
        }
        if (daftarUser[mid].name < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (foundIdx == -1) {
        cout << "Tidak ada user dengan nama \"" << target << "\"" << endl;
    } else {
        Table table;
        table.add_row({"No", "Nama", "No. HP", "Kota", "Role", "VIP"});
        table.add_row({
            "1",
            daftarUser[foundIdx].name,
            daftarUser[foundIdx].phone,
            daftarUser[foundIdx].alamat.kota,
            daftarUser[foundIdx].role,
            daftarUser[foundIdx].isVIP ? "Ya" : "Tidak"
        });
        table.column(0).format().width(5);
        table.column(1).format().width(18);
        table.column(2).format().width(14);
        table.column(3).format().width(12);
        table.column(4).format().width(8);
        table.column(5).format().width(6);
        cout << table << endl;
    }
    pressEnter();
}

// [MODUL 6] Jump Search User (exact match, O(√n))
void searchUserJump() {
    if (jumlahUser == 0) {
        cout << "Belum ada user terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "Masukkan nama user yang dicari (tepat): ";
    string target;
    getline(cin, target);

    sortUsersByName();

    int n = jumlahUser;
    int step = sqrt(n);
    int prev = 0;

    while (daftarUser[min(step, n) - 1].name < target) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            cout << "Tidak ada user dengan nama \"" << target << "\"" << endl;
            pressEnter();
            return;
        }
    }

    int foundIdx = -1;
    for (int i = prev; i < min(step, n); i++) {
        if (daftarUser[i].name == target) {
            foundIdx = i;
            break;
        }
    }

    if (foundIdx == -1) {
        cout << "Tidak ada user dengan nama \"" << target << "\"" << endl;
    } else {
        Table table;
        table.add_row({"No", "Nama", "No. HP", "Kota", "Role", "VIP"});
        table.add_row({
            "1",
            daftarUser[foundIdx].name,
            daftarUser[foundIdx].phone,
            daftarUser[foundIdx].alamat.kota,
            daftarUser[foundIdx].role,
            daftarUser[foundIdx].isVIP ? "Ya" : "Tidak"
        });
        table.column(0).format().width(5);
        table.column(1).format().width(18);
        table.column(2).format().width(14);
        table.column(3).format().width(12);
        table.column(4).format().width(8);
        table.column(5).format().width(6);
        cout << table << endl;
    }
    pressEnter();
}

void displayAllUsers() {
    if (jumlahUser == 0) {
        cout << "Belum ada user terdaftar." << endl;
        pressEnter();
        return;
    }

    Table table;
    table.add_row({"No", "Nama", "No. HP", "Kota", "Role", "VIP"});

    for (int i = 0; i < jumlahUser; i++) {
        table.add_row({
            toString(i + 1),
            daftarUser[i].name,
            daftarUser[i].phone,
            daftarUser[i].alamat.kota,
            daftarUser[i].role,
            // [MODUL 1] Ternary Operator
            daftarUser[i].isVIP ? "Ya" : "Tidak"
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(20);
    table.column(2).format().width(15);
    table.column(3).format().width(10);
    table.column(4).format().width(5);

    cout << table << endl;
    pressEnter();
}

void deleteUser() {
    int customerIdx[MAX_USERS];
    int count = 0;
    for (int i = 0; i < jumlahUser; i++) {
        if (daftarUser[i].role == "customer") {
            customerIdx[count] = i;
            count++;
        }
    }

    if (count == 0) {
        cout << "Tidak ada customer yang terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "   PILIH CUSTOMER YANG DIHAPUS   " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < count; i++) {
        int idx = customerIdx[i];
        cout << (i + 1) << ". " << daftarUser[idx].name
             << " - " << daftarUser[idx].phone
             << (daftarUser[idx].isVIP ? " (VIP)" : "") << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih customer (1-" << count << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= count) break;
        cout << "Masukkan angka 1 - " << count << "!" << endl;
    }

    int idx = customerIdx[pilihan - 1];

    cout << "Hapus customer " << daftarUser[idx].name << "? (y/n): ";
    string confirm;
    getline(cin, confirm);
    if (confirm != "y" && confirm != "Y") {
        cout << "Penghapusan dibatalkan." << endl;
        pressEnter();
        return;
    }

    string deletedID = daftarUser[idx].id;
    string deletedName = daftarUser[idx].name;

    // [MODUL 2] CRUD Array: Delete (geser elemen)
    for (int i = idx; i < jumlahUser - 1; i++) {
        daftarUser[i] = daftarUser[i + 1];
    }
    jumlahUser--;

    saveUsers();
    logAudit("HAPUS_USER", "ID: " + deletedID + ", Nama: " + deletedName);
    cout << "User berhasil dihapus!" << endl;
    pressEnter();
}

void setVIPStatus() {
    int customerIdx[MAX_USERS];
    int count = 0;
    for (int i = 0; i < jumlahUser; i++) {
        if (daftarUser[i].role == "customer") {
            customerIdx[count] = i;
            count++;
        }
    }

    if (count == 0) {
        cout << "Tidak ada customer yang terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "   PILIH CUSTOMER (ATUR VIP)     " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < count; i++) {
        int idx = customerIdx[i];
        cout << (i + 1) << ". " << daftarUser[idx].name
             << " - " << daftarUser[idx].phone
             << (daftarUser[idx].isVIP ? " (VIP)" : " (Reguler)") << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih customer (1-" << count << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= count) break;
        cout << "Masukkan angka 1 - " << count << "!" << endl;
    }

    int idx = customerIdx[pilihan - 1];

    // [MODUL 1] Operator Logika NOT (!): toggle nilai boolean
    daftarUser[idx].isVIP = !daftarUser[idx].isVIP;
    saveUsers();
    logAudit("VIP_TOGGLE", "User ID: " + daftarUser[idx].id + ", VIP: " + (daftarUser[idx].isVIP ? "true" : "false"));

    cout << "Status VIP user " << daftarUser[idx].name
         << " sekarang: " << (daftarUser[idx].isVIP ? "VIP" : "Reguler") << endl;
    pressEnter();
}

void changePassword(const string& userID) {
    int idx = findUserIndexByID(userID);
    if (idx == -1) {
        cout << "User tidak ditemukan!" << endl;
        pressEnter();
        return;
    }

    string oldPass, newPass, confirmPass;
    cout << "Password lama: ";
    getline(cin, oldPass);
    if (daftarUser[idx].password != oldPass) {
        cout << "Password lama salah!" << endl;
        pressEnter();
        return;
    }

    cout << "Password baru: ";
    getline(cin, newPass);
    if (newPass.length() < 4) {
        cout << "Password minimal 4 karakter!" << endl;
        pressEnter();
        return;
    }

    cout << "Konfirmasi password baru: ";
    getline(cin, confirmPass);
    if (newPass != confirmPass) {
        cout << "Konfirmasi password tidak cocok!" << endl;
        pressEnter();
        return;
    }

    daftarUser[idx].password = newPass;
    saveUsers();
    logAudit("CHANGE_PASSWORD", "User ID: " + userID);
    cout << "Password berhasil diubah!" << endl;
    pressEnter();
}
