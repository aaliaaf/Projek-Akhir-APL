#ifndef IPHONE_H // [MODUL 8] Include Guards
#define IPHONE_H

#include <string>
using namespace std;

#define MAX_IPHONES 100 // [MODUL 8] Macro

// [MODUL 2] Struct: deklarasi struct (tipe data abstrak)
struct iPhone {
    // [MODUL 2] Field struct
    string id;
    string name;
    string storage;
    float rentPrice; // [MODUL 1] Tipe Data Primitif: float
    string status;
    int totalRented; // [MODUL 1] Tipe Data Primitif: int
    // [MODUL 10] Kategori: warna, tahun rilis, kondisi
    string color;
    int year;
    string condition;
};

// [MODUL 2] Array of Struct: deklarasi global
extern iPhone daftarIPhone[MAX_IPHONES];
extern int jumlahIPhone;

// [MODUL 3] Prosedur (void)
void tambahIPhone();
void editIPhone();
void hapusIPhone();
void sortIPhonesByName();
void selectionSortByStorage();            // [MODUL 5] Selection Sort by storage
void quickSortIPhone(int low, int high); // [MODUL 5] Quick Sort (divide & conquer) by year
int partitionIPhone(int low, int high);  // [MODUL 5] Partisi untuk Quick Sort
void mergeSortIPhone(int left, int right); // [MODUL 5] Merge Sort (divide & conquer, stable)
void mergeIPhone(int left, int mid, int right); // [MODUL 5] Merge untuk Merge Sort
void interpolationSearchIPhone();        // [MODUL 6] Interpolation Search
void jumpSearchIPhone();                 // [MODUL 6] Jump Search O(√n)
void binarySearchIPhone();               // [MODUL 6] Binary Search O(log n)
void displayAllIPhones();
void searchIPhone();
void displayAvailableIPhones();
int findiPhoneByID(const string& id);   // [MODUL 3] Fungsi dengan return
void loadIPhones();
void saveIPhones();

#endif
