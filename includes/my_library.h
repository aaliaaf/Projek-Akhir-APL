// MODUL 8: External Library dengan Include Guards (#ifndef/#define/#endif)
// MODUL 8: Deklarasi fungsi — implementasi di utils/my_library.cpp
// MODUL 6: Interpolation Search — O(log log n)
#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <string>
#include <vector>

namespace mylib {

    // ========== STRING UTILITIES ==========
    // MODUL 8: transform via <algorithm>
    std::string toLower(std::string s);

    // ========== SEARCHING ALGORITHMS ==========
    // MODUL 6: Interpolation Search
    // Kompleksitas: O(log log n) rata-rata, O(n) worst case
    int interpolationSearchInt(const std::vector<int>& arr, int target);
}

#endif
