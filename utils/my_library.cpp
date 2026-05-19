// MODUL 8: Implementasi External Library (my_library.h)
// MODUL 8: <algorithm> transform, <cctype> tolower
// MODUL 6: Interpolation Search — O(log log n)
// MODUL 5: Sorting via <algorithm> (digunakan oleh pemanggil)

#include "../includes/my_library.h"
#include <algorithm>
#include <cctype>

namespace mylib {

    std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    int interpolationSearchInt(const std::vector<int>& arr, int target) {
        int low = 0, high = (int)arr.size() - 1;
        while (low <= high && target >= arr[low] && target <= arr[high]) {
            if (low == high) {
                if (arr[low] == target) return low;
                return -1;
            }
            if (arr[high] == arr[low]) break;
            int pos = low + ((double)(high - low) / (arr[high] - arr[low])) * (target - arr[low]);
            if (arr[pos] == target) return pos;
            if (arr[pos] < target) low = pos + 1;
            else high = pos - 1;
        }
        return -1;
    }
}
