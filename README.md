# Sistem Penyewaan iPhone

Proyek ini adalah aplikasi konsol berbasis C++17 untuk mengelola penyewaan iPhone, reservasi, data user, dan laporan transaksi. Aplikasi mendukung alur untuk guest, customer, dan admin, dengan penyimpanan data menggunakan file CSV agar data tetap tersimpan antar sesi.

## Fitur Utama

- Login, registrasi customer, dan mode guest.
- Lihat, cari, sewa, dan reservasi iPhone.
- Prioritas reservasi, termasuk prioritas VIP.
- Pengembalian iPhone dan perhitungan denda keterlambatan.
- Manajemen iPhone, user, penyewaan, dan reservasi oleh admin.
- Laporan transaksi dan invoice penyewaan ke folder `data/reports/`.

## Teknologi

- Bahasa: C++17
- Build system: xmake
- Library eksternal: `tabulate`
- Penyimpanan data: CSV

## Struktur Data

Data utama disimpan di folder `data/`:

- `iphones.csv`
- `users.csv`
- `rentals.csv`
- `reservations.csv`
- `reports/`

## Cara Menjalankan

1. Pastikan xmake sudah terpasang.
2. Jalankan build:

   ```bash
   xmake
   ```

3. Jalankan program:

   ```bash
   xmake run app
   ```

## Akun Default

- Admin default: `admin`
- Password default: `admin`

## Catatan

- Aplikasi akan mencari folder `data/` secara otomatis dari lokasi kerja saat dijalankan.
- Jika data belum tersedia, program akan tetap membuat file saat proses simpan berlangsung.

## Gambaran Singkat Alur Program

Program dimulai dari `main.cpp`, memuat data iPhone, user, penyewaan, dan reservasi, lalu menampilkan menu utama. Dari menu ini, user dapat memilih login, registrasi, atau menjelajah katalog iPhone. Setelah login, customer dapat melakukan sewa dan reservasi, sedangkan admin memiliki akses ke manajemen data dan laporan.