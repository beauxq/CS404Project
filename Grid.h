#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

class Grid
{
public:
    Grid() : row_count(0), column_count(0) {};
    Grid(const std::string& filename);
    Grid(const size_t& _row_count, const size_t& _column_count);
    /*
    Grid(const Grid& other);
    Grid& operator= (const Grid& other); */

    bool read_from_file(const std::string& filename);
    /** @returns success */

    void init(const size_t& _row_count, const size_t& _column_count);
    /** make grid of specified size filled with value 1 */

    std::string str();
    /** string representation of grid */

    static const unsigned int DEFAULT_VALUE;

protected:

private:
    size_t calculate_index(const size_t& row, const size_t& column) const;
    /** calculate the index in the value vector (0-based)
        from the row and column numbers (1-based) */

    bool read_error();
    /** set Grid to empty, output error, return false */

    std::vector<unsigned int> values;  // the cost of using each node
    size_t row_count;
    size_t column_count;
};

#endif // GRID_H
