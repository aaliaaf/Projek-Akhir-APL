// MODUL 1-3: Variabel, perulangan, fungsi, vector
// MODUL 5: Sorting menggunakan <algorithm> sort dengan lambda
// MODUL 8: <algorithm>, <map>, <vector>, <iomanip> setprecision

#include "../includes/helpers.h"
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

void laporanPendapatan(const vector<Transaksi>& histori) {
	header("LAPORAN PENDAPATAN");

	if (histori.empty()) {
		cout << "Belum ada data transaksi.\n";
		return;
	}

	double totalPendapatan = 0.0;
	double totalDenda = 0.0;
	int transaksiSelesai = 0;

	for (size_t i = 0; i < histori.size(); i++) {
		const bool selesai = !(histori[i].tglKembali.empty() || histori[i].tglKembali == "-");
		if (selesai) {
			transaksiSelesai++;
			totalPendapatan += histori[i].totalBayar;
			totalDenda += histori[i].denda;
		}
	}

	cout << "Jumlah transaksi selesai : " << transaksiSelesai << endl;
	cout << fixed << setprecision(0);
	cout << "Total pendapatan         : Rp " << totalPendapatan << endl;
	cout << "Total denda              : Rp " << totalDenda << endl;
	cout.unsetf(ios::floatfield);

	if (transaksiSelesai == 0) {
		cout << "Belum ada transaksi yang selesai dikembalikan.\n";
	}
}

void laporaniPhonePopuler(const vector<Transaksi>& histori) {
	header("LAPORAN IPHONE POPULER");

	if (histori.empty()) {
		cout << "Belum ada data transaksi.\n";
		return;
	}

	map<string, int> frekuensi;
	for (size_t i = 0; i < histori.size(); i++) {
		frekuensi[histori[i].iPhoneId]++;
	}

	vector<pair<string, int>> ranking(frekuensi.begin(), frekuensi.end());
	sort(ranking.begin(), ranking.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		if (a.second == b.second) return a.first < b.first;
		return a.second > b.second;
	});

	cout << "Peringkat iPhone berdasarkan frekuensi transaksi:\n";
	for (size_t i = 0; i < ranking.size(); i++) {
		cout << i + 1 << ". " << ranking[i].first << " | total disewa: " << ranking[i].second << " kali" << endl;
	}
}

void laporanUserAktif(const vector<Transaksi>& histori, const vector<User>& users) {
	header("LAPORAN USER AKTIF");

	if (histori.empty()) {
		cout << "Belum ada data transaksi.\n";
		return;
	}

	map<string, int> frekuensiSewa;
	for (size_t i = 0; i < histori.size(); i++) {
		frekuensiSewa[histori[i].userId]++;
	}

	vector<pair<string, int>> ranking(frekuensiSewa.begin(), frekuensiSewa.end());
	sort(ranking.begin(), ranking.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		if (a.second == b.second) return a.first < b.first;
		return a.second > b.second;
	});

	cout << "Peringkat user berdasarkan jumlah transaksi:\n";
	for (size_t i = 0; i < ranking.size(); i++) {
		string username = "(unknown)";
		string role = "-";

		for (size_t j = 0; j < users.size(); j++) {
			if (users[j].id == ranking[i].first) {
				username = users[j].username;
				role = users[j].role;
				break;
			}
		}

		cout << i + 1 << ". " << ranking[i].first
			 << " | " << username
			 << " | " << role
			 << " | total transaksi: " << ranking[i].second << endl;
	}
}

void laporanPendapatanPerBulan(const vector<Transaksi>& histori) {
	header("LAPORAN PENDAPATAN PER BULAN");

	map<string, double> pendapatanBulanan;
	map<string, int> transaksiBulanan;

	for (size_t i = 0; i < histori.size(); i++) {
		const bool selesai = !(histori[i].tglKembali.empty() || histori[i].tglKembali == "-");
		if (!selesai || histori[i].tglMulai.size() < 7) {
			continue;
		}

		string bulan = histori[i].tglMulai.substr(0, 7);
		pendapatanBulanan[bulan] += histori[i].totalBayar;
		transaksiBulanan[bulan] += 1;
	}

	if (pendapatanBulanan.empty()) {
		cout << "Belum ada transaksi selesai untuk dihitung per bulan.\n";
		return;
	}

	cout << fixed << setprecision(0);
	cout << "Bulan | Jumlah Transaksi | Total Pendapatan\n";
	for (map<string, double>::const_iterator it = pendapatanBulanan.begin(); it != pendapatanBulanan.end(); ++it) {
		cout << it->first << " | " << transaksiBulanan[it->first] << " | Rp " << it->second << endl;
	}
	cout.unsetf(ios::floatfield);
}

// MODUL 2: Array 2D — occupancy matrix (model × bulan)
void laporanOkupansiIphone(const vector<Transaksi>& histori, const vector<iPhone>& iphones) {
	header("LAPORAN OKUPANSI IPHONE");

	if (iphones.empty()) {
		cout << "Data iPhone kosong.\n";
		return;
	}

	// MODUL 2: Array 2D — baris = model iPhone, kolom = bulan (1..12)
	// occupancy[modelIdx][bulan] = total hari sewa di bulan itu
	const int MAX_MODEL = 50;
	const int BULAN_COUNT = 12;
	int occupancy[MAX_MODEL][BULAN_COUNT] = {0};
	int totalHariSewa[MAX_MODEL] = {0};

	int modelCount = min((int)iphones.size(), MAX_MODEL);

	for (size_t i = 0; i < histori.size(); i++) {
		int durasi = histori[i].durasi;
		if (durasi <= 0 && !histori[i].tglMulai.empty() && !histori[i].tglSelesai.empty()) {
			durasi = selisihHari(histori[i].tglMulai, histori[i].tglSelesai);
		}
		if (durasi <= 0) durasi = 1;

		for (int m = 0; m < modelCount; m++) {
			if (iphones[m].id == histori[i].iPhoneId) {
				int bulan = 0;
				if (histori[i].tglMulai.size() >= 7) {
					bulan = stoi(histori[i].tglMulai.substr(5, 2));
				}
				if (bulan >= 1 && bulan <= 12) {
					occupancy[m][bulan - 1] += durasi; // MODUL 2: akses array 2D
				}
				totalHariSewa[m] += durasi;
				break;
			}
		}
	}

	// Tabel ringkasan
	cout << "ID iPhone | Model         | Total Hari | Kontribusi\n";
	cout << fixed << setprecision(1);
	int grandTotal = 0;
	for (int m = 0; m < modelCount; m++) {
		grandTotal += totalHariSewa[m];
	}
	for (int m = 0; m < modelCount; m++) {
		double kontribusi = (grandTotal > 0) ? (100.0 * totalHariSewa[m] / grandTotal) : 0.0;
		cout << left << setw(10) << iphones[m].id << " | "
			 << setw(14) << iphones[m].model << " | "
			 << setw(10) << totalHariSewa[m] << " | "
			 << kontribusi << "%" << endl;
	}

	// MODUL 2: Tampilkan matrix 2D (model × bulan)
	cout << "\n--- Matriks Okupansi per Bulan (hari sewa) ---\n";
	cout << left << setw(14) << "Model";
	for (int b = 0; b < BULAN_COUNT; b++) {
		cout << " | " << setw(3) << left << ("B" + to_string(b + 1));
	}
	cout << "\n" << string(14 + BULAN_COUNT * 7, '-') << "\n";
	for (int m = 0; m < modelCount; m++) {
		cout << left << setw(14) << iphones[m].model;
		for (int b = 0; b < BULAN_COUNT; b++) {
			if (occupancy[m][b] > 0) {
				cout << " | " << setw(3) << left << occupancy[m][b];
			} else {
				cout << " | " << setw(3) << left << "-";
			}
		}
		cout << "\n";
	}
	cout.unsetf(ios::floatfield);
}

void laporanKonversiReservasi(const vector<Reservasi>& reservasi, const vector<Transaksi>& histori) {
	header("LAPORAN KONVERSI RESERVASI");

	const int totalReservasi = (int)reservasi.size();
	const int totalTransaksi = (int)histori.size();

	int reservasiAktif = 0;
	for (size_t i = 0; i < reservasi.size(); i++) {
		if (reservasi[i].isActive) reservasiAktif++;
	}

	const int reservasiSelesaiAtauBatal = totalReservasi - reservasiAktif;
	const int estimasiBatal = max(0, reservasiSelesaiAtauBatal - totalTransaksi);

	double conversionRate = 0.0;
	if (totalReservasi > 0) {
		conversionRate = 100.0 * totalTransaksi / totalReservasi;
	}

	cout << "Total reservasi                : " << totalReservasi << endl;
	cout << "Total transaksi tercatat       : " << totalTransaksi << endl;
	cout << "Reservasi aktif                : " << reservasiAktif << endl;
	cout << "Estimasi reservasi batal       : " << estimasiBatal << endl;
	cout << fixed << setprecision(2);
	cout << "Conversion rate (estimasi)     : " << conversionRate << "%" << endl;
	cout.unsetf(ios::floatfield);

	cout << "Catatan: conversion dihitung dari total transaksi dibanding total reservasi.\n";
}

void laporanTopUserBySpending(const vector<Transaksi>& histori, const vector<User>& users) {
	header("LAPORAN TOP USER BY SPENDING");

	map<string, double> totalBelanja;
	for (size_t i = 0; i < histori.size(); i++) {
		totalBelanja[histori[i].userId] += histori[i].totalBayar;
	}

	if (totalBelanja.empty()) {
		cout << "Belum ada data transaksi.\n";
		return;
	}

	vector<pair<string, double> > ranking(totalBelanja.begin(), totalBelanja.end());
	sort(ranking.begin(), ranking.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
		if (a.second == b.second) return a.first < b.first;
		return a.second > b.second;
	});

	cout << fixed << setprecision(0);
	for (size_t i = 0; i < ranking.size(); i++) {
		string username = "(unknown)";
		for (size_t j = 0; j < users.size(); j++) {
			if (users[j].id == ranking[i].first) {
				username = users[j].username;
				break;
			}
		}

		cout << i + 1 << ". " << ranking[i].first
			 << " | " << username
			 << " | total spending: Rp " << ranking[i].second << endl;
	}
	cout.unsetf(ios::floatfield);
}


