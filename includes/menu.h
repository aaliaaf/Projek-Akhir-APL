#ifndef MENU_H // [MODUL 8] Include Guards
#define MENU_H

#include <string>
using namespace std;

void showMainMenu();
void showGuestMenu();
void showCustomerMenu(const string& userID);
void showAdminMenu();

void customerLihatIPhone(const string& userID);
void customerCariIPhone(const string& userID);
void customerSewa(const string& userID);
void customerReservasi(const string& userID);
void customerRiwayat(const string& userID);
void customerStatusReservasi(const string& userID);

void adminManageIPhones();
void adminManageUsers();
void adminManageRentals();
void adminManageReservations();
void adminViewReports();

#endif
