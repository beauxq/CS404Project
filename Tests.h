#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <string>

namespace Tests
{
    bool test_equal(const std::string& first, const std::string& second, const std::string& test_name);

    void test_grid_class();
    void test_dp();
}

#endif // TESTS_H_INCLUDED
