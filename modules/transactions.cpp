// MODUL 1-5: Variabel, percabangan, perulangan, fungsi, struct
// MODUL 8: try, catch, throw dengan runtime_error & invalid_argument
// MODUL 8: <cmath>, <vector>, <algorithm>

#include "../includes/helpers.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <limits>
#include <stdexcept>

using namespace std;

// ========== HEADER ==========

void header(const string& judul) {
    cout << "\n+==========================================+\n";
    cout << "  " << judul << endl;
    cout << "+==========================================+\n";
}

// ========== HITUNG BIAYA ==========
// MODUL 8: Menggunakan <cmath> round
double hitungBiaya(const iPhone& ip, int durasi, int terlambat, UserLevel level) {
    double total = ip.hargaPerHari * durasi;

    // MODUL 8: round() dari <cmath> untuk pembulatan
    if (level == VIP) {
        total = round(total * (1.0 - DISKON_VIP));
    }

    if (terlambat > 0) {
        total += terlambat * DENDA_PER_HARI;
    }

    return round(total);
}

// ========== CHECK-IN / PENGEMBALIAN ==========
// MODUL 8: try-catch
void checkIniPhone(vector<Transaksi>& histori, vector<iPhone>& stok, vector<User>& usr) {
    try {
        header("PENGEMBALIAN IPHONE");

        // Cari transaksi yang masih aktif
        bool adaTransaksiAktif = false;
        for (const auto& t : histori) {
            if (t.tglKembali == "-" || t.tglKembali.empty()) {
                adaTransaksiAktif = true;
                break;
            }
        }

        if (!adaTransaksiAktif) {
            throw runtime_error("Tidak ada penyewaan aktif yang perlu dikembalikan.");
        }

        cout << "\nDaftar Penyewaan Aktif:\n";
        int no = 1;
        vector<int> indeksAktif;

        for (size_t i = 0; i < histori.size(); i++) {
            if (histori[i].tglKembali == "-" || histori[i].tglKembali.empty()) {
                cout << "  " << no << ". " << histori[i].idTransaksi
                     << " | iPhone: " << histori[i].iPhoneId
                     << " | User: " << histori[i].userId
                     << " | Selesai: " << histori[i].tglSelesai << endl;
                indeksAktif.push_back((int)i);
                no++;
            }
        }

        if (indeksAktif.empty()) {
            throw runtime_error("Tidak ada transaksi aktif.");
        }

        int pilih;
        cout << "\nPilih nomor penyewaan: ";
        cin >> pilih;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Input harus angka!");
        }

        if (pilih < 1 || pilih > (int)indeksAktif.size()) {
            throw out_of_range("Nomor pilihan tidak valid!");
        }

        int idx = indeksAktif[pilih - 1];
        Transaksi& trx = histori[idx];

        string tglKembali;
        cout << "Tanggal kembali (YYYY-MM-DD) [Enter untuk hari ini]: ";
        cin.ignore();
        getline(cin, tglKembali);

        if (tglKembali.empty()) {
            // MODUL 8: <ctime>
            time_t now = time(0);
            tm* ltm = localtime(&now);
            char buffer[11];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
            tglKembali = string(buffer);
        }

        if (!validasiTanggal(tglKembali)) {
            throw invalid_argument("Format tanggal kembali salah!");
        }

        trx.tglKembali = tglKembali;
        int terlambat = max(0, tanggalToInt(tglKembali) - tanggalToInt(trx.tglSelesai));

        User* u = cariUser(usr, trx.userId);
        UserLevel lvl = u ? u->level : BARU;

        iPhone* ip = nullptr;
        for (auto& i : stok) {
            if (i.id == trx.iPhoneId) {
                ip = &i;
                break;
            }
        }

        if (ip == nullptr) {
            throw runtime_error("iPhone tidak ditemukan dalam stok!");
        }

        trx.durasi = max(1, selisihHari(trx.tglMulai, trx.tglSelesai));
        trx.biayaSewa = hitungBiaya(*ip, trx.durasi, 0, lvl);
        trx.denda = terlambat * DENDA_PER_HARI;
        trx.totalBayar = trx.biayaSewa + trx.denda;

        ip->status = Tersedia;

        cout << "\n+==========================================+\n";
        cout << "|           STRUK PENGEMBALIAN             |\n";
        cout << "+==========================================+\n";
        cout << "| ID Transaksi  : " << setw(24) << left << trx.idTransaksi << " |\n";
        cout << "| iPhone        : " << setw(24) << left << trx.iPhoneId << " |\n";
        cout << "| Tanggal kembali: " << setw(24) << left << trx.tglKembali << " |\n";
        cout << "| Keterlambatan : " << setw(24) << left << terlambat << " hari |\n";
        cout << "| Biaya Sewa    : Rp " << setw(21) << left << fixed << setprecision(0) << trx.biayaSewa << " |\n";
        cout << "| Denda         : Rp " << setw(21) << left << trx.denda << " |\n";
        cout << "| Total Bayar   : Rp " << setw(21) << left << trx.totalBayar << " |\n";
        cout << "+==========================================+\n";

        cout << "\nPengembalian berhasil!" << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "Input tidak valid: " << e.what() << endl;
        LOG_ERROR(string("CheckIn validation: ") + e.what());
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    catch (const out_of_range& e) {
        cerr << "Pilihan tidak valid: " << e.what() << endl;
    }
}

// ========== BATALKAN RESERVASI (CUSTOMER) ==========

void batalkanReservasi(vector<Reservasi>& antrian, vector<iPhone>& stok, string userId) {
    try {
        header("BATALKAN RESERVASI");

        vector<int> indeksRes;
        int no = 1;
        for (size_t i = 0; i < antrian.size(); i++) {
            if (antrian[i].userId == userId && antrian[i].isActive) {
                cout << "  " << no << ". " << antrian[i].idReservasi
                     << " | iPhone: " << antrian[i].iPhoneId
                     << " | " << antrian[i].tglMulai << " - " << antrian[i].tglSelesai << endl;
                indeksRes.push_back((int)i);
                no++;
            }
        }

        if (indeksRes.empty()) {
            throw runtime_error("Tidak ada reservasi aktif untuk dibatalkan.");
        }

        int pilih;
        cout << "\nPilih nomor reservasi: ";
        cin >> pilih;

        if (cin.fail() || pilih < 1 || pilih > (int)indeksRes.size()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Pilihan tidak valid!");
        }

        int idx = indeksRes[pilih - 1];
        string iphoneId = antrian[idx].iPhoneId;

        iPhone* ip = cariiPhone(stok, iphoneId);
        if (ip != nullptr) {
            for (size_t j = 0; j < ip->jadwal.size(); j++) {
                if (ip->jadwal[j].tglMulai == antrian[idx].tglMulai &&
                    ip->jadwal[j].tglSelesai == antrian[idx].tglSelesai) {
                    ip->jadwal.erase(ip->jadwal.begin() + j);
                    break;
                }
            }
            if (ip->status == Disewa && ip->jadwal.empty()) {
                ip->status = Tersedia;
            }
        }

        antrian[idx].isActive = false;
        cout << "Reservasi " << antrian[idx].idReservasi << " berhasil dibatalkan." << endl;
    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}

// ========== BATALKAN RESERVASI (ADMIN) ==========

void batalkanReservasiByAdmin(vector<Reservasi>& antrian, vector<iPhone>& stok) {
    try {
        header("BATALKAN RESERVASI (ADMIN)");

        bool adaAktif = false;
        for (const auto& r : antrian) {
            if (r.isActive) adaAktif = true;
        }

        if (!adaAktif) {
            throw runtime_error("Tidak ada reservasi aktif.");
        }

        cout << "\nSemua Reservasi Aktif:\n";
        int no = 1;
        for (size_t i = 0; i < antrian.size(); i++) {
            if (antrian[i].isActive) {
                cout << "  " << no << ". " << antrian[i].idReservasi
                     << " | User: " << antrian[i].userId
                     << " | iPhone: " << antrian[i].iPhoneId
                     << " | " << antrian[i].tglMulai << " - " << antrian[i].tglSelesai << endl;
                no++;
            }
        }
        int totalAktif = no - 1;

        int pilih;
        cout << "\nPilih nomor (0 untuk batal): ";
        cin >> pilih;

        if (pilih < 1 || pilih > totalAktif) {
            cout << "Dibatalkan.\n";
            return;
        }

        int idx = 0, count = 0;
        for (size_t i = 0; i < antrian.size(); i++) {
            if (antrian[i].isActive) {
                count++;
                if (count == pilih) {
                    idx = (int)i;
                    break;
                }
            }
        }

        if (antrian[idx].isActive) {
            string iphoneId = antrian[idx].iPhoneId;
            iPhone* ip = cariiPhone(stok, iphoneId);
            if (ip != nullptr) {
                for (size_t j = 0; j < ip->jadwal.size(); j++) {
                    if (ip->jadwal[j].tglMulai == antrian[idx].tglMulai &&
                        ip->jadwal[j].tglSelesai == antrian[idx].tglSelesai) {
                        ip->jadwal.erase(ip->jadwal.begin() + j);
                        break;
                    }
                }
                if (ip->status == Disewa && ip->jadwal.empty()) {
                    ip->status = Tersedia;
                }
            }
            antrian[idx].isActive = false;
            cout << "Reservasi " << antrian[idx].idReservasi << " dibatalkan oleh admin." << endl;
        }
    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }
}
