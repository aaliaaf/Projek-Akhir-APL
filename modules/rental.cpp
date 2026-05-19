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
        ifstream file(resolveProjectPath("data/rentals.csv"));
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
    }
}

void saveRentals() {
    filesystem::path rentalsPath = resolveProjectPath("data/rentals.csv");
    filesystem::create_directories(rentalsPath.parent_path());

    ofstream file(rentalsPath);
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file rentals.csv!" << endl;
        return;
    }

    file << "rental_id,user_id,iphone_id,start_date,duration,total_price,status,late_fee" << endl;

    for (int i = 0; i < jumlahRental; i++) {
           file << escapeCSVField(daftarRental[i].rentalID) << ","
               << escapeCSVField(daftarRental[i].userID) << ","
               << escapeCSVField(daftarRental[i].iphoneID) << ","
               << escapeCSVField(daftarRental[i].startDate) << ","
             << toString(daftarRental[i].duration) << ","
             << toString(daftarRental[i].totalPrice) << ","
               << escapeCSVField(daftarRental[i].status) << ","
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

    int newID = 0;
    for (int i = 0; i < jumlahRental; i++) {
        int suffix = stringToInt(daftarRental[i].rentalID.substr(3));
        if (suffix > newID) newID = suffix;
    }
    newID++;
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

    // Invoice text file in reports folder
    filesystem::path reportsDir = resolveProjectPath("data/reports");
    filesystem::create_directories(reportsDir);

    ofstream inv(reportsDir / (rentalID + ".txt"));
    if (inv.is_open()) {
        inv << "INVOICE PENYEWAAN IPHONE" << endl;
        inv << endl;
        inv << "ID Rental: " << rentalID << endl;
        inv << "Tanggal: " << getCurrentDateTime() << endl;
        inv << "User ID: " << userID << endl;
        inv << endl;
        inv << "Detail iPhone" << endl;
        inv << "Nama: " << daftarIPhone[idx].name << endl;
        inv << "Storage: " << daftarIPhone[idx].storage << endl;
        inv << "Warna: " << daftarIPhone[idx].color << endl;
        inv << "Tahun: " << toString(daftarIPhone[idx].year) << endl;
        inv << "Harga/Hari: Rp" << toString(daftarIPhone[idx].rentPrice) << endl;
        inv << endl;
        inv << "Detail Sewa" << endl;
        inv << "Mulai: " << startDate << endl;
        inv << "Durasi: " << toString(duration) << " hari" << endl;
        inv << "Total: Rp" << toString(totalPrice) << endl;
        inv << endl;
        inv << "Terima kasih telah menggunakan layanan kami." << endl;
        inv.close();
        cout << "Invoice tersimpan: " << (reportsDir / (rentalID + ".txt")).string() << endl;
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
    auto parseDate = [](const string& date) {
        tm t{};
        t.tm_year = stringToInt(date.substr(0, 4)) - 1900;
        t.tm_mon = stringToInt(date.substr(5, 2)) - 1;
        t.tm_mday = stringToInt(date.substr(8, 2));
        t.tm_hour = 12;
        return t;
    };

    tm startTm = parseDate(daftarRental[idx].startDate);
    time_t startTime = mktime(&startTm);
    time_t expectedTime = startTime == static_cast<time_t>(-1)
        ? static_cast<time_t>(-1)
        : startTime + static_cast<time_t>(daftarRental[idx].duration) * 86400;

    tm todayTm = parseDate(getCurrentDate());
    time_t todayTime = mktime(&todayTm);
    int lateDays = 0;
    if (expectedTime != static_cast<time_t>(-1) && todayTime != static_cast<time_t>(-1) && difftime(todayTime, expectedTime) > 0) {
        lateDays = static_cast<int>(difftime(todayTime, expectedTime) / 86400.0);
        if (lateDays < 1) lateDays = 1;

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

static int reservationPriorityRank(const Reservation& reservation) {
    return reservation.priority == "VIP" ? 1 : 0;
}

static void sortReservationIndicesByPriority(const int* source, int count, int* destination) {
    for (int i = 0; i < count; i++) {
        destination[i] = source[i];
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int left = destination[j];
            int right = destination[j + 1];
            if (reservationPriorityRank(daftarReservasi[left]) < reservationPriorityRank(daftarReservasi[right])) {
                int temp = destination[j];
                destination[j] = destination[j + 1];
                destination[j + 1] = temp;
            }
        }
    }
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
    generateReportOverview();
    generateReportMonthly();
    generateReportByPhone();
    generateReportByUser();
}

void generateReportOverview() {
    float totalPendapatan = 0;
    float totalDenda = 0;
    int totalAktif = 0;
    int totalSelesai = 0;
    int totalOverdue = 0;
    int maxRented = -1;
    iPhone* maxPtr = nullptr;

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

    int totalKombinasi = jumlahIPhone > 1 ? faktorial(jumlahIPhone) / (2 * faktorial(jumlahIPhone - 2)) : 0;
    for (iPhone* ptr = daftarIPhone; ptr < daftarIPhone + jumlahIPhone; ptr++) {
        if (ptr->totalRented > maxRented) {
            maxRented = ptr->totalRented;
            maxPtr = ptr;
        }
    }

    Table overview;
    overview.add_row({"Metrik", "Nilai"});
    overview.add_row({"Total Pendapatan", formatCurrency(totalPendapatan)});
    overview.add_row({"Total Denda", formatCurrency(totalDenda)});
    overview.add_row({"Total Transaksi", toString(jumlahRental)});
    overview.add_row({"  - Aktif", toString(totalAktif)});
    overview.add_row({"  - Selesai", toString(totalSelesai)});
    overview.add_row({"  - Overdue (Kena Denda)", toString(totalOverdue)});
    overview.add_row({"Total iPhone", toString(jumlahIPhone)});
    float rataRata = jumlahRental > 0 ? totalPendapatan / jumlahRental : 0;
    overview.add_row({"Rata-rata/Transaksi", formatCurrency(rataRata)});
    overview.add_row({"Kombinasi 2 iPhone", toString(totalKombinasi)});
    if (maxPtr != nullptr && maxRented > 0) {
        overview.add_row({"iPhone Terpopuler", maxPtr->name + " (" + toString(maxRented) + "x)"});
    } else {
        overview.add_row({"iPhone Terpopuler", "Belum ada data"});
    }
    overview.column(0).format().width(30);
    overview.column(1).format().width(26);
    cout << "=== LAPORAN RINGKASAN ===" << endl;
    cout << overview << endl;
    logAudit("LAPORAN", "Overview generated");
}

void generateReportMonthly() {
    int statsBulan[12][4] = {0};
    for (int i = 0; i < jumlahRental; i++) {
        int bulan = stringToInt(daftarRental[i].startDate.substr(5, 2)) - 1;
        if (bulan < 0 || bulan > 11) continue;
        statsBulan[bulan][0]++;
        if (daftarRental[i].status == "active") statsBulan[bulan][1]++;
        else statsBulan[bulan][2]++;
        statsBulan[bulan][3] += daftarRental[i].totalPrice;
    }

    int bulanIdx[12];
    for (int i = 0; i < 12; i++) bulanIdx[i] = i;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            if (statsBulan[bulanIdx[j]][3] < statsBulan[bulanIdx[j + 1]][3]) {
                swapByPointer(&bulanIdx[j], &bulanIdx[j + 1]);
            }
        }
    }

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
        cout << "  " << (i + 1) << ". " << namaBulan << ": " << formatCurrency(statsBulan[bulanIdx[i]][3])
             << " (" << toString(statsBulan[bulanIdx[i]][0]) << " transaksi)" << endl;
    }
    logAudit("LAPORAN", "Monthly generated");
}

void generateReportByPhone() {
    Table tphone;
    tphone.add_row({"No", "ID", "Nama", "Total Sewa", "Pendapatan"});
    for (int i = 0; i < jumlahIPhone; i++) {
        long long countRent = 0;
        float revenue = 0;
        for (int j = 0; j < jumlahRental; j++) {
            if (daftarRental[j].iphoneID == daftarIPhone[i].id) {
                countRent++;
                revenue += daftarRental[j].totalPrice;
            }
        }
        tphone.add_row({
            toString(i + 1),
            daftarIPhone[i].id,
            daftarIPhone[i].name,
            toString((int)countRent),
            formatCurrency(revenue)
        });
    }
    tphone.column(0).format().width(4);
    tphone.column(1).format().width(8);
    tphone.column(2).format().width(24);
    tphone.column(3).format().width(12);
    tphone.column(4).format().width(16);
    cout << "\n=== Rincian per iPhone ===" << endl;
    cout << tphone << endl;
    logAudit("LAPORAN", "Per-phone generated");
}

void generateReportByUser() {
    Table tuser;
    tuser.add_row({"No", "ID", "Nama", "Transaksi", "Total Bayar"});
    for (int i = 0; i < jumlahUser; i++) {
        int tx = 0;
        float spent = 0;
        for (int j = 0; j < jumlahRental; j++) {
            if (daftarRental[j].userID == daftarUser[i].id) {
                tx++;
                spent += daftarRental[j].totalPrice;
            }
        }
        tuser.add_row({
            toString(i + 1),
            daftarUser[i].id,
            daftarUser[i].name,
            toString(tx),
            formatCurrency(spent)
        });
    }
    tuser.column(0).format().width(4);
    tuser.column(1).format().width(8);
    tuser.column(2).format().width(20);
    tuser.column(3).format().width(10);
    tuser.column(4).format().width(16);
    cout << "\n=== Rincian per User ===" << endl;
    cout << tuser << endl;
    logAudit("LAPORAN", "Per-user generated");
}

bool exportFullReport() {
    filesystem::path reportsDir = resolveProjectPath("data/reports");
    filesystem::create_directories(reportsDir);
    string fname = getCurrentDate();
    filesystem::path out = reportsDir / (fname + "-laporan.txt");
    ofstream f(out);
    if (!f.is_open()) return false;
    
    f << "=====================================\n";
    f << "    LAPORAN LENGKAP PENYEWAAN IPHONE\n";
    f << "=====================================\n";
    f << "Tanggal: " << getCurrentDateTime() << "\n\n";
    
    // Overview data
    float totalPendapatan = 0;
    float totalDenda = 0;
    int totalAktif = 0;
    int totalSelesai = 0;
    int totalOverdue = 0;
    int maxRented = -1;
    iPhone* maxPtr = nullptr;
    
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
    
    for (iPhone* ptr = daftarIPhone; ptr < daftarIPhone + jumlahIPhone; ptr++) {
        if (ptr->totalRented > maxRented) {
            maxRented = ptr->totalRented;
            maxPtr = ptr;
        }
    }
    
    // Write Overview
    f << "=== LAPORAN RINGKASAN ===\n";
    f << "Total Pendapatan       : " << formatCurrency(totalPendapatan) << "\n";
    f << "Total Denda            : " << formatCurrency(totalDenda) << "\n";
    f << "Total Transaksi        : " << toString(jumlahRental) << "\n";
    f << "  - Aktif              : " << toString(totalAktif) << "\n";
    f << "  - Selesai            : " << toString(totalSelesai) << "\n";
    f << "  - Overdue (Denda)    : " << toString(totalOverdue) << "\n";
    f << "Total iPhone           : " << toString(jumlahIPhone) << "\n";
    
    float rataRata = jumlahRental > 0 ? totalPendapatan / jumlahRental : 0;
    f << "Rata-rata/Transaksi    : " << formatCurrency(rataRata) << "\n";
    
    if (maxPtr != nullptr && maxRented > 0) {
        f << "iPhone Terpopuler       : " << maxPtr->name << " (" << toString(maxRented) << "x)\n";
    }
    f << "\n";
    
    // Write Per-iPhone breakdown
    f << "=== RINCIAN PER IPHONE ===\n";
    for (int i = 0; i < jumlahIPhone; i++) {
        long long countRent = 0;
        float revenue = 0;
        for (int j = 0; j < jumlahRental; j++) {
            if (daftarRental[j].iphoneID == daftarIPhone[i].id) {
                countRent++;
                revenue += daftarRental[j].totalPrice;
            }
        }
        f << "  " << (i + 1) << ". " << daftarIPhone[i].name << " (" << daftarIPhone[i].id << ")\n";
        f << "     Total Sewa : " << toString((int)countRent) << "\n";
        f << "     Pendapatan : " << formatCurrency(revenue) << "\n";
    }
    f << "\n";
    
    // Write Per-User breakdown
    f << "=== RINCIAN PER USER ===\n";
    for (int i = 0; i < jumlahUser; i++) {
        int tx = 0;
        float spent = 0;
        for (int j = 0; j < jumlahRental; j++) {
            if (daftarRental[j].userID == daftarUser[i].id) {
                tx++;
                spent += daftarRental[j].totalPrice;
            }
        }
        f << "  " << (i + 1) << ". " << daftarUser[i].name << " (" << daftarUser[i].id << ")\n";
        f << "     Transaksi  : " << toString(tx) << "\n";
        f << "     Total Bayar: " << formatCurrency(spent) << "\n";
    }
    f << "\n";
    
    f << "=====================================\n";
    f << "File ini dihasilkan secara otomatis.\n";
    f.close();
    logAudit("EKSPOR", "Full report exported to " + out.string());
    return true;
}
