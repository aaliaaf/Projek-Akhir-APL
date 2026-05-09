#include "../includes/helpers.h"
#include <algorithm>

using namespace std;

int getPriorityLevel(UserLevel level) {
    switch (level) {
    case VIP:     return 1;
    case REGULAR: return 2;
    case BARU:    return 3;
    default:      return 4;
    }
}

void urutkanAntrian(vector<Reservasi>& antrian, vector<User>& usr) {
    if (antrian.empty()) return;

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

        int priorityA = getPriorityLevel(levelA);
        int priorityB = getPriorityLevel(levelB);

        if (priorityA != priorityB) {
            return priorityA < priorityB;
        }

        return a.waktuBooking < b.waktuBooking;
        });
}

void prosesAntrianOtomatis(vector<Reservasi>& antrian, vector<iPhone>& stok,
    vector<Transaksi>& histori, vector<User>& usr) {

    header("PROSES ANTRIAN OTOMATIS");

    if (antrian.empty()) {
        cout << "Antrian kosong." << endl;
        return;
    }

    urutkanAntrian(antrian, usr);

    bool adaDiproses = false;

    for (size_t i = 0; i < antrian.size(); ) {
        Reservasi& res = antrian[i];

        if (!res.isActive) {
            i++;
            continue;
        }

        iPhone* targetIP = nullptr;
        for (size_t j = 0; j < stok.size(); j++) {
            if (stok[j].id == res.iPhoneId) {
                targetIP = &stok[j];
                break;
            }
        }

        if (targetIP != nullptr && targetIP->status == TERSEDIA) {
            targetIP->status = DISEWA;

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

            cout << "Reservasi Berhasil" << res.idReservasi
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
        cout << "Tidak ada reservasi yang dapat diproses saat ini (Stok habis atau iPhone tidak tersedia)." << endl;
    }
}

*UserLevel cariLevelUser(vector<User>& usr, string userId) {
    for (int i = 0; i < usr.size(); i++) {
        if (usr[i].id == userId) {
            return usr[i].level;
        }
    }
    return UserLevel::Regular;
}

void urutkanAntrian(vector<Reservasi>& antrian, vector<User>& usr) {
    for (int i = 0; i < antrian.size(); i++) {
        for (int j = 0; j < antrian.size() - 1; j++) {

            if (antrian[j].isActive == false || antrian[j + 1].isActive == false) continue;

            User userAwal = cariLevelUser(usr, antrian[j].userId);
            User userLanjut = cariLevelUser(usr, antrian[j + 1].userId);

            int levelAwal = 0;
            if (userAwal.level == UserLevel::VIP) levelAwal = 3;
            else if (userAwal.level == UserLevel::Premium) levelAwal = 2;
            else levelAwal = 1;

            int levelLanjut = 0;
            if (userLanjut.level == UserLevel::VIP) levelLanjut = 3;
            else if (userLanjut.level == UserLevel::Premium) levelLanjut = 2;
            else levelLanjut = 1;

            bool perluTukar = false;

            if (levelLanjut > levelAwal) {
                perluTukar = true;
            }
            else if (levelLanjut == levelAwal) {
                if (antrian[j + 1].waktuBooking < antrian[j].waktuBooking) {
                    perluTukar = true;
                }
            }

            if (perluTukar == true) {
                Reservasi temp = antrian[j];
                antrian[j] = antrian[j + 1];
                antrian[j + 1] = temp;
            }
        }
    }
}

void prosesAntrianOtomatis(vector<Reservasi>& antrian, vector<iPhone>& stok,
    vector<Transaksi>& histori, vector<User>& usr) {

    cout << "\n=== PROSES ANTRIAN OTOMATIS ===" << endl;
    int jumlahBerhasil = 0;

    for (int i = 0; i < antrian.size(); i++) {

        if (antrian[i].isActive == false) continue;

        string idRes = antrian[i].idReservasi;
        string idUser = antrian[i].userId;
        string idHP = antrian[i].iPhoneId;
        string tglMulai = antrian[i].tglMulai;
        string tglSelesai = antrian[i].tglSelesai;

        cout << "\n[Proses] Cek Reservasi: " << idRes << endl;

        int idxHP = -1;
        bool iPhoneAda = false;
        bool iPhoneSiap = false;

        for (int j = 0; j < stok.size(); j++) {
            if (stok[j].id == idHP) {
                iPhoneAda = true;
                idxHP = j;

                if (stok[j].status != StatusiPhone::Tersedia) {
                    cout << "  Status iPhone bukan 'Tersedia'. Melewati." << endl;
                    break;
                }

                bool bentrok = false;
                for (int k = 0; k < stok[j].jadwal.size(); k++) {
                    if (tglMulai <= stok[j].jadwal[k].tglSelesai && tglSelesai >= stok[j].jadwal[k].tglMulai) {
                        bentrok = true;
                        break;
                    }
                }

                if (bentrok == true) {
                    cout << "  Jadwal bentrok dengan booking lain. Melewati." << endl;
                }
                else {
                    iPhoneSiap = true;
                }
                break;
            }
        }

        if (iPhoneAda == false) {
            cout << "  iPhone tidak ditemukan di database." << endl;
            continue;
        }
        if (iPhoneSiap == false) continue;

        cout << "Syarat terpenuhi! Memproses sewa..." << endl;

        int durasi = selisihHari(tglMulai, tglSelesai);
        if (durasi < 1) durasi = 1; 

        UserLevel lvlUser = cariLevelUser(usr, idUser);

        double biaya = hitungBiaya(stok[idxHP], durasi, 0, lvlUser);

        Transaksi trxBaru;
        trxBaru.idTransaksi = "TRX-" + to_string(histori.size() + 1);
        trxBaru.userId = idUser;
        trxBaru.iPhoneId = idHP;
        trxBaru.tglMulai = tglMulai;
        trxBaru.tglSelesai = tglSelesai;
        trxBaru.tglKembali = "Belum";
        trxBaru.durasi = durasi;
        trxBaru.biayaSewa = biaya;
        trxBaru.denda = 0.0;
        trxBaru.totalBayar = biaya;

        histori.push_back(trxBaru);

        stok[idxHP].status = StatusiPhone::Disewa;
        JadwalSewa jadwalBaru;
        jadwalBaru.tglMulai = tglMulai;
        jadwalBaru.tglSelesai = tglSelesai;
        stok[idxHP].jadwal.push_back(jadwalBaru);

        antrian[i].isActive = false;

        jumlahBerhasil++;
        cout << "  Sewa berhasil dicatat! ID Transaksi: " << trxBaru.idTransaksi << endl;
    }

    cout << "\nSelesai. Total " << jumlahBerhasil << " reservasi berhasil diproses." << endl;
}*