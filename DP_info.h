#ifndef DP_INFO_H_INCLUDED
#define DP_INFO_H_INCLUDED

/** information on paths for dynamic programming */

#include <climits>
#include <vector>
#include <utility>

struct Path
{
    /** what I need to create the paths once they are found */

    bool min0_came_from;
    // direction, 0 from above, 1 from left

    bool min1_came_from_min;
    // 0  came from the shortest, 1 came from the 2nd shortest

    bool min1_came_from;
    // direction, 0 from above, 1 from left
};

struct DP_info
{
    /** infomation not needed to contruct the paths after found,
        but needed to find the paths */

    unsigned long long int min0_cost;
    // cost of shortest path to this node

    unsigned long long int min1_cost;
    // cost of second shortest path to this node

    bool in_up_indicating_pattern;
    // min2 came from min2 some number of times in the same direction
    // after coming from min2 in the perpendicular diretion
    // this indicates that up should be taken as min2_came_from
    // if the two choices are equal

    Path path;  // most recent step in the path

    DP_info() : min0_cost(ULLONG_MAX), min1_cost(ULLONG_MAX) {}
    DP_info(unsigned long long int min0) :  min0_cost(min0), min1_cost(ULLONG_MAX) {}
};

#endif // DP_INFO_H_INCLUDED
