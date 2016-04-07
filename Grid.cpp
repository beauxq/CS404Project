#include "Grid.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

const unsigned int Grid::DEFAULT_VALUE = 1;  // default cost of each node

Grid::Grid(const std::string& filename)
{
    read_from_file(filename);
}

Grid::Grid(const size_t& _row_count, const size_t& _column_count)
{
    init(_row_count, _column_count);
}

/*
TODO: get rid of this if I don't need it
Grid::Grid(const Grid& other)
{
    // TODO: copy ctor
}

Grid& Grid::operator= (const Grid& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    // TODO: assignment operator
    return *this;
}
*/

bool Grid::read_from_file(const std::string& filename)
{
    /** read grid data from text file
        @returns success */

    std::ifstream file_input(filename);
    char comma_discard;
    size_t coord_row, coord_column, value;

    if (! (file_input >> row_count))
        return read_error();

    if (! (file_input >> comma_discard))
        return read_error();

    if (! (file_input >> column_count))
        return read_error();

    values.assign(row_count * column_count, DEFAULT_VALUE);

    while (file_input >> coord_row)
    {
        if (! (file_input >> comma_discard))
            return read_error();
        if (! (file_input >> coord_column))
            return read_error();
        if (! (file_input >> comma_discard))
            return read_error();
        if (! (file_input >> value))
            return read_error();

        if (coord_row < 1 ||
            coord_row > row_count ||
            coord_column < 1 ||
            coord_column > column_count)
        {
            return read_error();
        }

        values[calculate_index(coord_row, coord_column)] = value;
    }

    return true;
}

void Grid::init(const size_t& _row_count, const size_t& _column_count)
{
    /** make grid of specified size filled with value 1 */

    row_count = _row_count;
    column_count = _column_count;
    values.assign(row_count * column_count, DEFAULT_VALUE);
}

std::string Grid::str()
{
    /** string representation of grid */

    std::string to_return = std::to_string(row_count) + ' ' + std::to_string(column_count);

    auto value = values.begin();  // declare outside of loop because I'm going through it twice

    // find max for spacing columns
    unsigned int max = 0;
    for ( ; value != values.end(); ++value)
    {
        if (*value > max)
            max = *value;
    }
    size_t max_length = std::to_string(max).size();

    // reset iterator
    value = values.begin();

    std::string value_to_write;
    size_t space_to_fill;

    for (size_t row = row_count; row > 0; --row)
    {
        to_return += '\n';
        for (size_t column = column_count; column > 0; )
        {
            value_to_write = std::to_string(*value);
            space_to_fill = max_length - value_to_write.size();  // space to fill
            while (space_to_fill)
            {
                to_return += ' ';
                --space_to_fill;
            }
            to_return += value_to_write;

            --column;  // this is where the control variable is changed

            if (column != 0)
            {
                to_return += ' ';
            }

            ++value;  // vector iterator
        }
    }

    return to_return;
}

size_t Grid::calculate_index(const size_t& row, const size_t& column) const
{
    /** calculate the index in the value vector (0-based)
        from the row and column numbers (1-based) */

    return (row - 1) * column_count + column - 1;
}

bool Grid::read_error()
{
    /** set Grid to empty, output error, return false */

    row_count = 0;
    column_count = 0;
    values.clear();

    std::cerr << "invalid input file\n";
    return false;
}
