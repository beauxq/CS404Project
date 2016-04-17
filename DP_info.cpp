#include "DP_info.h"

#include <vector>

bool path_less(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    /** c++ built in operator< for vectors is slower */

    size_t index = 0;
    size_t min_size = lhs.size() < rhs.size() ? lhs.size() : rhs.size();
    while (index < min_size)
    {
        if (lhs[index] < rhs[index])
            return true;
        if (rhs[index] < lhs[index])
            return false;
        // if ==, continue
        ++index;
    }
    return false;
}
