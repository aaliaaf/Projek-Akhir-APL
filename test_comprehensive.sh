#!/bin/bash
# ============================================================
# TEST KOMPREHENSIF — Sistem Penyewaan iPhone
# Menjalankan semua fitur secara otomatis via piped input
# ============================================================

set -o pipefail

APP="./bin/app"
PASS=0
FAIL=0

green='\033[0;32m'
red='\033[0;31m'
cyan='\033[0;36m'
nc='\033[0m'

cleanup() {
    rm -f /tmp/test_*.in /tmp/test_*.out /tmp/test_*.txt
}
trap cleanup EXIT

# ============================================================
# HELPER: run test with input file, check for expected string
# ============================================================
run_test() {
    local name="$1"
    local input_file="$2"
    local expect="$3"

    echo -e "\n${cyan}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${nc}"
    echo -e "${cyan}  TEST: $name${nc}"
    echo -e "${cyan}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${nc}"

    # Restore clean data before each test
    cp data/iphones.csv.orig data/iphones.csv 2>/dev/null || true
    cp data/users.csv.orig data/users.csv 2>/dev/null || true
    cp data/reservations.csv.orig data/reservations.csv 2>/dev/null || true
    cp data/transactions.csv.orig data/transactions.csv 2>/dev/null || true

    output=$($APP < "$input_file" 2>&1)
    local ret=$?
    echo "$output" | head -80

    if echo "$output" | grep -q "$expect"; then
        echo -e "${green}  ✓ PASS: Ditemukan '$expect'${nc}"
        PASS=$((PASS + 1))
        return 0
    else
        echo -e "${red}  ✗ FAIL: Tidak ditemukan '$expect'${nc}"
        echo "--- Output lengkap ---"
        echo "$output"
        FAIL=$((FAIL + 1))
        return 1
    fi
}

# ============================================================
# Backup original data
# ============================================================
echo "Backing up original data..."
for f in iphones users reservations transactions; do
    cp "data/$f.csv" "data/$f.csv.orig"
done

# ============================================================
# TEST 1: Admin — Login, Manajemen iPhone, Manajemen User
# ============================================================
cat > /tmp/test_admin_1.in << 'IN1'
1
admin
admin
1
1

2
IP001

3
iPhone 14

7
2
1

6
3
0

7
3
IN1

run_test "Admin: Login + iPhone CRUD + User List" /tmp/test_admin_1.in "Ditemukan"

# ============================================================
# TEST 2: Admin — Laporan Lengkap (7 submenu)
# ============================================================
cat > /tmp/test_admin_2.in << 'IN2'
1
admin
admin
6
1

2

3

4

5

6

7

8
7
3
IN2

run_test "Admin: Laporan Lengkap (7 jenis)" /tmp/test_admin_2.in "Pendapatan"

# ============================================================
# TEST 3: Admin — Check-In + Proses Antrian + Reservasi
# ============================================================
cat > /tmp/test_admin_3.in << 'IN3'
1
admin
admin
4
1

5

3
1

7
3
IN3

run_test "Admin: CheckIn + Antrian + Reservasi" /tmp/test_admin_3.in "Pengembalian berhasil"

# ============================================================
# TEST 4: Customer — Login + Pencarian + Reservasi Baru
# ============================================================
cat > /tmp/test_cust_1.in << 'IN4'
1
zidan
021
1

2
IP001

3
iPhone 14

4
IP003
2026-06-20
2026-06-22

10
3
IN4

run_test "Customer: Login + Search + Reservasi Baru" /tmp/test_cust_1.in "Reservasi berhasil dibuat"

# ============================================================
# TEST 5: Customer — Posisi Antrian (Interpolation Search)
# ============================================================
# Login as zidan (VIP, has reservation)
cat > /tmp/test_cust_2.in << 'IN5'
1
zidan
021
5

10
3
IN5

run_test "Customer: Posisi Antrian (Interpolation Search)" /tmp/test_cust_2.in "Posisi antrian: ke-"

# ============================================================
# TEST 6: Customer — Riwayat Transaksi + Profil
# ============================================================
cat > /tmp/test_cust_3.in << 'IN6'
1
zidan
021
8

9

10
3
IN6

run_test "Customer: Riwayat + Profil" /tmp/test_cust_3.in "RIWAYAT"

# ============================================================
# TEST 7: Register new user + langsung login
# ============================================================
cat > /tmp/test_register.in << 'IN7'
2
newuser
newpass

1
newuser
newpass
9

10
3
IN7

run_test "Register + Login user baru" /tmp/test_register.in "Profil"

# ============================================================
# SIMULATION: Full admin flow — all menu options in sequence
# ============================================================
cat > /tmp/test_admin_full.in << 'IN8'
1
admin
admin
1
1

2
IP001

3
iPhone 12

4
TestModel
75000
Baru

5
IPH1

80000

6
IPH1

7
2
1

2
U002

3
newuser2
pass2
Customer
1

6
3
0

4
1

5

6
1

2

3

4

5

6

7

8
7
3
IN8

run_test "Admin: FULL Flow (all submenu)" /tmp/test_admin_full.in "Terima kasih"

# ============================================================
# SIMULATION: Full customer flow — all menu options
# ============================================================
cat > /tmp/test_cust_full.in << 'IN9'
1
zidan
021
1

2
IP001

3
iPhone 12

5

7
1

8

9

10
3
IN9

run_test "Customer: FULL Flow" /tmp/test_cust_full.in "Terima kasih"

# ============================================================
# SUMMARY
# ============================================================
echo ""
echo -e "${cyan}============================================${nc}"
echo -e "${cyan}  HASIL TEST${nc}"
echo -e "${cyan}============================================${nc}"
echo -e "${green}  PASS: $PASS${nc}"
echo -e "${red}  FAIL: $FAIL${nc}"
echo -e "${cyan}============================================${nc}"

# Restore originals
for f in iphones users reservations transactions; do
    cp "data/$f.csv.orig" "data/$f.csv"
    rm -f "data/$f.csv.orig"
done

exit $FAIL
