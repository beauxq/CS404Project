#include <iostream>

#include "Tests.h"

void pass_fail_tests()
{
    int fail_count = Tests::test_grid_class();

    fail_count += Tests::test_path_algorithms();

    fail_count += Tests::compare_two_algorithms();

    // fail_count += Tests::test_big_grids();  // too big for DSPA

    std::cout << "Fail count: " << fail_count << std::endl;
}

void time_tests()
{
    Tests::time_dp();

    Tests::time_dspa();
}

int main()
{
    Tests::view_specific();

    return 0;
}
