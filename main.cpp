#include <iostream>

#include "Tests.h"

int main()
{
    int fail_count = Tests::test_grid_class();

    fail_count += Tests::test_path_algorithms();

    std::cout << "Fail count: " << fail_count << std::endl;

    return 0;
}
