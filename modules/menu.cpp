// [MODUL 1] Percabangan: if-else, switch-case, nested if
// [MODUL 1] Perulangan: do-while, for
// [MODUL 1] I/O: cout, cin, getline
// [MODUL 3] Prosedur (void)

#include "menu.h"
#include "user.h"
#include "iphone.h"
#include "rental.h"
#include "reservation.h"
#include "validator.h"
#include "utils.h"
#include <iostream>
using namespace std;

// [MODUL 1] Perulangan do-while (minimal 1x eksekusi)
// [MODUL 1] Percabangan switch-case
void showMainMenu() {
    int pilihan;

    do { // [MODUL 1] do-while loop
        clearScreen();
        cout << "=================================" << endl;
        cout << "   SISTEM PENYEWAAN IPHONE       " << endl;
        cout << "=================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register Customer" << endl;
        cout << "3. Surfing" << endl;
        cout << "4. Keluar" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 4);

        // [MODUL 1] switch-case: percabangan berdasarkan nilai
        switch (pilihan) { // [MODUL 1] switch
            case 1: // [MODUL 1] case
                login();
                break; // [MODUL 1] break
            case 2:
                registerUser();
                break;
            case 3:
                showGuestMenu();
                break;
            case 4:
                cout << "Terima kasih telah menggunakan sistem." << endl;
                break;
        }
    } while (pilihan != 4); // [MODUL 1] while condition
}

void showGuestMenu() {
    int pilihan;
    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "       SURFING (GUEST)           " << endl;
        cout << "=================================" << endl;
        cout << "1. Lihat Semua iPhone" << endl;
        cout << "2. Cari iPhone" << endl;
        cout << "3. Kembali" << endl;
        cout << "=================================" << endl;
        pilihan = getMenuChoice(1, 3);
        switch (pilihan) {
            case 1: displayAllIPhones(); break;
            case 2: searchIPhone(); break;
            case 3: break;
        }
    } while (pilihan != 3);
}

void showCustomerMenu(const string& userID) {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "        MENU CUSTOMER            " << endl;
        cout << "=================================" << endl;
        cout << "1. Lihat Daftar iPhone" << endl;
        cout << "2. Cari iPhone" << endl;
        cout << "3. Sewa iPhone" << endl;
        cout << "4. Reservasi iPhone" << endl;
        cout << "5. Status Reservasi Saya" << endl;
        cout << "6. Riwayat Penyewaan Saya" << endl;
        cout << "7. Ganti Password" << endl;
        cout << "8. Logout" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 8);

        switch (pilihan) {
            case 1:
                customerLihatIPhone(userID);
                break;
            case 2:
                customerCariIPhone(userID);
                break;
            case 3:
                customerSewa(userID);
                break;
            case 4:
                customerReservasi(userID);
                break;
            case 5:
                customerStatusReservasi(userID);
                break;
            case 6:
                customerRiwayat(userID);
                break;
            case 7:
                changePassword(userID);
                break;
            case 8:
                cout << "Logout berhasil." << endl;
                break;
        }
    } while (pilihan != 8);
}

void showAdminMenu() {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "         MENU ADMIN              " << endl;
        cout << "=================================" << endl;
        cout << "1. Manajemen iPhone" << endl;
        cout << "2. Manajemen User" << endl;
        cout << "3. Manajemen Penyewaan" << endl;
        cout << "4. Manajemen Reservasi" << endl;
        cout << "5. Laporan" << endl;
        cout << "6. Logout" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 6);

        switch (pilihan) {
            case 1:
                adminManageIPhones();
                break;
            case 2:
                adminManageUsers();
                break;
            case 3:
                adminManageRentals();
                break;
            case 4:
                adminManageReservations();
                break;
            case 5:
                adminViewReports();
                break;
            case 6:
                cout << "Logout berhasil." << endl;
                break;
        }
    } while (pilihan != 6);
}

// [MODUL 3] Prosedur: void, pass by value (string dikirim sebagai salinan)
void customerLihatIPhone(const string&) {
    displayAllIPhones();
}

void customerCariIPhone(const string&) {
    searchIPhone();
}

void customerSewa(const string& userID) {
    sewaIPhone(userID);
}

void customerReservasi(const string& userID) {
    buatReservasi(userID);
}

void customerRiwayat(const string& userID) {
    displayUserRentals(userID);
}

void customerStatusReservasi(const string& userID) {
    displayUserReservations(userID);
}

void adminManageIPhones() {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "     MANAJEMEN IPHONE            " << endl;
        cout << "=================================" << endl;
        cout << "1. Tambah iPhone" << endl;
        cout << "2. Edit iPhone" << endl;
        cout << "3. Hapus iPhone" << endl;
        cout << "4. Lihat Semua iPhone" << endl;
        cout << "5. Cari iPhone" << endl;
        cout << "6. Kembali" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 6);

        // [MODUL 1] switch-case dengan nested if di dalam case
        switch (pilihan) {
            case 1:
                tambahIPhone();
                break;
            case 2:
                editIPhone();
                break;
            case 3:
                hapusIPhone();
                break;
            case 4: {
                displayAllIPhones();
                if (jumlahIPhone == 0) break;
                cout << "\n--- Urutkan iPhone ---" << endl;
                cout << "1. Selection Sort (Storage)" << endl;
                cout << "2. Merge Sort (Nama Model)" << endl;
                cout << "3. Quick Sort (Tahun Rilis)" << endl;
                cout << "0. Kembali" << endl;
                int sub = getMenuChoice(0, 3);
                if (sub == 1) {
                    selectionSortByStorage();
                    displayAllIPhones();
                } else if (sub == 2) {
                    mergeSortIPhone(0, jumlahIPhone - 1);
                    cout << "Diurutkan dengan Merge Sort." << endl;
                    displayAllIPhones();
                } else if (sub == 3) {
                    quickSortIPhone(0, jumlahIPhone - 1);
                    cout << "Diurutkan dengan Quick Sort." << endl;
                    displayAllIPhones();
                }
                break;
            }
            case 5: {
                cout << "\n--- Metode Pencarian ---" << endl;
                cout << "1. Linear Search (O(n))" << endl;
                cout << "2. Binary Search (O(log n))" << endl;
                cout << "3. Interpolation Search (O(log log n))" << endl;
                cout << "4. Jump Search (O(√n))" << endl;
                cout << "0. Kembali" << endl;
                int sub = getMenuChoice(0, 4);
                if (sub == 1) searchIPhone();
                else if (sub == 2) binarySearchIPhone();
                else if (sub == 3) interpolationSearchIPhone();
                else if (sub == 4) jumpSearchIPhone();
                break;
            }
            case 6:
                break;
        }
    } while (pilihan != 6);
}

void adminManageUsers() {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "     MANAJEMEN USER              " << endl;
        cout << "=================================" << endl;
        cout << "1. Tambah User" << endl;
        cout << "2. Lihat Semua User" << endl;
        cout << "3. Edit User" << endl;
        cout << "4. Hapus User" << endl;
        cout << "5. Atur Status VIP" << endl;
        cout << "6. Cari User" << endl;
        cout << "7. Kembali" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 7);

        switch (pilihan) {
            case 1:
                tambahUser();
                break;
            case 2: {
                displayAllUsers();
                if (jumlahUser == 0) break;
                cout << "\n--- Urutkan User ---" << endl;
                cout << "1. Insertion Sort (Nama)" << endl;
                cout << "2. Quick Sort (Kota)" << endl;
                cout << "0. Kembali" << endl;
                int sub = getMenuChoice(0, 2);
                if (sub == 1) {
                    sortUsersByName();
                    cout << "Diurutkan dengan Insertion Sort (Nama)." << endl;
                    displayAllUsers();
                } else if (sub == 2) {
                    quickSortUser(0, jumlahUser - 1);
                    cout << "Diurutkan dengan Quick Sort (Kota)." << endl;
                    displayAllUsers();
                }
                break;
            }
            case 3:
                editUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                setVIPStatus();
                break;
            case 6: {
                cout << "\n--- Metode Pencarian User ---" << endl;
                cout << "1. Linear Search (O(n))" << endl;
                cout << "2. Binary Search (O(log n))" << endl;
                cout << "3. Jump Search (O(√n))" << endl;
                cout << "0. Kembali" << endl;
                int sub = getMenuChoice(0, 3);
                if (sub == 1) searchUserLinear();
                else if (sub == 2) searchUserBinary();
                else if (sub == 3) searchUserJump();
                break;
            }
            case 7:
                break;
        }
    } while (pilihan != 7);
}

void adminManageRentals() {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "   MANAJEMEN PENYEWAAN           " << endl;
        cout << "=================================" << endl;
        cout << "1. Lihat Semua Penyewaan" << endl;
        cout << "2. Lihat Penyewaan Aktif" << endl;
        cout << "3. Lihat Penyewaan Selesai" << endl;
        cout << "4. Cari Penyewaan by User" << endl;
        cout << "5. Pengembalian iPhone" << endl;
        cout << "6. Kembali" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 6);

        switch (pilihan) {
            case 1:
                displayAllRentals();
                break;
            case 2:
                displayRentalsByStatus("active");
                break;
            case 3:
                displayRentalsByStatus("completed");
                break;
            case 4:
                searchRentalsByUser();
                break;
            case 5:
                returnIPhone();
                break;
            case 6:
                break;
        }
    } while (pilihan != 6);
}

void adminManageReservations() {
    int pilihan;

    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "   MANAJEMEN RESERVASI           " << endl;
        cout << "=================================" << endl;
        cout << "1. Lihat Semua Reservasi" << endl;
        cout << "2. Proses Reservasi Manual" << endl;
        cout << "3. Batalkan Reservasi" << endl;
        cout << "4. Kembali" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 4);

        switch (pilihan) {
            case 1:
                displayAllReservations();
                break;
            case 2:
                processReservationManually();
                break;
            case 3:
                cancelReservation();
                break;
            case 4:
                break;
        }
    } while (pilihan != 4);
}

void adminViewReports() {
    int pilihan;
    do {
        clearScreen();
        cout << "=================================" << endl;
        cout << "         LAPORAN                 " << endl;
        cout << "=================================" << endl;
        cout << "1. Laporan Ringkasan" << endl;
        cout << "2. Statistik Bulanan" << endl;
        cout << "3. Rincian per iPhone" << endl;
        cout << "4. Rincian per User" << endl;
        cout << "5. Daftar Reservasi" << endl;
        cout << "6. Ekspor Laporan Lengkap" << endl;
        cout << "7. Kembali" << endl;
        cout << "=================================" << endl;

        pilihan = getMenuChoice(1, 7);
        switch (pilihan) {
            case 1:
                generateReportOverview();
                pressEnter();
                break;
            case 2:
                generateReportMonthly();
                pressEnter();
                break;
            case 3:
                generateReportByPhone();
                pressEnter();
                break;
            case 4:
                generateReportByUser();
                pressEnter();
                break;
            case 5:
                displayAllReservations();
                break;
            case 6:
                if (exportFullReport()) {
                    cout << "Laporan berhasil diekspor ke data/reports/" << endl;
                } else {
                    cout << "Gagal mengekspor laporan." << endl;
                }
                pressEnter();
                break;
            case 7:
                break;
        }
    } while (pilihan != 7);
}
