// ============================================================
// PROYEK AKHIR APL - SISTEM PENYEWAAN IPHONE
// dengan Jadwal dan Prioritas Reservasi
// ============================================================
// MODUL 1: Variabel, tipe data, input/output
// MODUL 2: Array, Struct, Vector
// MODUL 3: Fungsi & Prosedur
// MODUL 4: Pointer
// MODUL 5: Sorting (<algorithm> sort)
// MODUL 6: Binary Search (core_rental.cpp) & Interpolation Search (priority.cpp)
// MODUL 8: Error Handling (try/catch/throw), <vector>, <algorithm>, <cmath>,
//          External Library (my_library.h), Include Guards, ASSERT/LOG_ERROR
// BONUS: File CSV, Tampilan rapi, Prioritas Reservasi

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "../includes/helpers.h"

using namespace std;

// ========== DATABASE GLOBAL ==========
vector<iPhone> iphonesDB;
vector<User> usersDB;
vector<Reservasi> reservasiDB;
vector<Transaksi> transaksiDB;

// ========== UTILITY ==========

void tekanEnterLanjut() {
    cout << "\nTekan Enter untuk lanjut...";
    cin.get();
    if (cin.eof()) {
        cin.clear();
    }
}

// MODUL 8: Fungsi dengan exception
int bacaPilihan() {
    int pilihan;
    while (!(cin >> pilihan)) {
        if (cin.eof()) {
            throw runtime_error("Input tidak terduga (EOF). Program akan keluar.");
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan angka: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return pilihan;
}

// ========== MENU MANAJEMEN IPHONE ==========
// MODUL 1: Percabangan (switch-case)
// MODUL 6: Binary Search (case 2)

void menuManajemenIphone(vector<iPhone>& ip) {
    while (true) {
        cout << "\n+==========================================+\n";
        cout << "|           MANAJEMEN IPHONE              |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Lihat Semua iPhone                   |\n";
        cout << "| 2. Cari iPhone (Binary Search by ID)    |\n";
        cout << "| 3. Cari iPhone (Sequential by Model)    |\n";
        cout << "| 4. Tambah iPhone                        |\n";
        cout << "| 5. Edit iPhone                          |\n";
        cout << "| 6. Hapus iPhone                         |\n";
        cout << "| 7. Kembali                              |\n";
        cout << "+==========================================+\n";
        cout << "Pilihan: ";

        int pilihan = bacaPilihan();

        switch (pilihan) {
        case 1: {
            // BONUS: Tampilan tabel rapi
            tampilkaniPhone(ip);
            tekanEnterLanjut();
            break;
        }
        case 2: {
            // MODUL 6: Binary Search O(log n)
            header("CARI IPHONE (BINARY SEARCH)");
            cout << "Masukkan ID iPhone: ";
            string id;
            getline(cin, id);

            sortiPhoneByID(ip);
            iPhone* hasil = cariiPhoneBinary(ip, id);
            if (hasil) {
                cout << "  Binary Search: Ditemukan!" << endl;
                tampilkanDetailiPhone(*hasil);
            } else {
                cout << "  iPhone tidak ditemukan." << endl;
            }
            tekanEnterLanjut();
            break;
        }
        case 3: {
            // Sequential Search: O(n) — fallback untuk model
            header("CARI IPHONE (SEQUENTIAL SEARCH)");
            cout << "Masukkan keyword (ID/model): ";
            string keyword;
            getline(cin, keyword);

            iPhone* hasil = cariiPhone(ip, keyword);
            if (hasil) {
                tampilkanDetailiPhone(*hasil);
            } else {
                cout << "  iPhone tidak ditemukan." << endl;
            }
            tekanEnterLanjut();
            break;
        }
        case 4:
            tambahiPhone(ip);
            tekanEnterLanjut();
            break;
        case 5:
            editiPhone(ip);
            tekanEnterLanjut();
            break;
        case 6:
            hapusiPhone(ip);
            tekanEnterLanjut();
            break;
        case 7:
            return;
        default:
            cout << "Pilihan tidak valid.\n";
            tekanEnterLanjut();
        }
    }
}

// ========== MENU MANAJEMEN USER ==========

void menuManajemenUser(vector<User>& usr) {
    while (true) {
        cout << "\n+==========================================+\n";
        cout << "|            MANAJEMEN USER               |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Lihat Semua User                     |\n";
        cout << "| 2. Cari User                            |\n";
        cout << "| 3. Tambah User                          |\n";
        cout << "| 4. Edit User                            |\n";
        cout << "| 5. Hapus User                           |\n";
        cout << "| 6. Kembali                              |\n";
        cout << "+==========================================+\n";
        cout << "Pilihan: ";

        int pilihan = bacaPilihan();

        switch (pilihan) {
        case 1: {
            if (usr.empty()) {
                cout << "Belum ada data user.\n";
            } else {
                cout << "\nDaftar User:\n";
                for (size_t i = 0; i < usr.size(); ++i) {
                    cout << "  " << (i + 1) << ". " << usr[i].id
                         << " | " << usr[i].username
                         << " | " << usr[i].role
                         << " | Level: "
                         << (usr[i].level == VIP ? "VIP" :
                             usr[i].level == REGULAR ? "Regular" : "Baru")
                         << " | Sewa: " << usr[i].totalSewa << "x" << endl;
                }
            }
            tekanEnterLanjut();
            break;
        }
        case 2: {
            cout << "Masukkan ID user: ";
            string id;
            getline(cin, id);

            User* target = cariUser(usr, id);
            if (target) {
                tampilkanDetailUser(*target);
            } else {
                cout << "User tidak ditemukan.\n";
            }
            tekanEnterLanjut();
            break;
        }
        case 3:
            tambahUser(usr);
            tekanEnterLanjut();
            break;
        case 4:
            editUser(usr);
            tekanEnterLanjut();
            break;
        case 5:
            hapusUser(usr);
            tekanEnterLanjut();
            break;
        case 6:
            return;
        default:
            cout << "Pilihan tidak valid.\n";
            tekanEnterLanjut();
        }
    }
}

// ========== MENU LAPORAN ADMIN ==========

void menuLaporanAdmin(const vector<Transaksi>& trx, const vector<User>& usr,
                      const vector<iPhone>& ip, const vector<Reservasi>& res) {
    while (true) {
        cout << "\n+==========================================+\n";
        cout << "|               LAPORAN                   |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Pendapatan                           |\n";
        cout << "| 2. iPhone Populer                       |\n";
        cout << "| 3. User Aktif                           |\n";
        cout << "| 4. Pendapatan per Bulan                 |\n";
        cout << "| 5. Okupansi iPhone                      |\n";
        cout << "| 6. Konversi Reservasi                   |\n";
        cout << "| 7. Top User by Spending                 |\n";
        cout << "| 8. Kembali                              |\n";
        cout << "+==========================================+\n";
        cout << "Pilihan: ";

        int pilihan = bacaPilihan();

        switch (pilihan) {
        case 1: laporanPendapatan(trx); tekanEnterLanjut(); break;
        case 2: laporaniPhonePopuler(trx); tekanEnterLanjut(); break;
        case 3: laporanUserAktif(trx, usr); tekanEnterLanjut(); break;
        case 4: laporanPendapatanPerBulan(trx); tekanEnterLanjut(); break;
        case 5: laporanOkupansiIphone(trx, ip); tekanEnterLanjut(); break;
        case 6: laporanKonversiReservasi(res, trx); tekanEnterLanjut(); break;
        case 7: laporanTopUserBySpending(trx, usr); tekanEnterLanjut(); break;
        case 8: return;
        default: cout << "Pilihan tidak valid.\n"; tekanEnterLanjut();
        }
    }
}

// ========== WELCOME SCREEN ==========

void showWelcomeScreen() {
    cout << "\n";
    cout << "+==========================================+\n";
    cout << "|    SISTEM PENYEWAAN IPHONE               |\n";
    cout << "|    dengan Jadwal & Prioritas Reservasi   |\n";
    cout << "+==========================================+\n";
    cout << "| 1. Login                                 |\n";
    cout << "| 2. Register                              |\n";
    cout << "| 3. Exit                                  |\n";
    cout << "+==========================================+\n";
    cout << "Pilihan: ";
}

// ========== MENU CUSTOMER ==========

void menuCustomer(string userId, vector<iPhone>& ip, vector<User>& usr,
                  vector<Reservasi>& res, vector<Transaksi>& trx) {

    while (true) {
        cout << "\n+==========================================+\n";
        cout << "|             MENU CUSTOMER               |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Lihat iPhone Tersedia                |\n";
        cout << "| 2. Cari iPhone (Binary Search by ID)    |\n";
        cout << "| 3. Cari iPhone (Sequential by Model)    |\n";
        cout << "| 4. Buat Reservasi Baru                  |\n";
        cout << "| 5. Reservasi Aktif & Posisi Antrian     |\n";
        cout << "| 6. Batalkan Reservasi                   |\n";
        cout << "| 7. Kembalikan iPhone (Check-In)         |\n";
        cout << "| 8. Riwayat Transaksi Saya               |\n";
        cout << "| 9. Profil Saya                          |\n";
        cout << "| 10. Logout                              |\n";
        cout << "+==========================================+\n";
        cout << "Pilihan: ";

        int pilihan = bacaPilihan();

        switch (pilihan) {
        case 1:
            filteriPhoneByStatus(ip, Tersedia);
            tekanEnterLanjut();
            break;

        case 2: {
            // MODUL 6: Binary Search untuk Customer
            header("CARI IPHONE (BINARY SEARCH)");
            cout << "Masukkan ID iPhone: ";
            string id;
            getline(cin, id);

            sortiPhoneByID(ip);
            iPhone* hasil = cariiPhoneBinary(ip, id);
            if (hasil) {
                cout << "  Binary Search: Ditemukan!" << endl;
                tampilkanDetailiPhone(*hasil);
            } else {
                cout << "  iPhone tidak ditemukan." << endl;
            }
            tekanEnterLanjut();
            break;
        }

        case 3: {
            header("CARI IPHONE (SEQUENTIAL SEARCH)");
            cout << "Masukkan keyword (model/ID): ";
            string keyword;
            getline(cin, keyword);

            iPhone* hasil = cariiPhone(ip, keyword);
            if (hasil) {
                tampilkanDetailiPhone(*hasil);
            } else {
                cout << "iPhone tidak ditemukan.\n";
            }
            tekanEnterLanjut();
            break;
        }

        case 4: {
            // Melihat iPhone tersedia
            filteriPhoneByStatus(ip, Tersedia);

            string iphoneId;
            cout << "\nMasukkan ID iPhone yang ingin disewa: ";
            getline(cin, iphoneId);

            // MODUL 6: Binary Search untuk efisiensi
            sortiPhoneByID(ip);
            iPhone* target = cariiPhoneBinary(ip, iphoneId);
            if (!target || target->status != Tersedia) {
                cout << "iPhone tidak tersedia atau tidak ditemukan.\n";
                break;
            }

            cout << "Tanggal Mulai (YYYY-MM-DD): ";
            string tglMulai;
            getline(cin, tglMulai);

            cout << "Tanggal Selesai (YYYY-MM-DD): ";
            string tglSelesai;
            getline(cin, tglSelesai);

            if (!validasiTanggal(tglMulai) || !validasiTanggal(tglSelesai)) {
                cout << "Format tanggal tidak valid.\n";
                break;
            }

            // BONUS: Validasi bentrok jadwal
            if (cekBentrok(target->jadwal, tglMulai, tglSelesai)) {
                cout << "Jadwal bentrok dengan reservasi lain.\n";
                break;
            }

            Reservasi baru;
            baru.idReservasi = "RES-" + to_string(res.size() + 1);
            baru.userId = userId;
            baru.iPhoneId = iphoneId;
            baru.tglMulai = tglMulai;
            baru.tglSelesai = tglSelesai;
            baru.isActive = true;
            baru.waktuBooking = tanggalToInt(tglMulai);

            res.push_back(baru);

            // BONUS: Proses antrian prioritas
            // (jadwal akan ditambahkan oleh prosesAntrianOtomatis jika lolos)
            urutkanAntrian(res, usr);
            prosesAntrianOtomatis(res, ip, trx, usr);

            cout << "Reservasi berhasil dibuat!\n";
            tekanEnterLanjut();
            break;
        }

        case 5: {
            cout << "\nReservasi Aktif Anda:\n";
            bool ada = false;
            for (auto& r : res) {
                if (r.userId == userId && r.isActive) {
                    tampilkanDetailReservasi(r);
                    ada = true;
                }
            }
            if (!ada) cout << "  Tidak ada reservasi aktif.\n";

            cout << "\nPosisi Antrian Anda:\n";
            lihatPosisiAntrian(res, userId);
            tekanEnterLanjut();
            break;
        }

        case 6:
            batalkanReservasi(res, ip, userId);
            tekanEnterLanjut();
            break;

        case 7:
            checkIniPhone(trx, ip, usr);
            tekanEnterLanjut();
            break;

        case 8:
            tampilkanRiwayatUser(trx, res, userId);
            tekanEnterLanjut();
            break;

        case 9: {
            User* u = cariUser(usr, userId);
            if (u) tampilkanDetailUser(*u);
            tekanEnterLanjut();
            break;
        }

        case 10:
            simpanData(ip, usr, res, trx);
            cout << "Logout berhasil.\n";
            return;

        default:
            cout << "Pilihan tidak valid.\n";
        }
    }
}

// ========== MENU ADMIN ==========

void menuAdmin(vector<iPhone>& ip, vector<User>& usr, vector<Reservasi>& res,
               vector<Transaksi>& trx) {
    while (true) {
        cout << "\n+==========================================+\n";
        cout << "|              MENU ADMIN                 |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Manajemen iPhone                     |\n";
        cout << "| 2. Manajemen User                       |\n";
        cout << "| 3. Kelola Reservasi                     |\n";
        cout << "| 4. Proses Pengembalian (Check-In)       |\n";
        cout << "| 5. Proses Antrian Otomatis              |\n";
        cout << "| 6. Laporan                              |\n";
        cout << "| 7. Simpan & Logout                      |\n";
        cout << "+==========================================+\n";
        cout << "Pilihan: ";

        int pilihanAdmin = bacaPilihan();

        switch (pilihanAdmin) {
        case 1:
            menuManajemenIphone(ip);
            simpanData(ip, usr, res, trx);
            break;
        case 2:
            menuManajemenUser(usr);
            simpanData(ip, usr, res, trx);
            break;
        case 3:
            batalkanReservasiByAdmin(res, ip);
            simpanData(ip, usr, res, trx);
            tekanEnterLanjut();
            break;
        case 4:
            checkIniPhone(trx, ip, usr);
            simpanData(ip, usr, res, trx);
            tekanEnterLanjut();
            break;
        case 5:
            // BONUS: Proses antrian dengan prioritas
            prosesAntrianOtomatis(res, ip, trx, usr);
            simpanData(ip, usr, res, trx);
            tekanEnterLanjut();
            break;
        case 6:
            menuLaporanAdmin(trx, usr, ip, res);
            break;
        case 7:
            simpanData(ip, usr, res, trx);
            cout << "Data tersimpan. Logout berhasil.\n";
            return;
        default:
            cout << "Pilihan tidak valid.\n";
            tekanEnterLanjut();
        }
    }
}

// ========== MAIN ==========

int main() {
    // BONUS: Load data dari CSV
    loadData(iphonesDB, usersDB, reservasiDB, transaksiDB);

    int choice;
    bool isAuthenticated = false;
    string currentUserId = "";
    string currentRole = "";

    while (true) {
        while (!isAuthenticated) {
            showWelcomeScreen();

            choice = bacaPilihan();

            switch (choice) {
            case 1:
                // MODUL 8: Login dengan exception handling
                isAuthenticated = login(currentUserId, currentRole);
                break;
            case 2:
                // MODUL 8: Register dengan exception handling
                try {
                    registerUser();
                    simpanData(iphonesDB, usersDB, reservasiDB, transaksiDB);
                } catch (const exception& e) {
                    cerr << "Gagal mendaftar: " << e.what() << endl;
                }
                break;
            case 3:
                simpanData(iphonesDB, usersDB, reservasiDB, transaksiDB);
                cout << "Terima kasih! Sampai jumpa." << endl;
                return 0;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            }
        }

        if (isAuthenticated) {
            if (currentRole == "Admin") {
                menuAdmin(iphonesDB, usersDB, reservasiDB, transaksiDB);
            }
            else if (currentRole == "Customer") {
                menuCustomer(currentUserId, iphonesDB, usersDB, reservasiDB, transaksiDB);
            }

            isAuthenticated = false;
            currentUserId = "";
            currentRole = "";
        }
    }

    return 0;
}
