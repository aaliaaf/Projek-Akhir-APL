// MODUL 1-2: Variabel, struct, array/vector
// MODUL 2: Array (raw C-style array untuk lookup status)
// MODUL 3-4: Fungsi, pointer
// MODUL 6: Binary Search untuk pencarian iPhone (O(log n))
// MODUL 8: <vector>, <algorithm>, exception handling

#include "../includes/helpers.h"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

// MODUL 2: Raw C-style array — array statis untuk lookup nama status
// Menggantikan if-else chain dengan indexing array
const string STATUS_NAMES[] = {"Tersedia", "Disewa", "Maintenance", "Rusak"};
const int STATUS_COUNT = sizeof(STATUS_NAMES) / sizeof(STATUS_NAMES[0]); // MODUL 2: sizeof array

inline string statusToString(StatusiPhone s) {
    int idx = (int)s;
    // MODUL 2: Akses raw array dengan indeks
    if (idx >= 0 && idx < STATUS_COUNT) {
        return STATUS_NAMES[idx];
    }
    return "Tidak diketahui";
}

// ========== HELPER ==========

void cetakTabeliPhone(const vector<iPhone>& data) {
    if (data.empty()) {
        cout << "Tidak ada iPhone yang tersedia" << endl;
        return;
    }

    cout << "+------------------------------+" << endl;
    cout << "|      DAFTAR IPHONE           |" << endl;
    cout << "+------------------------------+" << endl;

    for (size_t i = 0; i < data.size(); i++) {
        string status = statusToString(data[i].status);

        cout << "| " << setw(2) << left << i + 1 << ". "
             << setw(10) << left << data[i].id
             << " | " << setw(15) << left << data[i].model
             << " | Rp " << setw(8) << left << fixed << setprecision(0) << data[i].hargaPerHari
             << " | " << setw(12) << left << status
             << " |" << endl;
    }
    cout << "+------------------------------+" << endl << endl;
}

// ========== MODUL 6: BINARY SEARCH ==========
// MODUL 6: Binary Search untuk mencari iPhone berdasarkan ID
// MODUL 6: Kompleksitas waktu: O(log n) — data harus terurut
// MODUL 6: Alternatif dari sequential search yang O(n)

iPhone* cariiPhoneBinary(vector<iPhone>& data, const string& id) {
    // MODUL 6: Binary Search
    // Prasyarat: data sudah terurut berdasarkan ID
    // Kompleksitas: O(log n)

    sortiPhoneByID(data); // Pastikan data terurut

    string idLower = mylib::toLower(id);
    int left = 0;
    int right = (int)data.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; // Hindari overflow

        ASSERT(mid >= 0 && mid < (int)data.size(),
               "Binary Search index mid out of range");

        string midLower = mylib::toLower(data[mid].id);

        if (midLower == idLower) {
            return &data[mid];
        }
        else if (midLower < idLower) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr; // Tidak ditemukan
}

// MODUL 6: Fungsi sorting pembantu untuk Binary Search
void sortiPhoneByID(vector<iPhone>& data) {
    // MODUL 8: Menggunakan <algorithm> sort
    sort(data.begin(), data.end(), [](const iPhone& a, const iPhone& b) {
        return mylib::toLower(a.id) < mylib::toLower(b.id);
    });
}

// MODUL 1-4: Sequential Search sebagai fallback (untuk keyword model)
// Case-insensitive: menggunakan mylib::toLower()
iPhone* cariiPhone(const vector<iPhone>& data, string keyword) {
    // Sequential Search: O(n)
    // Fallback ketika data tidak terurut atau mencari berdasarkan model
    string keyLower = mylib::toLower(keyword);

    for (size_t i = 0; i < data.size(); i++) {
        // Case-insensitive ID match
        if (mylib::toLower(data[i].id) == keyLower) {
            return const_cast<iPhone*>(&data[i]);
        }
        // Case-insensitive prefix match untuk model
        string modelLower = mylib::toLower(data[i].model);
        if (keyLower.length() <= modelLower.length()) {
            bool ketemu = true;
            for (size_t j = 0; j < keyLower.length(); j++) {
                if (keyLower[j] != modelLower[j]) {
                    ketemu = false;
                    break;
                }
            }
            if (ketemu) {
                return const_cast<iPhone*>(&data[i]);
            }
        }
    }
    return nullptr;
}

// MODUL 5: Bubble Sort untuk sorting iPhone berdasarkan model — O(n²)
static void bubbleSortByModel(vector<iPhone>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (mylib::toLower(arr[j].model) > mylib::toLower(arr[j + 1].model)) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void tampilkaniPhone(const vector<iPhone>& data) {
    header("DAFTAR IPHONE TERSEDIA");
    vector<iPhone> hasil;
    for (const auto& ip : data) {
        if (ip.status == Tersedia) {
            hasil.push_back(ip);
        }
    }
    // MODUL 5: Gunakan Bubble Sort untuk mengurutkan berdasarkan model
    bubbleSortByModel(hasil);
    cetakTabeliPhone(hasil);
    cout << "(Diurutkan dengan Bubble Sort — O(n²))\n";
}

void filteriPhoneByStatus(const vector<iPhone>& data, StatusiPhone statusFilter) {
    header("FILTER IPHONE BY STATUS");

    // MODUL 2: Gunakan array lookup, bukan if-else
    cout << "Status: " << statusToString(statusFilter) << endl << endl;

    vector<iPhone> hasil;
    for (const auto& ip : data) {
        if (ip.status == statusFilter) {
            hasil.push_back(ip);
        }
    }
    // MODUL 5: Bubble Sort untuk tampilan terurut
    bubbleSortByModel(hasil);
    cetakTabeliPhone(hasil);
    cout << "(Diurutkan dengan Bubble Sort — O(n²))\n";
}

void tampilkanDetailiPhone(const iPhone& ip) {
    // BONUS: Tampilan rapi dengan tabel
    cout << "\n+------------------------------------------+\n";
    cout << "|           DETAIL IPHONE                  |\n";
    cout << "+------------------------------------------+\n";
    cout << "| " << setw(15) << left << "ID"
         << ": " << setw(23) << left << ip.id << " |\n";
    cout << "| " << setw(15) << left << "Model"
         << ": " << setw(23) << left << ip.model << " |\n";
    cout << "| " << setw(15) << left << "Harga/Hari"
         << ": Rp " << setw(20) << left << fixed << setprecision(0) << ip.hargaPerHari << " |\n";

    cout << "| " << setw(15) << left << "Status"
         << ": " << setw(23) << left << statusToString(ip.status) << " |\n";
    cout << "| " << setw(15) << left << "Kondisi"
         << ": " << setw(23) << left << ip.kondisi << " |\n";
    cout << "+------------------------------------------+\n";

    if (ip.jadwal.empty()) {
        cout << "| Jadwal: - (Belum ada booking)           |\n";
    } else {
        cout << "| Jadwal:                                    |\n";
        for (const auto& js : ip.jadwal) {
            cout << "|  - " << js.tglMulai << " s/d " << js.tglSelesai
                 << string(max(0, 34 - (int)js.tglMulai.length() - (int)js.tglSelesai.length()), ' ')
                 << " |\n";
        }
    }
    cout << "+------------------------------------------+\n\n";
}

// MODUL 4: Pointer & dynamic memory (new/delete)
void tambahiPhone(vector<iPhone>& data) {
    // MODUL 8: try-catch untuk validasi input
    header("TAMBAH IPHONE");
    iPhone* newIP = nullptr; // MODUL 4: Pointer ke struct
    try {
        // MODUL 4: Alokasi memori dinamis dengan new
        newIP = new iPhone;
        newIP->id = "IPH" + to_string(data.size() + 1);

        cout << "Model: ";
        getline(cin >> ws, newIP->model);
        if (newIP->model.empty()) {
            throw invalid_argument("Model tidak boleh kosong!");
        }

        cout << "Harga per hari: ";
        cin >> newIP->hargaPerHari;
        if (cin.fail() || newIP->hargaPerHari <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Harga harus angka positif!");
        }

        cout << "Kondisi: ";
        getline(cin >> ws, newIP->kondisi);

        newIP->status = Tersedia;
        data.push_back(*newIP); // Copy ke vector

        // MODUL 4: Dealokasi dengan delete + nullptr
        delete newIP;
        newIP = nullptr;
        cout << "iPhone berhasil ditambahkan! ID: IPH" << data.size() << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "Input tidak valid: " << e.what() << endl;
        LOG_ERROR(string("Gagal tambah iPhone: ") + e.what());
        // MODUL 4: Pastikan memory tetap dibebaskan walau exception
        delete newIP;
        newIP = nullptr;
    }
}

void editiPhone(vector<iPhone>& data) {
    header("EDIT IPHONE");
    try {
        string id;
        cout << "Masukkan ID iPhone: ";
        cin >> id;

        // MODUL 6: Gunakan Binary Search untuk efisiensi
        iPhone* ip = cariiPhoneBinary(data, id);
        if (ip == nullptr) {
            throw runtime_error("iPhone dengan ID " + id + " tidak ditemukan!");
        }

        cout << "Model baru (" << ip->model << "): ";
        getline(cin >> ws, ip->model);
        cout << "Harga baru (Rp " << fixed << setprecision(0) << ip->hargaPerHari << "): ";
        cin >> ip->hargaPerHari;
        if (cin.fail() || ip->hargaPerHari <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw out_of_range("Harga harus bernilai positif!");
        }

        cout << "iPhone berhasil diupdate!" << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        LOG_ERROR(string("Edit iPhone gagal: ") + e.what());
    }
    catch (const out_of_range& e) {
        cerr << "Input di luar jangkauan: " << e.what() << endl;
    }
}

void hapusiPhone(vector<iPhone>& data) {
    header("HAPUS IPHONE");
    try {
        string id;
        cout << "Masukkan ID iPhone: ";
        cin >> id;

        // MODUL 6: Gunakan Binary Search
        iPhone* ip = cariiPhoneBinary(data, id);
        if (ip == nullptr) {
            throw runtime_error("iPhone dengan ID " + id + " tidak ditemukan!");
        }

        // MODUL 8: Gunakan <algorithm> find
        auto it = find_if(data.begin(), data.end(), [&id](const iPhone& i) {
            return i.id == id;
        });

        if (it != data.end()) {
            data.erase(it);
            cout << "iPhone berhasil dihapus!" << endl;
        }
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}


