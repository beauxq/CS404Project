#include "Tests.h"

#include <iostream>

#include "Grid.h"

void Tests::test_grid_class()
{
    Grid grid;

    std::cout << grid.str() << std::endl;

    grid.init(7, 4);

    std::cout << grid.str() << std::endl;

    grid.read_from_file("lol");  // test error

    std::cout << grid.str() << std::endl;

    grid.read_from_file("resources/CS404SP16RewardMatrixInput1.txt");

    std::cout << grid.str() << std::endl;
}
