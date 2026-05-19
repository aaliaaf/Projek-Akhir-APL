// [MODUL 1] I/O: cout, cin, getline
// [MODUL 2] Array, Struct, Array of Struct, CRUD
// [MODUL 3] Prosedur, Fungsi, Pass by Reference
// [MODUL 4] Pointer pada Array
// [MODUL 5] Selection Sort, Merge Sort, Quick Sort
// [MODUL 6] Linear Search, Binary Search, Jump Search, Interpolation Search
// [MODUL 8] Exception Handling, Library Internal

#include "iphone.h"
#include "csv_helper.h"
#include "utils.h"
#include "validator.h"
#include <iostream> // [MODUL 8] Library Internal
#include <fstream>
#include <sstream>
#include <tabulate/table.hpp> // [MODUL 8] Library External (tabulate)
#include <cmath> // [MODUL 8] Library Internal: <cmath>
#include <algorithm> // [MODUL 8] Library Internal: <algorithm>
using namespace std;
using namespace tabulate;

// [MODUL 2] Array of Struct: array global berisi struct iPhone
// [MODUL 3] Variabel Global
iPhone daftarIPhone[MAX_IPHONES];
int jumlahIPhone = 0;

// [MODUL 10] Data model iPhone realistis
struct ModelSpec {
    string name;
    int releaseYear;
    vector<string> storages;
    vector<string> colors;
    float basePrice;
};

const ModelSpec modelDB[] = {
    {"iPhone SE (3rd gen)", 2022, {"64GB","128GB","256GB"}, {"Midnight","Starlight","Red"}, 50000},
    {"iPhone 11",          2019, {"64GB","128GB","256GB"}, {"White","Black","Green","Yellow","Purple","Red"}, 60000},
    {"iPhone 12",          2020, {"64GB","128GB","256GB"}, {"Blue","Green","Black","White","Red","Purple"}, 70000},
    {"iPhone 12 mini",     2020, {"64GB","128GB","256GB"}, {"Blue","Green","Black","White","Red","Purple"}, 65000},
    {"iPhone 13",          2021, {"128GB","256GB","512GB"}, {"Pink","Blue","Midnight","Starlight","Green","Red"}, 80000},
    {"iPhone 13 mini",     2021, {"128GB","256GB","512GB"}, {"Pink","Blue","Midnight","Starlight","Green","Red"}, 75000},
    {"iPhone 14",          2022, {"128GB","256GB","512GB"}, {"Midnight","Starlight","Blue","Purple","Yellow","Red"}, 90000},
    {"iPhone 14 Plus",     2022, {"128GB","256GB","512GB"}, {"Midnight","Starlight","Blue","Purple","Yellow","Red"}, 95000},
    {"iPhone 14 Pro",      2022, {"128GB","256GB","512GB","1TB"}, {"Space Black","Silver","Gold","Deep Purple"}, 120000},
    {"iPhone 14 Pro Max",  2022, {"128GB","256GB","512GB","1TB"}, {"Space Black","Silver","Gold","Deep Purple"}, 130000},
    {"iPhone 15",          2023, {"128GB","256GB","512GB"}, {"Black","Blue","Green","Yellow","Pink"}, 100000},
    {"iPhone 15 Plus",     2023, {"128GB","256GB","512GB"}, {"Black","Blue","Green","Yellow","Pink"}, 105000},
    {"iPhone 15 Pro",      2023, {"128GB","256GB","512GB","1TB"}, {"Natural Titanium","Blue Titanium","White Titanium","Black Titanium"}, 140000},
    {"iPhone 15 Pro Max",  2023, {"256GB","512GB","1TB"}, {"Natural Titanium","Blue Titanium","White Titanium","Black Titanium"}, 150000},
    {"iPhone 16",          2024, {"128GB","256GB","512GB"}, {"Black","White","Pink","Teal","Ultramarine"}, 110000},
    {"iPhone 16 Plus",     2024, {"128GB","256GB","512GB"}, {"Black","White","Pink","Teal","Ultramarine"}, 115000},
    {"iPhone 16 Pro",      2024, {"128GB","256GB","512GB","1TB"}, {"Natural Titanium","Desert Titanium","White Titanium","Black Titanium"}, 150000},
    {"iPhone 16 Pro Max",  2024, {"256GB","512GB","1TB"}, {"Natural Titanium","Desert Titanium","White Titanium","Black Titanium"}, 160000},
};
const int MODEL_COUNT = sizeof(modelDB) / sizeof(modelDB[0]);

const string kondisiList[] = {"Baru", "Bekas - Mulus", "Bekas - Lecet", "Bekas - Rusak"};
const int KONDISI_COUNT = sizeof(kondisiList) / sizeof(kondisiList[0]);

// [MODUL 3] Fungsi memilih model iPhone
int pilihModel() {
    clearScreen();
    cout << "=================================" << endl;
    cout << "       PILIH MODEL IPHONE        " << endl;
    cout << "=================================" << endl;

    Table table;
    table.add_row({"No", "Model", "Tahun", "Harga/Hr"});
    for (int i = 0; i < MODEL_COUNT; i++) {
        table.add_row({toString(i + 1), modelDB[i].name, toString(modelDB[i].releaseYear), "Rp" + toString((int)modelDB[i].basePrice)});
    }
    table.column(0).format().width(5);
    table.column(1).format().width(24);
    table.column(2).format().width(8);
    table.column(3).format().width(14);
    cout << table << endl;
    cout << "0. Batal" << endl;
    cout << "=================================" << endl;

    int pilih;
    string input;
    while (true) {
        cout << "Pilih model: ";
        getline(cin, input);
        if (isCancelInput(input)) return -1;
        stringstream ss(input);
        if (ss >> pilih && ss.eof() && pilih >= 1 && pilih <= MODEL_COUNT) return pilih - 1;
        cout << "Masukkan angka 1-" << MODEL_COUNT << " atau 0 untuk batal!" << endl;
    }
}

// [MODUL 3] Fungsi memilih storage berdasarkan model
string pilihStorage(int modelIdx) {
    const auto& m = modelDB[modelIdx];
    clearScreen();
    cout << "=================================" << endl;
    cout << "     PILIH STORAGE " << m.name << endl;
    cout << "=================================" << endl;

    for (size_t i = 0; i < m.storages.size(); i++) {
        cout << (i + 1) << ". " << m.storages[i] << endl;
    }
    cout << "0. Batal" << endl;
    cout << "=================================" << endl;

    int pilih;
    string input;
    while (true) {
        cout << "Pilih storage: ";
        getline(cin, input);
        if (isCancelInput(input)) return "";
        stringstream ss(input);
        if (ss >> pilih && ss.eof() && pilih >= 1 && pilih <= (int)m.storages.size()) return m.storages[pilih - 1];
        cout << "Masukkan angka 1-" << m.storages.size() << " atau 0 untuk batal!" << endl;
    }
}

// [MODUL 3] Fungsi memilih warna berdasarkan model
string pilihWarna(int modelIdx) {
    const auto& m = modelDB[modelIdx];
    clearScreen();
    cout << "=================================" << endl;
    cout << "     PILIH WARNA " << m.name << endl;
    cout << "=================================" << endl;

    for (size_t i = 0; i < m.colors.size(); i++) {
        cout << (i + 1) << ". " << m.colors[i] << endl;
    }
    cout << "0. Batal" << endl;
    cout << "=================================" << endl;

    int pilih;
    string input;
    while (true) {
        cout << "Pilih warna: ";
        getline(cin, input);
        if (isCancelInput(input)) return "";
        stringstream ss(input);
        if (ss >> pilih && ss.eof() && pilih >= 1 && pilih <= (int)m.colors.size()) return m.colors[pilih - 1];
        cout << "Masukkan angka 1-" << m.colors.size() << " atau 0 untuk batal!" << endl;
    }
}

// [MODUL 3] Fungsi memilih kondisi
string pilihKondisi() {
    clearScreen();
    cout << "=================================" << endl;
    cout << "        PILIH KONDISI            " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < KONDISI_COUNT; i++) {
        cout << (i + 1) << ". " << kondisiList[i] << endl;
    }
    cout << "0. Batal" << endl;
    cout << "=================================" << endl;
    int pilih;
    string input;
    while (true) {
        cout << "Pilih kondisi: ";
        getline(cin, input);
        if (isCancelInput(input)) return "";
        stringstream ss(input);
        if (ss >> pilih && ss.eof() && pilih >= 1 && pilih <= KONDISI_COUNT) return kondisiList[pilih - 1];
        cout << "Masukkan angka 1-" << KONDISI_COUNT << " atau 0 untuk batal!" << endl;
    }
}

// [MODUL 3] Fungsi menghitung harga sewa berdasarkan model, storage, kondisi
float hitungHarga(int modelIdx, const string& storage, const string& kondisi) {
    float harga = modelDB[modelIdx].basePrice;

    // [MODUL 10] Tambahan harga berdasarkan storage
    if (storage == "256GB") harga += 5000;
    else if (storage == "512GB") harga += 10000;
    else if (storage == "1TB") harga += 15000;

    // [MODUL 10] Pengurangan harga berdasarkan kondisi
    if (kondisi == "Bekas - Mulus") harga -= 5000;
    else if (kondisi == "Bekas - Lecet") harga -= 15000;
    else if (kondisi == "Bekas - Rusak") harga -= 25000;

    if (harga < 15000) harga = 15000;
    return harga;
}

// [MODUL 8] Exception Handling: try-catch
void loadIPhones() {
    try { // [MODUL 8] try block
        ifstream file(resolveProjectPath("data/iphones.csv"));
        if (!file.is_open()) {
            return;
        }

        string line;
        getline(file, line); // [MODUL 1] getline

        jumlahIPhone = 0;
        // [MODUL 1] Perulangan while
        int lineNumber = 1;
        while (getline(file, line)) {
            lineNumber++;
            if (jumlahIPhone >= MAX_IPHONES) {
                cerr << "Warning: data iPhone melebihi kapasitas maksimum, baris " << lineNumber << " diabaikan." << endl;
                continue;
            }
            // [MODUL 2] Vector: tipe data kolektif
            vector<string> fields = splitCSVRow(line);
            if (fields.size() < 6) {
                cerr << "Warning: format iPhone tidak valid pada baris " << lineNumber << ", baris diabaikan." << endl;
                continue;
            }

            // [MODUL 8] Contoh throw logic_error: validasi data
            if (fields[1].empty()) {
                cerr << "Warning: nama iPhone kosong pada baris " << lineNumber << ", baris diabaikan." << endl;
                continue;
            }

            // [MODUL 2] CRUD Array: Create (memasukkan data ke array)
            daftarIPhone[jumlahIPhone].id = fields[0];
            daftarIPhone[jumlahIPhone].name = fields[1];
            daftarIPhone[jumlahIPhone].storage = fields[2];
            daftarIPhone[jumlahIPhone].rentPrice = stringToFloat(fields[3]);
            daftarIPhone[jumlahIPhone].status = fields[4];
            daftarIPhone[jumlahIPhone].totalRented = stringToInt(fields[5]);
            // [BUG FIX] Akses dengan boundary check menggunakan ternary
            daftarIPhone[jumlahIPhone].color = fields.size() > 6 ? fields[6] : "";
            daftarIPhone[jumlahIPhone].year = fields.size() > 7 ? stringToInt(fields[7]) : 0;
            daftarIPhone[jumlahIPhone].condition = fields.size() > 8 ? fields[8] : "";
            jumlahIPhone++; // [MODUL 1] Operator Increment: ++
        }

        file.close();
    } catch (const invalid_argument& e) { // [MODUL 8] Tipe Exception: invalid_argument
        cerr << "Error data iPhone: " << e.what() << endl;
    } catch (const out_of_range& e) { // [MODUL 8] Tipe Exception: out_of_range
        cerr << "Error rentang data: " << e.what() << endl;
    } catch (const length_error& e) { // [MODUL 8] Tipe Exception: length_error (harus sebelum logic_error)
        cerr << "Length error: " << e.what() << endl;
    } catch (const logic_error& e) { // [MODUL 8] Tipe Exception: logic_error (induk length_error)
        cerr << "Logic error: " << e.what() << endl;
    } catch (const runtime_error& e) { // [MODUL 8] Tipe Exception: runtime_error
        cerr << "Runtime error: " << e.what() << endl;
    } catch (const exception& e) { // [MODUL 8] catch block umum
        cerr << "Error loading iPhones: " << e.what() << endl;
    }
}

void saveIPhones() {
    filesystem::path iphonesPath = resolveProjectPath("data/iphones.csv");
    filesystem::create_directories(iphonesPath.parent_path());

    ofstream file(iphonesPath);
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file iphones.csv!" << endl;
        return;
    }

    file << "id,name,storage,rent_price,status,total_rented,color,year,condition" << endl;

    // [MODUL 1] Perulangan for
    for (int i = 0; i < jumlahIPhone; i++) {
           file << escapeCSVField(daftarIPhone[i].id) << ","
               << escapeCSVField(daftarIPhone[i].name) << ","
               << escapeCSVField(daftarIPhone[i].storage) << ","
             << toString(daftarIPhone[i].rentPrice) << ","
               << escapeCSVField(daftarIPhone[i].status) << ","
             << toString(daftarIPhone[i].totalRented) << ","
               << escapeCSVField(daftarIPhone[i].color) << ","
             << toString(daftarIPhone[i].year) << ","
               << escapeCSVField(daftarIPhone[i].condition) << endl;
    }

    file.close();
}

// [MODUL 3] Fungsi dengan return value (int)
// [MODUL 1] Operator Perbandingan: ==
int findiPhoneByID(const string& id) {
    for (int i = 0; i < jumlahIPhone; i++) {
        if (daftarIPhone[i].id == id) { // [MODUL 2] CRUD: Read (akses array)
            return i; // [MODUL 3] keyword return
        }
    }
    return -1;
}

void tambahIPhone() {
    // [MODUL 1] Percabangan if
    if (jumlahIPhone >= MAX_IPHONES) {
        cout << "Kapasitas iPhone penuh!" << endl;
        pressEnter();
        return;
    }

    // [MODUL 10] Input berbasis pilihan
    int modelIdx = pilihModel();
    if (modelIdx < 0) { cout << "Dibatalkan." << endl; pressEnter(); return; }

    string name = modelDB[modelIdx].name;

    string storage = pilihStorage(modelIdx);
    if (storage == "") { cout << "Dibatalkan." << endl; pressEnter(); return; }

    string color = pilihWarna(modelIdx);
    if (color == "") { cout << "Dibatalkan." << endl; pressEnter(); return; }

    string condition = pilihKondisi();
    if (condition == "") { cout << "Dibatalkan." << endl; pressEnter(); return; }

    int year = modelDB[modelIdx].releaseYear;
    float suggested = hitungHarga(modelIdx, storage, condition);

    // Konfirmasi harga
    clearScreen();
    cout << "=================================" << endl;
    cout << "   RINGKASAN IPHONE BARU         " << endl;
    cout << "=================================" << endl;
    cout << "Model   : " << name << endl;
    cout << "Storage : " << storage << endl;
    cout << "Warna   : " << color << endl;
    cout << "Tahun   : " << year << endl;
    cout << "Kondisi : " << condition << endl;
    cout << "---------------------------------" << endl;
    cout << "Harga sewa: Rp" << toString((int)suggested) << "/hari" << endl;
    cout << "=================================" << endl;

    string input;
    while (true) {
        cout << "Masukkan harga (Enter untuk Rp" << toString((int)suggested) << "): ";
        getline(cin, input);
        if (isCancelInput(input)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
        if (input == "") {
            break;
        }
        stringstream ss(input);
        float customPrice;
        if (ss >> customPrice && ss.eof() && customPrice >= 10000) {
            suggested = customPrice;
            break;
        }
        cout << "Harga minimal Rp10000!" << endl;
    }
    float price = suggested;

    int newID = 0;
    for (int i = 0; i < jumlahIPhone; i++) {
        int suffix = stringToInt(daftarIPhone[i].id.substr(2));
        if (suffix > newID) newID = suffix;
    }
    newID++;
    string id = formatID("IP", newID);

    // [MODUL 2] CRUD Array: Create (inisialisasi)
    // [MODUL 2] Pengaksesan field struct dengan operator .
    daftarIPhone[jumlahIPhone].id = id;
    daftarIPhone[jumlahIPhone].name = name;
    daftarIPhone[jumlahIPhone].storage = storage;
    daftarIPhone[jumlahIPhone].rentPrice = price;
    daftarIPhone[jumlahIPhone].status = "available";
    daftarIPhone[jumlahIPhone].totalRented = 0;
    daftarIPhone[jumlahIPhone].color = color;
    daftarIPhone[jumlahIPhone].year = year;
    daftarIPhone[jumlahIPhone].condition = condition;
    jumlahIPhone++;

    saveIPhones();
    logAudit("TAMBAH_IPHONE", "ID: " + id + ", Nama: " + name);
    cout << "iPhone " << name << " (" << color << ", " << year << ") berhasil ditambahkan dengan ID " << id << endl;
    pressEnter();
}

void editIPhone() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "    PILIH IPHONE YANG DIEDIT     " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < jumlahIPhone; i++) {
        // [MODUL 1] Ternary Operator: kondisi ? true : false
        cout << (i + 1) << ". " << daftarIPhone[i].name << " ("
             << daftarIPhone[i].storage << ") - "
             << (daftarIPhone[i].status == "available" ? "Tersedia" : "Disewa") << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih iPhone (1-" << jumlahIPhone << "): ";
        getline(cin, input);
        if (isCancelInput(input)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= jumlahIPhone) break;
        cout << "Masukkan angka 1 - " << jumlahIPhone << "!" << endl;
    }

    int idx = pilihan - 1;

    cout << "Edit iPhone: " << daftarIPhone[idx].name << endl;

    // [MODUL 10] Ganti model?
    clearScreen();
    cout << "Ganti model? (1. Ya / 2. Tidak / 0. Batal): ";
    string ganti;
    getline(cin, ganti);
    if (isCancelInput(ganti)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
    if (ganti == "1") {
        int m = pilihModel();
        if (m >= 0) {
            daftarIPhone[idx].name = modelDB[m].name;
            daftarIPhone[idx].year = modelDB[m].releaseYear;

            string s = pilihStorage(m);
            if (s != "") daftarIPhone[idx].storage = s;

            string c = pilihWarna(m);
            if (c != "") daftarIPhone[idx].color = c;
        }
    }
    // [MODUL 10] Ganti warna?
    cout << "Ganti warna? (1. Ya / 2. Tidak / 0. Batal): ";
    getline(cin, ganti);
    if (isCancelInput(ganti)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
    if (ganti == "1") {
        int mi = -1;
        for (int i = 0; i < MODEL_COUNT; i++) {
            if (modelDB[i].name == daftarIPhone[idx].name) { mi = i; break; }
        }
        if (mi >= 0) {
            string w = pilihWarna(mi);
            if (w != "") daftarIPhone[idx].color = w;
        }
    }
    // [MODUL 10] Ganti kondisi?
    cout << "Ganti kondisi? (1. Ya / 2. Tidak / 0. Batal): ";
    getline(cin, ganti);
    if (isCancelInput(ganti)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
    if (ganti == "1") {
        string k = pilihKondisi();
        if (k != "") daftarIPhone[idx].condition = k;
    }
    // [MODUL 10] Ganti harga?
    cout << "Ganti harga sewa? (1. Ya / 2. Tidak / 0. Batal): ";
    getline(cin, ganti);
    if (isCancelInput(ganti)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
    if (ganti == "1") {
        float suggested = hitungHarga(0, daftarIPhone[idx].storage, daftarIPhone[idx].condition);
        cout << "Harga baru (Enter untuk Rp" << toString((int)suggested) << "): ";
        string h;
        getline(cin, h);
        if (isCancelInput(h)) { cout << "Dibatalkan." << endl; pressEnter(); return; }
        if (h != "") {
            stringstream ss(h);
            float hp;
            if (ss >> hp && hp >= 10000) daftarIPhone[idx].rentPrice = hp;
        } else {
            daftarIPhone[idx].rentPrice = suggested;
        }
    }

    saveIPhones();
    logAudit("EDIT_IPHONE", "ID: " + daftarIPhone[idx].id);
    cout << "iPhone berhasil diedit!" << endl;
    pressEnter();
}

void hapusIPhone() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "   PILIH IPHONE YANG DIHAPUS     " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < jumlahIPhone; i++) {
        cout << (i + 1) << ". " << daftarIPhone[i].name << " ("
             << daftarIPhone[i].storage << ") - "
             << (daftarIPhone[i].status == "available" ? "Tersedia" : "Disewa") << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih iPhone (1-" << jumlahIPhone << "): ";
        getline(cin, input);
        if (isCancelInput(input)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= jumlahIPhone) break;
        cout << "Masukkan angka 1 - " << jumlahIPhone << "!" << endl;
    }

    int idx = pilihan - 1;

    cout << "Apakah Anda yakin ingin menghapus " << daftarIPhone[idx].name << "? (y/n): ";
    string confirm;
    getline(cin, confirm);
    if (isCancelInput(confirm)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }
    if (confirm != "y" && confirm != "Y") { // [MODUL 1] Operator Logika: &&
        cout << "Penghapusan dibatalkan." << endl;
        pressEnter();
        return;
    }

    string deletedID = daftarIPhone[idx].id;
    string deletedName = daftarIPhone[idx].name;

    // [MODUL 2] CRUD Array: Delete (geser elemen)
    for (int i = idx; i < jumlahIPhone - 1; i++) {
        daftarIPhone[i] = daftarIPhone[i + 1]; // [MODUL 2] Operator Penugasan: =
    }
    jumlahIPhone--; // [MODUL 1] Operator Decrement: --

    saveIPhones();
    logAudit("HAPUS_IPHONE", "ID: " + deletedID + ", Nama: " + deletedName);
    cout << "iPhone berhasil dihapus!" << endl;
    pressEnter();
}

// [MODUL 3] Fungsi untuk mengurutkan berdasarkan nama (digunakan binary search dll)
void sortIPhonesByName() {
    for (int i = 0; i < jumlahIPhone - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < jumlahIPhone; j++) {
            if (daftarIPhone[j].name < daftarIPhone[minIdx].name) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            iPhone temp = daftarIPhone[i];
            daftarIPhone[i] = daftarIPhone[minIdx];
            daftarIPhone[minIdx] = temp;
        }
    }
}

// [MODUL 5] Selection Sort - O(n²) berdasarkan storage
void selectionSortByStorage() {
    for (int i = 0; i < jumlahIPhone - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < jumlahIPhone; j++) {
            if (daftarIPhone[j].storage < daftarIPhone[minIdx].storage) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            iPhone temp = daftarIPhone[i];
            daftarIPhone[i] = daftarIPhone[minIdx];
            daftarIPhone[minIdx] = temp;
        }
    }
    logAudit("SORT_IPHONE", "Storage ASC (Selection Sort)");
}

// [MODUL 5] Quick Sort - O(n log n) average, divide & conquer
// [MODUL 5] Partisi: membagi array menjadi 2 bagian
int partitionIPhone(int low, int high) {
    int pivot = daftarIPhone[high].year; // [MODUL 5] Pivot: tahun rilis
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // [MODUL 1] Operator Perbandingan: <= (ascending by year)
        if (daftarIPhone[j].year <= pivot) {
            i++;
            // [MODUL 5] Swap elemen
            iPhone temp = daftarIPhone[i];
            daftarIPhone[i] = daftarIPhone[j];
            daftarIPhone[j] = temp;
        }
    }

    // [MODUL 5] Tempatkan pivot di posisi yang benar
    iPhone temp = daftarIPhone[i + 1];
    daftarIPhone[i + 1] = daftarIPhone[high];
    daftarIPhone[high] = temp;

    return i + 1;
}

// [MODUL 5] Quick Sort - Rekursif (divide & conquer)
// [MODUL 5] Divide: partisi, Conquer: rekursif
// [MODUL 3] Rekursif: fungsi memanggil dirinya sendiri
void quickSortIPhone(int low, int high) {
    if (low < high) {
        int pi = partitionIPhone(low, high); // [MODUL 5] Divide: partisi
        quickSortIPhone(low, pi - 1);        // [MODUL 5] Conquer: rekursif kiri
        quickSortIPhone(pi + 1, high);       // [MODUL 5] Conquer: rekursif kanan
    }
}

// [MODUL 5] Merge Sort - O(n log n), divide & conquer, stable sort
// [MODUL 5] Merge: menggabungkan 2 sub-array yang sudah terurut
void mergeIPhone(int left, int mid, int right) {
    // [MODUL 5] Tentukan ukuran sub-array kiri dan kanan
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // [MODUL 2] Array 1D: array sementara untuk merge (gunakan vector, bukan VLA)
    vector<iPhone> L(n1);
    vector<iPhone> R(n2);

    for (int i = 0; i < n1; i++) L[i] = daftarIPhone[left + i];
    for (int j = 0; j < n2; j++) R[j] = daftarIPhone[mid + 1 + j];

    int i = 0, j = 0, k = left;
    // [MODUL 5] Merge: gabungkan dengan perbandingan
    while (i < n1 && j < n2) {
        if (L[i].name <= R[j].name) {
            daftarIPhone[k] = L[i];
            i++;
        } else {
            daftarIPhone[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) { daftarIPhone[k] = L[i]; i++; k++; }
    while (j < n2) { daftarIPhone[k] = R[j]; j++; k++; }
}

// [MODUL 5] Merge Sort - Rekursif, divide & conquer
void mergeSortIPhone(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // [MODUL 6] Boundary handling: cegah overflow
        mergeSortIPhone(left, mid);          // [MODUL 5] Divide: rekursif kiri
        mergeSortIPhone(mid + 1, right);     // [MODUL 5] Divide: rekursif kanan
        mergeIPhone(left, mid, right);       // [MODUL 5] Conquer: merge
    }
}

// [MODUL 6] Binary Search - O(log n)
// [MODUL 6] Syarat: data harus terurut (sudah di-sort)
void binarySearchIPhone() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    sortIPhonesByName(); // [MODUL 6] Pastikan data terurut

    cout << "Masukkan nama iPhone yang dicari (tepat): ";
    string target;
    getline(cin, target);
    if (isCancelInput(target)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }

    int left = 0, right = jumlahIPhone - 1;
    int foundIdx = -1;

    // [MODUL 6] Binary Search: divide & conquer
    while (left <= right) {
        // [MODUL 6] Boundary handling: mid = left + (right-left)/2
        // [MODUL 6] Mencegah overflow pada indeks besar
        int mid = left + (right - left) / 2;

        if (daftarIPhone[mid].name == target) {
            foundIdx = mid;
            break;
        }
        if (daftarIPhone[mid].name < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (foundIdx == -1) {
        cout << "Tidak ada iPhone dengan nama \"" << target << "\"" << endl;
    } else {
        Table table;
        table.add_row({"ID", "Nama iPhone", "Storage", "Harga/Hari", "Status"});
        table.add_row({
            daftarIPhone[foundIdx].id,
            daftarIPhone[foundIdx].name,
            daftarIPhone[foundIdx].storage,
            "Rp" + toString(daftarIPhone[foundIdx].rentPrice),
            daftarIPhone[foundIdx].status == "available" ? "Tersedia" : "Disewa"
        });
        table.column(0).format().width(8);
        table.column(1).format().width(20);
        table.column(2).format().width(10);
        table.column(3).format().width(14);
        table.column(4).format().width(10);
        cout << table << endl;
    }
    pressEnter();
}

// [MODUL 6] Interpolation Search - O(log log n) untuk data terurut merata
// [MODUL 6] Lebih cepat dari Binary Search untuk distribusi merata
void interpolationSearchIPhone() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    // [MODUL 5] Gunakan Quick Sort: divide & conquer
    quickSortIPhone(0, jumlahIPhone - 1);

    cout << "Masukkan harga sewa yang dicari: ";
    int targetPrice = getValidInt("Harga: ");

    int low = 0, high = jumlahIPhone - 1;
    int foundIdx = -1;

    // [MODUL 6] Interpolation Search: perkiraan posisi berdasarkan nilai
    while (low <= high && targetPrice >= daftarIPhone[low].rentPrice
           && targetPrice <= daftarIPhone[high].rentPrice) {
        // [MODUL 6] Boundary handling & pencegahan division by zero
        if (daftarIPhone[high].rentPrice == daftarIPhone[low].rentPrice) {
            // [MODUL 6] Jika semua nilai sama, lakukan linear search
            for (int i = low; i <= high; i++) {
                if (daftarIPhone[i].rentPrice == targetPrice) {
                    foundIdx = i;
                    break;
                }
            }
            break;
        }

        // [MODUL 6] Rumus Interpolation Search: pos = low + ((target - arr[low]) * (high - low)) / (arr[high] - arr[low])
        // [MODUL 6] Pencegahan division by zero: sudah di-handle di atas
        int pos = low + ((targetPrice - daftarIPhone[low].rentPrice)
                        * (high - low))
                        / (daftarIPhone[high].rentPrice - daftarIPhone[low].rentPrice);

        // [MODUL 6] Boundary handling: pastikan pos dalam rentang
        if (pos < low || pos > high) break;

        if (daftarIPhone[pos].rentPrice == targetPrice) {
            foundIdx = pos;
            break;
        }
        if (daftarIPhone[pos].rentPrice < targetPrice) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }

    if (foundIdx == -1) {
        cout << "Tidak ada iPhone dengan harga Rp" << targetPrice << endl;
    } else {
        Table table;
        table.add_row({"ID", "Nama iPhone", "Storage", "Harga/Hari", "Status"});
        table.add_row({
            daftarIPhone[foundIdx].id,
            daftarIPhone[foundIdx].name,
            daftarIPhone[foundIdx].storage,
            "Rp" + toString(daftarIPhone[foundIdx].rentPrice),
            daftarIPhone[foundIdx].status == "available" ? "Tersedia" : "Disewa"
        });
        table.column(0).format().width(8);
        table.column(1).format().width(20);
        table.column(2).format().width(10);
        table.column(3).format().width(14);
        table.column(4).format().width(10);
        cout << table << endl;
    }
    pressEnter();
}

// [MODUL 6] Jump Search - O(√n) untuk data terurut
// [MODUL 6] Melompat dengan step √n, lalu linear search di blok
void jumpSearchIPhone() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    sortIPhonesByName();

    cout << "Masukkan nama iPhone yang dicari: ";
    string target;
    getline(cin, target);
    if (isCancelInput(target)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }

    int n = jumlahIPhone;
    int step = sqrt(n); // [MODUL 6] Step = √n
    int prev = 0;

    // [MODUL 6] Jump Search: lompat dengan step
    while (daftarIPhone[min(step, n) - 1].name < target) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            cout << "Tidak ada iPhone dengan nama \"" << target << "\"" << endl;
            pressEnter();
            return;
        }
    }

    // [MODUL 6] Linear Search dalam blok yang ditemukan
    int foundIdx = -1;
    for (int i = prev; i < min(step, n); i++) {
        if (daftarIPhone[i].name == target) {
            foundIdx = i;
            break;
        }
    }

    if (foundIdx == -1) {
        cout << "Tidak ada iPhone dengan nama \"" << target << "\"" << endl;
    } else {
        Table table;
        table.add_row({"ID", "Nama iPhone", "Storage", "Harga/Hari", "Status"});
        table.add_row({
            daftarIPhone[foundIdx].id,
            daftarIPhone[foundIdx].name,
            daftarIPhone[foundIdx].storage,
            "Rp" + toString(daftarIPhone[foundIdx].rentPrice),
            daftarIPhone[foundIdx].status == "available" ? "Tersedia" : "Disewa"
        });
        table.column(0).format().width(8);
        table.column(1).format().width(20);
        table.column(2).format().width(10);
        table.column(3).format().width(14);
        table.column(4).format().width(10);
        cout << table << endl;
    }
    pressEnter();
}

void displayAllIPhones() {
    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    const int PER_PAGE = 5;
    int totalPages = (jumlahIPhone + PER_PAGE - 1) / PER_PAGE;
    int page = 1;
    string input;

    while (true) {
        clearScreen();
        cout << "=== DAFTAR IPHONE (Halaman " << page << "/" << totalPages << ") ===" << endl;

        Table table;
        table.add_row({"No", "ID", "Nama", "Storage", "Harga", "Warna", "Thn", "Kondisi", "Status", "Sewa"});

        int start = (page - 1) * PER_PAGE;
        int end = start + PER_PAGE;
        if (end > jumlahIPhone) end = jumlahIPhone;

        for (int i = start; i < end; i++) {
            table.add_row({
                toString(i + 1),
                daftarIPhone[i].id,
                daftarIPhone[i].name,
                daftarIPhone[i].storage,
                "Rp" + toString(daftarIPhone[i].rentPrice),
                daftarIPhone[i].color,
                toString(daftarIPhone[i].year),
                daftarIPhone[i].condition,
                daftarIPhone[i].status == "available" ? "Tersedia" : "Disewa",
                toString(daftarIPhone[i].totalRented)
            });
        }

        table.column(0).format().width(4);
        table.column(1).format().width(5);
        table.column(2).format().width(18);
        table.column(3).format().width(8);
        table.column(4).format().width(12);
        table.column(5).format().width(14);
        table.column(6).format().width(5);
        table.column(7).format().width(8);
        table.column(8).format().width(10);
        table.column(9).format().width(6);

        cout << table << endl;
        cout << "1. Halaman berikutnya" << endl;
        cout << "2. Halaman sebelumnya" << endl;
        cout << "0. Kembali" << endl;
        cout << "Pilih: ";
        getline(cin, input);
        if ((input == "1" || input == "n") && page < totalPages) page++;
        else if ((input == "2" || input == "p") && page > 1) page--;
        else if (input == "0" || input == "q") break;
    }
}

// [MODUL 6] Linear Search - O(n) untuk mencari substring (case-insensitive)
void searchIPhone() {
    cout << "Masukkan nama iPhone yang dicari: ";
    string keyword;
    getline(cin, keyword);
    if (isCancelInput(keyword)) { cout << "Input dibatalkan." << endl; pressEnter(); return; }

    Table table;
    table.add_row({"No", "Nama iPhone", "Storage", "Harga/Hari", "Status"});

    int count = 0;
    // [MODUL 1] Perulangan for
    for (int i = 0; i < jumlahIPhone; i++) {
        string lowerName = "";
        string lowerKeyword = "";
        // [MODUL 1] foreach loop: range-based for pada string
        for (char c : daftarIPhone[i].name) lowerName += tolower(c);
        for (char c : keyword) lowerKeyword += tolower(c);

        // [MODUL 6] Linear Search: cek setiap elemen satu per satu
        if (lowerName.find(lowerKeyword) != string::npos) {
            count++;
            table.add_row({
                toString(count),
                daftarIPhone[i].name,
                daftarIPhone[i].storage,
                "Rp" + toString(daftarIPhone[i].rentPrice),
                daftarIPhone[i].status == "available" ? "Tersedia" : "Disewa"
            });
        }
    }

    if (count == 0) {
        cout << "Tidak ada iPhone yang cocok dengan \"" << keyword << "\"" << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(20);
        table.column(2).format().width(10);
        table.column(3).format().width(14);
        table.column(4).format().width(10);

        cout << table << endl;
    }

    pressEnter();
}

void displayAvailableIPhones() {
    int count = 0;
    int availIdx[MAX_IPHONES];
    for (int i = 0; i < jumlahIPhone; i++) {
        if (daftarIPhone[i].status == "available") {
            availIdx[count] = i;
            count++;
        }
    }

    if (count == 0) {
        cout << "Tidak ada iPhone yang tersedia saat ini." << endl;
        return;
    }

    Table table;
    table.add_row({"No", "Nama iPhone", "Storage", "Harga/Hari"});

    for (int i = 0; i < count; i++) {
        int idx = availIdx[i];
        table.add_row({
            toString(i + 1),
            daftarIPhone[idx].name,
            daftarIPhone[idx].storage,
            "Rp" + toString(daftarIPhone[idx].rentPrice)
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(20);
    table.column(2).format().width(10);
    table.column(3).format().width(14);

    cout << table << endl;
}
