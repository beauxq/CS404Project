#include "Grid.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>  // pair

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

std::string Grid::visual_str(const std::vector<bool>& path /* = empty */)
{
    /** visual representation of grid
        path to display found path */

    size_t next_index_on_path = path.empty() ? values.size() : 0;  // no path is drawn if the path is empty
    auto path_itr = path.begin();

    std::string to_return = std::to_string(row_count) + ' ' + std::to_string(column_count);

    size_t value_index = 0;  // declare outside of loop because I'm going through it twice

    // find max for spacing columns
    unsigned int max = 0;
    for ( ; value_index < values.size(); ++value_index)
    {
        if (values[value_index] > max)
            max = values[value_index];
    }
    size_t max_length = std::to_string(max).size();

    // reset value to beginning
    value_index = 0;

    std::string value_to_write;
    size_t space_to_fill;

    for (size_t row = row_count; row > 0; --row)
    {
        to_return += '\n';
        for (size_t column = column_count; column > 0; )
        {
            value_to_write = std::to_string(values[value_index]);
            space_to_fill = max_length - value_to_write.size();  // space to fill
            while (space_to_fill)
            {
                to_return += ' ';
                --space_to_fill;
            }
            to_return += value_to_write;

            // draw path
            if (value_index == next_index_on_path)
            {
                to_return += '-';
                next_index_on_path += *path_itr ? 1 : column_count;  // find next node on path
                ++path_itr;
            }
            else  // not on path
            {
                to_return += ' ';
            }

            --column;  // this is where the control variable is changed

            if (column)  // if there are more columns in this row
            {
                to_return += ' ';
            }

            ++value_index;  // vector iterator
        }
    }

    return to_return;
}

std::string Grid::data_str(const bool& omit_ones /* = true */ )
{
    /** output data as given in input files */

    size_t row = 1;
    size_t column = 1;

    std::string to_return = std::to_string(row_count) + ", " + std::to_string(column_count) + '\n';

    for (auto value = values.begin(); value != values.end(); ++value)
    {
        if (! (*value == 1 && omit_ones))
            to_return += std::to_string(row) + ", " + std::to_string(column) + ", " + std::to_string(*value) + '\n';

        ++column;
        if (column > column_count)
        {
            column = 1;
            ++row;
        }
    }

    return to_return;
}

std::string Grid::path_str(const std::vector<bool>& path)
{
    /** zeros and ones to represent path */

    std::string to_return = std::to_string(path[0]);

    auto step = path.begin();
    ++step;

    for ( ; step != path.end(); ++step)
    {
        to_return += ' ' + std::to_string(*step);
    }

    return to_return;
}

void Grid::dp_find_min_paths()
{
    /** get two shortest paths - dynamic programming
        @returns paths backwards (end to start, m,n to 1,1)
                 first = shortest, second = second shortest */

    dp_infos.clear();  // make sure there's no info already in the table

    // node 1, 1  top left
    dp_infos.push_back(DP_info(values[0]));  // the minimum cost to get to 1, 1 is the cost of 1, 1
    // dp_infos[0].path.min1_came_from_min = 0;  // null, beginning of path  // TODO: get rid of min1 came from min (always 1)
    dp_infos[0].path.min2_came_from_min = 0;  // null, beginning of path

    // every node, left to right, top to bottom
    for (size_t current_node = 1; current_node < values.size(); ++current_node)
    {
        // top row
        if (current_node < column_count)
        {
            dp_infos.push_back(DP_info(values[current_node] + dp_infos[current_node - 1].min1_cost));

            dp_infos[current_node].path.min1_came_from = 1;  // came from left
            // dp_infos[current_node].path.min1_came_from_min = 1;  // came from shortest
            dp_infos[current_node].path.min2_came_from_min = 0;  // null, there is no 2nd shortest path to anything on the top row
        }
        // left column
        else if (! (current_node % column_count))
        {
            dp_infos.push_back(DP_info(values[current_node] + dp_infos[current_node - column_count].min1_cost));

            dp_infos[current_node].path.min1_came_from = 0;  // came from above
            // dp_infos[current_node].path.min1_came_from_min = 1;  // came from shortest
            dp_infos[current_node].path.min2_came_from_min = 0;  // null, there is no 2nd shortest path to anything in left column
        }
        else  // neither top row, nor left column
        {
            dp_infos.push_back(DP_info());

            two_mins_of_four(current_node);
        }
    }
    // done finding paths

    // build paths (in reverse)
    std::vector<bool> first_in_reverse, second_in_reverse;
    size_t first_index = values.size() - 1;
    size_t second_index = first_index;
    bool second_path_for_second = true;  // whether we want the shortest path
                                         // to where we currently are in the second shortest path,
                                         // or the second shortest
    for (size_t i = row_count + column_count - 2; i > 0; --i)
    {
        // shortest path
        first_in_reverse.push_back(dp_infos[first_index].path.min1_came_from);
        if (dp_infos[first_index].path.min1_came_from)  // if came from left
            --first_index;
        else  // came from above
            first_index -= column_count;

        // second shortest path
        if (second_path_for_second)
        {
            second_in_reverse.push_back(dp_infos[second_index].path.min2_came_from);
            if (dp_infos[second_index].path.min2_came_from_min == 1)  // check to see if we need to switch to the shortest path
                second_path_for_second = false;
            if (dp_infos[second_index].path.min2_came_from)  // if came from left
                --second_index;
            else  // came from above
                second_index -= column_count;
        }
        else  // second shortest path is shortest path from here to beginning
        {
            second_in_reverse.push_back(dp_infos[second_index].path.min1_came_from);
            if (dp_infos[second_index].path.min1_came_from)  // if came from left
                --second_index;
            else  // came from above
                second_index -= column_count;
        }
    }

    // reverse orders
    shortest_if_found.clear();
    second_shortest_if_found.clear();
    for (int i = first_in_reverse.size() - 1; i >= 0; --i)
    {
        shortest_if_found.push_back(first_in_reverse[i]);
        second_shortest_if_found.push_back(second_in_reverse[i]);
    }
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

void Grid::two_mins_of_four(const size_t& current_node)
{
    /** primary per-node comparison and choosing of dynamic programming algorithm */

    size_t above = current_node - column_count;
    size_t left = current_node - 1;

    if (dp_infos[above].min1_cost < dp_infos[left].min1_cost)
    {
        // min1 comes from min1 above
        dp_infos[current_node].min1_cost = dp_infos[above].min1_cost + values[current_node];

        // dp_infos[current_node].path.min1_came_from_min = 1;  // min1  // TODO: get rid of this (always 1)
        dp_infos[current_node].path.min1_came_from = 0;  // above

        if (dp_infos[above].min2_cost < dp_infos[left].min1_cost)
        {
            // min2 comes from min2 above
            dp_infos[current_node].min2_cost = dp_infos[above].min2_cost + values[current_node];

            dp_infos[current_node].path.min2_came_from_min = 2;  // min2
            dp_infos[current_node].path.min2_came_from = 0;  // above
        }
        else  // above min2 >= left min1
        {
            // min2 comes from min1 left
            dp_infos[current_node].min2_cost = dp_infos[left].min1_cost + values[current_node];

            dp_infos[current_node].path.min2_came_from_min = 1;  // min1
            dp_infos[current_node].path.min2_came_from = 1;  // left
        }
    }
    // prefer left when == because that means above came earlier,
    // in more significant digit when path is interpreted as a number
    else  // above min1 >= left min1
    {
        // min1 comes from min1 left
        dp_infos[current_node].min1_cost = dp_infos[left].min1_cost + values[current_node];

        // dp_infos[current_node].path.min1_came_from_min = 1;  // min1  // TODO: get rid of this (always 1)
        dp_infos[current_node].path.min1_came_from = 1;  // left

        if (dp_infos[above].min1_cost < dp_infos[left].min2_cost)
        {
            // min2 comes from min1 above
            dp_infos[current_node].min2_cost = dp_infos[above].min1_cost + values[current_node];

            dp_infos[current_node].path.min2_came_from_min = 1;  // min1
            dp_infos[current_node].path.min2_came_from = 0;  // above
        }
        else  // above min1 >= left min2
        {
            // min2 comes from min2 left
            dp_infos[current_node].min2_cost = dp_infos[left].min2_cost + values[current_node];

            dp_infos[current_node].path.min2_came_from_min = 2;  // min2
            dp_infos[current_node].path.min2_came_from = 1;  // left
        }
    }
}
