// MODUL 8: Include Guards (#ifndef/#define/#endif)
// MODUL 2: Struct dan Nested Struct (JadwalSewa di dalam iPhone)
#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"
#include <string>
#include <vector>

struct JadwalSewa {
    std::string tglMulai;
    std::string tglSelesai;
};

struct iPhone {
    std::string id;
    std::string model;
    double hargaPerHari;
    StatusiPhone status;
    std::string kondisi;
    std::vector<JadwalSewa> jadwal;
};

struct User {
    std::string id;
    std::string username;
    std::string password;
    std::string role;
    UserLevel level;
    int totalSewa;
};

struct Reservasi {
    std::string idReservasi;
    std::string userId;
    std::string iPhoneId;
    std::string tglMulai;
    std::string tglSelesai;
    bool isActive;
    int waktuBooking;
};

struct Transaksi {
    std::string idTransaksi;
    std::string userId;
    std::string iPhoneId;
    std::string tglMulai;
    std::string tglSelesai;
    std::string tglKembali;
    int durasi;
    double biayaSewa;
    double denda;
    double totalBayar;
};

#endif
