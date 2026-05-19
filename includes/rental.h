#ifndef RENTAL_H // [MODUL 8] Include Guards
#define RENTAL_H

#include <string>
using namespace std;

#define MAX_RENTALS 200

struct Rental {
    string rentalID;
    string userID;
    string iphoneID;
    string startDate;
    int duration;
    float totalPrice;
    string status;
    float lateFee;
};

extern Rental daftarRental[MAX_RENTALS];
extern int jumlahRental;

void sewaIPhone(const string& userID);
void returnIPhone();
void displayAllRentals();
void displayRentalsByStatus(const string& status);
void searchRentalsByUser();
void displayUserRentals(const string& userID);
void generateReport();
void generateReportOverview();
void generateReportMonthly();
void generateReportByPhone();
void generateReportByUser();
bool exportFullReport();
void loadRentals();
void saveRentals();

#endif
