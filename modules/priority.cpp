// MODUL 5: Sorting (menggunakan sort dari <algorithm>)
// MODUL 6: Interpolation Search untuk reservasi berdasarkan waktuBooking
// MODUL 8: <algorithm>, <vector>, exception handling

#include "../includes/helpers.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

// ========== PRIORITY LEVEL ==========

int getPriorityLevel(UserLevel level) {
    switch (level) {
    case VIP:     return 1;
    case REGULAR: return 2;
    case BARU:    return 3;
    default:      return 4;
    }
}

// ========== BONUS: PRIORITY QUEUE ==========
// BONUS: Sistem Prioritas Reservasi: Member/Premium > Reguler
void urutkanAntrian(vector<Reservasi>& antrian, vector<User>& usr) {
    if (antrian.empty()) return;

    // MODUL 5: Sorting menggunakan <algorithm>
    // MODUL 8: sort dengan lambda comparator
    sort(antrian.begin(), antrian.end(), [&usr](const Reservasi& a, const Reservasi& b) {
        User* userA = nullptr;
        User* userB = nullptr;

        for (size_t i = 0; i < usr.size(); i++) {
            if (usr[i].id == a.userId) userA = &usr[i];
            if (usr[i].id == b.userId) userB = &usr[i];
            if (userA != nullptr && userB != nullptr) break;
        }

        UserLevel levelA = userA ? userA->level : BARU;
        UserLevel levelB = userB ? userB->level : BARU;

        // Prioritas: VIP (1) > REGULAR (2) > BARU (3)
        int prioA = getPriorityLevel(levelA);
        int prioB = getPriorityLevel(levelB);

        if (prioA != prioB) return prioA < prioB;
        // Jika prioritas sama, urut berdasarkan waktu booking (FIFO)
        return a.waktuBooking < b.waktuBooking;
    });
}

// ========== MODUL 6: INTERPOLATION SEARCH ==========
// MODUL 6: Interpolation Search untuk mencari reservasi berdasarkan waktuBooking
// MODUL 6: Kompleksitas: O(log log n) rata-rata, O(n) worst case
// MODUL 6: Prasyarat: data sudah terurut ascending berdasarkan waktuBooking

int cariReservasiInterpolation(const vector<Reservasi>& antrian, int waktuBooking) {
    if (antrian.empty()) return -1;

    // MODUL 6: Bangun array waktuBooking untuk dicari
    vector<int> waktuArr;
    for (const auto& res : antrian) {
        waktuArr.push_back(res.waktuBooking);
    }

    // MODUL 6: Gunakan Interpolation Search dari my_library.h
    // MODUL 8: <algorithm>, <vector>
    return mylib::interpolationSearchInt(waktuArr, waktuBooking);
}

// ========== PROSES ANTRIAN OTOMATIS ==========
// BONUS: Validasi bentrok jadwal

void prosesAntrianOtomatis(vector<Reservasi>& antrian, vector<iPhone>& stok,
    vector<Transaksi>& histori, vector<User>& usr) {

    // MODUL 8: try-catch untuk proses antrian
    try {
        header("PROSES ANTRIAN OTOMATIS");

        if (antrian.empty()) {
            cout << "Antrian kosong." << endl;
            return;
        }

        // Urutkan antrian berdasarkan prioritas
        urutkanAntrian(antrian, usr);

        bool adaDiproses = false;

        for (size_t i = 0; i < antrian.size(); ) {
            Reservasi& res = antrian[i];

            if (!res.isActive) {
                i++;
                continue;
            }

            // Cari iPhone yang dipesan
            iPhone* targetIP = nullptr;
            for (size_t j = 0; j < stok.size(); j++) {
                if (stok[j].id == res.iPhoneId) {
                    targetIP = &stok[j];
                    break;
                }
            }

            if (targetIP == nullptr) {
                throw runtime_error("iPhone dengan ID " + res.iPhoneId + " tidak ditemukan di stok!");
            }

            // BONUS: Validasi bentrok jadwal
            if (cekBentrok(targetIP->jadwal, res.tglMulai, res.tglSelesai)) {
                cout << "Jadwal reservasi " << res.idReservasi
                     << " bentrok. Dilewati." << endl;
                i++;
                continue;
            }

            if (targetIP->status == Tersedia) {
                // Proses: Disewa
                targetIP->status = Disewa;

                // Tambahkan ke jadwal
                JadwalSewa js{res.tglMulai, res.tglSelesai};
                targetIP->jadwal.push_back(js);

                Transaksi trx;
                trx.idTransaksi = "T" + to_string(histori.size() + 1);
                trx.userId = res.userId;
                trx.iPhoneId = res.iPhoneId;
                trx.tglMulai = res.tglMulai;
                trx.tglSelesai = res.tglSelesai;
                trx.tglKembali = "-";

                trx.durasi = selisihHari(res.tglMulai, res.tglSelesai);
                if (trx.durasi <= 0) trx.durasi = 1;

                User* u = cariUser(usr, res.userId);
                UserLevel lvl = u ? u->level : BARU;

                trx.biayaSewa = hitungBiaya(*targetIP, trx.durasi, 0, lvl);
                trx.denda = 0;
                trx.totalBayar = trx.biayaSewa;

                histori.push_back(trx);
                res.isActive = false;

                cout << "Reservasi " << res.idReservasi
                     << " (User: " << res.userId << ") diproses. iPhone "
                     << targetIP->model << " sekarang disewa." << endl;

                adaDiproses = true;
                antrian.erase(antrian.begin() + i);
            }
            else {
                i++;
            }
        }

        if (!adaDiproses) {
            cout << "Tidak ada reservasi yang dapat diproses saat ini." << endl;
        }
    }
    catch (const runtime_error& e) {
        cerr << "Error saat memproses antrian: " << e.what() << endl;
        LOG_ERROR(string("Proses antrian gagal: ") + e.what());
    }
    catch (const exception& e) {
        cerr << "Error tak terduga: " << e.what() << endl;
        LOG_ERROR(string("Exception: ") + e.what());
    }
}
