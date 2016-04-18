#include "Grid.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>  // priority queue
#include <climits>

#include "DSPA_path.h"
#include "DP_info.h"

const unsigned int Grid::DEFAULT_VALUE = 1;  // default cost of each node

Grid::Grid(const std::string& filename)
{
    read_from_file(filename);
}

Grid::Grid(const size_t& _row_count, const size_t& _column_count)
{
    init(_row_count, _column_count);
}

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

    clear_found_paths();

    return true;
}

void Grid::init(const size_t& _row_count, const size_t& _column_count)
{
    /** make grid of specified size filled with value 1 */

    row_count = _row_count;
    column_count = _column_count;
    values.assign(row_count * column_count, DEFAULT_VALUE);

    clear_found_paths();
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

    if (path.empty())
        return std::string();

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
    /** get two shortest paths - dynamic programming */

    if (values.size() < 4)
        return;  // there are not 2 paths

    DP_info dp_info(column_count);

    // node 1, 1  top left
    dp_info.min0_costs[0] = values[0];  // the minimum cost to get to 1, 1 is the cost of 1, 1

    // rest of top row
    for (size_t current_node = 1; current_node < column_count; ++current_node)
    {
        dp_info.min0_costs[current_node] = dp_info.min0_costs[current_node - 1] + values[current_node];

        dp_info.min0_paths[current_node] = dp_info.min0_paths[current_node - 1];
        dp_info.min0_paths[current_node].push_back(1);  // right
    }

    size_t complete_grid_index = column_count;

    // each row (after the top row)
    for (dp_info.current_row = 1; dp_info.current_row < row_count; ++dp_info.current_row)
    {
        // progress indicator
        if (! (dp_info.current_row % 200))
            std::cout << "row: " << dp_info.current_row << std::endl;

        for (size_t current_node = 0; current_node < column_count; ++current_node)
        {
            if (! (current_node))  // left column
            {
                dp_info.min0_costs[current_node] += values[complete_grid_index];

                dp_info.min0_paths[current_node].push_back(0);  // down
            }
            else  // neither top row, nor left column
            {
                two_mins_of_four(current_node, complete_grid_index, dp_info);
            }

            ++complete_grid_index;
        }
    }
    // done finding paths

    // store paths
    clear_found_paths();

    size_t last = column_count - 1;

    shortest_if_found = dp_info.min0_paths[last];
    second_shortest_if_found = dp_info.min1_paths[last];

    cost_of_shortest_if_found = dp_info.min0_costs[last];
    cost_of_second_shortest_if_found = dp_info.min1_costs[last];
}

void Grid::dspa_find_min_paths()
{
    /** get two shortest paths - dijkstra modification */

    if (values.size() < 4)
        return;  // there are not 2 paths

    const int8_t TARGET_PATH_COUNT = 2;
    const size_t LAST_NODE = values.size() - 1;

    std::vector<DSPA_path> final_paths;  // this will hold the to paths that we find
    std::vector<int8_t> path_counts(values.size(), 0);  // how many paths found for each node
    std::priority_queue<DSPA_path, std::vector<DSPA_path>, comparator> path_min_heap;  // all of the paths we find
    DSPA_path working_path, new_path;

    // the first path is the first node alone
    new_path.cost = values[0];
    new_path.end_node = 0;
    path_min_heap.push(new_path);

    // for progress indicator
    unsigned long long int size_shown = 0;

    while ((!path_min_heap.empty()) && path_counts[LAST_NODE] < TARGET_PATH_COUNT)
    {
        working_path = path_min_heap.top();
        path_min_heap.pop();

        // progress indicator
        if ((! (working_path.cost % 400)) && working_path.cost > size_shown)
        {
            std::cout << "took path from heap with cost: " << working_path.cost << std::endl;
            size_shown = working_path.cost;
        }

        ++path_counts[working_path.end_node];

        if (working_path.end_node == LAST_NODE)
            final_paths.push_back(working_path);  // this is a path for the final result

        if (path_counts[working_path.end_node] <= TARGET_PATH_COUNT)
        {
            // if not the right-most column
            if ((working_path.end_node + 1) % column_count)
            {
                new_path.end_node = working_path.end_node + 1;  // the node to the right of the end of the path
                new_path.cost = working_path.cost + values[new_path.end_node];
                new_path.path = working_path.path;
                new_path.path.push_back(1);  // path to the right

                path_min_heap.push(new_path);
            }
            // if not the bottom row
            if (working_path.end_node < (values.size() - column_count))
            {
                new_path.end_node = working_path.end_node + column_count;  // the node below the end of the path
                new_path.cost = working_path.cost + values[new_path.end_node];
                new_path.path = working_path.path;
                new_path.path.push_back(0);  // path down

                path_min_heap.push(new_path);
            }

        }
    }

    if (final_paths.size() < TARGET_PATH_COUNT)
        return;  // didn't find enough paths (grid either m by 1 or 1 by n)

    shortest_if_found = final_paths[0].path;
    second_shortest_if_found = final_paths[1].path;

    cost_of_shortest_if_found = final_paths[0].cost;
    cost_of_second_shortest_if_found = final_paths[1].cost;
}

void Grid::clear_found_paths()
{
    /** empty shortest paths and costs ULLONG_MAX */

    shortest_if_found.clear();
    second_shortest_if_found.clear();
    cost_of_shortest_if_found = ULLONG_MAX;
    cost_of_second_shortest_if_found = ULLONG_MAX;
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

    init(0, 0);

    std::cerr << "invalid input file\n";
    return false;
}

void Grid::two_mins_of_four(const size_t& current_node,
                            const size_t& complete_grid_index,
                            DP_info& dp_info)
{
    /** primary per-node comparison and choosing of dynamic programming algorithm */

    size_t left = current_node - 1;

    if (dp_info.min0_costs[current_node] < dp_info.min0_costs[left])
    {
        // min0 comes from min0 above
        dp_info.min0_costs[current_node] += values[complete_grid_index];
        dp_info.min0_paths[current_node].push_back(0);  // down

        if (dp_info.min1_costs[current_node] < dp_info.min0_costs[left])
        {
            // min1 comes from min1 above
            dp_info.min1_costs[current_node] += values[complete_grid_index];
            dp_info.min1_paths[current_node].push_back(0);  // down
        }
        else if (dp_info.min1_costs[current_node] > dp_info.min0_costs[left])
        {
            // min1 comes from min0 left
            dp_info.min1_costs[current_node] = dp_info.min0_costs[left] + values[complete_grid_index];
            dp_info.min1_paths[current_node] = dp_info.min0_paths[left];
            dp_info.min1_paths[current_node].push_back(1);  // right
        }
        else  // costs ==, and min0 comes from above
        {
            if (path_less(dp_info.min1_paths[current_node], dp_info.min0_paths[left]))
            {
                // min1 comes from min1 above
                dp_info.min1_costs[current_node] += values[complete_grid_index];
                dp_info.min1_paths[current_node].push_back(0);  // down
            }
            else  // min1 above path > min0 left path
            {
                // min1 comes from min0 left
                dp_info.min1_costs[current_node] = dp_info.min1_costs[left] + values[complete_grid_index];
                dp_info.min1_paths[current_node] = dp_info.min0_paths[left];
                dp_info.min1_paths[current_node].push_back(1);  // right
            }
        }
    }
    // prefer left when == because that means above came earlier,
    // in more significant digit when path is interpreted as a number
    else  // above min0 >= left min0
    {
        // handle second shortest path first because handling shortest first would overwrite data we need (from row above)
        if (dp_info.min0_costs[current_node] < dp_info.min1_costs[left])
        {
            // min1 comes from min0 above
            dp_info.min1_costs[current_node] = dp_info.min0_costs[current_node] + values[complete_grid_index];
            dp_info.min1_paths[current_node] = dp_info.min0_paths[current_node];
            dp_info.min1_paths[current_node].push_back(0);  // down
        }
        else if (dp_info.min0_costs[current_node] > dp_info.min1_costs[left])
        {
            // min1 comes from min1 left
            dp_info.min1_costs[current_node] = dp_info.min1_costs[left] + values[complete_grid_index];
            dp_info.min1_paths[current_node] = dp_info.min1_paths[left];
            dp_info.min1_paths[current_node].push_back(1);  // right
        }
        else  // costs ==, and min0 comes from left
        {
            if (path_less(dp_info.min0_paths[current_node], dp_info.min1_paths[left]))
            {
                // min1 comes from min0 above
                dp_info.min1_costs[current_node] = dp_info.min0_costs[current_node] + values[complete_grid_index];
                dp_info.min1_paths[current_node] = dp_info.min0_paths[current_node];
                dp_info.min1_paths[current_node].push_back(0);  // down
            }
            else  // min0 above path > min1 left path
            {
                // min1 comes from min1 left
                dp_info.min1_costs[current_node] = dp_info.min1_costs[left] + values[complete_grid_index];
                dp_info.min1_paths[current_node] = dp_info.min1_paths[left];
                dp_info.min1_paths[current_node].push_back(1);  // right
            }
        }
        // min0 comes from min0 left
        dp_info.min0_costs[current_node] = dp_info.min0_costs[left] + values[complete_grid_index];
        dp_info.min0_paths[current_node] = dp_info.min0_paths[left];
        dp_info.min0_paths[current_node].push_back(1);  // right
    }
}
