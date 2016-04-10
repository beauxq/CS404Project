#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include "DP_info.h"

class Grid
{
public:
    Grid() : row_count(0), column_count(0) {};
    Grid(const std::string& filename);
    Grid(const size_t& _row_count, const size_t& _column_count);
    /* TODO: get rid of copy constructor and assignment operator?
    Grid(const Grid& other);
    Grid& operator= (const Grid& other); */

    bool read_from_file(const std::string& filename);
    /** @returns success */

    void init(const size_t& _row_count, const size_t& _column_count);
    /** make grid of specified size filled with value 1 */

    std::string str(const std::vector<bool>& path = std::vector<bool>());
    /** string representation of grid
        path to display found path */

    void dp_find_min_paths();
    /** get two shortest paths - dynamic programming */

    // getters
    const std::vector<bool>& get_shortest_if_found() const { return shortest_if_found; };
    const std::vector<bool>& get_second_shortest_if_found() const { return second_shortest_if_found; };

    static const unsigned int DEFAULT_VALUE;

protected:

private:
    size_t calculate_index(const size_t& row, const size_t& column) const;
    /** calculate the index in the value vector (0-based)
        from the row and column numbers (1-based) */

    bool read_error();
    /** set Grid to empty, output error, return false */

    void two_mins_of_four(const size_t& current_node);
    /** primary per-node comparison and choosing of dynamic programming algorithm */

    std::vector<unsigned int> values;  // the cost of using each node
    std::vector<DP_info> dp_infos;
    size_t row_count;
    size_t column_count;

    std::vector<bool> shortest_if_found;
    std::vector<bool> second_shortest_if_found;
};

#endif // GRID_H
