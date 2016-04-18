#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>
#include <climits>

#include "DP_info.h"

class Grid
{
public:
    Grid() : row_count(0),
             column_count(0),
             cost_of_shortest_if_found(ULLONG_MAX),
             cost_of_second_shortest_if_found(ULLONG_MAX) {};
    Grid(const std::string& filename);
    Grid(const size_t& _row_count, const size_t& _column_count);

    bool read_from_file(const std::string& filename);
    /** @returns success */

    void init(const size_t& _row_count, const size_t& _column_count);
    /** make grid of specified size filled with value 1 */

    std::string visual_str(const std::vector<bool>& path = std::vector<bool>());
    /** visual representation of grid
        path to display found path */

    std::string data_str(const bool& omit_ones = true);
    /** output data as given in input files */

    std::string path_str(const std::vector<bool>& path);
    /** zeros and ones to represent path */

    void dp_find_min_paths();
    /** get two shortest paths - dynamic programming */

    void dspa_find_min_paths();
    /** get two shortest paths - dijkstra modification */

    void clear_found_paths();
    /** empty shortest paths and costs ULLONG_MAX */

    // getters
    const std::vector<bool>& get_shortest_if_found() const { return shortest_if_found; }
    const std::vector<bool>& get_second_shortest_if_found() const { return second_shortest_if_found; }
    const unsigned long long int& get_cost_of_shortest_if_found() const { return cost_of_shortest_if_found; }
    const unsigned long long int& get_cost_of_second_shortest_if_found() const { return cost_of_second_shortest_if_found; }
    const size_t& get_row_count() const { return row_count; }
    const size_t& get_column_count() const { return column_count; }

    static const unsigned int DEFAULT_VALUE;  // 1

protected:

private:
    size_t calculate_index(const size_t& row, const size_t& column) const;
    /** calculate the index in the value vector (0-based)
        from the row and column numbers (1-based) */

    bool read_error();
    /** set Grid to empty, output error, return false */

    void two_mins_of_four(const size_t& current_node,
                          const size_t& complete_grid_index,
                          DP_info& dp_infos);
    /** primary per-node comparison and choosing of dynamic programming algorithm */

    std::vector<unsigned int> values;  // the cost of using each node
    size_t row_count;
    size_t column_count;

    std::vector<bool> shortest_if_found;
    std::vector<bool> second_shortest_if_found;
    unsigned long long int cost_of_shortest_if_found;
    unsigned long long int cost_of_second_shortest_if_found;
};

#endif // GRID_H
