# SISTEM PENYEWAAN IPHONE
### dengan Jadwal dan Prioritas Reservasi

Proyek akhir mata kuliah **Algoritma dan Pemrograman Lanjut (APL)**.

---

## 📋 DAFTAR ISI

1. [Deskripsi Proyek](#-deskripsi-proyek)
2. [Struktur Direktori](#-struktur-direktori)
3. [Pemetaan Modul ke Fitur](#-pemetaan-modul-ke-fitur)
4. [Fitur & Hak Akses](#-fitur--hak-akses)
5. [Cara Compile & Run](#-cara-compile--run)
6. [Skenario Test](#-skenario-test)
7. [Panduan Git Collaboration](#-panduan-git-collaboration)

---

## 🎯 DESKRIPSI PROYEK

Sistem manajemen penyewaan iPhone berbasis **C++ console** dengan:
- **CRUD lengkap** untuk Device iPhone, Data Pelanggan, dan Data Reservasi
- **Multiuser**: Admin & Customer dengan sistem login
- **Prioritas Reservasi**: VIP > Regular > Baru (Queue berbasis prioritas)
- **Pencarian efisien**: Binary Search (O(log n)) & Interpolation Search (O(log log n))
- **Error Handling**: try-catch-throw dengan exception spesifik
- **External Library**: `my_library.h` dengan include guards
- **File I/O**: Simpan & load data ke CSV

---

## 📁 STRUKTUR DIREKTORI

```
proyek-akhir-apl/
│
├── includes/                  # Header files
│   ├── constants.h            # Konstanta, enum, ASSERT/LOG_ERROR macro
│   ├── structs.h              # Definisi struct (iPhone, User, Reservasi, Transaksi)
│   ├── helpers.h              # Deklarasi fungsi global
│   └── my_library.h           # External library (MODUL 8): string, math, search utils
│
├── modules/                   # Implementasi modul
│   ├── core_rental.cpp        # CRUD iPhone + Binary Search (MODUL 6)
│   ├── user_manager.cpp       # Login/Register + try-catch (MODUL 8)
│   ├── priority.cpp           # Priority Queue + Interpolation Search (MODUL 6)
│   ├── transactions.cpp       # Check-in/out + try-catch (MODUL 8)
│   └── reports.cpp            # Laporan-laporan admin
│
├── utils/                     # Utility functions
│   ├── validator.cpp          # Validasi tanggal, bentrok jadwal + try-catch + __FILE__
│   └── file_io.cpp            # Load/save data ke CSV
│
├── src/
│   └── main.cpp               # Entry point, menu navigasi
│
├── data/                      # Database CSV (auto-generated)
│   ├── iphones.csv
│   ├── users.csv
│   ├── reservations.csv
│   └── transactions.csv
│
├── modul/                     # Materi modul (PDF)
│   ├── MODUL 1 - Pengenalan C++.pdf
│   ├── MODUL 6 - Searching.pdf
│   └── MODUL 8 - Error Handling & Library.pdf
│
├── xmake.lua                  # Build config xmake
├── Makefile                   # Build config GNU Make / g++
└── README.md                  # Dokumentasi ini
```

---

## 📌 PEMETAAN MODUL KE FITUR

| Modul | Materi | Implementasi | File |
|-------|--------|--------------|------|
| **1** | Variabel, Tipe Data, I/O | `string`, `int`, `double`, `cin`/`cout` | Semua file |
| **2** | Array, Struct, Vector | `struct iPhone/User/Reservasi/Transaksi`, `vector<>` | `includes/structs.h`, semua `.cpp` |
| **3** | Fungsi & Prosedur | Semua fungsi dengan parameter & return value | Semua file |
| **4** | Pointer | `iPhone* cariiPhone()`, `updateStatusiPhone(iPhone*)` | `core_rental.cpp:29,139` |
| **5** | Sorting | `sort()` dari `<algorithm>` untuk antrian & laporan | `priority.cpp:25`, `reports.cpp` |
| **6** | **Binary Search** | `cariiPhoneBinary()` — O(log n) untuk ID iPhone | `core_rental.cpp:29-52` |
| **6** | **Interpolation Search** | `cariReservasiInterpolation()` — O(log log n) rata-rata | `priority.cpp:52-68` |
| **6** | **Kompleksitas Waktu** | Komentar O(log n), O(n) pada setiap algoritma | `core_rental.cpp`, `priority.cpp` |
| **8** | **try/catch/throw** | `invalid_argument`, `runtime_error`, `out_of_range` | `user_manager.cpp`, `transactions.cpp`, `validator.cpp`, `core_rental.cpp` |
| **8** | **Library standar** | `<vector>`, `<algorithm>`, `<cmath>` | Semua file |
| **8** | **External Library** | `my_library.h` dengan `#ifndef/#define/#endif` | `includes/my_library.h` |
| **8** | **Include Guards** | `#ifndef CONSTANTS_H`, `#ifndef STRUCTS_H`, `#ifndef HELPERS_H` | Semua header |
| **8** | **Debugging** | `ASSERT` macro, `LOG_ERROR(__FILE__, __LINE__, __func__)` | `includes/constants.h:25-38`, `validator.cpp`, `core_rental.cpp` |
| **BONUS** | File CSV | `loadData()` / `simpanData()` via `data/*.csv` | `utils/file_io.cpp` |
| **BONUS** | Tampilan Rapi | Tabel, box drawing, navigasi menu | `main.cpp`, `core_rental.cpp` |
| **BONUS** | Prioritas Queue | VIP > Regular > Baru + FIFO | `priority.cpp` |
| **BONUS** | Validasi Bentrok | Binary Search untuk deteksi bentrok jadwal | `validator.cpp:84-120` |

---

## 👥 FITUR & HAK AKSES

### Admin
| Menu | Deskripsi |
|------|-----------|
| Manajemen iPhone | CRUD (Create, Read, Update, Delete) iPhone |
| Manajemen User | CRUD User, atur role & level |
| Kelola Reservasi | Batalkan reservasi customer |
| Proses Pengembalian | Check-in iPhone |
| Proses Antrian | Jalankan prioritas antrian |
| Laporan | 7 jenis laporan (pendapatan, popularitas, okupansi, dll) |

### Customer
| Menu | Deskripsi |
|------|-----------|
| Lihat iPhone | Filter berdasarkan status Tersedia |
| Cari iPhone | Binary Search (by ID) / Sequential (by Model) |
| Buat Reservasi | Pilih iPhone, tentukan tanggal |
| Reservasi Aktif | Lihat reservasi & posisi antrian |
| Batalkan Reservasi | Hanya milik sendiri |
| Check-In | Kembalikan iPhone |
| Riwayat Transaksi | Histori peminjaman |
| Profil | Lihat data diri |

---

## 🔧 CARA COMPILE & RUN

### Opsi 1: GNU Make (Recommended)

```bash
# Compile
make

# Compile + Run
make run

# Debug mode (ASSERT aktif)
make debug

# Bersihkan binary
make clean
```

### Opsi 2: xmake

```bash
# Build
xmake build

# Run
xmake run app

# Debug
xmake build -m debug
xmake run app
```

### Opsi 3: Manual g++

```bash
g++ -std=c++17 -Wall -Wextra -Iincludes \
    src/main.cpp \
    modules/core_rental.cpp \
    modules/priority.cpp \
    modules/reports.cpp \
    modules/transactions.cpp \
    modules/user_manager.cpp \
    utils/file_io.cpp \
    utils/validator.cpp \
    -o bin/app

./bin/app
```

---

## 🧪 SKENARIO TEST

### 1. Login Multiuser

**Admin:**
```
Username: admin
Password: admin
-> Masuk ke Menu Admin (7 menu)
```

**Customer:**
```
Username: zidan
Password: 021
-> Masuk ke Menu Customer (10 menu)
```

### 2. Trigger Error Handling

| Skenario | Exception | File Terkait |
|----------|-----------|-------------|
| Login dengan username kosong | `invalid_argument` | `user_manager.cpp:30` |
| Input harga iPhone <= 0 | `invalid_argument` | `core_rental.cpp:169` |
| Cari iPhone tidak ditemukan | `runtime_error` | `core_rental.cpp:188` |
| Tanggal di luar rentang (2023 atau 2031) | `out_of_range` | `validator.cpp:47-50` |
| Input pilihan menu bukan angka | Input validation loop | `main.cpp:50-55` |
| Reservasi iPhone tidak tersedia | `runtime_error` | `transactions.cpp:71` |

### 3. Jalankan Searching

**Binary Search (O(log n)):**
1. Login sebagai Admin
2. Menu Manajemen iPhone -> Cari iPhone (Binary Search by ID)
3. Masukkan ID: `IPH003` (atau ID iPhone lainnya)
4. Lihat detail iPhone ditemukan

**Interpolation Search (O(log log n)):**
1. Lakukan beberapa reservasi
2. Fungsi `cariReservasiInterpolation()` dipanggil untuk mencari reservasi
3. (Terintegrasi di proses antrian)

### 4. Prioritas Queue (BONUS)

1. Login sebagai Customer (level BARU/REGULAR) buat reservasi
2. Login sebagai Customer lain (level VIP) buat reservasi
3. Admin jalankan "Proses Antrian Otomatis"
4. Observasi: reservasi VIP diproses lebih dulu

### 5. ASSERT Macro

Untuk menguji ASSERT, compile dengan `make debug`:
- ASSERT akan aktif dan memvalidasi kondisi kritis (null pointer, index bounds)
- Contoh: `ASSERT(ptr != nullptr, "...")` di `updateStatusiPhone()`

---

## 🌿 PANDUAN GIT COLLABORATION

### Strategi Branching

```
main        ← branch utama (hanya untuk merge稳定)
  ├── dev   ← branch pengembangan
  │   ├── feature/login        ← fitur login
  │   ├── feature/crud-iphone  ← CRUD iPhone
  │   ├── feature/reservasi    ← Reservasi
  │   ├── feature/reports      ← Laporan
  │   └── feature/searching    ← Searching (Modul 6)
```

### Setup Awal

```bash
# 1. Clone repository
git clone <url-repo>
cd proyek-akhir-apl

# 2. Buat branch utama pengembangan
git checkout -b dev

# 3. Push ke remote
git push -u origin dev
```

### Alur Kerja per Anggota

**Anggota A — Fitur Login & User Management:**
```bash
# Buat branch dari dev
git checkout dev
git pull origin dev
git checkout -b feature/login

# Kerjakan fitur...
# Setelah selesai:
git add .
git commit -m "feat: implementasi login dengan exception handling"
git push -u origin feature/login

# Buat Pull Request di GitHub: feature/login -> dev
```

**Anggota B — CRUD iPhone & Searching:**
```bash
git checkout dev
git pull origin dev
git checkout -b feature/crud-iphone

# Kerjakan...
git add .
git commit -m "feat: CRUD iPhone + Binary Search"
git push -u origin feature/crud-iphone

# Buat Pull Request: feature/crud-iphone -> dev
```

**Anggota C — Reservasi & Prioritas:**
```bash
git checkout dev
git pull origin dev
git checkout -b feature/reservasi

# Kerjakan...
git add .
git commit -m "feat: sistem reservasi dengan prioritas queue"
git push -u origin feature/reservasi

# Buat Pull Request: feature/reservasi -> dev
```

### Pull Request & Code Review

1. **Buat PR** di GitHub dari `feature/xxx` ke `dev`
2. **Review**: Minimal 1 anggota lain review kode
3. **Periksa**: Tidak ada konflik, compile sukses, fitur berfungsi
4. **Merge**: Setelah disetujui, merge ke `dev`

### Final Merge ke main

```bash
# Setelah semua fitur selesai di dev:
git checkout main
git pull origin main
git merge dev
git push origin main
```

### Tips Hindari Konflik

1. **Pisahkan file kerja**: Setiap anggota kerja di file berbeda
   - Anggota A: `modules/user_manager.cpp`
   - Anggota B: `modules/core_rental.cpp`
   - Anggota C: `modules/priority.cpp`, `modules/transactions.cpp`
2. **Selalu pull sebelum push**: `git pull origin dev --rebase`
3. **Komunikasi**: Laporkan file yang sedang dikerjakan di grup
4. **Commit kecil & sering**: Hindari commit besar yang banyak perubahan

---

## 📚 DEFAULT CREDENTIALS

| Username | Password | Role | Level |
|----------|----------|------|-------|
| admin | admin | Admin | BARU |
| zidan | 021 | Customer | VIP |
| desti | 001 | Customer | VIP |
| ahmad | pass123 | Customer | REGULAR |
| siti | siti456 | Customer | VIP |
| budi | budi789 | Customer | BARU |

---

## ⚙️ KOMPILASI

```
   ___         __  __  ___  _  _    ___
  / __| ___   |  \/  ||_ _|| \| |  / __|
 | (__ / _ \  | |\/| | | | | .` | | (__
  \___|\___/  |_|  |_||___||_|\_|  \___|

  g++ -std=c++17 -Wall -Wextra -Iincludes \
      src/main.cpp modules/*.cpp utils/*.cpp \
      -o bin/app

  ./bin/app
```

---

*Dibuat untuk memenuhi tugas Proyek Akhir APL — Semester 2*
