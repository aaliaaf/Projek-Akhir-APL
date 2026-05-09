#include "../includes/helpers.h"
#include <iostream>
#include <vector>

using namespace std;

void cetakTabeliPhone(const vector<iPhone>& data) {
    cout << "No  ID        Model            Harga/Hari    Status      Kondisi" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    if (data.size() == 0) {
        cout << "Tidak ada iPhone yang tersedia" << endl;
    }
    else {
        for (int i = 0; i < data.size(); i++) {
            cout << (i + 1) << "   " << data[i].id << "  " << data[i].model;
            cout << "\t\t Rp " << data[i].hargaPerHari;

            if (data[i].status == StatusiPhone::Tersedia) cout << "\t Tersedia";
            else if (data[i].status == StatusiPhone::Disewa) cout << "\t Disewa";
            else if (data[i].status == StatusiPhone::Maintenance) cout << "\t Maintenance";
            else if (data[i].status == StatusiPhone::Rusak) cout << "\t Rusak";
            else cout << "\t Tidak diketahui";

            cout << "\t " << data[i].kondisi << endl;
        }
    }
    cout << "------------------------------------------------------------------------" << endl;
}
void tampilkaniPhone(const vector<iPhone>& data) {
    cout << "DAFTAR iPHONE TERSEDIA" << endl;

    vector<iPhone> hasil;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].status == StatusiPhone::Tersedia) {
            hasil.push_back(data[i]);
        }
    }

    cetakTabeliPhone(hasil);
}
void filteriPhoneByStatus(const vector<iPhone>& data, StatusiPhone statusFilter) {
    cout << "FILTER iPHONE" << endl;

    cout << "Status Filter: ";
    if (statusFilter == StatusiPhone::Tersedia) cout << "Tersedia";
    else if (statusFilter == StatusiPhone::Disewa) cout << "Disewa";
    else if (statusFilter == StatusiPhone::Maintenance) cout << "Maintenance";
    else if (statusFilter == StatusiPhone::Rusak) cout << "Rusak";
    else cout << "Tidak diketahui";
    cout << endl << endl;

    vector<iPhone> hasil;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].status == statusFilter) {
            hasil.push_back(data[i]);
        }
    }
    cetakTabeliPhone(hasil);
}
void tampilkanDetailiPhone(const iPhone& ip) {
    cout << "DETAIL iPHONE" << endl;
    cout << "ID        : " << ip.id << endl;
    cout << "Model     : " << ip.model << endl;
    cout << "Harga/Hari: Rp " << ip.hargaPerHari << endl;

    cout << "Status    : ";
    if (ip.status == StatusiPhone::Tersedia) cout << "Tersedia";
    else if (ip.status == StatusiPhone::Disewa) cout << "Disewa";
    else if (ip.status == StatusiPhone::Maintenance) cout << "Maintenance";
    else if (ip.status == StatusiPhone::Rusak) cout << "Rusak";
    else cout << "Unknown";
    cout << endl;

    cout << "Kondisi   : " << ip.kondisi << endl;
    cout << "Jadwal Sewa: ";

    if (ip.jadwal.size() == 0) {
        cout << "- (Belum ada booking)" << endl;
    }
    else {
        cout << endl;
        for (int i = 0; i < ip.jadwal.size(); i++) {
            cout << "  - " << ip.jadwal[i].tglMulai << " s/d " << ip.jadwal[i].tglSelesai << endl;
        }
    }
}
iPhone* cariiPhone(const vector<iPhone>& data, string keyword) {
    for (int i = 0; i < data.size(); i++) {

        if (data[i].id == keyword) {
            return (iPhone*)&data[i];
        }
        if (keyword.length() <= data[i].model.length()) {
            bool ketemu = true;
            for (int j = 0; j < keyword.length(); j++) {
                if (keyword[j] != data[i].model[j]) {
                    ketemu = false;
                    break;
                }
            }
            if (ketemu == true) {
                return (iPhone*)&data[i];
            }
        }
    }
    return 0;
}