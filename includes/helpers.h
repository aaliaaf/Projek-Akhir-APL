// MODUL 8: Include Guards
#ifndef HELPERS_H
#define HELPERS_H

#include "structs.h"
#include "my_library.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// ========== ROLE ROUTING ==========
void menuAdmin(std::vector<iPhone>& ip, std::vector<User>& usr,
               std::vector<Reservasi>& res, std::vector<Transaksi>& trx);
void menuCustomer(std::string userId, std::vector<iPhone>& ip,
                  std::vector<User>& usr, std::vector<Reservasi>& res,
                  std::vector<Transaksi>& trx);

// ========== CORE RENTAL (CRUD iPhone) ==========
void tambahiPhone(std::vector<iPhone>& data);
void editiPhone(std::vector<iPhone>& data);
void hapusiPhone(std::vector<iPhone>& data);
void tampilkaniPhone(const std::vector<iPhone>& data);
void tampilkanDetailiPhone(const iPhone& ip);
void filteriPhoneByStatus(const std::vector<iPhone>& data, StatusiPhone status);

// MODUL 6: Binary Search untuk pencarian iPhone
iPhone* cariiPhoneBinary(std::vector<iPhone>& data, const std::string& id);
iPhone* cariiPhone(const std::vector<iPhone>& data, std::string keyword);
void sortiPhoneByID(std::vector<iPhone>& data);

// ========== USER MANAGER ==========
extern std::vector<User> usersDB;
bool login(std::string& outUserId, std::string& outRole);
void registerUser();
void registerUser(std::vector<User>& data);
void tambahUser(std::vector<User>& data);
void editUser(std::vector<User>& data);
void hapusUser(std::vector<User>& data);
User* cariUser(const std::vector<User>& data, std::string id);
void tampilkanRiwayatUser(const std::vector<Transaksi>& trx,
                          const std::vector<Reservasi>& res, std::string userId);
void lihatPosisiAntrian(const std::vector<Reservasi>& res, std::string userId);
void tampilkanDetailUser(const User& u);
void tampilkanDetailReservasi(const Reservasi& r);

// ========== PRIORITY & QUEUE ==========
// MODUL 6: Interpolation Search untuk Reservasi
void urutkanAntrian(std::vector<Reservasi>& antrian, std::vector<User>& usr);
void prosesAntrianOtomatis(std::vector<Reservasi>& antrian,
                           std::vector<iPhone>& stok,
                           std::vector<Transaksi>& histori,
                           std::vector<User>& usr);
int cariReservasiInterpolation(const std::vector<Reservasi>& antrian,
                                int waktuBooking);

// ========== TRANSACTION & COST ==========
void checkIniPhone(std::vector<Transaksi>& histori, std::vector<iPhone>& stok,
                   std::vector<User>& usr);
void batalkanReservasi(std::vector<Reservasi>& antrian, std::vector<iPhone>& stok,
                       std::string userId);
void batalkanReservasiByAdmin(std::vector<Reservasi>& antrian,
                              std::vector<iPhone>& stok);
double hitungBiaya(const iPhone& ip, int durasi, int terlambat, UserLevel level);

// ========== REPORTS (Admin Only) ==========
void laporanPendapatan(const std::vector<Transaksi>& histori);
void laporaniPhonePopuler(const std::vector<Transaksi>& histori);
void laporanUserAktif(const std::vector<Transaksi>& histori,
                      const std::vector<User>& users);
void laporanPendapatanPerBulan(const std::vector<Transaksi>& histori);
void laporanOkupansiIphone(const std::vector<Transaksi>& histori,
                           const std::vector<iPhone>& iphones);
void laporanKonversiReservasi(const std::vector<Reservasi>& reservasi,
                              const std::vector<Transaksi>& histori);
void laporanTopUserBySpending(const std::vector<Transaksi>& histori,
                              const std::vector<User>& users);

// ========== VALIDATOR & I/O ==========
// MODUL 8: Fungsi dengan try/catch/throw
bool validasiTanggal(std::string tgl);
int tanggalToInt(std::string tgl);
int selisihHari(std::string mulai, std::string selesai);
bool cekBentrok(const std::vector<JadwalSewa>& jadwal,
                std::string mulai, std::string selesai);
void loadData(std::vector<iPhone>& ip, std::vector<User>& usr,
              std::vector<Reservasi>& res, std::vector<Transaksi>& trx);
void simpanData(const std::vector<iPhone>& ip, const std::vector<User>& usr,
                const std::vector<Reservasi>& res,
                const std::vector<Transaksi>& trx);

// ========== UI ==========
void header(const std::string& judul);

#endif
