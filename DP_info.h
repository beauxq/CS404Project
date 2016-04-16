#ifndef DP_INFO_H_INCLUDED
#define DP_INFO_H_INCLUDED

/** information on paths for dynamic programming */

#include <climits>
#include <vector>

struct DP_info
{
    /** infomation needed to find the paths */

    std::vector<unsigned long long int> min0_costs;
    // costs of shortest path to each node

    std::vector<unsigned long long int> min1_costs;
    // costs of second shortest path to each node

    size_t current_row;

    std::vector<std::vector<bool> > min0_paths;
    std::vector<std::vector<bool> > min1_paths;

    DP_info() : current_row(0) {}
    DP_info(const size_t& column_count) : min0_costs(column_count),
                                          min1_costs(column_count, ULLONG_MAX),
                                          current_row(0),
                                          min0_paths(column_count),
                                          min1_paths(column_count) {}
};

#endif // DP_INFO_H_INCLUDED
