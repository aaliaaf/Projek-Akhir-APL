// [MODUL 1] I/O: cout, cin, getline, Operator Aritmatika (*, +=, ?:)
// [MODUL 2] Array, Struct, Array of Struct
// [MODUL 3] Prosedur, Fungsi, Pass by Reference, Variabel Global/Lokal
// [MODUL 4] Pointer: address-of (&), dereference (*), arrow operator (->), pointer arithmetic
// [MODUL 8] Exception Handling

#include "rental.h"
#include "iphone.h"
#include "user.h"
#include "reservation.h"
#include "csv_helper.h"
#include "utils.h"
#include "validator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <tabulate/table.hpp> // [MODUL 8] Library External (tabulate)
using namespace std;
using namespace tabulate;

Rental daftarRental[MAX_RENTALS];
int jumlahRental = 0;

void loadRentals() {
    try {
        ifstream file("data/rentals.csv");
        if (!file.is_open()) {
            return;
        }

        string line;
        getline(file, line);

        jumlahRental = 0;
        while (getline(file, line) && jumlahRental < MAX_RENTALS) {
            vector<string> fields = splitCSVRow(line);
            if (fields.size() < 7) continue;

            daftarRental[jumlahRental].rentalID = fields[0];
            daftarRental[jumlahRental].userID = fields[1];
            daftarRental[jumlahRental].iphoneID = fields[2];
            daftarRental[jumlahRental].startDate = fields[3];
            daftarRental[jumlahRental].duration = stringToInt(fields[4]);
            daftarRental[jumlahRental].totalPrice = stringToFloat(fields[5]);
            daftarRental[jumlahRental].status = fields[6];
            daftarRental[jumlahRental].lateFee = fields.size() > 7 ? stringToFloat(fields[7]) : 0;
            jumlahRental++;
        }

        file.close();
    } catch (const exception& e) {
        cerr << "Error loading rentals: " << e.what() << endl;
        jumlahRental = 0;
    }
}

void saveRentals() {
    ofstream file("data/rentals.csv");
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file rentals.csv!" << endl;
        return;
    }

    file << "rental_id,user_id,iphone_id,start_date,duration,total_price,status,late_fee" << endl;

    for (int i = 0; i < jumlahRental; i++) {
        file << daftarRental[i].rentalID << ","
             << daftarRental[i].userID << ","
             << daftarRental[i].iphoneID << ","
             << daftarRental[i].startDate << ","
             << toString(daftarRental[i].duration) << ","
             << toString(daftarRental[i].totalPrice) << ","
             << daftarRental[i].status << ","
             << toString(daftarRental[i].lateFee) << endl;
    }

    file.close();
}

void sewaIPhone(const string& userID) {
    int availIdx[MAX_IPHONES];
    int availCount = 0;
    for (int i = 0; i < jumlahIPhone; i++) {
        if (daftarIPhone[i].status == "available") {
            availIdx[availCount] = i;
            availCount++;
        }
    }

    if (availCount == 0) {
        cout << "Tidak ada iPhone yang tersedia saat ini." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "    PILIH iPhone TERSEDIA        " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < availCount; i++) {
        int idx = availIdx[i];
        cout << (i + 1) << ". " << daftarIPhone[idx].name << " ("
             << daftarIPhone[idx].storage << ") - Rp"
             << toString(daftarIPhone[idx].rentPrice) << "/hari" << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih iPhone (1-" << availCount << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= availCount) break;
        cout << "Masukkan angka 1 - " << availCount << "!" << endl;
    }

    int idx = availIdx[pilihan - 1];

    string startDate = getValidDateSplit();

    int duration = getValidInt("Durasi sewa (hari): ");
    if (duration < 1) {
        cout << "Durasi minimal 1 hari!" << endl;
        pressEnter();
        return;
    }

    // [MODUL 1] Operator Aritmatika: * (perkalian)
    float totalPrice = daftarIPhone[idx].rentPrice * duration;

    cout << "=================================" << endl;
    cout << "RINCIAN SEWA:" << endl;
    cout << "iPhone: " << daftarIPhone[idx].name << " (" << daftarIPhone[idx].storage << ")" << endl;
    cout << "Durasi: " << duration << " hari" << endl;
    cout << "Harga/hari: Rp" << toString(daftarIPhone[idx].rentPrice) << endl;
    cout << "Total: Rp" << toString(totalPrice) << endl;
    cout << "=================================" << endl;
    cout << "Konfirmasi sewa? (y/n): ";
    string confirm;
    getline(cin, confirm);
    if (confirm != "y" && confirm != "Y") {
        cout << "Penyewaan dibatalkan." << endl;
        pressEnter();
        return;
    }

    int newID = jumlahRental + 1;
    string rentalID = formatID("RNT", newID);

    daftarRental[jumlahRental].rentalID = rentalID;
    daftarRental[jumlahRental].userID = userID;
    daftarRental[jumlahRental].iphoneID = daftarIPhone[idx].id;
    daftarRental[jumlahRental].startDate = startDate;
    daftarRental[jumlahRental].duration = duration;
    daftarRental[jumlahRental].totalPrice = totalPrice;
    daftarRental[jumlahRental].status = "active";
    jumlahRental++;

    // [MODUL 1] Operator Increment: ++
    daftarIPhone[idx].status = "rented";
    daftarIPhone[idx].totalRented++;

    saveRentals();
    saveIPhones();

    logAudit("SEWA_IPHONE", "ID: " + rentalID + ", iPhone: " + daftarIPhone[idx].name);

    // Invoice markdown
    system("mkdir -p invoices 2>/dev/null");
    ofstream inv("invoices/" + rentalID + ".md");
    if (inv.is_open()) {
        inv << "# INVOICE PENYEWAAN IPHONE" << endl;
        inv << endl;
        inv << "**ID Rental:** " << rentalID << endl;
        inv << "**Tanggal:** " << getCurrentDateTime() << endl;
        inv << "**User ID:** " << userID << endl;
        inv << endl;
        inv << "## Detail iPhone" << endl;
        inv << "- **Nama:** " << daftarIPhone[idx].name << endl;
        inv << "- **Storage:** " << daftarIPhone[idx].storage << endl;
        inv << "- **Warna:** " << daftarIPhone[idx].color << endl;
        inv << "- **Tahun:** " << toString(daftarIPhone[idx].year) << endl;
        inv << "- **Harga/Hari:** Rp" << toString(daftarIPhone[idx].rentPrice) << endl;
        inv << endl;
        inv << "## Detail Sewa" << endl;
        inv << "- **Mulai:** " << startDate << endl;
        inv << "- **Durasi:** " << toString(duration) << " hari" << endl;
        inv << "- **Total:** Rp" << toString(totalPrice) << endl;
        inv << endl;
        inv << "---" << endl;
        inv << "*Terima kasih telah menggunakan layanan kami.*" << endl;
        inv.close();
        cout << "Invoice tersimpan: invoices/" << rentalID << ".md" << endl;
    }

    cout << "Penyewaan berhasil! ID Rental: " << rentalID << endl;
    pressEnter();
}

void returnIPhone() {
    int activeIdx[MAX_RENTALS]; // [MODUL 2] Array 1D
    int activeCount = 0;
    for (int i = 0; i < jumlahRental; i++) {
        if (daftarRental[i].status == "active") {
            activeIdx[activeCount] = i;
            activeCount++;
        }
    }

    if (activeCount == 0) {
        cout << "Tidak ada penyewaan aktif saat ini." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "   PILIH PENYEWAAN AKTIF         " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < activeCount; i++) {
        int idx = activeIdx[i];
        int phoneIdx = findiPhoneByID(daftarRental[idx].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : daftarRental[idx].iphoneID;
        int userIdx = findUserIndexByID(daftarRental[idx].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarRental[idx].userID;
        cout << (i + 1) << ". " << phoneName << " - " << userName
             << " - " << daftarRental[idx].startDate
             << " (" << daftarRental[idx].duration << " hari)"
             << " - Rp" << toString(daftarRental[idx].totalPrice) << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih penyewaan (1-" << activeCount << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= activeCount) break;
        cout << "Masukkan angka 1 - " << activeCount << "!" << endl;
    }

    int idx = activeIdx[pilihan - 1];

    daftarRental[idx].status = "completed";

    // Hitung denda overdue
    string expectedDate = daftarRental[idx].startDate;
    int y, m, d, dur = daftarRental[idx].duration;
    y = stringToInt(expectedDate.substr(0, 4));
    m = stringToInt(expectedDate.substr(5, 2));
    d = stringToInt(expectedDate.substr(8, 2));
    d += dur;
    while (d > 30) { d -= 30; m++; if (m > 12) { m = 1; y++; } }
    stringstream expSS;
    expSS << y << "-" << setw(2) << setfill('0') << m << "-" << setw(2) << setfill('0') << d;
    string expectedReturn = expSS.str();

    string today = getCurrentDate();
    int lateDays = 0;
    if (today > expectedReturn) {
        int ty, tm, td, ey, em, ed;
        ty = stringToInt(today.substr(0, 4));
        tm = stringToInt(today.substr(5, 2));
        td = stringToInt(today.substr(8, 2));
        ey = stringToInt(expectedReturn.substr(0, 4));
        em = stringToInt(expectedReturn.substr(5, 2));
        ed = stringToInt(expectedReturn.substr(8, 2));
        lateDays = (ty - ey) * 365 + (tm - em) * 30 + (td - ed);
        if (lateDays < 0) lateDays = 0;

        int phoneIdx2 = findiPhoneByID(daftarRental[idx].iphoneID);
        float dailyRate = phoneIdx2 != -1 ? daftarIPhone[phoneIdx2].rentPrice : 0;
        daftarRental[idx].lateFee = dailyRate * 0.1f * lateDays; // denda 10% per hari
        cout << "Peringatan: Terlambat " << lateDays << " hari! Denda: Rp" << toString(daftarRental[idx].lateFee) << endl;
    }

    int phoneIdx = findiPhoneByID(daftarRental[idx].iphoneID);
    if (phoneIdx != -1) {
        daftarIPhone[phoneIdx].status = "available";
    }

    saveRentals();
    saveIPhones();

    cout << "iPhone berhasil dikembalikan!" << endl;
    logAudit("RETURN_IPHONE", "ID: " + daftarRental[idx].rentalID + ", Denda: Rp" + toString(daftarRental[idx].lateFee));

    processReservations(daftarRental[idx].iphoneID);

    pressEnter();
}

void displayAllRentals() {
    if (jumlahRental == 0) {
        cout << "Belum ada transaksi penyewaan." << endl;
        pressEnter();
        return;
    }

    Table table;
    table.add_row({"No", "User", "iPhone", "Tgl Mulai", "Durasi", "Total", "Status"});

    for (int i = 0; i < jumlahRental; i++) {
        int phoneIdx = findiPhoneByID(daftarRental[i].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : daftarRental[i].iphoneID;
        int userIdx = findUserIndexByID(daftarRental[i].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarRental[i].userID;
        table.add_row({
            toString(i + 1),
            userName,
            phoneName,
            daftarRental[i].startDate,
            toString(daftarRental[i].duration) + " hari",
            "Rp" + toString(daftarRental[i].totalPrice),
            daftarRental[i].status == "active" ? "Aktif" : "Selesai"
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(16);
    table.column(2).format().width(20);
    table.column(3).format().width(12);
    table.column(4).format().width(8);
    table.column(5).format().width(14);
    table.column(6).format().width(8);

    cout << table << endl;
    pressEnter();
}

void displayRentalsByStatus(const string& status) {
    Table table;
    table.add_row({"No", "User", "iPhone", "Tgl Mulai", "Durasi", "Total", "Denda", "Status"});

    int num = 0;
    for (int i = 0; i < jumlahRental; i++) {
        if (daftarRental[i].status != status && status != "semua") continue;
        num++;
        int phoneIdx = findiPhoneByID(daftarRental[i].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : daftarRental[i].iphoneID;
        int userIdx = findUserIndexByID(daftarRental[i].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarRental[i].userID;
        table.add_row({
            toString(num),
            userName,
            phoneName,
            daftarRental[i].startDate,
            toString(daftarRental[i].duration) + " hari",
            "Rp" + toString(daftarRental[i].totalPrice),
            daftarRental[i].lateFee > 0 ? "Rp" + toString(daftarRental[i].lateFee) : "-",
            daftarRental[i].status == "active" ? "Aktif" : "Selesai"
        });
    }

    if (num == 0) {
        cout << "Tidak ada penyewaan dengan status tersebut." << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(16);
        table.column(2).format().width(20);
        table.column(3).format().width(12);
        table.column(4).format().width(8);
        table.column(5).format().width(14);
        table.column(6).format().width(14);
        table.column(7).format().width(8);
        cout << table << endl;
    }
    pressEnter();
}

void searchRentalsByUser() {
    cout << "Masukkan User ID atau nama user: ";
    string keyword;
    getline(cin, keyword);

    Table table;
    table.add_row({"No", "User", "iPhone", "Tgl Mulai", "Durasi", "Total", "Status"});

    int num = 0;
    for (int i = 0; i < jumlahRental; i++) {
        int userIdx = findUserIndexByID(daftarRental[i].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarRental[i].userID;
        if (daftarRental[i].userID != keyword && userName.find(keyword) == string::npos) continue;
        num++;
        int phoneIdx = findiPhoneByID(daftarRental[i].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";
        table.add_row({
            toString(num),
            userName,
            phoneName,
            daftarRental[i].startDate,
            toString(daftarRental[i].duration) + " hari",
            "Rp" + toString(daftarRental[i].totalPrice),
            daftarRental[i].status == "active" ? "Aktif" : "Selesai"
        });
    }

    if (num == 0) {
        cout << "Tidak ada penyewaan untuk user \"" << keyword << "\"" << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(16);
        table.column(2).format().width(20);
        table.column(3).format().width(12);
        table.column(4).format().width(8);
        table.column(5).format().width(14);
        table.column(6).format().width(8);
        cout << table << endl;
    }
    pressEnter();
}

void displayUserRentals(const string& userID) {
    Table table;
    table.add_row({"No", "iPhone", "Tgl Mulai", "Durasi", "Total", "Status"});

    int num = 0;
    for (int i = 0; i < jumlahRental; i++) {
        if (daftarRental[i].userID == userID) {
            num++;
            int phoneIdx = findiPhoneByID(daftarRental[i].iphoneID);
            string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";

            table.add_row({
                toString(num),
                phoneName,
                daftarRental[i].startDate,
                toString(daftarRental[i].duration) + " hari",
                "Rp" + toString(daftarRental[i].totalPrice),
                daftarRental[i].status == "active" ? "Aktif" : "Selesai"
            });
        }
    }

    if (num == 0) {
        cout << "Anda belum memiliki riwayat penyewaan." << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(20);
        table.column(2).format().width(12);
        table.column(3).format().width(8);
        table.column(4).format().width(14);
        table.column(5).format().width(8);
        cout << table << endl;
    }

    pressEnter();
}

// [MODUL 4] Pointer pada Struct: menggunakan pointer untuk iterasi array
// [MODUL 4] Address-of (&): mengambil alamat memori variabel
// [MODUL 4] Dereference (*): mengakses nilai dari pointer
// [MODUL 4] Arrow operator (->): mengakses field struct melalui pointer
void generateReport() {
    float totalPendapatan = 0;
    float totalDenda = 0;
    int totalAktif = 0;
    int totalSelesai = 0;
    int totalOverdue = 0;
    int maxRented = -1;
    // [MODUL 4] Deklarasi pointer ke struct iPhone
    iPhone* maxPtr = nullptr; // [MODUL 4] Inisialisasi pointer dengan nullptr

    // [MODUL 1] Operator Aritmatika: += (penugasan)
    for (int i = 0; i < jumlahRental; i++) {
        if (daftarRental[i].status == "completed") {
            totalPendapatan += daftarRental[i].totalPrice;
            totalSelesai++;
        } else {
            totalAktif++;
        }
        if (daftarRental[i].lateFee > 0) {
            totalDenda += daftarRental[i].lateFee;
            totalOverdue++;
        }
    }

    // [MODUL 2] Array 2D: statistik sewa per bulan [bulan][jenis]
    // jenis: 0=total, 1=aktif, 2=selesai, 3=pendapatan
    int statsBulan[12][4] = {0};
    for (int i = 0; i < jumlahRental; i++) {
        int bulan = stringToInt(daftarRental[i].startDate.substr(5, 2)) - 1;
        if (bulan < 0 || bulan > 11) continue;
        statsBulan[bulan][0]++;
        if (daftarRental[i].status == "active") statsBulan[bulan][1]++;
        else statsBulan[bulan][2]++;
        statsBulan[bulan][3] += daftarRental[i].totalPrice;
    }

    // [MODUL 4] Pointer sebagai parameter: urutkan bulan pakai swapByPointer
    // [MODUL 4] Cari bulan dengan pendapatan tertinggi & terendah pakai pointer swap
    int bulanIdx[12];
    for (int i = 0; i < 12; i++) bulanIdx[i] = i;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            if (statsBulan[bulanIdx[j]][3] < statsBulan[bulanIdx[j + 1]][3]) {
                swapByPointer(&bulanIdx[j], &bulanIdx[j + 1]);
            }
        }
    }

    // [MODUL 3] Rekursif: faktorial untuk kombinasi
    int totalKombinasi = jumlahIPhone > 1 ? faktorial(jumlahIPhone) / (2 * faktorial(jumlahIPhone - 2)) : 0;

    // [MODUL 4] Pointer pada Array: iterasi menggunakan pointer arithmetic
    // [MODUL 4] daftarIPhone adalah pointer ke elemen pertama array
    // [MODUL 4] ptr < daftarIPhone + jumlahIPhone: pointer arithmetic
    for (iPhone* ptr = daftarIPhone; ptr < daftarIPhone + jumlahIPhone; ptr++) {
        // [MODUL 4] Arrow operator (->): mengakses field melalui pointer
        if (ptr->totalRented > maxRented) {
            maxRented = ptr->totalRented;
            maxPtr = ptr; // [MODUL 4] Simpan alamat pointer
        }
    }

    Table table;
    table.add_row({"Metrik", "Nilai"});

    table.add_row({"Total Pendapatan", "Rp" + toString(totalPendapatan)});
    table.add_row({"Total Denda", "Rp" + toString(totalDenda)});
    table.add_row({"Total Transaksi", toString(jumlahRental)});
    table.add_row({"  - Aktif", toString(totalAktif)});
    table.add_row({"  - Selesai", toString(totalSelesai)});
    table.add_row({"  - Overdue (Kena Denda)", toString(totalOverdue)});
    table.add_row({"Total iPhone", toString(jumlahIPhone)});

    // [MODUL 6] Boundary handling: cek division by zero
    // [MODUL 1] Ternary Operator untuk mencegah pembagian dengan 0
    float rataRata = jumlahRental > 0 ? totalPendapatan / jumlahRental : 0;
    table.add_row({"Rata-rata/Transaksi", "Rp" + toString(rataRata)});

    // [MODUL 3] Rekursif: tampilkan kombinasi
    table.add_row({"Kombinasi 2 iPhone", toString(totalKombinasi)});

    // [MODUL 4] Null check pada pointer sebelum dereference
    if (maxPtr != nullptr && maxRented > 0) {
        table.add_row({"iPhone Terpopuler", maxPtr->name + " (" + toString(maxRented) + "x)"});
    } else {
        table.add_row({"iPhone Terpopuler", "Belum ada data"});
    }

    table.column(0).format().width(24);
    table.column(1).format().width(26);

    cout << table << endl;

    // [MODUL 2] Tampilkan array 2D: statistik bulanan
    cout << "\n=== Statistik Bulanan (Pendapatan) ===" << endl;
    cout << "Bulan dengan pendapatan tertinggi:" << endl;
    for (int i = 0; i < 3 && i < 12; i++) {
        int b = bulanIdx[i] + 1;
        string namaBulan;
        switch (b) {
            case 1: namaBulan = "Jan"; break; case 2: namaBulan = "Feb"; break;
            case 3: namaBulan = "Mar"; break; case 4: namaBulan = "Apr"; break;
            case 5: namaBulan = "Mei"; break; case 6: namaBulan = "Jun"; break;
            case 7: namaBulan = "Jul"; break; case 8: namaBulan = "Agu"; break;
            case 9: namaBulan = "Sep"; break; case 10: namaBulan = "Okt"; break;
            case 11: namaBulan = "Nov"; break; case 12: namaBulan = "Des"; break;
        }
        cout << "  " << (i + 1) << ". " << namaBulan << ": Rp" << toString(statsBulan[bulanIdx[i]][3])
             << " (" << toString(statsBulan[bulanIdx[i]][0]) << " transaksi)" << endl;
    }

    logAudit("LAPORAN", "Report generated");
    pressEnter();
}
