// [MODUL 5] Bubble Sort - O(n²) untuk prioritas reservasi
// [MODUL 8] Exception Handling

#include "reservation.h"
#include "iphone.h"
#include "user.h"
#include "csv_helper.h"
#include "utils.h"
#include "validator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tabulate/table.hpp> // [MODUL 8] Library External (tabulate)
using namespace std;
using namespace tabulate;

Reservation daftarReservasi[MAX_RESERVATIONS];
int jumlahReservasi = 0;

void loadReservations() {
    try {
        ifstream file(resolveProjectPath("data/reservations.csv"));
        if (!file.is_open()) {
            return;
        }

        string line;
        getline(file, line);

        jumlahReservasi = 0;
        while (getline(file, line) && jumlahReservasi < MAX_RESERVATIONS) {
            vector<string> fields = splitCSVRow(line);
            if (fields.size() < 6) continue;

            daftarReservasi[jumlahReservasi].reservationID = fields[0];
            daftarReservasi[jumlahReservasi].userID = fields[1];
            daftarReservasi[jumlahReservasi].iphoneID = fields[2];
            daftarReservasi[jumlahReservasi].reservationDate = fields[3];
            daftarReservasi[jumlahReservasi].priority = fields[4];
            daftarReservasi[jumlahReservasi].status = fields[5];
            jumlahReservasi++;
        }

        file.close();
    } catch (const exception& e) {
        cerr << "Error loading reservations: " << e.what() << endl;
        jumlahReservasi = 0;
    }
}

void saveReservations() {
    filesystem::path reservationsPath = resolveProjectPath("data/reservations.csv");
    filesystem::create_directories(reservationsPath.parent_path());

    ofstream file(reservationsPath);
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file reservations.csv!" << endl;
        return;
    }

    file << "reservation_id,user_id,iphone_id,reservation_date,priority,status" << endl;

    for (int i = 0; i < jumlahReservasi; i++) {
        file << daftarReservasi[i].reservationID << ","
             << daftarReservasi[i].userID << ","
             << daftarReservasi[i].iphoneID << ","
             << daftarReservasi[i].reservationDate << ","
             << daftarReservasi[i].priority << ","
             << daftarReservasi[i].status << endl;
    }

    file.close();
}

// [MODUL 5] Bubble Sort - O(n²)
// [MODUL 5] Mengurutkan reservasi berdasarkan prioritas (VIP > Regular)
// [MODUL 5] Descending: VIP didahulukan
void sortReservationsByPriority() {
    for (int i = 0; i < jumlahReservasi - 1; i++) {
        for (int j = 0; j < jumlahReservasi - i - 1; j++) {
            // [MODUL 1] Ternary Operator untuk konversi prioritas ke numerik
            int priorityA = (daftarReservasi[j].priority == "VIP") ? 1 : 0;
            int priorityB = (daftarReservasi[j + 1].priority == "VIP") ? 1 : 0;

            if (priorityA < priorityB) {
                // [MODUL 5] Swap manual
                Reservation temp = daftarReservasi[j];
                daftarReservasi[j] = daftarReservasi[j + 1];
                daftarReservasi[j + 1] = temp;
            }
        }
    }
}

int findReservasiByID(const string& id) {
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].reservationID == id) {
            return i;
        }
    }
    return -1;
}

void buatReservasi(const string& userID) {
    if (jumlahReservasi >= MAX_RESERVATIONS) {
        cout << "Maaf, antrean reservasi penuh!" << endl;
        pressEnter();
        return;
    }

    if (jumlahIPhone == 0) {
        cout << "Belum ada iPhone terdaftar." << endl;
        pressEnter();
        return;
    }

    cout << "=================================" << endl;
    cout << "   PILIH iPhone UNTUK RESERVASI  " << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < jumlahIPhone; i++) {
        cout << (i + 1) << ". " << daftarIPhone[i].name << " ("
             << daftarIPhone[i].storage << ") - Rp"
             << toString(daftarIPhone[i].rentPrice) << "/hari - "
             << (daftarIPhone[i].status == "available" ? "Tersedia" : "Disewa") << endl;
    }

    int pilihan;
    string input;
    while (true) {
        cout << "Pilih iPhone (1-" << jumlahIPhone << "): ";
        getline(cin, input);
        stringstream ss(input);
        if (ss >> pilihan && ss.eof() && pilihan >= 1 && pilihan <= jumlahIPhone) break;
        cout << "Masukkan angka 1 - " << jumlahIPhone << "!" << endl;
    }

    int idx = pilihan - 1;

    // [MODUL 1] Percabangan if: cek status VIP
    int userIdx = findUserIndexByID(userID);
    string priority = "Regular";
    if (userIdx != -1 && daftarUser[userIdx].isVIP) { // [MODUL 1] Operator Logika: &&
        priority = "VIP"; // [MODUL 1] Operator Penugasan: =
    }

    int newID = jumlahReservasi + 1;
    string resID = formatID("RSV", newID);

    daftarReservasi[jumlahReservasi].reservationID = resID;
    daftarReservasi[jumlahReservasi].userID = userID;
    daftarReservasi[jumlahReservasi].iphoneID = daftarIPhone[idx].id;
    daftarReservasi[jumlahReservasi].reservationDate = getCurrentDate();
    daftarReservasi[jumlahReservasi].priority = priority;
    daftarReservasi[jumlahReservasi].status = "waiting";
    jumlahReservasi++;

    sortReservationsByPriority(); // [MODUL 5] Bubble Sort
    saveReservations();

    cout << "Reservasi berhasil! ID Reservasi: " << resID << endl;
    cout << "Prioritas: " << priority << endl;
    cout << "Anda akan mendapat notifikasi ketika iPhone tersedia." << endl;
    pressEnter();
}

void processReservations(const string& iphoneID) {
    cout << "Memeriksa antrean reservasi untuk iPhone ini..." << endl;

    bool found = false;
    for (int i = 0; i < jumlahReservasi; i++) {
        // [MODUL 1] Operator Logika: &&
        if (daftarReservasi[i].iphoneID == iphoneID &&
            daftarReservasi[i].status == "waiting") {
            int userIdx = findUserIndexByID(daftarReservasi[i].userID);
            string userName = userIdx != -1 ? daftarUser[userIdx].name : "Unknown";

            cout << "Reservasi ditemukan:" << endl;
            cout << "ID: " << daftarReservasi[i].reservationID << endl;
            cout << "User: " << userName << endl;
            cout << "Prioritas: " << daftarReservasi[i].priority << endl;
            cout << "Tanggal: " << daftarReservasi[i].reservationDate << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Tidak ada antrean reservasi untuk iPhone ini." << endl;
    }
}

void cancelReservation() {
    if (jumlahReservasi == 0) {
        cout << "Belum ada reservasi." << endl;
        pressEnter();
        return;
    }

    int waitingIdx[MAX_RESERVATIONS];
    int waitingCount = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].status == "waiting") {
            waitingIdx[waitingCount] = i;
            waitingCount++;
        }
    }

    if (waitingCount == 0) {
        cout << "Tidak ada reservasi yang menunggu untuk dibatalkan." << endl;
        pressEnter();
        return;
    }

    Table table;
    table.add_row({"No", "ID", "User", "iPhone", "Tgl Reservasi", "Prioritas"});

    for (int i = 0; i < waitingCount; i++) {
        int idx = waitingIdx[i];
        int phoneIdx = findiPhoneByID(daftarReservasi[idx].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";
        int userIdx = findUserIndexByID(daftarReservasi[idx].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarReservasi[idx].userID;

        table.add_row({
            toString(i + 1),
            daftarReservasi[idx].reservationID,
            userName,
            phoneName,
            daftarReservasi[idx].reservationDate,
            daftarReservasi[idx].priority
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(10);
    table.column(2).format().width(16);
    table.column(3).format().width(20);
    table.column(4).format().width(14);
    table.column(5).format().width(10);

    cout << "Daftar reservasi menunggu:" << endl;
    cout << table << endl;

    cout << "Pilih reservasi yang dibatalkan (nomor/ID, 0 untuk batal): ";
    string input;
    getline(cin, input);
    if (isCancelInput(input)) {
        cout << "Pembatalan dibatalkan." << endl;
        pressEnter();
        return;
    }

    int idx = -1;
    int nomor;
    stringstream ss(input);
    if (ss >> nomor && ss.eof() && nomor >= 1 && nomor <= waitingCount) {
        idx = waitingIdx[nomor - 1];
    } else {
        idx = findReservasiByID(input);
    }

    if (idx == -1) {
        cout << "Reservasi tidak ditemukan!" << endl;
        pressEnter();
        return;
    }

    if (daftarReservasi[idx].status != "waiting") {
        cout << "Reservasi sudah diproses, tidak bisa dibatalkan." << endl;
        pressEnter();
        return;
    }

    string deletedID = daftarReservasi[idx].reservationID;
    string deletedUser = daftarReservasi[idx].userID;
    for (int i = idx; i < jumlahReservasi - 1; i++) {
        daftarReservasi[i] = daftarReservasi[i + 1];
    }
    jumlahReservasi--;

    saveReservations();
    logAudit("CANCEL_RESERVATION", "ID: " + deletedID + ", User: " + deletedUser);
    cout << "Reservasi berhasil dibatalkan." << endl;
    pressEnter();
}

void processReservationManually() {
    if (jumlahReservasi == 0) {
        cout << "Belum ada reservasi." << endl;
        pressEnter();
        return;
    }

    int waitingIdx[MAX_RESERVATIONS];
    int waitingCount = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].status == "waiting") {
            waitingIdx[waitingCount] = i;
            waitingCount++;
        }
    }

    if (waitingCount == 0) {
        cout << "Tidak ada reservasi yang menunggu diproses." << endl;
        pressEnter();
        return;
    }

    Table table;
    table.add_row({"No", "ID", "User", "iPhone", "Tgl Reservasi", "Prioritas"});

    for (int i = 0; i < waitingCount; i++) {
        int idx = waitingIdx[i];
        int phoneIdx = findiPhoneByID(daftarReservasi[idx].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";
        int userIdx = findUserIndexByID(daftarReservasi[idx].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarReservasi[idx].userID;

        table.add_row({
            toString(i + 1),
            daftarReservasi[idx].reservationID,
            userName,
            phoneName,
            daftarReservasi[idx].reservationDate,
            daftarReservasi[idx].priority
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(10);
    table.column(2).format().width(16);
    table.column(3).format().width(20);
    table.column(4).format().width(14);
    table.column(5).format().width(10);

    cout << "Daftar reservasi menunggu:" << endl;
    cout << table << endl;

    cout << "Pilih reservasi (nomor/ID, 0 untuk batal): ";
    string input;
    getline(cin, input);
    if (isCancelInput(input)) {
        cout << "Proses dibatalkan." << endl;
        pressEnter();
        return;
    }

    int idx = -1;
    int nomor;
    stringstream ss(input);
    if (ss >> nomor && ss.eof() && nomor >= 1 && nomor <= waitingCount) {
        idx = waitingIdx[nomor - 1];
    } else {
        idx = findReservasiByID(input);
    }

    if (idx == -1) {
        cout << "Reservasi tidak ditemukan!" << endl;
        pressEnter();
        return;
    }

    if (daftarReservasi[idx].status != "waiting") {
        cout << "Reservasi sudah diproses." << endl;
        pressEnter();
        return;
    }

    daftarReservasi[idx].status = "processed";
    saveReservations();
    logAudit("PROCESS_RESERVATION", "ID: " + daftarReservasi[idx].reservationID);
    cout << "Reservasi berhasil diproses." << endl;
    pressEnter();
}

void displayAllReservations() {
    if (jumlahReservasi == 0) {
        cout << "Belum ada reservasi." << endl;
        pressEnter();
        return;
    }

    sortReservationsByPriority();

    Table table;
    table.add_row({"No", "User", "iPhone", "Tgl Reservasi", "Prioritas", "Status"});

    for (int i = 0; i < jumlahReservasi; i++) {
        int phoneIdx = findiPhoneByID(daftarReservasi[i].iphoneID);
        string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";
        int userIdx = findUserIndexByID(daftarReservasi[i].userID);
        string userName = userIdx != -1 ? daftarUser[userIdx].name : daftarReservasi[i].userID;
        string statusText = (daftarReservasi[i].status == "waiting") ? "Menunggu" : "Diproses";

        table.add_row({
            toString(i + 1),
            userName,
            phoneName,
            daftarReservasi[i].reservationDate,
            daftarReservasi[i].priority,
            statusText
        });
    }

    table.column(0).format().width(5);
    table.column(1).format().width(16);
    table.column(2).format().width(20);
    table.column(3).format().width(14);
    table.column(4).format().width(10);
    table.column(5).format().width(10);

    cout << table << endl;
    pressEnter();
}

void displayUserReservations(const string& userID) {
    Table table;
    table.add_row({"No", "iPhone", "Tgl Reservasi", "Prioritas", "Status"});

    int num = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].userID == userID) {
            num++;
            int phoneIdx = findiPhoneByID(daftarReservasi[i].iphoneID);
            string phoneName = phoneIdx != -1 ? daftarIPhone[phoneIdx].name : "Unknown";
            string statusText = (daftarReservasi[i].status == "waiting") ? "Menunggu" : "Diproses";

            table.add_row({
                toString(num),
                phoneName,
                daftarReservasi[i].reservationDate,
                daftarReservasi[i].priority,
                statusText
            });
        }
    }

    if (num == 0) {
        cout << "Anda belum memiliki reservasi." << endl;
    } else {
        table.column(0).format().width(5);
        table.column(1).format().width(20);
        table.column(2).format().width(14);
        table.column(3).format().width(10);
        table.column(4).format().width(10);
        cout << table << endl;
    }

    pressEnter();
}
