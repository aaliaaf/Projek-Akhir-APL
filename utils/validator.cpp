// MODUL 8: try, catch, throw dengan out_of_range
// MODUL 8: <cmath>, debugging __FILE__, __LINE__, __func__
// MODUL 8: Pencatatan error menggunakan LOG_ERROR macro

#include "../includes/helpers.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <stdexcept>

using namespace std;

// MODUL 8: Validasi tanggal dengan try-catch
// MODUL 8: Menggunakan __FILE__, __LINE__, __func__ untuk debugging
bool validasiTanggal(string tgl) {
    try {
        // BONUS: Validasi input dengan loop
        if (tgl.length() != 10) {
            throw invalid_argument("Panjang tanggal harus 10 karakter (YYYY-MM-DD)");
        }
        if (tgl[4] != '-' || tgl[7] != '-') {
            throw invalid_argument("Format harus YYYY-MM-DD (dengan tanda strip)");
        }

        for (int i = 0; i < 10; i++) {
            if (i != 4 && i != 7 && !isdigit(tgl[i])) {
                throw invalid_argument("Karakter bukan digit pada posisi " + to_string(i));
            }
        }

        // MODUL 8: <cmath> abs untuk validasi rentang
        int tahun = stoi(tgl.substr(0, 4));
        int bulan = stoi(tgl.substr(5, 2));
        int hari = stoi(tgl.substr(8, 2));

        if (tahun < 2024 || tahun > 2030) {
            throw out_of_range("Tahun harus antara 2024 - 2030");
        }
        if (bulan < 1 || bulan > 12) {
            throw out_of_range("Bulan harus antara 01 - 12");
        }

        // MODUL 8: abs() dari <cmath>
        int maxHari = 31;
        if (bulan == 2) {
            maxHari = 28;
        } else if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) {
            maxHari = 30;
        }

        if (hari < 1 || hari > maxHari) {
            throw out_of_range(
                "Hari tidak valid untuk bulan " + to_string(bulan) +
                " (maks: " + to_string(maxHari) + ")"
            );
        }

        return true;
    }
    catch (const invalid_argument& e) {
        cerr << "[VALIDASI TANGGAL] " << e.what() << endl;
        // MODUL 8: Pencatatan error dengan __FILE__, __LINE__, __func__
        LOG_ERROR(string("validasiTanggal: ") + e.what() + " | Input: " + tgl);
        return false;
    }
    catch (const out_of_range& e) {
        cerr << "[VALIDASI TANGGAL] " << e.what() << endl;
        LOG_ERROR(string("validasiTanggal: ") + e.what() + " | Input: " + tgl);
        return false;
    }
    catch (const exception& e) {
        cerr << "[VALIDASI TANGGAL] Error tak terduga: " << e.what() << endl;
        LOG_ERROR(string("validasiTanggal unexpected: ") + e.what());
        return false;
    }
}

// MODUL 8: Konversi tanggal dengan exception handling
int tanggalToInt(string tgl) {
    try {
        if (tgl.length() != 10) {
            throw invalid_argument("Format tanggal tidak valid");
        }
        string tahun = tgl.substr(0, 4);
        string bulan = tgl.substr(5, 2);
        string hari = tgl.substr(8, 2);

        int t = stoi(tahun + bulan + hari);
        return t;
    }
    catch (const exception& e) {
        LOG_ERROR(string("tanggalToInt gagal: ") + e.what() + " | Input: " + tgl);
        return 0;
    }
}

// MODUL 8: Selisih hari dengan <ctime> dan <cmath>
int selisihHari(string mulai, string selesai) {
    try {
        if (!validasiTanggal(mulai) || !validasiTanggal(selesai)) {
            throw invalid_argument("Salah satu tanggal tidak valid");
        }

        // MODUL 8: Parsing string
        int tMulaiY = stoi(mulai.substr(0, 4));
        int tMulaiM = stoi(mulai.substr(5, 2));
        int tMulaiD = stoi(mulai.substr(8, 2));

        int tSelesaiY = stoi(selesai.substr(0, 4));
        int tSelesaiM = stoi(selesai.substr(5, 2));
        int tSelesaiD = stoi(selesai.substr(8, 2));

        tm tMulaiTM = {};
        tMulaiTM.tm_mday = tMulaiD;
        tMulaiTM.tm_mon  = tMulaiM - 1;
        tMulaiTM.tm_year = tMulaiY - 1900;

        tm tSelesaiTM = {};
        tSelesaiTM.tm_mday = tSelesaiD;
        tSelesaiTM.tm_mon  = tSelesaiM - 1;
        tSelesaiTM.tm_year = tSelesaiY - 1900;

        time_t a = mktime(&tMulaiTM);
        time_t b = mktime(&tSelesaiTM);

        if (a == (time_t)-1 || b == (time_t)-1) {
            throw runtime_error("Gagal konversi ke time_t");
        }

        double diff = difftime(b, a) / (60 * 60 * 24);

        // MODUL 8: abs() dari <cmath>
        return (int)round(abs(diff));
    }
    catch (const exception& e) {
        LOG_ERROR(string("selisihHari gagal: ") + e.what());
        return 1;
    }
}

// MODUL 6: Binary Search untuk cek bentrok jadwal
// Kompleksitas: O(n log n) sorting + O(log n) search = O(n log n)
bool cekBentrok(const vector<JadwalSewa>& jadwal, string mulai, string selesai) {
    if (jadwal.empty()) return false;

    int m = tanggalToInt(mulai);
    int s = tanggalToInt(selesai);

    // MODUL 6: Binary Search membutuhkan data terurut
    // Buat salinan dan urutkan berdasarkan tglMulai
    vector<JadwalSewa> sorted = jadwal;
    sort(sorted.begin(), sorted.end(), [](const JadwalSewa& a, const JadwalSewa& b) {
        string ta = a.tglMulai.substr(0,4) + a.tglMulai.substr(5,2) + a.tglMulai.substr(8,2);
        string tb = b.tglMulai.substr(0,4) + b.tglMulai.substr(5,2) + b.tglMulai.substr(8,2);
        return ta < tb;
    });

    // MODUL 6: Binary Search untuk deteksi bentrok
    // Kompleksitas pencarian: O(log n)
    int low = 0, high = (int)sorted.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        ASSERT(mid >= 0 && mid < (int)sorted.size(),
               "cekBentrok binary search index out of range");

        int jm = tanggalToInt(sorted[mid].tglMulai);
        int js = tanggalToInt(sorted[mid].tglSelesai);

        if (s < jm) {
            // Jadwal baru selesai SEBELUM jadwal existing dimulai → aman, cari ke kiri
            high = mid - 1;
        }
        else if (m > js) {
            // Jadwal baru mulai SESUDAH jadwal existing selesai → aman, cari ke kanan
            low = mid + 1;
        }
        else {
            // Saling tumpang tindih → bentrok!
            return true;
        }
    }

    return false;
}
