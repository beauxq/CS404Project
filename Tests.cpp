#include "Tests.h"

#include <iostream>

#include "Grid.h"

void Tests::test_grid_class()
{
    Grid grid;

    if (grid.str() != "0 0")
        std::cout << "FAIL: default constructed grid and str\n";

    grid.init(7, 4);

    if (grid.str() != "7 4\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1")
        std::cout << "FAIL: grid.init(7, 4) and str\n";

    Grid grid2(7, 4);

    if (grid2.str() != "7 4\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1\n1 1 1 1")
        std::cout << "FAIL: Grid grid2(7, 4) and str\n";

    std::cout << "test non-existent file:\n";
    grid.read_from_file("lol");
    if (grid.str() != "0 0")
        std::cout << "FAIL: bad file read_from_file and str\n";

    std::cout << "\nread_from_file: input file 1\n";
    grid.read_from_file("resources/CS404SP16RewardMatrixInput1.txt");
    std::cout << grid.str() << std::endl;

    std::cout << "\nfilename constructor: input file 2:\n";
    Grid grid3("resources/CS404SP16RewardMatrixInput2.txt");
    std::cout << grid3.str() << std::endl;
}

void Tests::test_dp()
{
    Grid grid(7, 4);

    grid.dp_find_min_paths();

    std::cout << "did something\n";
}
