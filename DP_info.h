#ifndef DP_INFO_H_INCLUDED
#define DP_INFO_H_INCLUDED

/** information on paths for dynamic programming */

#include <climits>
#include <vector>
#include <utility>

struct Path
{
    uint8_t min1_came_from_min;
    // 0 null, 1 came from the shortest, 2 came from the 2nd shortest

    bool min1_came_from;
    // direction, 0 from above, 1 from left

    uint8_t min2_came_from_min;
    bool min2_came_from;
};

struct DP_info
{
    unsigned long long int min1_cost;
    unsigned long long int min2_cost;
    /*
    std::vector<bool> path;
    // note: normally a bool is 8 bits (or possibly more),
    // but the c++ standard library vector<bool> is optimized for storage, one bit per value

    // TODO: this? or just store the most recent step in the path?
    */

    Path path;  // most recent step in the path

    DP_info() : min1_cost(ULLONG_MAX), min2_cost(ULLONG_MAX) {}
    DP_info(unsigned long long int min1) :  min1_cost(min1), min2_cost(ULLONG_MAX) {}
};

#endif // DP_INFO_H_INCLUDED
