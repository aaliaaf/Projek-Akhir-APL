// [MODUL 1] Entry point program
// [MODUL 3] Fungsi main dengan return value (int)
// [MODUL 3] Prosedur: pemanggilan fungsi void
// [MODUL 8] Include user-defined headers (user.h, iphone.h, rental.h, reservation.h, menu.h)

#include "user.h"
#include "iphone.h"
#include "rental.h"
#include "reservation.h"
#include "menu.h"
#include <iostream>
using namespace std;

// [MODUL 3] Fungsi main: return int
int main() {
    // Inisialisasi data dari file
    loadIPhones();
    loadUsers();
    loadRentals();
    loadReservations();

    // Tampilkan menu utama
    showMainMenu();

    // [MODUL 3] return 0 menandakan program berakhir sukses
    return 0;
}

//..
